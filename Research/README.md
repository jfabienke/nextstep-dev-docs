# Research

This directory contains historical research documents exploring the people, decisions, and context behind NeXTSTEP development.

## Documents

### MemoryManagementArchitects.md

**Question:** Who designed NeXT's retain/release/autorelease memory management system?

**Answer:** A collaborative effort led by:
- **Blaine Garst** — Primary architect of reference counting
- **Bertrand Serlet** — Leader of Foundation framework effort
- **Ali Ozer** — Co-author of Foundation classes
- **Trey Matteson** — Co-inventor on autorelease patent

Based on Computer History Museum oral histories and US Patent 7716450B2 (filed 1995).

---

### RichPage_NeXTdimension.md *(Comprehensive)*

**Question:** Who designed the NeXTdimension graphics accelerator board?

**Answer:**
- **Rich Page** — Primary hardware architect (VP Digital Hardware Engineering)
- **Mike Paquette** — Wrote the GaCK kernel, worked on Display PostScript

#### Rich Page's Contributions
- Selected the Intel i860 processor ("Cray on a chip")
- Advocated for 32-bit color with 8-bit alpha channel
- Designed the coprocessor architecture with dedicated memory
- Led the NRW (NeXT RISC Workstation) project — explored M88K and dual PPC 601 variants
- Admitted in NeXTWORLD interview that the optical disc was "probably" a mistake

#### Mike Paquette's Contributions

**Before NeXT (Pixar, ~1987):**
- Co-authored "Parallel Computers for Graphics Applications" (ACM 1987) with **Pat Hanrahan** (2019 Turing Award winner)
- Built **Chap** (Channel Processor) — 4-channel SIMD for RGBA compositing
- Built **Flap** (Floating point Array Processor) — for 3D graphics
- Developed **C4 language** — parallel C extension
- Work commercialized as the **Pixar Image Computer**

**At NeXT (~1988-1997):**
- Wrote **GaCK** (Graphics aCcelerator Kernel) — custom Mach-compatible OS for NeXTdimension i860
- Worked on **Display PostScript (DPS)**
- Developed **NEXTTIME** — QuickTime-compatible video player
- Worked on `moviola.next.com` server (named after 1924 film editing machine)
- When asked about video editing software (Jan 1997): "Not yet..." — editor may have been planned but never shipped

**GaCK Firmware Analysis (2024-2025):**
Modern reverse-engineering reveals GaCK is a **complete Mach-compatible multitasking OS** with 217 trap handlers, 73 lock operations, and 22 memory contexts. The firmware contains two OS layers: a Bootstrap Graphics HAL (32 KB) and the full GaCK kernel (160 KB) with Display PostScript interpreter. Paquette's statement "not Mach" is accurate — it's not the CMU codebase, but custom code implementing a Mach-compatible interface.

**At Apple (1997+):**
- Became **Quartz architect**, "spent a decade re-creating Display PostScript as Quartz"

#### The Hardware-Software Divide
The board never reached its potential due to the i860's programming difficulty — the software team couldn't port full Display PostScript to it. This "cultural rift" between Page's hardware team and Tevanian's software team explains why hardware attribution was harder to find than software attribution.

#### Sources
- Computer History Museum oral histories (CHM 102780988, 102740462)
- Game Engine Black Book: DOOM (Fabien Sanglard, 2018)
- "Parallel Computers for Graphics Applications" (ACM 1987)
- NeXTWORLD magazine interview with Rich Page
- comp.sys.next.* Usenet archives via nextcomputers.org
- **nextdimension firmware reverse-engineering** — 192 KB verified i860 code analyzed, revealing GaCK's true architecture

---

### NeXTdimensionArchitects.md *(Superseded)*

Initial research document before Rich Page oral history Part 2 was discovered. Kept for reference on research methodology. See RichPage_NeXTdimension.md for complete findings.

---

## Key People Identified

