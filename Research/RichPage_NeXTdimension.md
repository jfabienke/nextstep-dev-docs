# Rich Page: Architect of the NeXTdimension

**Research Date:** January 2026
**Primary Sources:**
- Rich Page oral history, parts 1 & 2 (CHM 102780988, 102740462)
- Game Engine Black Book: DOOM (Fabien Sanglard, 2018)

---

## Summary

**Rich Page** was the primary architect of the NeXTdimension (ND) board, NeXT's high-end graphics accelerator that shipped in 1991. **Mike Paquette** wrote the custom kernel (GaCK) that ran on the i860 processor. The project represented Page's most ambitious hardware gamble at NeXT — an attempt to turn the NeXTcube into a world-class graphics workstation that could compete with Silicon Graphics (SGI).

---

## The Architect

### Rich Page — VP Digital Hardware Engineering

Page led all hardware development at NeXT as co-founder and VP of Digital Hardware Engineering. While the original NeXT Cube was revolutionary for its software, Page felt it was underpowered for high-end graphics. The NeXTdimension was his solution.

**The Founding Team (1985):**

In the iconic NeXT founding team photo, Page appears bearded in the back row, second from left:

| Position | Person | Role |
|----------|--------|------|
| Back row, L→R | George Crow | VP Analog Hardware Engineering |
| | **Rich Page** | VP Digital Hardware Engineering |
| | Steve Jobs | CEO |
| | Bud Tribble | VP Software Engineering |
| | Dan'l Lewin | VP Marketing |
| | (6th person) | Possibly Bruce Blumberg |
| Front row | (male) | Early employee |
| | Susan Barnes | Chief Financial Officer |

Page had just left Apple as an Apple Fellow (awarded 1983) to join Jobs. He was responsible for Apple's decision to use the Motorola 68000 in the Lisa and Macintosh.

**Key Contributions:**
- Selected the Intel i860 processor
- Advocated for 32-bit color with 8-bit alpha channel
- Designed the coprocessor architecture with dedicated memory
- Led the NRW (NeXT RISC Workstation) follow-on project

### Mike Paquette — GaCK Kernel Author

**Mike Paquette** was the NeXT engineer who wrote the kernel that ran on the NeXTdimension's i860 processor.

**Background: Pixar (pre-NeXT)**

Before joining NeXT, Paquette was at **Pixar** where he co-authored a seminal 1987 ACM paper with **Pat Hanrahan** (later 2019 Turing Award winner), Adam Levinthal, and Jim Lawson:

> **"Parallel Computers for Graphics Applications"** (ACM 1987)

At Pixar, Paquette worked on:
- **Chap (Channel Processor)** — 4-channel SIMD integer processor for RGBA image compositing
- **Flap (Floating point Array Processor)** — for 3D shading and transformations
- **C4 language** — parallel C extension for graphics programming
- The **Pixar Image Computer** — commercial product based on this research

The Chap/Flap architecture achieved 60-200x speedup over VAX 11/780 for graphics operations. Key insight: graphics primitives (pixels, vertices) are small data structures ideal for SIMD parallel processing.

**The NeXT Connection:** Steve Jobs bought Pixar from Lucasfilm in 1986. Paquette moved from one Jobs company (Pixar) to another (NeXT), bringing deep expertise in parallel graphics processing and alpha channel compositing — exactly what the NeXTdimension would need.

**Known Email Addresses (from Usenet archives):**
- `mpaque@wco.com` — personal account
- `mpaque@next.com` — "NeXT business mail only, please"
- `mpaquette@moviola.next.com` (June 1997) — the `moviola` subdomain was likely NeXT's video/media team server (named after the 1924 Moviola film editing machine), consistent with his work on NEXTTIME

**Usenet Signature (Jan 1, 1997):**
> "I don't speak for my employer, whoever it is, and they don't speak for me."

The "whoever it is" is poignant — this was 11 days after Apple announced the NeXT acquisition (Dec 20, 1996).

**On Video Editing Software (Jan 1, 1997):**
When asked if there was any NeXTSTEP video editing software, Paquette replied simply: "Not yet..." — NEXTTIME was a player, not an editor. Given his work on the `moviola.next.com` server (named after editing equipment), video editing software may have been planned but never shipped.

**Key Contributions at NeXT:**
- Wrote **GaCK** (Graphics aCcelerator Kernel) — the custom kernel for NeXTdimension
- Worked on **Display PostScript (DPS)** — the graphics rendering system
- Developed **NEXTTIME** — NeXT's QuickTime-compatible video player ($99, supported QuickTime file format and Cinepak compression)
- Created IOHIDevice code at NeXT (May 22, 1992)
- Discussed **NRW** variants in Usenet posts (M88K and dual PPC 601 configurations)

**Later at Apple:**
- Became **Quartz architect**, spending "a decade re-creating Display PostScript as Quartz"

