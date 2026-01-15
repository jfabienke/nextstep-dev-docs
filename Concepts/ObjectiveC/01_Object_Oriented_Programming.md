# Object-Oriented Programming

Programming languages have traditionally divided the world into two parts—data and operations on data. Data is static and immutable, except as the operations may change it. The procedures and functions that operate on data have no lasting state of their own; they're useful only in their ability to affect data.

This division is, of course, grounded in the way computers work, so it's not one that you can easily ignore or push aside. Like the equally pervasive distinctions between matter and energy and between nouns and verbs, it forms the background against which we work. At some point, all programmers—even object-oriented programmers—must lay out the data structures that their programs will use and define the functions that will act on the data.

With a procedural programming language like C, that's about all there is to it. The language may offer various kinds of support for organizing data and functions, but it won't divide the world any differently. Functions and data structures are the basic elements of design.

Object-oriented programming doesn't so much dispute this view of the world as restructure it at a higher level. It groups operations and data into modular units called objects and lets you combine objects into structured networks to form a complete program. In an object-oriented programming language, objects and object interactions are the basic elements of design.

Every object has both state (data) and behavior (operations on data). In that, they're not much different from ordinary physical objects. It's easy to see how a mechanical device, such as a pocket watch or a piano, embodies both state and behavior. But almost anything that's designed to do a job does too. Even simple things with no moving parts such as an ordinary bottle combine state (how full the bottle is, whether or not it's open, how warm its contents are) with behavior (the ability to dispense its contents at various flow rates, to be opened or closed, to withstand high or low temperatures).

It's this resemblance to real things that gives objects much of their power and appeal. They can not only model components of real systems, but equally as well fulfill assigned roles as components in software systems.

---

## Interface and Implementation

As humans, we're constantly faced with myriad facts and impressions that we must make sense of. To do so, we have to abstract underlying structure away from surface details and discover the fundamental relations at work. Abstractions reveal causes and effects, expose patterns and frameworks, and separate what's important from what's not. They're at the root of understanding.

To invent programs, you need to be able to capture the same kinds of abstractions and express them in the program design.

It's the job of a programming language to help you do this. The language should facilitate the process of invention and design by letting you encode abstractions that reveal the way things work. It should let you make your ideas concrete in the code you write. Surface details shouldn't obscure the architecture of your program.

All programming languages provide devices that help express abstractions. In essence, these devices are ways of grouping implementation details, hiding them, and giving them, at least to some extent, a common interface—much as a mechanical object separates its interface from its implementation.

Looking at such a unit from the inside, as the implementor, you'd be concerned with what it's composed of and how it works. Looking at it from the outside, as the user, you're concerned only with what it is and what it does. You can look past the details and think solely in terms of the role that the unit plays at a higher level.

### Units of Abstraction in C

The principal units of abstraction in the C language are structures and functions. Both, in different ways, hide elements of the implementation:

**Data Structures**: C structures group data elements into larger units which can then be handled as single entities. While some code must delve inside the structure and manipulate the fields separately, much of the program can regard it as a single thing—not as a collection of elements, but as what those elements taken together represent. One structure can include others, so a complex arrangement of information can be built from simpler layers.

In modern C, the fields of a structure live in their own name space—that is, their names won't conflict with identically-named data elements outside the structure. Partitioning the program name space is essential for keeping implementation details out of the interface.

**Functions**: Functions encapsulate behaviors that can be used repeatedly without being reimplemented. Data elements local to a function, like the fields within a structure, are protected within their own name space. Functions can reference (call) other functions, so quite complex behaviors can be built from smaller pieces.

Functions are reusable. Once defined, they can be called any number of times without again considering the implementation. The most generally useful functions can be collected in libraries and reused in many different applications. All the user needs is the function interface, not the source code.

However, unlike data elements, functions aren't partitioned into separate name spaces. Each function must have a unique name. Although the function may be reusable, its name is not.

### Moving to Object-Oriented Programming

C structures and functions are able to express significant abstractions, but they maintain the distinction between data and operations on data. In a procedural programming language, the highest units of abstraction still live on one side or the other of the data-versus-operations divide.

Object-oriented programming languages don't lose any of the virtues of structures and functions. But they go a step further and add a unit capable of abstraction at a higher level, a unit that hides the interaction between a function and its data.

