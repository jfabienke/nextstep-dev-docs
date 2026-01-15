# Advanced Record List Techniques

The DBRecordList objects that were used in the previous chapter were intentionally simple. This simplicity was concentrated in three areas:

1. Their property lists contained only "natural" properties; in other words, properties that were gotten directly from DBEntities objects.
2. When the DBRecordLists were told to fetch, all corresponding records were retrieved.
3. The source entities, as passed as the second argument to the `setProperties:ofSource:`, were all simple DBEntities objects.

In reality, all three of these are more flexible:

1. The property list can contain objects other than those that are found in a database model. Specifically, you can create your own DBExpression objects and add them to a DBRecordList's property list.
2. You can restrict, or "qualify," the range of records that are retrieved by employing a DBQualifier object when you tell a DBRecordList to fetch.
3. By setting the source of one DBRecordList to a value retrieved by another, you can create a master-detail correspondence between the two objects.

The following sections examine the DBExpression and DBQualifier classes and show how to create a master-detail set-up using DBRecordList objects.

---

## DBExpression

A DBExpression object encapsulates an expression that, when evaluated, represents a column of data that's stored in a DBRecordList. The DBExpression class adopts the DBProperties protocol, thus a DBExpression object is a property. Anywhere a property object is called for—most notably in DBRecordList's `setProperties:ofSource:` and the `set/getValue:forProperty:at:` methods—you can use a DBExpression object.

DBExpression objects can do things that natural properties can't:

1. Create a join through a to-one relationship
2. Specify data that's derived from the manipulation of other properties
3. Change the data type of a property

You can also use a DBExpression to encapsulate, or "cover" an existing property.

### Traversing To-One Relationships (Joining Tables)

Perhaps the most important facet of a DBExpression object is that it allows you to store, in the same DBRecordList, properties from entities that are related through a to-one relationship. For example, consider a model with Book and Author entities where Book has a `toAuthor` relationship.

You can't pluck properties from related entities and set them in the same DBRecordList object because you wouldn't be able to provide a single entity object as the source argument to the `setProperties:ofSource:` method. However, since the Book and Author entities are related through a to-one relationship, it's not unreasonable to expect to be able to use a single DBRecordList to hold records that contain, for example, a book's title and the name of the book's author.

This is where a DBExpression object comes in. To create a DBExpression for the Author.name attribute that's gotten through the `toAuthor` relationship, you describe the author's name attribute as `toAuthor.name` rooted at Book. This is performed through the DBExpression's `initForEntity:fromDescription:` method:

```objc
/* The database object is assumed to exist. */
id bookEntity = [database entityNamed:"Book"];
DBExpression *authorName = [[DBExpression alloc]
    initForEntity:bookEntity
    fromDescription:"toAuthor.name"];
```

