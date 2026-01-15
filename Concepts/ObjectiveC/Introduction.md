# Introduction

Object-oriented programming, like most interesting new developments, builds on some old ideas, extends them, and puts them together in novel ways. The result is a clear step forward for the art of programming. An object-oriented approach makes programs more intuitive to design, faster to develop, more amenable to modification, and easier to understand. It leads not only to new ways of constructing programs, but also to new ways of conceiving the programming task.

Nevertheless, object-oriented programming presents some formidable obstacles to those who would like to understand what it's all about or begin trying it out. It introduces a new way of doing things that may seem strange at first, and it comes with an extensive terminology that can take some getting used to. The terminology will help in the end, but it's not always easy to learn. Moreover, there are as yet few full-fledged object-oriented development environments available to try out. It can be difficult to get started.

That's where this book comes in. It's designed to help you become familiar with object-oriented programming and get over the hurdle its terminology presents. It spells out some of the implications of object-oriented design and tries to give you a flavor of what writing an object-oriented program is really like. It fully documents the Objective-C language, an object-oriented programming language based on standard C, and introduces the most extensive object-oriented development environment currently available—NEXTSTEP™.

The book is intended for readers who might be interested in:

- Learning about object-oriented programming
- Finding out about the NEXTSTEP development environment
- Programming in Objective-C

NeXT supplies its own compiler for the Objective-C language (a modification of the GNU C compiler) and a run-time system to carry out the dynamic functions of the language. It has tested and made steady improvements to both over the years; this book describes the latest release (Release 3), which includes provisions for declaring and adopting protocols and setting the scope of instance variables.

Throughout this manual and in other NeXT documentation, the term "Objective-C" refers to the language as implemented for the NEXTSTEP development environment and presented here.

---

## The Development Environment

Every object-oriented development environment worthy of the name consists of at least three parts:

1. **A library of objects and software kits**
2. **A set of development tools**
3. **An object-oriented programming language**

### Software Kits

NEXTSTEP comes with an extensive library. It includes several software kits containing definitions for objects that you can use "off the shelf" or adapt to your program's needs. The kits include:

- **Application Kit™** — for building a graphical user interface
- **Database Kit™** — for interacting with a database server
- **3D Graphics Kit™** — for constructing and manipulating three-dimensional images
- **Sound Kit™** — for recording, editing, and playing sounds
- **Other kits** — documented in the NEXTSTEP General Reference

All these kits are thoroughly documented in the NEXTSTEP General Reference.

### Development Tools

NEXTSTEP also includes some exceptional development tools for putting together applications:

- **Interface Builder™** — a program that lets you design an application graphically and assemble its user interface on-screen
- **Project Builder** — a project-management program that provides graphical access to the compiler, the debugger, documentation, a program editor, and other tools

These programs are documented in NEXTSTEP Development Tools and Techniques.

### The Programming Language

This book is about the third component of the development environment—the programming language. All NEXTSTEP software kits are written in the Objective-C language. To get the benefit of the kits, applications must also use Objective-C.

Objective-C is implemented as a set of extensions to the C language. It's designed to give C a full capability for object-oriented programming, and to do so in a simple and straightforward way. Its additions to C are few and are mostly based on Smalltalk, one of the first object-oriented programming languages.

This book both introduces the object-oriented model that Objective-C is based upon and fully documents the language. It concentrates on the Objective-C extensions to C, not on the C language itself. There are many good books available on C; this manual doesn't attempt to duplicate them.

Because this isn't a book about C, it assumes some prior acquaintance with that language. However, it doesn't have to be an extensive acquaintance. Object-oriented programming in Objective-C is sufficiently different from procedural programming in standard C that you won't be hampered if you're not an experienced C programmer.

---

## Why Objective-C

The Objective-C language was chosen for the NEXTSTEP development environment for a variety of reasons.

### Object-Oriented Design

First and foremost, it's an object-oriented language. The kind of functionality that's packaged in the NEXTSTEP software kits can only be delivered through object-oriented techniques. This manual will explain how the kits work and why this is the case.

### Compatibility with C

Second, because Objective-C is an extension of standard ANSI C, existing C programs can be adapted to use the software kits without losing any of the work that went into their original development. Since Objective-C incorporates C, you get all the benefits of C when working within Objective-C. You can choose when to do something in an object-oriented way (define a new class, for example) and when to stick to procedural programming techniques (define a structure and some functions instead of a class).

### Simplicity

