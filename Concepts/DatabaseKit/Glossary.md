# Glossary

**adaptor**
A mechanism that connects a Database Kit application to a particular database server. An adaptor translates the generic commands issued by the application into server-specific commands.

**archive model**
One of the two formats for database model files. This binary format, identified by the extension `.dbmodel`, is created by the DBModeler application.

**ASCII model**
One of the two formats for database model files. This format, identified by the extension `.dbmodela`, can be created or edited with a text editor but must be converted into an archive model before it can be used by an application.

**attribute**
In Entity-Relationship modeling, an identifiable part of an entity. For example, name could be an attribute of an Employee entity.

**buffer-editor**
A mechanism for operating on a copy of a portion of the data in a database server rather than directly on the server's data. The modified data can then be written back to the server.

**column**
In a relational database, the dimension of a table that groups attributes. For example, a table that holds book records might have a column titled "Title".

**compound primary key**
A combination of attributes in an entity that, taken in combination, are guaranteed to represent a unique value for each record.

**compound relationship**
A relationship that is based on more than one pair of relationship keys.

**data dictionary**
In relational databases, the system tables that describe the organization of data in a particular database.

**Database Kit**
In NEXTSTEP, the object-oriented kit and associated tools that help you create front-end applications that connect to database servers.

**Database Kit palette**
In Interface Builder, the collection of objects displayed in the Palettes window that you use in creating Database Kit applications.

**database model**
A file that defines, in Entity-Relationship terms, the organization of data in a database server. DBModeler creates database models in binary format (archive models) and in ASCII format (ASCII models).

**database server**
A data storage and retrieval system. Database servers typically run on a dedicated computer and are accessed by client applications over a network.

**database server adaptor**
A mechanism that connects a Database Kit application to a particular database server. The adaptor translates Database Kit commands into commands understood by the server.

**DBModeler**
One of the tools associated with the Database Kit. You use the DBModeler application to create database models.

**destination entity**
In describing a relationship between two entities, the entity to which the relationship points. Given an instance of the source entity and a relationship key, the destination entity is searched for instances that have a matching key value.

**destination key**
In describing a relationship between two entities, the relationship key in the destination entity.

**entity**
In entity-relationship modeling, a distinguishable thing about which data is kept. Employees could be one entity in a corporate database.

**Entity-Relationship modeling**
A discipline for examining and representing the components and interrelationships in a database system. Also known as E-R modeling, this discipline factors a database system into entities, attributes, and relationships.

**equijoin**
A join operation in which only those source records that satisfy the join condition appear in the result.

**fetch**
In Database Kit applications, to copy record data from the database server into the client application.

**foreign key**
An attribute added to an entity to create a relationship key. This attribute is the primary key of the related entity.

**join**
An operation that lets you access data from two tables at the same time, based on the values contained in related columns.

**many-to-many relationship**
A relationship in which each record in the source entity may correspond to more than one record in the destination entity, and each record in the destination may correspond to more than one record in the source.

**model path**
A typographic encoding of pathways in an E-R diagram. For example, the name attribute of the Employee entity can be represented as the model path `Employee.name`.

**modeling objects**
A general term that encompasses the concepts of entities, attributes, and relationships.

**outer join**
A join operation in which all source records are included in the result—even those that don't satisfy the join condition.

**primary key**
An attribute of an entity that's guaranteed to represent a unique value for each record.

**properties**
An entity's attributes and relationships.

**record**
The set of data values that describes a single instance of an entity.

**reflexive relationship**
A relationship in which the source and destination entities are the same.

**relational database**
A database designed according to the relational model, which uses the discipline of Entity-Relationship modeling and the data design standards called normal forms.

**relationship**
An affiliation between two entities. For example, the Book and Borrower entities could be linked with a relationship so that it would be possible to find the borrower for any given book.

**relationship key**
The attribute in either of two entities by which the two entities are linked.

**row**
In a relational database, the dimension of a table that groups attributes into records.

**source entity**
In describing a relationship between two entities, the entity from which the relationship originates. Given an instance of the source entity and a relationship key, the destination entity is searched for instances that have a matching key.

**source key**
The relationship key in the source entity.

**table**
A two-dimensional display of data values corresponding to an entity. The columns of a table represent characteristics of the entity and the rows represent instances of the entity.

**to-many relationship**
A relationship in which for each source record there may be more than one corresponding destination record.

**to-one relationship**
A relationship in which for each source record there's no more than one corresponding destination record.

**unidirectional relationship**
A relationship in which the path that leads from the source to the destination entity can't be traveled in the opposite direction.

**update**
In Database Kit applications, to modify records in the database server based on changes to the corresponding records in the client application.

---

*Release 3.3 Copyright ©1995 NeXT Computer, Inc. All Rights Reserved.*