### NeXT Founding Team (1985)

| Person | Role | Key Contribution |
|--------|------|------------------|
| **Steve Jobs** | CEO | Company founder, vision |
| **Rich Page** | VP Digital Hardware Engineering | NeXTdimension architect, i860 selection, alpha channel |
| **George Crow** | VP Analog Hardware Engineering | Video I/O circuitry, power systems |
| **Bud Tribble** | VP Software Engineering | Early software leadership |
| **Dan'l Lewin** | VP Marketing | Sales and marketing strategy |
| **Susan Barnes** | CFO | Financial management |

### Engineers and Technical Staff

| Person | Role | Key Contribution |
|--------|------|------------------|
| **Avie Tevanian** | Director of System Software | Mach kernel, 15-person team, later Apple SVP |
| **Mike Paquette** | Engineer (Pixar → NeXT → Apple) | GaCK kernel, DPS, NEXTTIME, Quartz architect |
| **Blaine Garst** | Engineer | Reference counting, later ARC at Apple |
| **Bertrand Serlet** | Manager | Foundation framework leader, later Apple SVP |
| **Ali Ozer** | Engineer | Foundation classes co-author |
| **John Seamons** | Engineer | Ported Mach to NeXT hardware (with Demoney) |
| **Mike Demoney** | Engineer | Ported Mach to NeXT hardware (with Seamons) |
| **Keith Ohlfs** | Illustrator/Programmer | Created NeXT artwork with custom image software |
| **Pat Hanrahan** | Pixar (Paquette's colleague) | 2019 Turing Award, co-authored Chap/Flap paper |
| **Rick Rashid** | CMU (Tevanian's advisor) | Mach kernel creator, later Microsoft Research |

---

## Methodology

Research in this directory draws from:

### Primary Sources
- **Computer History Museum oral histories** — First-person accounts from NeXT engineers
- **Patent records** — Technical documentation with inventor attribution (US Patent 7716450B2)
- **Academic papers** — ACM publications with author attribution
- **Usenet archives** — comp.sys.next.* posts preserved at nextcomputers.org
- **Contemporary magazines** — NeXTWORLD interviews and articles
- **Firmware reverse-engineering** — i860 ROM disassembly revealing GaCK kernel architecture

### Secondary Sources
- **Technical histories** — Game Engine Black Book: DOOM (Fabien Sanglard)
- **Wikipedia and wikis** — NeXT Wiki, Apple Wiki
- **Forum discussions** — nextcomputers.org community research
- **Magazine archives** — NeXTWORLD 1991-1992 issues on Archive.org
- **Historical photographs** — NeXT founding team photo (1985)

### Research Approach
The goal is to connect the "what" (the documentation we're preserving) with the "who" and "why" (the people and decisions that created it). Hardware attribution proved harder to find than software attribution due to:
- Different documentation cultures (software engineers filed patents, published papers)
- The "cultural rift" between hardware and software teams at NeXT
- Hardware work seen as team effort under division leadership

---

## Timeline of Key Events

| Date | Event |
|------|-------|
| 1985 | Rich Page co-founds NeXT with Steve Jobs |
| 1986 | Steve Jobs buys Pixar from Lucasfilm |
| 1987 | Paquette co-authors Chap/Flap paper at Pixar |
| 1988 | Original NeXT Cube ships |
| 1989 | Intel releases i860 processor |
| 1990 Sept | NeXTdimension announced |
| 1991 April | NeXTdimension ships |
| 1992 | Rich Page leaves NeXT |
| 1993 | NeXT exits hardware business |
| 1995 | Autorelease patent filed (Garst, Serlet, Ozer, Matteson) |
| 1996 Dec 20 | Apple announces NeXT acquisition |
| 1997 | NeXT team joins Apple; Paquette begins Quartz work |
| 2001 | Mac OS X ships with Quartz (Paquette's architecture) |

---

*Last updated: January 2026*
