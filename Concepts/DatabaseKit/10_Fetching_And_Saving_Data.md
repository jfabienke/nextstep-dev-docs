# Fetching and Saving Data

This chapter examines the methods and techniques you use to move data between the server and your application. It's divided into two major sections:

1. **Fetching** — The data-fetching demonstration given in Chapter 8 was far from the whole story. The Database Kit lets you adjust the fetching machinery.
2. **Saving and validating data** — One of the thornier problems confronting a database application developer is to ensure that the data that's drawn into the application and modified there will "fit" when it's sent back to the server. The checks through which a client of a server, such as your application, attempts to ensure that its data will fit is called "validation" or "verification". The Database Kit provides a few validation techniques that any application should be able to use.

---

## Details of Fetching

There are a number of ways that you can tinker with the fetching machinery. You can:

- Perform the fetch in a background thread
- Limit the number of records that are retrieved during a single fetch
- Set the order of the records that are retrieved
- Concatenate the results of consecutive fetches
- Fetch a specific record based on its primary key value

These manipulations are examined in the following sections. But first, the DBRecordList's "retrieve status," upon which some of these tinkerings depends, is described.

### Retrieve Status

A DBRecordList maintains a "retrieve status," a constant that tells you where the DBRecordList is with regard to a "fetching session". There are five retrieve states, returned through the `currentRetrieveStatus` method:

- **`DB_NotReady`** — This is the status of a freshly made DBRecordList; it indicates that the object doesn't contain any properties. If you try to fetch while the DBRecordList isn't ready, the fetch will fail.
- **`DB_Ready`** — This status indicates that the DBRecordList is ready for a fetch. More specifically, it means that the object has had its properties set, but hasn't fetched any records.
- **`DB_FetchInProgress`** — You can only catch a DBRecordList in the act of fetching if the object is set to fetch in the background; this status indicates that a background fetch has commenced.
- **`DB_FetchLimitReached`** — This is used to indicate that a previous fetch operation was completed because the record limit was reached.
- **`DB_FetchCompleted`** — This status means that the previous fetch operation is finished, and that the entire range of records, given the latest qualification, have been retrieved from the server.

The normal sequence of states for a DBRecordList depends on how the object's fetching mechanism is set up:

| Invoking this method... | ...induces this status |
|--------|--------|
| `init`, `clear`, or `empty` | `DB_NotReady` |
| `setProperties:ofSource:` | `DB_Ready` |
| `fetchUsingQualifier:` | `DB_FetchInProgress` (background fetches only) |
| | `DB_FetchLimitReached` (record-limited fetches only) |
| | `DB_FetchCompleted` (foreground fetches only) |

### Fetching in the Background

By default, fetches are performed in the foreground: The fetch method doesn't return until it's finished fetching. You can also set a DBRecordList to fetch in the background, thus allowing the fetch method to return immediately while the fetch continues asynchronously in its own process thread.

To set the "fetch strategy" of a DBRecordList, you pass one of these DBRecordListRetrieveMode constants to the `setRetrieveMode:` method:

- **`DB_SynchronousStrategy`** — This is the default foreground strategy.
- **`DB_BackgroundStrategy`** — Records are fetched in the background, allowing your application to proceed. However, if you try to move the DBRecordList's cursor to a record that hasn't yet been fetched, the cursor-positioning method will block until the specified record is retrieved.
- **`DB_BackgroundNoBlockingStrategy`** — This is the same as `DB_BackgroundStrategy`, except that it doesn't cause the cursor-moving methods to block; if you try to point the cursor to an as-yet-unretrieved record, the method will fail and immediately return.

The `retrieveMode` method returns a DBRecordList's current fetch strategy.

> **Warning:** You should only set a DBRecordList to fetch in the background if you're using the Application Kit and you have its main event loop running.

#### Checking for the Completion of a Background Fetch

After you've started a background fetch, you'll probably want to know when the fetch is complete. To be so informed, you need to supply a "binder" delegate that implements the `binderDidFetch:` method:

```objc
/* Declare the interface for a class that implements the
   binderDidFetch: method. */
@interface MyBinderDelegate : Object
{}
- binderDidFetch:aBinder;
@end

/*  Define the class. */
@implementation MyBinderDelegate
- binderDidFetch:aBinder
{
    /*  Do what you need to do here. The return value is ignored, but
        it isn't declared void, so, to be polite, we return self. */
    return self;
}
```

Then you set the DBRecordList's binder delegate to an instance of your class:

```objc
MyBinderDelegate *aDelegate = [[MyBinderDelegate alloc] init];
DBRecordList *aRecordList = [[DBRecordList alloc] init];

[aRecordList setBinderDelegate:aDelegate];
```