**From Game Engine Black Book: DOOM:**
> "The NeXTdimension ran a custom kernel which was designed to do soft real-time management of multiple threads within a single address space... The kernel was called 'Graphics aCcelerator Kernel, or 'GaCK'. Yes, this was a jape at the funny capitalization of the company name. It was not Mach, or BSD, or Minix, or Linux."
> — M Paquette, NeXT Engineer

**Critical Correction:** Earlier accounts stated the NeXTdimension ran a "stripped-down Mach kernel." Paquette clarified it was custom. However, modern firmware reverse-engineering (see below) reveals a more nuanced picture.

### GaCK Firmware Analysis (2024-2025)

Reverse-engineering of the NeXTdimension ROM firmware (192 KB verified i860 code) reveals that GaCK is far more sophisticated than previously understood:

**GaCK is a complete Mach-compatible multitasking operating system:**

| Feature | Count | Implication |
|---------|-------|-------------|
| Trap instructions | 217 | Complete interrupt/exception/syscall infrastructure |
| Lock operations | 73 | Concurrent execution support |
| Memory context switches | 22 | Full multitasking (21 %dirbase writes) |
| Privilege transitions | 31 | User/kernel mode separation |
| Performance breakpoints | 88 | Built-in profiling support |

**Two operating systems in firmware:**
1. **Bootstrap Graphics HAL** (32 KB @ 0xF8000000) — Microkernel-level graphics subsystem with 82 primitives, zero traps, single-threaded
2. **GaCK Mach Kernel** (160 KB @ 0xF8008000) — Full Mach-compatible OS with Display PostScript interpreter

**Reconciling Paquette's statement:** GaCK is not Mach *itself* (not the CMU codebase), but it implements a **Mach-compatible interface** equivalent to Mach 2.5. It's custom code that speaks the Mach protocol — a peer kernel to the host 68040's Mach, not a port of it.

**Architectural significance:** The NeXTdimension is not a graphics accelerator — it's an **entirely independent computer** running its own OS, providing Display PostScript services to the host. This peer-to-peer kernel architecture was decades ahead of modern GPU compute models (CUDA, OpenCL).

**Source:** Firmware analysis from the nextdimension reverse-engineering project, including `GACK_KERNEL_HARDWARE_SCAN.md` and `FINAL_ARCHITECTURAL_REVELATION.md`. See the [nextcomputers.org forum](https://www.nextcomputers.org/forums/) for community discussion of these findings.

**Career Arc:**
Paquette's work spanned the entire graphics stack at NeXT: from low-level kernel (GaCK), through rendering (DPS), to media playback (NEXTTIME). This directly informed his later role at Apple architecting Quartz. The compositing architecture pioneered on the NeXTdimension became the foundation for macOS's window server.

---

## Technical Vision

### 1. The Intel i860: "A Cray on a Chip"

Rich Page was a fierce advocate for the **Intel i860 RISC processor** (released 1989). While the industry was split between various RISC chips (MIPS, SPARC), Page chose the i860 because Intel marketed it as a "supercomputer on a single chip."

**The Vision:**
Page didn't just want a graphics card — he wanted a **coprocessor**. The i860 on the ND board had:
- Its own dedicated memory (up to 32 MB)
- Its own custom kernel (GaCK — written by Mike Paquette)
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
- Multiple processor configurations were explored:
  - Dual i860 processors (original concept)
  - **Motorola 88000 (M88K)** variant
  - **Dual PowerPC 601** variant
- Would have been NeXT's answer to high-end SGI workstations

**Historical Note:** Mike Paquette discussed NRW configurations in his comp.sys.next Usenet posts, revealing that both M88K and dual PPC 601 designs were seriously considered before NeXT abandoned hardware entirely.

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

## Historical Notes

### Early NeXTSTEP Development

According to Mike Paquette's Usenet posts (discovered via nextcomputers.org forums), NeXT did extensive work on early NeXTSTEP pre-releases (versions as early as 0.1) on **Sun M68K workstations** because NeXT's own M68K hardware wasn't ready yet. This explains why NeXTSTEP was always relatively portable — it was never developed exclusively on NeXT hardware from the start.

---

## Contemporary Interviews

### NeXTWORLD Interview: "Rich Page looks ahead"
**Source:** NeXTWORLD magazine (Simson L. Garfinkel, Senior Editor)

Rich Page's candid assessments of NeXT's decisions:

**On the optical disc:**
> "Was that a mistake? **Probably...**"
> "The combination of the Winchester and the optical disc made a good product, but it was too expensive. And the optical-only system didn't work very well."

**On processors:**
> "A year or two ago, you might have been inclined to pick MIPS. Yet if you'd done that, today you would be asking yourself, 'Why did I pick MIPS? It looks like they're going out of business.'"
>
> "That basically leaves SPARC, 88K... And you don't want to go with Intel, because... they charge you extra for DOS compatibility."

**On the keyboard (everyone hated it):**
> "**Everybody hates the new keyboard.** We'll probably change our keyboard... What happened on this one was that in the process of internationalizing and solving certain problems, we caused certain problems — like moving the backslash and the pipe for the UNIX folks. That was probably a mistake."

**On portables (prescient):**
> "I think that what people want is **the ability to carry their NeXTstation around and use it when they get somewhere**. Transportability is the important issue."

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

**Rich Page** was definitively the hardware architect of the NeXTdimension board, while **Mike Paquette** wrote the custom GaCK kernel that ran on the i860 processor.

Page's vision included:
- Using the i860 as a true coprocessor running its own kernel
- 32-bit color with 8-bit alpha channel for professional compositing
- Offloading Display PostScript entirely to dedicated hardware

Paquette's GaCK kernel was specifically designed for:
- Soft real-time management of multiple threads
- Single address space operation
- Graphics workload optimization

While the software never fully realized the hardware's potential (due to the i860's programming difficulty), the technical decisions made by both engineers became foundational to modern graphics systems.