Suppose, for example, that you have a group of functions that all act on a particular data structure. You want to make those functions easier to use by, as far as possible, taking the structure out of the interface. So you supply a few additional functions to manage the data. All the work of manipulating the data structure—allocating it, initializing it, getting information from it, modifying values within it, keeping it up to date, and freeing it—is done through the functions.

With these changes, the structure has become an opaque token that other programmers never need to look inside. They can concentrate on what the functions do, not how the data is organized. You've taken the first step toward creating an object.

The next step is to give this idea support in the programming language and completely hide the data structure so that it doesn't even have to be passed between the functions. The data becomes an internal implementation detail; all that's exported to users is a functional interface.

With this step, the interface to the functions has become much simpler. This progression from thinking about functions and data structures to thinking about object behaviors is the essence of object-oriented programming.

---

## The Object Model

The insight of object-oriented programming is to combine state and behavior—data and operations on data—in a high-level unit, an object, and to give it language support. An object is a group of related functions and a data structure that serves those functions.

### Components of an Object

- **Methods** — The functions that belong to an object. These are the operations an object can perform.
- **Instance Variables** — The fields of an object's data structure. These store the state of the object.

The methods wrap around the instance variables and hide them from the rest of the program.

By combining both state and behavior in a single unit, an object becomes more than either alone; the whole really is greater than the sum of its parts. An object is a kind of self-sufficient "subprogram" with jurisdiction over a specific functional area. It can play a full-fledged modular role within a larger program design.

---

## The Messaging Metaphor

Every programming paradigm comes with its own terminology and metaphors. Object-oriented programming comes with the particularly evocative metaphor of objects as "actors."

There's a tendency, for example, to think of objects as "actors" and to endow them with human-like intentions and abilities. It's tempting sometimes to talk about an object "deciding" what to do about a situation, "asking" other objects for information, "delegating" responsibility to another object, or "managing" a process.

Rather than think in terms of functions or methods doing the work, as you would in a procedural programming language, this metaphor asks you to think of objects as "performing" their methods. Objects are not passive containers for state and behavior, but are said to be the agents of the program's activity.

This is actually a useful metaphor. An object is like an actor in a couple of respects: It has a particular role to play within the overall design of the program, and within that role it can act fairly independently of the other parts of the program. It interacts with other objects as they play their own roles, but is self-contained and to a certain extent can act on its own.

### Messages

The principal metaphor of object-oriented programming is the idea that objects communicate through "messages." Instead of calling a method as you would a function, you send a message to an object requesting it to perform one of its methods.

Although it can take some getting used to, this metaphor leads to a useful way of looking at methods and objects. It abstracts methods away from the particular data they act on and concentrates on behavior instead.

Methods are a vocabulary of abstract behaviors. To invoke one of those behaviors, you have to make it concrete by associating the method with an object. This is done by naming the object as the "receiver" of a message. The object you choose as receiver will determine the exact operation that's initiated.

Since methods belong to objects, they can be invoked only through a particular receiver. Different receivers can have different implementations of the same method, so different receivers can do different things in response to the same message.

---

## Classes

A program can have more than one object of the same kind. Objects of the same kind are said to belong to the same class. All members of a class are able to perform the same methods and have matching sets of instance variables. They also share a common definition; each kind of object is defined just once.

Defining an object creates a template for a kind of object. It defines a class of objects. The template can be used to produce any number of similar objects—instances of the class.

### Memory Allocation

Memory is allocated for the instance variables of each new object, but there's no need to allocate memory for methods. All an instance needs is access to its methods, and all instances of the same class share access to the same set of methods. There's only one copy of the methods in memory, no matter how many instances of the class are created.

---

## Modularity

To a C programmer, a "module" is nothing more than a file containing source code. Breaking a large program into different files is a convenient way of splitting it into manageable pieces. Each piece can be worked on independently and compiled alone, then integrated with other pieces when the program is linked.

Object-oriented programming languages support the use of file containers for source code, but they also add a logical module to the language—class definitions. It's often the case that each class is defined in its own source file, so logical modules are matched to container modules.

Class definitions create logical units within the construction of the program—they act as modular units within the language itself—regardless of how many files the source code is located in.

---

## Reusability

A principal goal of object-oriented programming is to make the code you write as reusable as possible—to have it serve many different situations and applications.

Reusability is influenced by a variety of different factors, including:

- How reliable and bug-free the code is
- How clear the documentation is
- How simple and straightforward the programming interface is
- How efficiently the code performs its tasks
- How full the feature set is