Moreover, Objective-C is a simple language. Its syntax is small, unambiguous, and easy to learn. Object-oriented programming, with its self-conscious terminology and emphasis on abstract design, often presents a steep learning curve to new recruits. A well-organized language like Objective-C can make becoming a proficient object-oriented programmer that much less difficult. The size of this manual is a testament to the simplicity of Objective-C. It's not a big book—and Objective-C is fully documented in just two of its chapters.

### Dynamic Binding and Flexibility

Objective-C is the most dynamic of the object-oriented languages based on C. The compiler throws very little away, so a great deal of information is preserved for use at run time. Decisions that otherwise might be made at compile time can be postponed until the program is running. This gives Objective-C programs unusual flexibility and power.

Objective-C's dynamism yields two big benefits that are hard to get with other nominally object-oriented languages:

1. **Dynamic Binding Support** — Objective-C supports an open style of dynamic binding, a style that can accommodate a simple architecture for interactive user interfaces. Messages are not necessarily constrained by either the class of the receiver or the method selector, so a software kit can allow for user choices at run time and permit developers freedom of expression in their design. (Terminology like "dynamic binding," "message," "class," "receiver," and "selector" will be explained in due course in this manual.)

2. **Advanced Development Tools** — Objective-C's dynamism enables the construction of sophisticated development tools. An interface to the run-time system provides access to information about running applications, so it's possible to develop tools that monitor, intervene, and reveal the underlying structure and activity of Objective-C applications. Interface Builder could not have been developed with a less dynamic language. (The full interface to the run-time system is documented in the NEXTSTEP General Reference manual.)

---

## How the Manual is Organized

This manual is divided into five chapters and three appendices. The chapters are:

### Chapter 1: Object-Oriented Programming

Chapter 1, "Object-Oriented Programming," discusses the rationale for object-oriented programming languages and introduces much of the terminology. It develops the ideas behind object-oriented programming techniques. If you're already familiar with object-oriented programming and are interested only in Objective-C, you may want to skip this chapter and go directly to Chapter 2.

### Chapter 2: The Objective-C Language

Chapter 2, "The Objective-C Language," describes the basic concepts and syntax of Objective-C. It covers many of the same topics as Chapter 1, but looks at them from the standpoint of the Objective-C language. It reintroduces the terminology of object-oriented programming, but in the context of Objective-C.

### Chapter 3: Objective-C Extensions

Chapter 3, "Objective-C Extensions," concentrates on two of the principal innovations introduced into the language as part of NEXTSTEP Objective-C—categories and protocols. It also takes up static typing and lesser used aspects of the language.

### Chapter 4: The Run-Time System

Chapter 4, "The Run-Time System," looks at the Object class and how Objective-C programs interact with the run-time system. In particular, it examines the paradigms for allocating and initializing new objects, dynamically loading new classes at run time, archiving objects, and forwarding messages to other objects.

### Chapter 5: Programming in Objective-C

Chapter 5, "Programming in Objective-C," tries to give a flavor of what programming with the NEXTSTEP software kits is like. As an example, it describes how you would go about programming with the Application Kit, the software kit that's used to build and run a graphical user interface.

### Appendices

The three appendices contain reference material that might be useful for understanding the language:

**Appendix A, "Objective-C Language Summary,"** lists and briefly comments on all the Objective-C extensions to the C language.

**Appendix B, "Reference Manual for the Objective-C Language,"** presents, uncommented, a formal grammar of the Objective-C extensions to the C language. This reference manual is meant to be read as a companion to the reference manual for C presented in The C Programming Language by Brian W. Kernighan and Dennis M. Ritchie, published by Prentice Hall.

**Appendix C, "The Object Class,"** is a full specification of the Object class, the root class that all other Objective-C classes inherit from. This specification is equivalent to the one found in the NEXTSTEP General Reference manual and is presented here for convenience.

---

## Conventions

Where this manual discusses functions, methods, and other programming elements, it makes special use of bold and italic fonts.

- **Bold** denotes words or characters that are to be taken literally (typed as they appear)
- *Italic* denotes words that represent something else or can be varied

For example, the syntax:

```
@interface ClassName ( CategoryName )
```

means that `@interface` and the two parentheses are required, but that you can choose the class name and category name.

Where method syntax is shown, mainly in Appendix C, the method name is bold, parameters are italic, and other elements (mainly data types) are in regular font. For example:

```
write:(NXTypedStream *)stream
```

Where example code is shown, ellipsis indicates the parts, often substantial parts, that have been omitted:

```objc
- write:(NXTypedStream *)stream
{
    [super write:stream];
    . . .
    return self;
}
```

The conventions used in the reference manual are described there (Appendix B).

---

*Release 3.3 Copyright ©1995 NeXT Computer, Inc. All Rights Reserved.*