Page's departure in 1992 marked the end of NeXT's hardware ambitions, but his influence persists in every composited window and transparent UI element in macOS and iOS today. Paquette continued this legacy at Apple, where he spent a decade architecting Quartz — essentially re-creating Display PostScript for the modern era.

---

## Sources

### Primary
- [Page, Rich oral history, part 1 (CHM 102780988)](https://www.computerhistory.org/collections/catalog/102780988)
- [Page, Rich oral history, part 2 (CHM 102740462)](https://www.computerhistory.org/collections/catalog/102740462)
- **Game Engine Black Book: DOOM** — Fabien Sanglard, 2018 (contains Mike Paquette quotes about GaCK kernel)
- **"Parallel Computers for Graphics Applications"** — Levinthal, Hanrahan, Paquette, Lawson (ACM 1987) — Paquette's pre-NeXT work at Pixar on Chap/Flap processors

### Secondary
- [NeXTdimension - Wikipedia](https://en.wikipedia.org/wiki/NeXTdimension)
- [Intel i860 - Wikipedia](https://en.wikipedia.org/wiki/Intel_i860)
- [Rich Page - Wikipedia](https://en.wikipedia.org/wiki/Rich_Page)
- [NeXTdimension History Documentation](../../nextdimension/docs/hardware/nextdimension-history.md) (local)
- [Slashdot: Former Apple Quartz Architect Describes Quartz History (2003)](https://apple.slashdot.org/story/03/02/11/1920216/former-apple-quartz-architect-describes-quartz-history) — References Mike Paquette as Quartz architect

### NeXTWORLD Magazine
- **"Rich Page looks ahead"** — Interview by Simson L. Garfinkel, NeXTWORLD Senior Editor. Candid assessments of optical disc, processors, keyboard, and portable strategy.
- **NeXTWORLD Vol. 1 No. 4 (Winter 1991)** — [Archive.org](https://archive.org/details/NeXTWORLDVol.1No.4Winter1991) — Contains Avie Tevanian profile (Director of System Software, 15-person team), names John Seamons and Mike Demoney as Mach porters.
- **NeXTWORLD 1991-1992 collection** — [Archive.org](https://archive.org/details/nextworld-1992) — Full magazine scans with technical articles and interviews.

### Forum & Usenet Archives
- [nextcomputers.org: Hosting for OPENSTEP 4.2 boot disk (topic 191)](https://www.nextcomputers.org/forums/index.php?topic=191.0) — Contains Mike Paquette Usenet post from June 27, 1997, and discussion of his work on DPS, NEXTTIME, and NRW
- [nextcomputers.org: Usenet Soft.htm (1996)](https://www.nextcomputers.org/NeXTfiles/Docs/Usenet/news/1996/Soft.htm) — Dec 31, 1996 post from Paquette about 2.88MB floppy drives (Sony/Toshiba joint development, NeXT among first to use them)
- [nextcomputers.org: Usenet Bugs.htm (1996-1997)](https://www.nextcomputers.org/NeXTfiles/Docs/Usenet/news/1996/Bugs.htm) — Jan 14, 1997 post from Paquette on Dynamic Libraries
- **comp.sys.next.*** Usenet archives — Mike Paquette's posts discussing NRW variants (M88K, dual PPC 601) and early NeXTSTEP development on Sun M68K workstations

---

*This research completes the investigation into who designed the NeXTdimension board, answering the question left open in NeXTdimensionArchitects.md.*

## Related Research

- [Research README](README.md) — Full list of NeXT engineers and timeline
- [MemoryManagementArchitects.md](MemoryManagementArchitects.md) — Software counterpart: Garst, Serlet, Ozer, Matteson
- [NeXTdimensionArchitects.md](NeXTdimensionArchitects.md) — Earlier research (superseded by this document)

---

*Last updated: January 2026*

*Changelog:*
- *Added Mike Paquette attribution from Game Engine Black Book: DOOM*
- *Added NRW variants (M88K, dual PPC 601) and NEXTTIME from nextcomputers.org forum / Usenet archives*
- *Added GaCK firmware reverse-engineering findings*
- *Added Paquette's Pixar background and ACM 1987 paper*