Class definitions lend themselves to reusable code in ways that functions do not. There are various things you can do to make functions more reusable, but their reusability is inherently limited in at least three ways:

1. **Function Name Conflicts** — Function names are global variables; each function must have a unique name. This makes it difficult to rely heavily on library code. Classes can share programming interfaces; when the same naming conventions are used over and over again, a great deal of functionality can be packaged with a relatively small interface.

2. **Packaging** — Functions are selected from a library one at a time. Objects come as packages of functionality. They provide integrated services, so users of an object-oriented library won't get bogged down piecing together their own solutions.

3. **Data Encapsulation** — Functions are typically tied to particular kinds of data structures. Because an object hides its data, it doesn't have this problem. This is one of the principal reasons why classes can be reused more easily than functions.

---

## Mechanisms of Abstraction

Objects couldn't be used as effective units of abstraction without the support of various language mechanisms. Two of the most important mechanisms are:

1. **Encapsulation**
2. **Polymorphism**

### Encapsulation

To design effectively at any level of abstraction, you need to be able to leave details of implementation behind and think in terms of units that group those details under a common interface. For a programming unit to be truly effective, the barrier between interface and implementation must be absolute. The interface must encapsulate the implementation—hide it from other parts of the program.

Encapsulation protects an implementation from unintended actions and inadvertent access.

In C, a function is clearly encapsulated; its implementation is inaccessible to other parts of the program. Method implementations are similarly encapsulated, but, more importantly, so are an object's instance variables. They're hidden inside the object and invisible outside it. The encapsulation of instance variables is sometimes also called information hiding.

Hiding the information in instance variables actually frees you as a programmer. If any part of an object's implementation could leak out and become accessible to other parts of the program, it would tie the hands both of the object's implementor and of those who would use the object. Neither could make modifications without first checking with the other.

### Polymorphism

This ability of different objects to respond, each in its own way, to identical messages is called polymorphism.

Polymorphism results from the fact that every class lives in its own name space. The names assigned within a class definition won't conflict with names assigned anywhere outside it. This is true both of the instance variables in an object's data structure and of the object's methods:

- Just as the fields of a C structure are in a protected name space, so are an object's instance variables.
- Method names are also protected. Unlike the names of C functions, method names aren't global symbols. The name of a method in one class can't conflict with method names in other classes.

Method names are part of an object's interface. When a message is sent requesting an object to do something, the message names the method the object should perform. Because different objects can have different methods with the same name, the meaning of a message must be understood relative to the particular object that receives the message.

The main benefit of polymorphism is that it simplifies the programming interface. It permits conventions to be established that can be reused in class after class. Instead of inventing a new name for each new function you add to a program, the same names can be reused.

---

## Inheritance

The easiest way to explain something new is to start with something old. Similarly, object-oriented programming languages permit you to base a new class definition on a class already defined.

The base class is called a **superclass**; the new class is its **subclass**. The subclass definition specifies only how it differs from the superclass; everything else is taken to be the same.

Nothing is copied from superclass to subclass. Instead, the two classes are connected so that the subclass **inherits** all the methods and instance variables of its superclass.

### Class Hierarchies

Any class can be used as a superclass for a new class definition. A class can simultaneously be a subclass of another class and a superclass for its own subclasses. Any number of classes can thus be linked in a hierarchy of inheritance.

Every inheritance hierarchy begins with a **root class** that has no superclass. From the root class, the hierarchy branches downward. Each class inherits from its superclass, and through its superclass, from all the classes above it in the hierarchy. Every class inherits from the root class.

Each new class is the accumulation of all the class definitions in its inheritance chain.

### Subclass Modifications

A subclass can make three kinds of changes to the definition it inherits through its superclass:

1. **Expand** — It can expand the class definition it inherits by adding new methods and instance variables. This is the most common reason for defining a subclass.

2. **Replace** — It can modify the behavior it inherits by replacing an existing method with a new version. This is done by implementing a new method with the same name as one that's inherited.

3. **Refine** — It can refine or extend the behavior it inherits by replacing an existing method with a new version, but still retain the old version by incorporating it in the new method.

---

## Dynamism

Object-oriented programming seeks to make programs as dynamic and fluid as possible. It shifts much of the burden of decision making from compile time and link time to run time.

Three kinds of dynamism are especially important for object-oriented design:

### Dynamic Typing

The compiler typically complains if the code you write assigns a value to a type that can't accommodate it. However, there are times when type checking can interfere with the benefits you get from polymorphism.