When the fetch is complete, the binder delegate is sent the `binderDidFetch:` message.

#### Background Fetches and Retrieval Status

After a background fetch has begun, the DBRecordList's retrieval status is set to `DB_FetchInProgress`. It's important to note that the status doesn't change when the fetch is complete—it remains `DB_FetchInProgress` until the DBRecordList is cleared, emptied, or has its properties reset.

#### Stopping a Background Fetch

If you get tired of waiting for a background fetch to complete, you can tell it to stop by sending the `cancelFetch` message to the DBRecordList. This doesn't wipe out the fetch entirely—records that have already been fetched and placed in the DBRecordList aren't thrown away—it simply stops the fetch process. Cancelling a background fetch thwarts the `binderDidFetch:` delegate message.

### Record Limit

When a DBRecordList is commanded to fetch, it normally fetches the entire range of records that pass the qualification. You can set the maximum number of records that you want a fetch to retrieve through the `setRecordLimit:` method. The `currentRecordLimit` method returns the currently set record limit; a return of `DB_NoIndex` indicates that there is no limit (this is the default).

> **Important:** For the record limit to have an effect, the DBRecordList's fetching strategy must be `DB_SynchronousStrategy`: You can't set the record limit of a DBRecordList that fetches in the background.

There are two ways to tell if the record limit was reached during a fetch:

1. The DBRecordList's status is set to `DB_FetchLimitReached`
2. A `recordStream:willFailForReason:` message is sent to the DBRecordList's delegate, with `DB_RecordLimitReached` as the second argument

#### Fetch Limit Reached Status

The `DB_FetchLimitReached` status is essentially the same as `DB_FetchCompleted`. This status doesn't affect a subsequent fetch: The next fetch doesn't continue where the previous one left off, it begins fetching at the "top" of the server's list of records.

If the record limit is greater than the number of candidate records in the server, the DBRecordList's status, after a fetch, is set to `DB_FetchCompleted`. (If the two values are equal, the status is `DB_FetchLimitReached`, even though all the records were fetched.)

#### The Delegate Message

When, during a fetch, a DBRecordList's record limit is reached, the object's delegate is sent a `recordStream:willFailForReason:` message. The delegate's response to this boolean message is important:

- A response of `YES` tells the DBRecordList to obey the record limit; the fetch is halted and the object's status set to `DB_FetchLimitReached`
- A response of `NO` tells the DBRecordList to ignore the limit and continue fetching

### Sorting Records

DBRecordList provides methods that let you declare the basis upon which fetched records are sorted into the record table. This is done by associating a "retrieve order" constant with a particular property in the DBRecordList object. There are three retrieve order constants:

- **`DB_NoOrder`**
- **`DB_AscendingOrder`**
- **`DB_DescendingOrder`**

The association between a retrieve order and a property is established through the `addRetrieveOrder:for:` method:

```objc
/* We'll assume that the objects used below exist; "employees" is a
   DBRecordList; "name" is a property object rooted at "employees"
   source entity. */
[employees addRetrieveOrder:DB_AscendingOrder for:name];
```

When the DBRecordList fetches, the records are sorted into alphabetical order based on the values for the name property.

#### Secondary Sorting

You can provide a tie-breaker by specifying the retrieve order for yet another property. The order in which retrieve-ordered properties are applied during a fetch is the order in which the `addRetrieveOrder:for:` messages were sent:

```objc
/* The firstName and lastName objects are assumed to exist. */
[employees addRetrieveOrder:DB_AscendingOrder for:lastName];
[employees addRetrieveOrder:DB_AscendingOrder for:firstName];
```

### Concatenating Fetches

By default, a DBRecordList empties itself before it fetches data. You can tell a DBRecordList to retain "old" data by passing `NO` as the second argument to the `fetchUsingQualifier:empty:` method. This causes the results of the new fetch to be appended to the records that are already in the DBRecordList.

---

## Saving and Validating Data

Writing modified data back to the server is simple: You invoke DBRecordList's `saveModifications` method. The method steps through the record table and identifies those records that contain modified data. As it checks for modified data, the `saveModifications` method also performs an "integrity" test that warns you if the data that you're saving will overwrite someone else's changes.

The Database Kit does nothing, at the time data is saved, to check data value "correctness" or data format consistency. However:

- Checking the value for an attribute is highly dependent on context—only your application can determine whether a value is appropriate
- The data type of a field in a record is immutable. The value in a particular field is converted to the data type declared by the attribute; it maintains that type regardless of value modifications

### Finding Modified Data

There are three boolean DBRecordList methods that help you find modified data:

