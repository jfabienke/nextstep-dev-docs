# Preface

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

---

## Introduction

Some of the most formidable operating system software to understand—and to write—is the I/O subsystem. Device drivers are essential components of the I/O subsystem. They control the peripherals required for a multipurpose, flexible computer system.

In some systems, device drivers are closely entwined with the operating system, requiring that you have an extensive knowledge of the implementation of the operating system to write a driver.

Writing a driver for the NEXTSTEP system doesn't demand such difficult prerequisites. You can write a NEXTSTEP device driver in a very modular fashion, without knowing a great deal about NEXTSTEP. NeXT has packaged together the software and tools you need to write in a driver into the Driver Kit, a part of the NEXTSTEP Developer software. Writing a device driver using the Driver Kit is more like writing an application using the NEXTSTEP Application Kit than like writing a driver for other operating systems.

The Driver Kit provides a framework to help you create device drivers for computers running NEXTSTEP. Although every driver is unique, drivers do have common elements. The Driver Kit generalizes the software required for a driver, removing the hardware-specific details. To create a driver, you essentially fill in the hardware-dependent "blanks" in the Driver Kit software with code that performs the desired operations on your hardware.

By using the structure that the Driver Kit offers, you can greatly reduce the time and effort required to write a driver. The conceptual model of a Driver Kit driver is simpler than that of a driver on other systems. This design simplifies writing a driver and eliminates many of the problems that make debugging drivers difficult.

This document is part 3 of NEXTSTEP Operating System Software. Chapter 1, "Driver Kit Architecture," introduces you to the structure of the Driver Kit. You learn about designing a Driver Kit driver in Chapter 2, "Designing a Driver." Chapter 3, "Support for Specific Devices," acquaints you with some of the details needed to write specific types of drivers such as network drivers. The fourth, and last, concepts chapter, "Building, Configuring, and Debugging Drivers," describes these topics. Chapter 5, "Driver Kit Reference," discusses the classes and other associated tools provided by the Driver Kit.

**Note:** The Driver Kit is supported on all NEXTSTEP platforms except 680x0-based computers.

---

## Before You Read This Document

This document covers only the parts of driver writing that are specific to the Driver Kit. To understand this document, however, you need to be familiar with several topics that aren't covered here. Some of these topics are discussed in other NeXT documentation.

### NeXT Documentation to Read

You need to know the Objective C language, since the Driver Kit is written in this language. Objective C provides a set of simple, object-oriented extensions to ANSI C.

NEXTSTEP systems use the Mach operating system. Writing most drivers requires that you understand such Mach concepts as tasks and threads, and writing many requires familiarity with Mach ports and Mach messages. The Mach Kit contains useful tools such as facilities for locks. Driver Kit drivers are a part of the Mach kernel and are known as loadable kernel servers, so you must be familiar with this concept as well. Access to most of the Mach facilities you need is included with the Driver Kit in its set of Mach functions.

The following table shows where you can learn about these topics:

| Topic | Where to Read about It |
|-------|------------------------|
| Objective C language | Chapters 1, 2, and 3, NEXTSTEP Object-Oriented Programming and the Objective C Language |
| Mach operating system | Chapter 1, NEXTSTEP Operating System Software (read the introduction, "Design Philosophy," and "The Mach Kernel") |
| Mach Kit | Chapter 9, NEXTSTEP General Reference |
| Loadable kernel servers | Chapter 5, NEXTSTEP Operating System Software |

You can get updates to NeXT documentation on archive servers through the NeXTanswers program. Send e-mail to `nextanswers@next.com` with the two-word subject: `INDEX HELP`. Or if you can't receive NeXT mail, add a third word, `ASCII`. You'll receive the current index of documents and instructions for requesting more information.

### Other Reading

It's helpful if you know how to write a device driver on some system other than NEXTSTEP. If you haven't written a driver before, see "Suggested Reading" in the Appendix for a list of books that can help you learn about drivers. If you've never written a driver for a multitasking operating system, you should familiarize yourself with the issues involved. The "Suggested Reading" section also lists books that deal with these issues.

Finally, you should be very familiar with the hardware your driver will control. Besides your device's documentation, you'll also need specifications for the bus your device attaches to. Some sources of bus documentation are listed in "Suggested Reading" in the Appendix.

---

**Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.**
