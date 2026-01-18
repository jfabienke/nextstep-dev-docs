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

### RichPage_NeXTdimension.md *(Complete)*

**Question:** Who designed the NeXTdimension graphics accelerator board?

**Answer:** **Rich Page** — Primary architect of the NeXTdimension.

Page's key contributions:
- Selected the Intel i860 processor ("Cray on a chip")
- Advocated for 32-bit color with 8-bit alpha channel
- Designed the coprocessor architecture with dedicated Mach kernel
- Led the NRW (NeXT RISC Workstation) follow-on project

The board never reached its potential due to the i860's programming difficulty — the software team couldn't port full Display PostScript to it. This "cultural rift" between Page's hardware team and Tevanian's software team explains why hardware attribution was harder to find than software attribution.

Based on Computer History Museum oral histories, parts 1 & 2 (CHM 102780988, 102740462).

### NeXTdimensionArchitects.md *(Superseded)*

Initial research document before Rich Page oral history Part 2 was discovered. Kept for reference on research methodology. See RichPage_NeXTdimension.md for complete findings.

---

## Methodology

Research in this directory draws from:
- **Computer History Museum oral histories** — First-person accounts from NeXT engineers
- **Patent records** — Technical documentation with inventor attribution
- **Contemporary documentation** — The NeXTSTEP 3.3 Developer Documentation itself
- **Secondary sources** — Technical histories and retrospectives

The goal is to connect the "what" (the documentation we're preserving) with the "who" and "why" (the people and decisions that created it).