- **`isModified`** — tells you if the object's record table has been modified such that it might not correspond to data on the server. Returns `YES` if records have been added or deleted from the table, or if the value of a field within a record has been set.
- **`isModifiedAt:`** — takes a record index argument and returns `YES` if the record is new or if the value of any of the record's fields has been set.
- **`isModifiedForProperty:at:`** — looks at the specific record field designated by the property object and record index arguments and returns `YES` if its value has been set or if the record is new.

### The Integrity Test

The Database Kit performs a "data integrity" test to make sure that the data in the records that you're saving won't overwrite someone else's changes. The manner in which the test is conducted depends on the adaptor that you're using.

#### The Oracle Test

The Oracle integrity test examines each modified record and determines if any field in that record has changed on the server since the record was fetched. If a field has been changed, the record won't be used to update the server's data.

Key points about the Oracle integrity test:

- Data is accepted or rejected on a record-by-record basis
- The entire record is rejected if any field has been changed on the server
- After the save, your record table will still hold modified data for rejected records

#### The Sybase Test

The Sybase integrity test proceeds much like that of the Oracle test: A just-fetched copy of your records is used to determine if the records have changed since the time that they were fetched. Unlike Oracle, the Sybase mechanism doesn't automatically reject a record that shows a discrepancy. By default, all modifications are accepted, even if they overwrite someone else's changes.

Key points about the Sybase integrity test:

- Sybase will accept changes to individual fields, unlike Oracle which accepts or rejects whole records
- When the integrity test finds a questionable record, it sends the DBRecordList's delegate a `recordStream:willFailForReason:` message
- The delegate can implement this method to thwart the impending overwrite

### Controlling a Save

The `saveModifications` method sends the DBRecordList's delegate a `recordStream:willFailForReason:` message if there's a problem saving the data. The message's second argument is a DBFailureCode constant that describes the failing situation.

#### Error Codes for DBRecordList Setup

The first two failure codes indicate an error in the set up of the DBRecordList:

- **`DB_RecordStreamNotReady`** — The DBRecordList isn't ready to save, probably because it hasn't had its property list set.
- **`DB_NoRecordKey`** — The DBRecordList doesn't have a primary key attribute.

For both of these errors, the `saveModifications` method returns immediately with the value `DB_NoIndex`.

#### Record Error Codes

The other error codes indicate a "record error." These codes can be passed as the second argument in a `recordStream:willFailForReason:` message:

- **`DB_RecordKeyNotUnique`** — The fetch that was performed by the integrity test found more than one record with matching primary key values.
- **`DB_TransactionError`** — The current record failed the integrity test because the analogous record on the server couldn't be found.
- **`DB_RecordHasChanged`** — The current record failed the integrity test because of changed data.
- **`DB_AdaptorError`** — The adaptor encountered an error while writing the current record to (or deleting it from) the server.

For each of these record errors, the boolean value returned by `recordStream:willFailForReason:` is important:

- If it returns `YES`, the entire transaction is aborted. Any previous records that were successfully saved are "rolled back".
- If it returns `NO`, the save process isn't aborted. What happens to the current record depends on the adaptor.

#### Return Values from saveModifications

| Return Value | Meaning |
|---|---|
| `DB_NoIndex` | The save was aborted (or never got to the first record) |
| `1` | The DBRecordList may contain some unsaved records |
| `0` | The save was completely successful |

### Implementing the Delegate Method

When `recordStream:willFailForReason:` is invoked, the DBRecordList is guaranteed to be "pointing to" the record that caused the failure. The following methods can be used within a delegate's implementation of this method:

- `getValue:forProperty:`
- `getRecordKeyValue:`
- `currentPosition`

> **Warning:** These "record cursor" methods should only be used in the implementation of the `recordStream:willFailForReason:` method. They should otherwise be avoided.

The `currentPosition` method is particularly useful, for you can use the value it returns (the record index of the failed record) to determine which properties were modified.

Example implementation:

```objc
- (BOOL) recordStream:streamOrList
willFailForReason:(DBFailureCode)aCode
{
    switch (aCode) {
        case DB_RecordKeyNotUnique:
        case DB_TransactionError:
        case DB_RecordHasChanged:
        case DB_AdaptorError:
            /*  We'll assume that locProp, which represents the
                "locations" property, is declared globally. */
            if ([streamOrList isModifiedForProperty:locProp
                at:[streamOrList currentPosition]])
                return YES;
            else
                return NO;
        default:
            return YES;
    }
}
```

> **Warning:** You shouldn't try to fix the error or in any other way modify the record in the implementation of this method. By the time the method is invoked, it's too late for the failed record.

> **Warning:** If you're using the Sybase adaptor provided by the Database Kit, you also must not fetch during the `recordStream:willFailForReason:` method.

---

*Release 3.3 Copyright ©1995 NeXT Computer, Inc. All Rights Reserved.*
