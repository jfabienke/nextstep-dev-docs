# 3.3 Release Notes: Documentation

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

This file contains release notes for the 3.3, 3.2, 3.1, and 3.0 releases of the documentation for NEXTSTEP developers.

---

## Notes Specific to Release 3.3

These changes have been made in the documentation for this release.

### New Features

- The *Working With Interface Builder* manual has been added. It can be found in `/NextLibrary/Documentation/NextDev/DevTools/NewInterfaceBuilder` and is also available in printed form.
- Documentation has been added describing the Foundation Kit, a group of classes that provides a new primitive class `NSObject` and provides the functionality of the Common classes (List, Hash, Storage, and so on) as well as other features such as strings and collections. The Foundation Reference is available on-line in `/NextLibrary/Documentation/NextDev/Foundation`.

### Changes

- The documentation for the Driver Kit has been expanded and enhanced. It can be found in `/NextLibrary/Documentation/NextDev/OperatingSystem/Part3_DriverKit` and is indexed for access through Digital Librarian.
- *NEXTSTEP General Reference's* Introduction and Root Class chapters now include reference to Foundation. The DataBase Kit chapter references the Enterprise Objects Framework. The chapter on Video has been removed since this has been removed from NEXTSTEP.

---

## Notes Specific to Release 3.2

These changes have been made in the documentation for this release.

### New Features

The following new features have been added since Release 3.1.

- An on-line version of the *NEXTSTEP Database Kit* manual is in `/NextLibrary/Documentation/NextDev/NextDev/Concepts/DatabaseKit`. The printed version of this manual will be available soon from Addison-Wesley Publishing Company.
- An on-line version of *NEXTSTEP Object Oriented Programming and the Objective C Language* is in `/NextLibrary/Documentation/NextDev/NextDev/Concepts/ObjectiveC` and is now available in printed form from Addison Wesley Publishing Company.
- Header Viewer documentation can be found in `/NextLibrary/Documentation/NextDev/DevTools/ApA_HeaderViewer`.
- The Driver Kit documentation has been expanded and improved. It can be found in `/NextLibrary/Documentation/NextDev/OperatingSystem/Part3_DriverKit`.

---

## Notes Specific to Release 3.1

The documentation for Release 3.1 has been updated and improved in various ways. The most notable changes are listed here.

### New Features

The following new features have been added since Release 3.0.

- An on-line version of *NEXTSTEP Object Oriented Programming and the Objective C Language* is in `/NextLibrary/Documentation/NextDev/NextDev/Concepts/ObjectiveC`. The printed version of this manual will be available soon from Addison Wesley Publishing Company.
- The *NEXTSTEP Programming Interface Summary* manual is now available on line in `/NextLibrary/Documentation/NextDev/Summaries`. This manual was previously available only in printed form.
- Documentation for the new Device Driver Kit is available in `/NextLibrary/Documentation/NextDev/OperatingSystem/Part3_DriverKit`.
- A guide to making your applications portable is available in `/NextLibrary/Documentation/NextDev/Concepts/PortabilityGuide.rtf`. (Note to 3.1 pre-release users: the byte-swapping API for floating-point numbers has changed for the final version of Release 3.1.)
- Documentation for Novell® NetWare® is available in `/NextLibrary/Documentation/Novell`.

Because of the new on-line documents mentioned above, additional targets have been added to the NEXTSTEP developer bookshelf located in `/NextLibrary/Bookshelves`.

### Changes

- With the removal of the Phone Kit from the software release, the corresponding documentation has also been removed.
- *Quick RenderMan Interface and Implementation Specification*, has been moved from the release notes to `/NextLibrary/Documentation/Pixar/QRMSpec.rtfd`.
- In Release 3.1, several on-line manuals are divided into fewer, larger files than in Release 3.0.

---

## Notes Specific to Release 3.0

These notes were included with Release 3.0.

### New Features

There are two new Digital Librarian bookshelves located in `/NextLibrary/Bookshelves`: `NextDeveloper.bshlf` and `SysAdmin.bshlf`.

**NextDeveloper.bshlf:** In previous releases, all the NeXT developer documentation was indexed under one target; in Release 3.0 there are individually indexed targets for the various manuals in the NeXT Developer's Library. This change should make it easier for you to find what you're looking for, since you can narrow the search to just the manuals you're interested in. If your old default bookshelf contains the 2.0 NeXT Developer target you can continue to use it, but you may wish to remove it and start using the targets on the NextDeveloper bookshelf instead.

**SysAdmin.bshlf:** This bookshelf contains targets for the Networking and System Administration manual, the UNIX man pages, and administrative release notes.

### NeXT Developer's Library

**NEXTSTEP User Interface Guidelines**
How to best design the user interface for a NeXT application.

**NEXTSTEP General Reference**
Details about classes, functions, operators, and other programming elements, listed alphabetically within each category for easy reference.

**NEXTSTEP Development Tools**
Descriptions of the tools used in developing a NeXT application, including Project Builder, Interface Builder, Terminal, Edit, and the compiler and debugger.

**NEXTSTEP Operating System Software**
A description of Mach, the NeXT computer's operating system, and other low-level software; also, a discussion of how to write loadable kernel servers, such as device drivers and network protocols, for NeXT computers.

**NEXTSTEP Programming Interface**
Brief summaries of reference information related to NEXTSTEP.