The first argument is an entity object; the second (the object's "description") is a string that gives a model path that's rooted at the first-argument entity, and that ends at a named property in a related entity.

The `authorName` DBExpression object and the natural book properties can then be placed in the same DBRecordList:

```objc
List *aList = [[List alloc] init];
DBRecordList *bookRecordList = [[DBRecordList alloc] init];

/* Add the previously created DBExpression object to the list. */
[aList addObject:authorName];

/* Add a natural book property to the list. */
[aList addObject:[bookEntity propertyNamed:"title"]];
[bookRecordList setProperties:aList ofSource:bookEntity];
```

When you tell the DBRecordList to fetch, the records that are retrieved will contain the title and author of each book that's represented in the database. The correspondence between titles and authors is correctly enforced.

### Nested Relationships

A DBExpression can traverse nested to-one relationships:

```objc
/* bookEntity is assumed to exist. */
DBExpression *unionAddress =
    [[DBExpression alloc]
    initForEntity:bookEntity
    fromDescription:"toAuthor.toUnion.address"];
```

### Property Facets

Since the DBExpression class conforms to the DBProperties protocol, you can send a DBExpression object any of the messages that are defined by that protocol. In particular, you can ask a DBExpression for its entity, name, property type, and so on. The responses that a relationship-traversing DBExpression gives to these messages are:

| Method | Response |
|--------|----------|
| `name` | The object's name is its description. For example, the name of the `authorName` DBExpression is `toAuthor.name`. |
| `entity` | The entity is that at which the DBExpression is rooted; in other words, it's the object that you used as the first argument to the `initForEntity:fromDescription:` method. |
| `propertyType` | The property type object of a relationship-traversing DBExpression is (accurately) retrieved from the foreign property. |
| `isSingular` | The response to this message is also accurately retrieved from the foreign property. |
| `isReadOnly` | You can't alter the values that are held for a relationship-traversing DBExpression. Such objects are, in practice, read-only. |
| `isKey` | A relationship-traversing DBExpression can't represent a primary key of the root entity. |

### Specifying Derived Data

The second use of DBExpression objects lets you create properties that represent derived data, or values that are created by manipulating the values for other properties. For example, consider a simple Box entity that contains the attributes height, width, and depth. Furthermore, for each box record, you want to compute the volume and store it as a separate property:

```objc
DBExpression *volume =
    [[DBExpression alloc]
    initForEntity:boxEntity
    fromDescription:"height * width * depth"];
```

The DBExpression that you've created can be set in a DBRecordList's property list. When the DBRecordList fetches data, the volume computation is automatically performed using the values for the height, width, and depth properties of each record:

```objc
/*  boxRecordList is the DBRecordList that fetched box records.
    val is a DBValue that's assumed to exist. */
[boxRecordList getValue:val forProperty:volume at:n];
```

### Description String Evaluation

When you fetch data into a DBRecordList that contains a DBExpression, the string that describes the DBExpression is parsed and evaluated to form a statement in the server's query language:

1. **Parsing** a description string simply means that the individual components are identified and separated
2. **Substitution symbols** (if any) are replaced by the corresponding arguments
3. The components are **evaluated** by the Database Kit. It looks at a component, decides if it recognizes it as an object and, if it does, sends that object an `expressionValue` or `stringValue` message. Anything the Database Kit doesn't recognize is left unevaluated.
4. Finally, the string is **sent to the adaptor**, which applies the parsed and Kit-evaluated description to each record that's involved in the transaction.

During its evaluation, the Database Kit performs a second task: It decides whether the DBExpression represents derived data. The rule for determining the intent of a DBExpression is fairly simple: If the description string contains anything other than references to property objects, the DBExpression is assumed to represent derived data.

This determination is important because it regulates:

- The data type of DBExpression object that represents derived data is always a string, even if the computation naturally results in numeric data
- Derived-data DBExpressions are read-only. You can't write derived-data back to the server.

### Description String Format

The format of a DBExpression's description argument is similar to printf-type statements: It consists of a quoted string containing the property names, operators, and substitution symbols needed to construct the desired expression, followed by values to be substituted for the symbols.

| Symbol | Value |
|--------|-------|
| `%s` | A constant string (`const char *`) |
| `%p` | A string that names one of the entity's properties |
| `%d` | An `int` |
| `%f` | A `double` or `float` |
| `%@` | An object that conforms to the DBExpressionValues protocol or that implements the `stringValue` method |
| `%%` | No value—this passes a single `%` literally |

For example:

```objc
float higher = 2.5;
DBExpression *newHeight =
    [[DBExpression alloc]
    initForEntity:boxEntity
    fromDescription:"%p + %f", "height", higher];
```

After parsing and evaluating, the Database Kit converts this string into the expression `HEIGHT + 2.5` (where "HEIGHT" is assumed to be the private name of the height property) and sends the expression to the adaptor.

String values are placed in single quotes by the Database Kit. For example:

```objc
float higher = 2.5;
DBExpression *calcHeight =
    [[DBExpression alloc]
    initForEntity:boxEntity
    fromDescription:"%p %s %f %s %p + %f",
    "height", " plus ", higher, " equals ",
    "height", higher];
```

produces an expression that appears as `HEIGHT ' plus ' 2.5 ' equals ' HEIGHT + 2.5`.

### The DBExpressionValues Protocol

The DBExpressionValues protocol, which declares the `expressionValue` method, is conformed to by DBValue and DBExpression objects, and by the entity and property objects that are retrieved from a database model. Any of these objects, therefore, can be used in a DBExpression object's description as the replacement value for the `%@` symbol.

#### Nested DBExpressions

DBExpression is one of the classes that conforms to the DBExpressionValues protocol; this means that you can created "nested" DBExpression objects:

```objc
float evenHigher = 3.6;
DBExpression *higherHeight =
    [[DBExpression alloc]
    initForEntity:boxEntity
    fromDescription:"%@ + %f", newHeight, evenHigher];
```

#### Using Other Objects in your DBExpression

If the object that's substituted for the `%@` symbol doesn't conform to the DBExpressionValues protocol, then it's evaluated by the Database Kit through the `stringValue` method. This is a handy way to get the value from a user interface object, such as a Slider or a TextField, into a DBExpression:

```objc
/* aSlider is assumed to exist as part of the user interface. */
DBExpression *newestHeight =
    [[DBExpression alloc]
    initForEntity:boxEntity
    fromDescription:"%p + %@", "height", aSlider];
```

When the `newestHeight` object is evaluated, the value of `aSlider`, converted to a string by `stringValue`, is placed in the statement that's passed to the adaptor.

### Casting a Property's Data Type

The property objects that you retrieve from a database model have a data type that can't be changed. However, you can recast it by placing the property in a DBExpression. This is done through the `initForEntity:fromName:usingType:` method:

```objc
DBExpression *doubleHeight =
    [[DBExpression alloc]
    initForEntity:boxEntity
    fromName:"height"
    usingType:"d"];
```

The first two arguments identify, by entity object and property name, the property that you want to cast. The final argument uses the data type string convention from Chapter 6 (where `@` = object, `*` = string, `i` = integer, `f` = float, `d` = double).

### Covering a Property

Finally, you can use DBExpression objects to represent natural properties:

```objc
DBExpression *boxHeight =
    [[DBExpression alloc]
    initForEntity:boxEntity
    fromDescription:"height"];
```

This use of a DBExpression doesn't provide any new functionality. However, you may find it beneficially consistent to store, in a DBRecordList, only DBExpression objects.

---

## DBQualifier

DBQualifier objects let you define the range of records that are retrieved during a fetch based on a formula that compares, at each record, the values for various properties. For example, let's say you want to retrieve employee records for all employees that make more than $5000 a month:

```objc
id emp = [db entityNamed:"Employee"];
DBQualifier *fiveGEmps =
    [[DBQualifier alloc]
    initForEntity:emp
    fromDescription:"salary > 5000"];
```

Any properties that are named in the description string are assumed to be rooted at the entity given by the first argument.

### Description Format

A DBQualifier's description string obeys the same formatting rules laid out for DBExpression objects:

```objc
id emp = [db entityNamed:"Employee"];
id salary = [emp propertyNamed:"salary"];
int bucksAMonth = 5000;

DBQualifier *fiveGEmps =
    [[DBQualifier alloc]
    initForEntity:emp
    fromDescription:"%@ > %d",
    salary, bucksAMonth];
```

### Multiple Qualifications

A DBQualifier's description can contain more than one qualification, provided the query language defines symbols that perform logical operations such as union and intersection:

```objc
id emp = [db entityNamed:"Employee"];
id salary = [emp propertyNamed:"salary"];
id commission = [emp propertyNamed:"commission"];
int bucksAMonth = 5000;
float commissionRate = 25.0;

DBQualifier *fiveGEmps =
    [[DBQualifier alloc]
    initForEntity:emp
    fromDescription:"(%@ > %d) OR (%@ > %f)",
    salary, bucksAMonth,
    commission, commissionRate];
```

You can build a multiple-qualification DBQualifier object through successive invocations of the `addDescription:` method:

```objc
/* We start with an empty description. */
DBQualifier *fiveGEmps =
    [[DBQualifier alloc]
    initForEntity:emp
    fromDescription:""];

/* Now add the desired qualifiers. */
[fiveGEmps addDescription:"(%@ > %d)", salary, bucksAMonth];
[fiveGEmps addDescription:" OR "];
[fiveGEmps addDescription:"(%@ > %f)", commission, commissionRate];
```

### Applying a DBQualifier

There's only one thing you can do with a DBQualifier: Use it as the argument to a fetch message:

```objc
DBRecordList *employeeList = [[DBRecordList alloc] init];
List *propList = [[List alloc] init];
id emp = [db entityNamed:"Employee"];
DBQualifier *fiveGEmps =
    [[DBQualifier alloc]
    initForEntity:emp
    fromDescription:"salary > 5000"];

/*  Configure the DBRecordList. */
[propList addObject:[emp propertyNamed:"name"]];
[propList addObject:[emp propertyNamed:"salary"]];
[employeeList setProperties:propList ofSource:emp];

/* Fetch data while applying the DBQualifier. */
[employeeList fetchUsingQualifier:fiveGEmps];
```

When applying a DBQualifier, note the following rules:

1. For a qualified fetch to work, the DBQualifier's entity must match the entity of the DBRecordList that's doing the fetching. If they don't match, the fetch will fail.
2. The property objects that are used in a DBQualifier's description needn't be present in the DBRecordList's list of properties.

### Qualifying Across a Relationship

The properties that are used in a DBQualifier needn't all be directly contained in the object's root entity: A DBQualifier can compare values that are gotten through to-one relationships:

```objc
DBRecordList *employeeList = [[DBRecordList alloc] init];
List *propList = [[List alloc] init];
id emp = [db entityNamed:"Employee"];

/*  Create an object for the "toDepartment.name" attribute. */
id deptName =
    [[[emp propertyNamed:"toDepartment"]
    propertyValue]
    propertyNamed:"name"];

/* The qualification compares the related attribute to the given
   string value. */
DBQualifier *salesFolk =
    [[DBQualifier alloc]
    initForEntity:emp
    fromDescription:"%@ LIKE 'SALES'",
    deptName];

/*  Configure the DBRecordList. */
[propList addObject:[emp propertyNamed:"name"]];
[propList addObject:[emp propertyNamed:"salary"]];
[employeeList setProperties:propList ofSource:emp];

/* Fetch data while applying the DBQualifier. */
[employeeList fetchUsingQualifier:salesFolk];
```

### DBExpressions in a DBQualifier

A DBQualifier can use DBExpression objects in its description string. When a fetch is performed, the DBExpression's description is evaluated first, and then the DBQualifier is applied:

```objc
DBExpression *volume =
    [[DBExpression alloc]
    initForEntity:boxEntity
    fromDescription:"height * width * depth"];

DBQualifier *roomy =
    [[DBQualifier alloc]
    initForEntity:boxEntity
    fromDescription:"%@ > 150.0", volume];
```

During a fetch that's qualified by `roomy`, the enclosed volume DBExpression is evaluated and its result is used in roomy's qualification formula. The entities of all three objects—the DBExpression, DBQualifier, and the fetching DBRecordList—must be the same.

---

## Master-Detail Record Tables

To create a master-detail set-up you need two DBRecordList objects—one to act as master and the other as detail—and a DBValue object. Assembly proceeds thus:

1. Set the master DBRecordList's property list. One of the properties that it must include, for it to be a master, is a relationship.
2. Prepare the detail property list, but don't set it yet. In other words, create and fill the List object that you'll pass to `setProperties:ofSource:`, but don't invoke the method. The properties that you add to the List object must be gotten from the entity that's retrieved by sending `propertyValue` to the master's relationship.
3. Tell the master DBRecordList to fetch.
4. Get a value for the master's relationship through the `getValue:forProperty:at:` method.
5. Send `setProperties:ofSource:` to the detail DBRecordList object, passing the List object that was prepared in step 2 as the property list argument, and the DBValue that was gotten in step 4 as the source.
6. Tell the detail DBRecordList to fetch.
7. Repeat steps 4-6 for each set of detail records that you want to store in the detail DBRecordList.

For example, consider a model for the Department and Employee entities where more than one employee may work in a department:

```objc
/* Create two DBRecordLists, a List object for setting properties,
   and a DBValue to be used after the master has fetched.  */
DBRecordList *masterDept = [[DBRecordList alloc] init];
DBRecordList *detailEmp = [[DBRecordList alloc] init];
List *props = [[List alloc] init];
DBValue *toEmpVal = [[DBValue alloc] init];

/* Create objects to store the desired entity and properties.
   We'll assume that db, a DBDatabase object, exists.*/
id dept = [db entityNamed:"Department"];
id deptName = [dept propertyNamed:"name"];
id toEmp = [dept propertyNamed:"toEmployee"];

/* Configure the master DBRecordList. */
[props addObject:deptName];
[props addObject:toEmp];
[masterDept setProperties:props ofSource:nil];

/* Empty the List object and reuse it to hold the detail properties. */
[props empty];
[props addObject:[[toEmp propertyValue] propertyNamed:"name"]];
[props addObject:[[toEmp propertyValue] propertyNamed:"salary"]];

/* Fetch the master records. */
[masterDept fetchUsingQualifier:nil];

/* Get the first record's value for the relationship. */
[masterDept getValue:toEmpVal forProperty:toEmp at:0];

/* Set the detail DBRecordList and fetch. */
[detailEmp setProperties:props ofSource:toEmpVal];
[detailEmp fetchUsingQualifier:nil];
```

To get the employee records for another department (in other words, for the department that's represented by another record in the master DBRecordList), you simply move the cursor, ask for the value of the relationship in the current record, and reset the properties in the detail DBRecordList and tell it to fetch.

---

*Release 3.3 Copyright ©1995 NeXT Computer, Inc. All Rights Reserved.*
