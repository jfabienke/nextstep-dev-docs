# Appendix — Suggested Readings on Writing Device Drivers

**Release 3.3**
**Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.**

---

These references provide useful information for developers writing device drivers for NeXTSTEP. The list includes NeXT documentation, books on operating systems and hardware architectures, and other technical references.

## NeXT Documentation

### General Reference
- *NeXTSTEP General Reference*
- *NeXTSTEP Development Tools and Techniques*

### Operating System Software
- *Mach System* (available in `/NextLibrary/Documentation/NextDev/OperatingSystem/`)
  - Introduction to Mach concepts and services
  - Memory management, task management, and IPC
  - Device driver interface

### Driver Kit Documentation
- *Driver Kit Introduction* — Overview of the Driver Kit architecture
- *Driver Kit Architecture* — Detailed architecture description
- *Driver Kit Class Reference* — API documentation for all Driver Kit classes

## Mach Operating System

### Books
- **Mach 3 Kernel Principles**  
  *R. Baron, D. Black, W. Bolosky, J. Chew, R. Golub, D. Tevanian*  
  Available from OSF (Open Software Foundation)

- **The Mach System**  
  *Accetta, M., Baron, R., Bolosky, W., Golub, D., Rashid, R., Tevanian, A., Young, M.*  
  Published in: Communications of the ACM, December 1986

- **Programming Under Mach**  
  *Boykin, J., Kirschen, D., Langerman, A., LoVerso, S.*  
  Addison-Wesley, 1993  
  ISBN: 0-201-52739-1

## General Driver Writing

### Books
- **Writing Device Drivers**  
  Sun Microsystems  
  SunSoft Press, 1994

- **The Undocumented PC**  
  *Frank van Gilluwe*  
  Addison-Wesley, 1994  
  ISBN: 0-201-62277-7  
  Hardware reference for PC architecture

- **PC System Programming**  
  *Michael Tischer and Bruno Jennrich*  
  Abacus, 1995  
  Detailed information on PC hardware and BIOS programming

## Bus Architectures

### EISA (Extended Industry Standard Architecture)
- **EISA System Architecture**  
  *MindShare, Inc.*  
  Addison-Wesley, 1995  
  ISBN: 0-201-40995-X

### ISA (Industry Standard Architecture)
- **ISA System Architecture**  
  *MindShare, Inc.*  
  Addison-Wesley, 1995  
  ISBN: 0-201-40996-8

### PCI (Peripheral Component Interconnect)
- **PCI System Architecture**  
  *MindShare, Inc.*  
  Addison-Wesley, 1995  
  ISBN: 0-201-40993-3

- **PCI Local Bus Specification**  
  Revision 2.1  
  PCI Special Interest Group, 1995  
  Available from: PCI SIG, P.O. Box 14070, Portland, OR 97214

### PCMCIA (PC Card)
- **PCMCIA System Architecture**  
  *MindShare, Inc.*  
  Addison-Wesley, 1995  
  ISBN: 0-201-40991-7

- **PC Card Standard**  
  Release 2.1  
  Personal Computer Memory Card International Association (PCMCIA), 1993  
  Available from: PCMCIA, 2635 North First Street, Suite 209, San Jose, CA 95134

## Display Drivers

### References
- **Video Demystified: A Handbook for the Digital Engineer**  
  *Keith Jack*  
  HighText Publications, 1993  
  ISBN: 1-878707-09-4

- **Programmer's Guide to the EGA and VGA Cards**  
  *Richard F. Ferraro*  
  Addison-Wesley, 1994  
  ISBN: 0-201-62490-7

- **VESA BIOS Extension (VBE) Core Functions**  
  Version 2.0  
  Video Electronics Standards Association (VESA), 1994

## Network Drivers

### References
- **TCP/IP Illustrated, Volume 1: The Protocols**  
  *W. Richard Stevens*  
  Addison-Wesley, 1994  
  ISBN: 0-201-63346-9

- **UNIX Network Programming**  
  *W. Richard Stevens*  
  Prentice Hall, 1990  
  ISBN: 0-13-949876-1

- **Internetworking with TCP/IP, Volume 1: Principles, Protocols, and Architecture**  
  *Douglas E. Comer*  
  Prentice Hall, 1995  
  ISBN: 0-13-216987-8

## Hardware Specifications

### Obtaining Hardware Specifications

Many hardware manufacturers provide detailed technical specifications for their devices. When writing drivers, contact the hardware vendor directly for:

- **Data sheets** — Electrical and timing specifications
- **Programming guides** — Register descriptions and command sequences
- **Application notes** — Implementation examples and errata
- **Sample code** — Reference driver implementations

### Industry Standards Organizations

- **IEEE** (Institute of Electrical and Electronics Engineers)  
  445 Hoes Lane, Piscataway, NJ 08855-1331  
  Standards for Ethernet, SCSI, and other interfaces

- **ANSI** (American National Standards Institute)  
  11 West 42nd Street, New York, NY 10036  
  National standards coordination

- **ISO** (International Organization for Standardization)  
  International standards including networking protocols

---

## Additional Resources

### Online Documentation

NeXTSTEP provides extensive online documentation in the following locations:

```
/NextLibrary/Documentation/NextDev/
    ├── Concepts/         # Conceptual documentation
    ├── OperatingSystem/  # Mach and UNIX system documentation
    ├── DevTools/         # Development tools documentation
    └── GeneralRef/       # API reference documentation
```

Use **Digital Librarian** to search and browse this documentation interactively.

### NeXT Technical Support

For questions specific to NeXTSTEP driver development:

- **NeXT Developer Support**  
  Phone: 1-800-TRY-NEXT  
  Email: devsupport@next.com

- **NeXT FTP Site**  
  ftp://ftp.next.com/pub/  
  Sample code, technical notes, and updates

---

**Note:** ISBN numbers, addresses, and contact information are from the 1995 timeframe and may no longer be current. This list is preserved for historical reference.
