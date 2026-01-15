# NeXTSTEP 3.3 Developer Documentation

## Project Overview
This directory contains an archival copy of the **NeXTSTEP 3.3 Developer Documentation** (circa 1995). It serves as a historical reference for the operating system and frameworks that eventually evolved into macOS, iOS, and the Cocoa API.

The content covers the Objective-C language, the Foundation Kit, the Database Kit, Development Tools (like Interface Builder), and low-level Assembly programming for supported architectures (m68k, i386).

## Directory Structure

### `Assembler/`
Documentation regarding the system assembler and architecture-specific instructions.
- **Part1_General**: General assembly syntax and directives.
- **Part2_ArchSpecific**: Instruction sets for Motorola 68000 and Intel i386 processors.

### `Concepts/`
Deep-dive conceptual guides on various parts of the NeXTSTEP environment.
- **DatabaseKit**: Extensive documentation on the Enterprise Objects Framework ancestor.
- **ObjectiveC**: Core language documentation (OOP concepts, Runtime, Syntax).
- **Other**: Localization, Fonts, Performance tuning, etc.

### `DevTools/`
Manuals for the suite of tools provided with NeXTSTEP.
- **Interface Builder**: The precursor to modern Xcode's Interface Builder.
- **Project Builder**: The IDE (ancestor to Xcode).
- **Compiler/Debugger**: GNU C Compiler (GCC) and GDB usage in this environment.

### `Foundation/`
API reference and guides for the **Foundation Kit**, providing the base classes for NeXTSTEP applications (strings, arrays, values, etc.). This is the direct ancestor of the modern Apple `Foundation` framework.

### `GeneralReference/`
(Currently empty or sparse) Placeholder for additional reference materials.

## Usage Guidelines

### Navigation
*   **Primary Entry Point**: The `README.md` file acts as a Table of Contents.
*   **⚠️ Important Warning**: The internal links in `README.md` often use "slugs" (e.g., `01_OOP.md`) that **do not match** the actual filenames on disk (e.g., `01_Object_Oriented_Programming.md`).
*   **Recommended Action**: Do not rely solely on the paths provided in `README.md`. Use the **directory structure** as the source of truth. Use `glob` or `search_file_content` to locate specific topics if the README link fails.

### Context for Development
While this is historical documentation, it is highly relevant for:
1.  Understanding the origins of Cocoa/Objective-C patterns (Delegation, Target-Action, etc.).
2.  Working with legacy NeXTSTEP/OpenStep systems.
3.  Porting older software to modern macOS/Gnustep.

### File Formats
*   All documentation is in standard **Markdown**.
*   Files are text-heavy with some code examples (Objective-C, Assembly, C).
