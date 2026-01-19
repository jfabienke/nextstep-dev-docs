# Who Designed the NeXTdimension Board?

**Research Date:** January 2026
**Status:** ✅ **SUPERSEDED** — See [RichPage_NeXTdimension.md](RichPage_NeXTdimension.md) for complete findings

---

## Summary

> **This document is historical.** It represents initial research before discovering Rich Page's CHM oral history Part 2 and Mike Paquette's GaCK attribution. Kept for reference on research methodology.
>
> **Definitive Answer:**
> - **Rich Page** — Primary hardware architect (i860 selection, alpha channel, coprocessor design)
> - **Mike Paquette** — Wrote GaCK (Graphics aCcelerator Kernel), the custom OS for the i860
>
> See [RichPage_NeXTdimension.md](RichPage_NeXTdimension.md) for comprehensive research.

---

## Original Research (Pre-Discovery)

The content below represents the state of research *before* the key sources were found. It is preserved to document the research methodology.

### What we knew at the time:
- The board was developed under **Rich Page** (VP Digital Hardware Engineering)
- Analog components likely involved **George Crow** (VP Analog Hardware Engineering)
- The kernel was likely overseen by **Avie Tevanian's** team
- Display PostScript work involved engineers whose names were not publicly documented

### What we didn't know:
- The specific engineer(s) who designed the i860-based graphics architecture → **Answer: Rich Page**
- Who wrote the kernel for the i860 processor → **Answer: Mike Paquette (GaCK)**
- Who worked on the Display PostScript acceleration → **Answer: Mike Paquette**

---

## Hardware Leadership at NeXT

### Rich Page — VP Digital Hardware Engineering

Richard A. Page (born May 15, 1951) was a NeXT co-founder and VP of Digital Hardware Engineering. He led development of all NeXT hardware including the NeXTcube, NeXTstation, and NeXTdimension.

**Background:**
- Bachelor's degrees in math and physics, Master's in computer science from San Jose State University
- Worked at Fairchild Semiconductor (1972), Hewlett-Packard (HP 3000)
- Apple Computer: Led Lisa development, became Apple Fellow (1983)
- Convinced Apple to use Motorola 68000 for Lisa and Macintosh
- Left Apple with Steve Jobs in 1985 to co-found NeXT
- Left NeXT in 1992, shortly before hardware division closure

