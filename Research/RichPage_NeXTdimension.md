# Rich Page: Architect of the NeXTdimension

**Research Date:** January 2026
**Primary Source:** Rich Page oral history, parts 1 & 2 (CHM 102780988, 102740462)

---

## Summary

**Rich Page** was the primary architect of the NeXTdimension (ND) board, NeXT's high-end graphics accelerator that shipped in 1991. The project represented his most ambitious hardware gamble at NeXT — an attempt to turn the NeXTcube into a world-class graphics workstation that could compete with Silicon Graphics (SGI).

---

## The Architect

### Rich Page — VP Digital Hardware Engineering

Page led all hardware development at NeXT as co-founder and VP of Digital Hardware Engineering. While the original NeXT Cube was revolutionary for its software, Page felt it was underpowered for high-end graphics. The NeXTdimension was his solution.

**Key Contributions:**
- Selected the Intel i860 processor
- Advocated for 32-bit color with 8-bit alpha channel
- Designed the coprocessor architecture with dedicated memory
- Led the NRW (NeXT RISC Workstation) follow-on project

---

## Technical Vision

### 1. The Intel i860: "A Cray on a Chip"

Rich Page was a fierce advocate for the **Intel i860 RISC processor** (released 1989). While the industry was split between various RISC chips (MIPS, SPARC), Page chose the i860 because Intel marketed it as a "supercomputer on a single chip."

**The Vision:**
Page didn't just want a graphics card — he wanted a **coprocessor**. The i860 on the ND board had:
- Its own dedicated memory (up to 32 MB)
- Its own stripped-down Mach kernel
- Independent execution capability

**The Goal:**
The original plan was to offload the entire **Display PostScript (DPS)** engine from the main Motorola CPU to the i860. If successful, this would have made drawing windows and 3D objects nearly instantaneous.

### 2. 32-bit "True Color" and the Alpha Channel

Page insisted on a **32-bit architecture** for the NeXTdimension:
- 24-bit RGB color (16.7 million colors)
- 8-bit Alpha channel (256 levels of transparency)

At a time when 8-bit color (256 colors) was the standard, this was a massive technical leap.

**The Alpha Channel Advocacy:**
Page was a major proponent of the 8-bit Alpha channel, which handled **transparency**. This enabled:

- **Compositing** — layering windows or video with varying levels of see-through effects
- **Video Integration** — live video input (NTSC/PAL) in a window with translucent overlays
- **Professional Graphics** — effects previously only possible on SGI workstations

**Practical Example:**
You could watch live TV on your desktop while the Alpha channel allowed you to overlay translucent menus or graphics on top of the live broadcast without flickering.

---

## The Hardware-Software Divide

### The "Cultural Rift"

The NeXTdimension is often cited as the source of a significant **cultural rift** between Page's hardware team and Avie Tevanian's software team.

### The Programming Nightmare

The i860 was notoriously difficult to program:
- Required manual instruction scheduling to avoid pipeline stalls
- Compiler couldn't automatically optimize code
- VLIW architecture demanded expert-level low-level programming
- Debugging was extremely challenging

### The Result

Despite Page's hardware being "insanely" powerful on paper:
- The software team struggled to port the full Display PostScript engine
- The i860 ended up acting as a "glorified pixel mover"
- The main Motorola 68040 still did most of the rendering work
- The board never reached its theoretical peak performance

**From the history documentation:**
> "Display PostScript never actually ran on the board so the Intel i860 never did much more than move blocks of color data around."

---

## The NRW: The Road Not Taken

### NeXT RISC Workstation (NRW)

Page's work on the NeXTdimension was actually a **stepping stone** to a much larger project: the **NeXT RISC Workstation (NRW)**.

**The Vision:**
- Next-generation NeXT computer
- Potentially powered by dual i860 processors
- Later designs considered PowerPC
- Would have been NeXT's answer to high-end SGI workstations

**The Fallout:**
As NeXT struggled financially, an internal battle intensified:
- Should NeXT stay in the hardware business?
- Or become a software-only company?

Page, a hardware purist who believed NeXT's soul was in its custom silicon, found himself at odds with the emerging "software-first" strategy.

### Page's Departure

Rich Page left NeXT in 1992 — shortly before the hardware division was shuttered entirely. His departure marked the end of NeXT's hardware ambitions and the beginning of its transformation into a software company.

---

## Timeline

