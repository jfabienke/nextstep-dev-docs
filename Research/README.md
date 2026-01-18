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

### NeXTdimensionArchitects.md

**Question:** Who designed the NeXTdimension graphics accelerator board?

**Answer:** Incomplete — specific engineers not definitively identified.

**What we know:**
- **Rich Page** (VP Digital Hardware Engineering) led all hardware development
- **George Crow** (VP Analog Hardware Engineering) handled video/analog circuitry
- **Avie Tevanian's** team likely oversaw the Mach kernel port to i860

**What remains unknown:**
- The specific engineer(s) who designed the i860-based graphics architecture
- Who ported the Mach kernel to the i860 processor
- Who worked on Display PostScript acceleration (never completed)

Unlike the Foundation Kit where patents and oral histories provided clear attribution, the NeXTdimension's designers remain undocumented in publicly available sources. The document includes recommendations for further research.

---

## Methodology

Research in this directory draws from:
- **Computer History Museum oral histories** — First-person accounts from NeXT engineers
- **Patent records** — Technical documentation with inventor attribution
- **Contemporary documentation** — The NeXTSTEP 3.3 Developer Documentation itself
- **Secondary sources** — Technical histories and retrospectives

The goal is to connect the "what" (the documentation we're preserving) with the "who" and "why" (the people and decisions that created it).