**CHM Oral History:** [Page, Rich oral history (102780988)](https://www.computerhistory.org/collections/catalog/102780988)
- 3-hour interview conducted November 7, 2018 by Hansen Hsu
- Discusses DSP chip integration, optical drive, and Ethernet controller
- **Note:** The summary does not mention NeXTdimension specifically

### George Crow — VP Analog Hardware Engineering

George L. Crow Jr. was another NeXT co-founder who handled analog hardware engineering, including power supplies and video circuitry.

**Background:**
- Member of original Apple Macintosh team (1984)
- Designed analog board with power supply and video circuitry for Macintosh 128K
- Left Apple in 1985 to co-found NeXT
- After NeXT: UMAX Computer Corporation, Truevision
- Returned to Apple in 1999, retired in 2006

**Relevance to NeXTdimension:**
The NeXTdimension included significant analog components:
- S-Video input and output
- Composite video I/O
- RGB output
- Genlock capability
- Video capture circuitry

These would have fallen under Crow's analog hardware division, though no documentation confirms his direct involvement.

### Avie Tevanian — VP Software Engineering

Avie Tevanian led NeXTSTEP development, having co-created the Mach kernel at Carnegie Mellon University.

**Relevance to NeXTdimension:**
*[Original speculation — later corrected]*: We initially assumed the NeXTdimension ran a stripped-down Mach kernel on its i860 processor.

**Correction:** Mike Paquette clarified that GaCK was *not* Mach — it was custom code implementing a Mach-compatible interface. See [RichPage_NeXTdimension.md](RichPage_NeXTdimension.md) for the full story.

---

## The NeXT Founding Team

A photo from [stoneschool.com](http://www.stoneschool.com/Work/NeXT_Team.html) shows the early NeXT team:

| Name | Role |
|------|------|
| Steve Jobs | CEO |
| Rich Page | VP Digital Hardware Engineering |
| George Crow | VP Analog Hardware Engineering |
| Bud Tribble | VP Software Engineering |
| Susan Barnes | Chief Financial Officer |
| Susan Kare | Creative Director |
| Dan'l Lewin | (Marketing/Sales) |
| Tom Carlisle | Facilities Director |
| Lynn Takahashi Franklin | Office Manager |
| Leo Hourvitz | Software Engineering |
| Randy Heffner | VP Manufacturing |
| Gary Moore | General Counsel |
| Bruce Blumberg | Director of Product Marketing |

**Notable:** Leo Hourvitz later became Technical Director at Pixar (Supervising TD on "Geri's Game").

---

## Technical Architecture

### What the NeXTdimension Was

| Component | Specification |
|-----------|---------------|
| Form Factor | Full-length NeXTbus card |
| Processor | Intel i860 @ 33 MHz |
| RAM | 8-64 MB |
| VRAM | 4 MB |
| Resolution | 1120 × 832 @ 32-bit color |
| Alpha Channel | 8-bit transparency |
| Video I/O | S-Video, Composite, RGB |
| Price | $3,995 (1991) |

### Design Goals vs. Reality

**Original Vision:**
- Full Display PostScript offloading to i860
- PostScript interpreter running on the NeXTdimension
- JPEG compression via C-Cube CL550 chip
- Complete graphics coprocessor architecture

**What Shipped:**
- Partial DPS acceleration only
- PostScript interpreter remained on host 68040
- JPEG chip never shipped (supply/performance issues)
- i860 mostly moved pixels rather than rendering PostScript

> "Display PostScript never actually ran on the board so the Intel i860 never did much more than move blocks of color data around." — Wikipedia

---

## Why Attribution is Difficult

### Contrast with Foundation Kit

For the Foundation Kit memory management, we found:

1. **US Patent 7716450B2** (filed 1995) naming:
   - Blaine Garst
   - Ali Ozer
   - Bertrand Serlet
   - Trey Matteson

2. **CHM Oral Histories** explicitly stating:
   - "Garst introduced reference counting to Objective-C"
   - "Serlet led the Foundation framework effort"

### For NeXTdimension, we lack:

1. **No accessible patents** specifically for the NeXTdimension hardware
   - NeXT may have filed patents, but they're not easily discoverable
   - Company name variations (NeXT, Inc. / NeXT Computer, Inc. / NeXT Software, Inc.) complicate searches

2. **Oral histories don't detail NeXTdimension**
   - Rich Page's CHM interview summary doesn't mention it
   - No known interviews with engineers who worked on the board specifically

3. **Hardware vs. Software documentation**
   - Software engineers often publish papers, file patents, give talks
   - Hardware engineers historically had less public attribution
   - Board-level design was often team efforts without individual credit

---

## Possible Avenues for Further Research

### 1. Full Rich Page Oral History
The CHM interview is 3 hours long. The full video may contain NeXTdimension details not in the summary.

**Access:** [CHM Catalog 102780988](https://www.computerhistory.org/collections/catalog/102780988)

### 2. George Crow Interview
George Crow gave an interview to Fast Company about the Macintosh era. He may have given other interviews covering NeXT.

**Known:** [Fast Company Interview](https://www.fastcompany.com/3025497/next-cofounder-george-crow-reflects-on-engineering-the-first-macintosh) (blocked for web fetch)

### 3. NeXT Employee Directories
Historical employee lists or organizational charts from 1990-1991 might exist in:
- NeXT corporate archives (if preserved)
- Stanford University archives (Jobs donated materials)
- Computer History Museum collections

### 4. Patent Search
A more thorough patent search using:
- USPTO PatFT and AppFT databases
- Assignee: "NeXT" variations
- Filing dates: 1989-1993
- Classifications: graphics, video, coprocessor

### 5. NeXTWORLD Magazine Archives
NeXTWORLD magazine (1991-1994) may have profiled the engineering team.

### 6. Douglas Menuez Photography
Douglas Menuez photographed NeXT extensively. His archives might identify engineers.

---

## What We Can Conclude

> **Note:** This section reflects our conclusions *before* discovering key sources. All "Unknown" items have since been resolved.

### Definite:
- **Rich Page** led the hardware division that created the NeXTdimension
- **George Crow** led analog hardware (video I/O circuitry)
- The board was a **team effort** under their leadership

### Probable (at the time):
- ~~**Avie Tevanian's** team handled the Mach kernel port to i860~~ → **Corrected:** Mike Paquette wrote GaCK independently

### Unknown (now resolved):
- ~~Specific individual(s) who architected the i860-based graphics system~~ → **Rich Page**
- ~~Who ported Mach to the i860~~ → **Mike Paquette wrote GaCK (custom, not a Mach port)**
- ~~Who worked on the Display PostScript acceleration~~ → **Mike Paquette**
- Who designed the JPEG daughter board prototypes — *still unknown*

---

## Historical Significance

Despite incomplete attribution, the NeXTdimension was significant:

1. **First desktop 32-bit color with alpha channel** — pioneered transparency in desktop computing

2. **Dual-processor graphics architecture** — early example of GPU-like design with dedicated graphics processor

3. **Integrated video I/O** — desktop video editing years before FireWire

4. **Technical ambition** — attempted full Display PostScript offloading, even if never completed

5. **Price/performance** — workstation features at $15K (system) vs. $25K+ for SGI

The board represents NeXT's engineering ambition and the challenges of building advanced hardware at a small company competing with industry giants.

---

## Sources

### Computer History Museum
- [Rich Page Oral History (102780988)](https://www.computerhistory.org/collections/catalog/102780988)
- [Blaine Garst Oral History (102717171)](https://www.computerhistory.org/collections/catalog/102717171)
- [Avie Tevanian Oral History (102706885)](https://www.computerhistory.org/collections/catalog/102706885)

### Wikipedia
- [NeXTdimension](https://en.wikipedia.org/wiki/NeXTdimension)
- [NeXT](https://en.wikipedia.org/wiki/NeXT)
- [Intel i860](https://en.wikipedia.org/wiki/Intel_i860)
- [Rich Page](https://en.wikipedia.org/wiki/Rich_Page)
- [George Crow](https://en.wikipedia.org/wiki/George_Crow)

### Other Sources
- [Early NeXT People Photo](http://www.stoneschool.com/Work/NeXT_Team.html)
- [NeXT Wiki - NeXTdimension](https://next.fandom.com/wiki/NeXTdimension)
- [nextdimension-history.md](/Users/jvindahl/Development/nextdimension/docs/hardware/nextdimension-history.md) (local documentation)

---

## Recommendation (Completed)

> **Research Complete:** The question "Who designed the NeXTdimension?" has been answered.
>
> See [RichPage_NeXTdimension.md](RichPage_NeXTdimension.md) for the definitive account.

The approaches that succeeded:

1. ✅ **Watch Rich Page's full oral history** at CHM — Part 2 (102740462) covered NeXTdimension
2. ✅ **Search technical histories** — Game Engine Black Book: DOOM contained Mike Paquette's GaCK quote
3. ✅ **Post inquiry on nextcomputers.org forums** — Forum archives led to Usenet posts from Paquette
4. ✅ **Firmware reverse-engineering** — Modern analysis of NeXTdimension ROM confirmed GaCK architecture

---

*This document preserved as a record of the research process.*

*See [RichPage_NeXTdimension.md](RichPage_NeXTdimension.md) for the complete findings.*
