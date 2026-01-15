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
3. [Database Models](Concepts/DatabaseKit/03_Models.md)
4. [Creating a Database Project](Concepts/DatabaseKit/04_Project.md)
5. [Database Kit Classes and Protocols](Concepts/DatabaseKit/05_Database_Kit_Classes_And_Protocols.md)
6. [Model Emulation](Concepts/DatabaseKit/06_ModelEmulation.md)
7. [Connecting to the Server](Concepts/DatabaseKit/07_Connecting.md)
8. [Data Storage, Retrieval, and Manipulation](Concepts/DatabaseKit/08_DataStorage.md)
9. [Advanced Record List Techniques](Concepts/DatabaseKit/09_AdvancedRecordList.md)
10. [Fetching and Saving Data](Concepts/DatabaseKit/10_AdvancedFetching.md)
- [Glossary](Concepts/DatabaseKit/Glossary.md)
- [Appendix A — Adapter Default Parameters](Concepts/DatabaseKit/Appendix_A_Adaptor_Default_Parameters.md)

#### Other Concepts
- [Exception Handling](Concepts/ExceptionHandling.md)
- [Preparing Fonts](Concepts/Fonts.md)
- [Preparing an Application for Installation by the Installer](Concepts/Installer.md)
- [Localization](Concepts/Localization.md)

#### Objective-C
- [Introduction](Concepts/ObjectiveC/00_Introduction.md)
1. [Object-Oriented Programming](Concepts/ObjectiveC/01_OOP.md)
2. [The Objective C Language](Concepts/ObjectiveC/02_CoreObjC.md)
3. [Objective C Extensions](Concepts/ObjectiveC/03_MoreObjC.md)
4. [The Run-Time System](Concepts/ObjectiveC/04_RunTime.md)
5. [Programming in Objective C](Concepts/ObjectiveC/05_Programming.md)
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
- [H — Application Performance Checklist](Concepts/Performance/H_Checklist.md)
- [I — Listings for the Timing Class](Concepts/Performance/I_TimingClassListings.md)

#### Portability
- [Building Portable NEXTSTEP Applications](Concepts/PortabilityGuide.md)

---

### [Pre-3.0 Concepts](Concepts/Pre3.0_Concepts/)

- [Important Information](Concepts/Pre3.0_Concepts/00_Intro.md)
1. [System Overview](Concepts/Pre3.0_Concepts/01_SysOver.md)
2. [The NeXT User Interface](Concepts/Pre3.0_Concepts/02_UserIntf.md)
3. [Object-Oriented Programming and Objective C](Concepts/Pre3.0_Concepts/03_ObjC.md)
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

### [Foundation](Foundation/)

- [Introduction](Foundation/Introduction.md)

#### Classes
- [NSArchiver](Foundation/Classes/NSArchiver.md)
- [NSArray Class Cluster](Foundation/Classes/NSArrayClassCluster.md)
- [NSAssertionHandler](Foundation/Classes/NSAssertionHandler.md)
- [NSAutoreleasePool](Foundation/Classes/NSAutoreleasePool.md)
- [NSBundle](Foundation/Classes/NSBundle.md)
- [NSCalendarDate](Foundation/Classes/NSCalendarDate.md)
- [NSCharacterSet Class Cluster](Foundation/Classes/NSCharacterSetClassCluster.md)
- [NSCoder](Foundation/Classes/NSCoder.md)
- [NSData Class Cluster](Foundation/Classes/NSDataClassCluster.md)
- [NSDate Class Cluster](Foundation/Classes/NSDateClassCluster.md)
- [NSDictionary Class Cluster](Foundation/Classes/NSDictionaryClassCluster.md)
- [NSEnumerator](Foundation/Classes/NSEnumerator.md)
- [NSException](Foundation/Classes/NSException.md)
- [NSNotificationCenter](Foundation/Classes/NSNotificationCenter.md)
- [NSNotification Class Cluster](Foundation/Classes/NSNotificationClassCluster.md)
- [NSNumber](Foundation/Classes/NSNumber.md)
- [NSObject](Foundation/Classes/NSObject.md)
- [NSScanner](Foundation/Classes/NSScanner.md)
- [NSString Class Cluster](Foundation/Classes/NSStringClassCluster.md)
- [NSTimeZone Class Cluster](Foundation/Classes/NSTimeZoneClassCluster.md)
- [NSUnarchiver](Foundation/Classes/NSUnarchiver.md)
- [NSValue](Foundation/Classes/NSValue.md)

#### Functions
- [NSAllocateObject / NSDeallocateObject](Foundation/Functions/FoundationFunctions.md)
- [Living in a Hybrid World](Foundation/HybridWorld.md)

#### Protocols
- [NSCoding](Foundation/Protocols/NSCoding.md)
- [NSCopying](Foundation/Protocols/NSCopying.md)
- [NSMutableCopying](Foundation/Protocols/NSMutableCopying.md)
- [NSObject Protocol](Foundation/Protocols/NSObject.md)

#### Types and Constants
- [Defined Types](Foundation/TypesAndConstants/FoundationTypesAndConstants.md)

---

### [General Reference](GeneralReference/)

*Additional reference materials to be added.*