If it's possible to wait until run time to discover the class of a variable, any kind of object could be assigned to it. Depending on the class of the receiver, a message might invoke different versions of the method and produce very different results.

### Dynamic Binding

Dynamic binding is delaying the decision of exactly which method to perform until the program is running.

Messages invoke methods indirectly. Every message expression must find a method implementation to "call." To find that method, the messaging machinery must check the class of the receiver and locate its implementation of the method named in the message. When this is done at run time, the method is dynamically bound to the message. When it's done by the compiler, the method is statically bound.

### Dynamic Loading

The usual rule has been that, before a program can run, all its parts must be linked together in one file. Some object-oriented programming environments overcome this constraint and allow different parts of an executable program to be kept in different files. Each piece is dynamically loaded and linked with the rest of the program as it's launched.

---

## Structuring Programs

Object-oriented programs have two kinds of structure:

1. **The Inheritance Hierarchy** — This explains how objects are related by type.
2. **The Object Network** — This is evident in the pattern of message passing as the program runs. These messages reveal a network of object connections.

### Outlet Connections

Part of the task of designing an object-oriented program is to arrange the object network. Some connections can be entirely transitory—lasting only as long as a chain of messages. But not all connections between objects can be handled on the fly.

The simplest way to record persistent connections is for each object to have instance variables that keep track of the other objects it must communicate with. These instance variables—termed **outlets** because they record the outlets for messages—define the principal connections between objects in the program network.

Outlet connections reveal the structure of the application. They link objects into a communicating network.

### Intrinsic and Extrinsic Connections

**Intrinsic outlets** represent objects that are part of another object. When an object is freed or archived, the objects that its intrinsic outlets point to must be freed or archived with it.

**Extrinsic outlets** capture the organization of the program at a higher level. They record connections between relatively independent program subcomponents. When an object with extrinsic outlets is freed, those external objects remain in place.

---

## Aggregation and Decomposition

Another part of the design task is deciding when to add functionality to an existing class by defining a subclass and when to define an independent class.

It's generally better to try for reusable code and avoid having large classes that do so many things that they can't be adapted to other situations. When objects are designed as components, they become that much more reusable.

Dividing functionality between different classes doesn't necessarily complicate the programming interface. If one class keeps another object private, the private object's interface wouldn't have to be published for users of the outer class.

---

## Models and Kits

When you design an object-oriented program, you are, in effect, putting together a computer simulation of how something works. Object networks look and behave like models of real systems.

Designing an object-oriented program doesn't necessarily entail writing great amounts of code. The reusability of class definitions means that the opportunity is great for building a program largely out of classes devised by others.

Reusable classes come from many sources. Object-oriented programming environments typically come with class libraries. These constitute **software kits** that can be used to build a variety of different kinds of applications.

When you use a kit, you:

1. Initialize and arrange instances of kit classes
2. Define subclasses of kit classes
3. Define new classes of your own to work with classes defined in the kit

The kit sets up part of an object network for your program and provides part of its class hierarchy. Your own code completes the program model started by the kit.

---

## Structuring the Programming Task

Object-oriented programming not only structures programs in a new way, it also helps structure the programming task itself.

### Collaboration

Complex software requires an extraordinary collaborative effort among people who must be individually creative, yet still make what they do fit exactly with what others are doing.

The object-oriented approach offers ways of dealing with complexity in the organization of the work. The reusability of object-oriented code means that programmers can collaborate effectively even when they work on different projects at different times or are in different organizations, just by sharing their code in libraries.

### Organizing Projects

Object-oriented programming helps restructure the programming task in ways that benefit collaboration:

- **Large-Scale Design** — When programs are designed at a high level of abstraction, the division of labor is more easily conceived.
- **Separated Interfaces** — Since each class encapsulates its implementation and has its own name space, there's no need to coordinate implementation details.
- **Modularization** — Different people can work on different classes. Each implementation task is isolated from the others.
- **Simple Interfaces** — The polymorphism of object-oriented programs yields simpler programming interfaces.
- **Dynamic Decisions** — Less information needs to be supplied at compile time to make two pieces of code work together.
- **Inherited Code** — Inheritance is a way of reusing code, simplifying both the design and programming task.
- **Tested Code** — The more software you can borrow from others and incorporate in your own programs, the less you have to do yourself.

---

*Release 3.3 Copyright ©1995 NeXT Computer, Inc. All Rights Reserved.*
