# NeXTSTEP 3.3 Developer Documentation

**Source:** [nextcomputers.org/files/manuals/nd/](https://www.nextcomputers.org/files/manuals/nd/)
**Last Updated:** 2023-02-06

---

## Contents

### [Assembler](Assembler/)

#### Part 1 — General
1. [Using the Assembler](Assembler/Part1_General/01_Using.md)
2. [Assembly Language Syntax](Assembler/Part1_General/02_Syntax.md)
3. [Assembly Language Statements](Assembler/Part1_General/03_Layout.md)
4. [Assembler Directives](Assembler/Part1_General/04_Directives.md)

#### Part 2 — Architecture Specific
- [M68000 Addressing Modes and Assembler Instructions](Assembler/Part2_ArchSpecific/M68k_Instructions.md)
- [i386 Addressing Modes and Assembler Instructions](Assembler/Part2_ArchSpecific/i386_Instructions.md)

---

### [Concepts](Concepts/)

#### DatabaseKit
- [Introduction](Concepts/DatabaseKit/Introduction.md)
1. [Overview](Concepts/DatabaseKit/01_Overview.md)
2. [Entity-Relationship Modeling](Concepts/DatabaseKit/02_ERModeling.md)
3. [Database Models](Concepts/DatabaseKit/03_Models.md) <!-- Missing -->
4. [Creating a Database Project](Concepts/DatabaseKit/04_Creating_Database_Project.md)
5. [Database Kit Classes and Protocols](Concepts/DatabaseKit/05_Database_Kit_Classes_And_Protocols.md)
6. [Model Emulation](Concepts/DatabaseKit/06_Model_Emulation.md)
7. [Connecting to the Server](Concepts/DatabaseKit/07_Connecting_To_The_Server.md)
8. [Data Storage, Retrieval, and Manipulation](Concepts/DatabaseKit/08_Data_Storage_Retrieval_And_Manipulation.md)
9. [Advanced Record List Techniques](Concepts/DatabaseKit/09_Advanced_Record_List_Techniques.md)
10. [Fetching and Saving Data](Concepts/DatabaseKit/10_Fetching_And_Saving_Data.md)
- [Glossary](Concepts/DatabaseKit/Glossary.md)
- [Appendix A — Adapter Default Parameters](Concepts/DatabaseKit/Appendix_A_Adaptor_Default_Parameters.md)

#### Other Concepts
- [Exception Handling](Concepts/ExceptionHandling.md) <!-- Missing -->
- [Preparing Fonts](Concepts/FontPreparation/01_Preparing_Fonts.md)
- [Preparing an Application for Installation by the Installer](DevTools/Installer/01_Preparing_Application_For_Installation.md)
- [Localization](Concepts/Localization/01_Localization.md)

#### Objective-C
- [Introduction](Concepts/ObjectiveC/Introduction.md)
1. [Object-Oriented Programming](Concepts/ObjectiveC/01_Object_Oriented_Programming.md)
2. [The Objective C Language](Concepts/ObjectiveC/02_The_Objective_C_Language.md)
3. [Objective C Extensions](Concepts/ObjectiveC/03_Objective_C_Extensions.md)
4. [The Run-Time System](Concepts/ObjectiveC/04_The_Run_Time_System.md)
5. [Programming in Objective C](Concepts/ObjectiveC/05_Programming_in_Objective_C.md)
- [Appendix A — Objective C Language Summary](Concepts/ObjectiveC/AppendixA_Summary.md)
- [Appendix B — Reference Manual for the Objective C Language](Concepts/ObjectiveC/AppendixB_Grammar.md)
- [Appendix C — The Object Class](Concepts/ObjectiveC/AppendixC_Object.md)

#### Performance
- [A — Measuring Performance and Memory Usage](Concepts/Performance/A_MeasuringPerformance.md)
- [B — Improving Launch Times](Concepts/Performance/B_ImprovingLaunchTimes.md)
- [C — Improving Drawing Performance](Concepts/Performance/C_ImprovingDrawingPerformance.md)
- [D — Improving Response Time](Concepts/Performance/D_ImprovingResponseTime.md)
- [E — Reducing Memory Usage](Concepts/Performance/E_ReducingMemoryUsage.md)
- [F — Zone Allocation](Concepts/Performance/F_ZoneAllocation.md)
- [G — Link Optimization](Concepts/Performance/G_LinkOptimization.md)
- [H — Application Performance Checklist](Concepts/Performance/H_ApplicationPerformanceChecklist.md)
- [I — Listings for the Timing Class](Concepts/Performance/I_TimingClassListings.md)

#### Mach
- [1. Concepts](Mach/01_Concepts.md)
- [2. Using Mach Messages](Mach/02_Messages.md)
- [3. Using Loadable Kernel Servers](Mach/03_KernelServers.md)
- [4. Mach Functions](Mach/04_Functions.md) <!-- Partial: C-Thread and some Mach kernel functions -->
- [5. Overview of Loadable Kernel Servers](Mach/05_Overview.md)
- [6. Designing Loadable Kernel Servers](Mach/06_Designing.md)
- [7. NeXTbus Device Drivers](Mach/07_NeXTbus.md)
- [8. Network Modules](Mach/08_Network.md)
- [9. Building, Loading, and Debugging Loadable Kernel Servers](Mach/09_Building.md)
- [10. Kernel Support Functions](Mach/10_KernelSupportFunctions.md) <!-- Partial: General Functions only -->
- [Appendix A — Utilities for Loadable Kernel Servers](Mach/AppendixA_Utilities.md)
- [Appendix B — The ROM Monitor and NMI Mini-Monitor](Mach/AppendixB_ROMMonitor.md)
- [Appendix C — Summary of Kernel Support Functions](Mach/AppendixC_FunctionSummary.md)

#### Portability
- [Building Portable NEXTSTEP Applications](Concepts/PortabilityGuide.md)

---

### [Pre-3.0 Concepts](Concepts/Pre3.0_Concepts/)

- [Important Information](Concepts/Pre3.0_Concepts/00_Intro.md)
1. [System Overview](Concepts/Pre3.0_Concepts/01_System_Overview.md)
2. [The NeXT User Interface](Concepts/Pre3.0_Concepts/02_User_Interface.md)
3. [Object-Oriented Programming and Objective C](Concepts/Pre3.0_Concepts/03_ObjC.md) <!-- Missing -->
4. [Drawing](Concepts/Pre3.0_Concepts/04_Drawing.md)
5. [Events](Concepts/Pre3.0_Concepts/05_Events.md)
6. [Program Structure](Concepts/Pre3.0_Concepts/06_ProgStructure.md)
7. [Program Dynamics](Concepts/Pre3.0_Concepts/07_ProgDynam.md)
8. [Interface Builder](Concepts/Pre3.0_Concepts/08_InterfaceBuilder.md)
9. [User-Interface Objects](Concepts/Pre3.0_Concepts/09_UIObjects.md)
10. [Support Objects and Functions](Concepts/Pre3.0_Concepts/10_Support.md)

---

### [Developer Tools](DevTools/)

- [Introduction](DevTools/00_Introduction.md)
1. [Putting Together a NEXTSTEP Application](DevTools/01_PuttingTogether.md)
2. [The Project Builder Application](DevTools/02_ProjectBuilder.md)
3. [The Interface Builder Application](DevTools/03_InterfaceBuilder.md)

#### New Interface Builder
1. [Composing](DevTools/NewInterfaceBuilder/01_Composing.md)
2. [Setting Object Attributes](DevTools/NewInterfaceBuilder/02_ObjAttributes.md)
3. [Making and Managing Connections](DevTools/NewInterfaceBuilder/03_Connections.md)
4. [Creating a Class](DevTools/NewInterfaceBuilder/04_CreatingClass.md)
5. [Using Dynamic Palettes](DevTools/NewInterfaceBuilder/05_DynPalettes.md)
6. [Working With Multiple Nib Files](DevTools/NewInterfaceBuilder/06_MultipleNibs.md)

#### Applications
- [The Edit Application](DevTools/04_Edit.md)
- [The Terminal Application](DevTools/05_Terminal.md)
- [The Icon Builder Application](DevTools/06_IconBuilder.md)
- [The DBModeler Application](DevTools/07_DBModeler.md)
- [The MallocDebug Application](DevTools/08_MallocDebug.md)
- [The Process Monitor Application](DevTools/09_ProcessMonitor.md)
- [The PostScript Previewers: Yap and pft](DevTools/10_YAP.md)

#### Compiler & Debugger
- [The GNU C Compiler](DevTools/11_Compiler.md)
- [The GNU C Preprocessor](DevTools/12_Preprocessor.md)
- [The GNU Source-Level Debugger](DevTools/13_Debugger.md)
- [Mach Object Files](DevTools/14_MachO.md)

#### Tutorials
- [Building a Simple Application](DevTools/15_SimpleApp.md)
- [Building a One-Button Calculator](DevTools/16_CalculatorApp.md)
- [Building a Text Editor Using Multiple Nib Files](DevTools/17_TextApp.md)
- [Building a Custom Palette](DevTools/18_CustomPalette.md)

#### Appendices
- [Appendix A — The Header Viewer Application](DevTools/AppendixA_HeaderViewer.md)

---

### [Driver Kit](DriverKit/)

- [Introduction (Preface)](DriverKit/Introduction.md)
1. [Driver Kit Architecture](DriverKit/01_Architecture.md)
2. [Designing a Driver](DriverKit/02_Designing.md)
3. [Support for Specific Devices](DriverKit/03_SpecificDevices.md)
4. [Building, Configuring, and Debugging Drivers](DriverKit/04_BuildingConfiguring.md)
5. [Driver Kit Reference](DriverKit/05_Reference.md)
   - [Classes](DriverKit/05_Reference/Classes.md)
   - [Functions](DriverKit/05_Reference/Functions.md)
   - [Protocols](DriverKit/05_Reference/Protocols.md)
   - [Types and Constants](DriverKit/05_Reference/Types.md)
   - [More Types and Constants](DriverKit/05_Reference/MoreTypesAndConstants.md)
- [Appendix — Suggested Reading](DriverKit/Appendix_SuggestedReading.md) <!-- Missing -->

---

### [Foundation](Foundation/)

- [Introduction](Foundation/Introduction.md) <!-- Missing -->
1. [The Foundation Kit](Foundation/01_FoundationKit.md)

#### Classes
- [NSArchiver](Foundation/Classes/NSArchiver.md)
- [NSArray Class Cluster](Foundation/Classes/NSArrayClassCluster.md)
- [NSAssertionHandler](Foundation/Classes/NSAssertionHandler.md)
- [NSAutoreleasePool](Foundation/Classes/NSAutoreleasePool.md)
- [NSBundle](Foundation/Classes/NSBundle.md)
- [NSCalendarDate](Foundation/Classes/NSCalendarDate.md) <!-- Missing -->
- [NSCharacterSet Class Cluster](Foundation/Classes/NSCharacterSetClassCluster.md) <!-- Missing -->
- [NSCoder](Foundation/Classes/NSCoder.md) <!-- Missing -->
- [NSData Class Cluster](Foundation/Classes/NSDataClassCluster.md) <!-- Missing -->
- [NSDate Class Cluster](Foundation/Classes/NSDateClassCluster.md) <!-- Missing -->
- [NSDictionary Class Cluster](Foundation/Classes/NSDictionaryClassCluster.md) <!-- Missing -->
- [NSEnumerator](Foundation/Classes/NSEnumerator.md) <!-- Missing -->
- [NSException](Foundation/Classes/NSException.md) <!-- Missing -->
- [NSNotificationCenter](Foundation/Classes/NSNotificationCenter.md) <!-- Missing -->
- [NSNotification Class Cluster](Foundation/Classes/NSNotificationClassCluster.md) <!-- Missing -->
- [NSNumber](Foundation/Classes/NSNumber.md) <!-- Missing -->
- [NSObject](Foundation/Classes/NSObject.md) <!-- Missing -->
- [NSScanner](Foundation/Classes/NSScanner.md) <!-- Missing -->
- [NSString Class Cluster](Foundation/Classes/NSStringClassCluster.md) <!-- Missing -->
- [NSTimeZone Class Cluster](Foundation/Classes/NSTimeZoneClassCluster.md) <!-- Missing -->
- [NSUnarchiver](Foundation/Classes/NSUnarchiver.md) <!-- Missing -->
- [NSValue](Foundation/Classes/NSValue.md) <!-- Missing -->

#### Functions
- [NSAllocateObject / NSDeallocateObject](Foundation/Functions/FoundationFunctions.md) <!-- Missing -->
- [Living in a Hybrid World](Foundation/HybridWorld.md) <!-- Missing -->

#### Protocols
- [NSCoding](Foundation/Protocols/NSCoding.md) <!-- Missing -->
- [NSCopying](Foundation/Protocols/NSCopying.md) <!-- Missing -->
- [NSMutableCopying](Foundation/Protocols/NSMutableCopying.md) <!-- Missing -->
- [NSObject Protocol](Foundation/Protocols/NSObject.md) <!-- Missing -->

#### Types and Constants
- [Defined Types](Foundation/TypesAndConstants/FoundationTypesAndConstants.md) <!-- Missing -->

---

### [General Reference](GeneralReference/)

- [Configuration Keys](GeneralReference/ConfigurationKeys.md)

---

### [Release Notes](ReleaseNotes/)

- [Contents](ReleaseNotes/00_Contents.md)
- [Developer Preamble](ReleaseNotes/DeveloperPreamble.md)
- [ANSI C Libraries](ReleaseNotes/ANSICLibraries.md)
- [Application Kit](ReleaseNotes/ApplicationKit.md)
- [C Compiler](ReleaseNotes/CCompiler.md)
- [C Library Support for Localization](ReleaseNotes/CLibraryLocalization.md)
- [Compiler Tools](ReleaseNotes/CompilerTools.md)
- [Database Kit](ReleaseNotes/DatabaseKit.md)
- [Debugger (GDB)](ReleaseNotes/Debugger.md)
- [Distributed Objects](ReleaseNotes/DistributedObjects.md)
- [Documentation](ReleaseNotes/Documentation.md)
- [DSP](ReleaseNotes/DSP.md)
- [Edit](ReleaseNotes/Edit.md)
- [Emacs and vi](ReleaseNotes/EmacsAndVi.md)
- [Event Status Driver](ReleaseNotes/EventStatusDriver.md)
- [Fat Files (Multiple Architecture Binaries)](ReleaseNotes/FatFiles.md)
- [Fax](ReleaseNotes/Fax.md)
- [GNU Source Code](ReleaseNotes/GNUSource.md)
- [Header Files](ReleaseNotes/HeaderFiles.md)
- [Installer](ReleaseNotes/Installer.md)
- [Interface Builder](ReleaseNotes/InterfaceBuilder.md)
- [libg++](ReleaseNotes/Libgxx.md)
- [Loadable Kernel Servers](ReleaseNotes/LoadableKernelServers.md)
- [Malloc Debug](ReleaseNotes/MallocDebug.md)
- [MIDI Driver](ReleaseNotes/MIDIDriver.md)
- [Music Kit](ReleaseNotes/MusicKit.md)
- [NetInfo Kit](ReleaseNotes/NetInfoKit.md)
- [Objective C Run-Time System](ReleaseNotes/ObjectiveCRuntime.md)
- [Objective C++](ReleaseNotes/ObjectiveCPlusPlus.md)
- [Object Links](ReleaseNotes/ObjectLinks.md)
- [Operating System](ReleaseNotes/OperatingSystem.md)
- [Preferences](ReleaseNotes/Preferences.md)
- [Preprocessor](ReleaseNotes/Preprocessor.md)
- [Project Builder](ReleaseNotes/ProjectBuilder.md)
- [Sound Kit](ReleaseNotes/SoundKit.md)
- [Sound Library](ReleaseNotes/SoundLibrary.md)
- [Tablet Driver](ReleaseNotes/TabletDriver.md)
- [Window Server](ReleaseNotes/WindowServer.md)
- [Workspace Manager](ReleaseNotes/WorkspaceManager.md)
- [3D Graphics Kit](ReleaseNotes/3DGraphicsKit.md)