| Date | Event |
|------|-------|
| 1985 | Page co-founds NeXT with Steve Jobs |
| 1988 | Original NeXT Cube ships |
| 1989 | Intel releases i860 processor |
| 1990 Sept | NeXTdimension announced |
| 1991 Feb | JPEG chip removed due to supply issues |
| 1991 April | NeXTdimension ships |
| 1992 | Rich Page leaves NeXT |
| 1993 | NeXT exits hardware business |

---

## Legacy and Influence

### Industry Impact

Rich Page's NeXTdimension board enabled NeXT to enter the **film and scientific visualization** industries. It was used by early pioneers in:
- Digital video production
- 3D modeling and rendering
- Scientific visualization
- Broadcast graphics

### Lasting Technical Contributions

Even though the hardware division was shuttered shortly after Page left, his technical decisions became industry standards:

| Page's Decision (1990) | Industry Standard (Today) |
|------------------------|---------------------------|
| 32-bit color | Universal in displays |
| 8-bit alpha channel | Core of all compositing systems |
| Transparency/compositing | Foundation of modern window managers |
| Coprocessor architecture | Precursor to modern GPU design |

**The alpha-channel transparency** that Page championed eventually defined:
- macOS window compositing (Quartz)
- iOS graphics architecture
- Modern GPU rendering pipelines

---

## Oral History Sources

### Part 1: The Original Cube Era (1985-1988)
**[Page, Rich oral history | 102780988](https://www.computerhistory.org/collections/catalog/102780988)**
- Date: November 7, 2018
- Duration: 3 hours, 1 minute
- Interviewer: Hansen Hsu

Covers:
- The "Channel Chip" (custom DMA controller)
- Optical Storage Processor
- Custom Ethernet controller
- Motorola 56001 DSP integration

### Part 2: The Color Era and Beyond (1989-1992)
**[Page, Rich oral history, part 2 | 102740462](https://www.computerhistory.org/collections/catalog/102740462)**
- Date: November 29, 2018
- Duration: 2 hours, 48 minutes, 25 seconds
- Interviewer: Hansen Hsu

Covers:
- i860 advocacy and selection
- NeXTdimension engineering
- NRW (NeXT RISC Workstation) project
- Automated factory management
- NeXTstation second-generation design
- Departure from NeXT (1992)
- Post-NeXT ventures (Sierra Research, Next Sierra)

---

## Comparison: Hardware vs. Software Attribution

### Why We Found Software Architects Easily

For the Foundation Kit memory management system, we found:
- **US Patent 7716450B2** naming Garst, Serlet, Ozer, Matteson
- **CHM oral histories** explicitly crediting individuals
- **Academic papers** and conference talks

### Why Hardware Attribution Was Harder

For the NeXTdimension:
- No accessible patents naming specific inventors
- Hardware design historically had less individual attribution
- Board-level work was seen as team effort under division leadership
- The "cultural rift" meant software and hardware teams operated separately

### The Key Insight

The split between Page's hardware team and Tevanian's software team explains why:
- **Software architects** (Garst, Serlet, Ozer) are well-documented
- **Hardware architects** (Page) required deeper research into oral histories

Page was the **hardware counterpart** to the software architects — but hardware engineering culture of that era didn't emphasize individual attribution the way software (with its patents and papers) did.

---

## Conclusion

**Rich Page** was definitively the architect of the NeXTdimension board. His vision included:
- Using the i860 as a true coprocessor running its own Mach kernel
- 32-bit color with 8-bit alpha channel for professional compositing
- Offloading Display PostScript entirely to dedicated hardware

While the software never fully realized his hardware's potential (due to the i860's programming difficulty), Page's technical decisions — particularly the alpha channel — became foundational to modern graphics systems.

His departure in 1992 and NeXT's subsequent exit from hardware marked the end of an era, but his influence persists in every composited window and transparent UI element in macOS and iOS today.

---

## Sources

### Primary
- [Page, Rich oral history, part 1 (CHM 102780988)](https://www.computerhistory.org/collections/catalog/102780988)
- [Page, Rich oral history, part 2 (CHM 102740462)](https://www.computerhistory.org/collections/catalog/102740462)

### Secondary
- [NeXTdimension - Wikipedia](https://en.wikipedia.org/wiki/NeXTdimension)
- [Intel i860 - Wikipedia](https://en.wikipedia.org/wiki/Intel_i860)
- [Rich Page - Wikipedia](https://en.wikipedia.org/wiki/Rich_Page)
- [NeXTdimension History Documentation](../../nextdimension/docs/hardware/nextdimension-history.md) (local)

---

*This research completes the investigation into who designed the NeXTdimension board, answering the question left open in NeXTdimensionArchitects.md.*
