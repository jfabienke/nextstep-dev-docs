# Who Architected NeXT's Memory Management System?

**Research Date:** January 2026
**Sources:** Computer History Museum Oral Histories, US Patent Records

---

## Summary

The retain/release/autorelease memory management system — the foundation of Objective-C's manual reference counting (MRC) and later Automatic Reference Counting (ARC) — was co-designed by a team of engineers at NeXT Computer in the early 1990s:

| Engineer | Primary Contribution |
|----------|---------------------|
| **Blaine Garst** | Added reference counting to the memory management system; key technical advocate |
| **Bertrand Serlet** | Led Foundation framework team; co-designed protocols |
| **Ali Ozer** | Co-authored Foundation classes; autorelease pool implementation |
| **Trey Matteson** | Co-inventor on autorelease patent |

---

## The Question

While converting NeXTSTEP 3.3 Developer Documentation to Markdown, we noticed how remarkably modern the Foundation Kit's memory management patterns appeared — `retain`, `release`, and `autorelease` are essentially the same primitives that power iOS and macOS development today, 30 years later.

This prompted the question: **Who designed this system?**

---

## Evidence

### Patent US7716450B2 — "Transparent Local and Distributed Memory Management System"

| Field | Value |
|-------|-------|
| **Priority Date** | January 31, 1995 |
| **Inventors** | Blaine Garst, Ali Ozer, Bertrand Serlet, Trey Matteson |
| **Original Assignee** | NeXT Software Inc. |
| **Status** | Expired May 22, 2015 |

The patent covers the autorelease pool mechanism:

> "An autorelease pool is created at the beginning of a new duty cycle. The autorelease pool retains the newly allocated memory space during the duty cycle. The autorelease pool is automatically disposed of at the end of the duty cycle. As a result of disposing the autorelease pool, the newly allocated memory space is reclaimed."

This is exactly the pattern documented in the Foundation Kit Introduction we converted:

```objc
- (NSArray *)sprockets
{
    NSArray *array;
    array = [[NSArray alloc] initWithObjects:mainSprocket,
                                              auxiliarySprocket, nil];
    return [array autorelease];
}
```

---

### Computer History Museum Oral Histories

#### Blaine Garst (CHM Catalog #102717171)

> "Garst **introduced reference counting to Objective-C's memory management system**... Working alongside Serlet and Ali Ozer, Garst authored numerous original classes within the Foundation framework."

> "At NeXT, Garst collaborated with Bertrand Serlet to add **protocols** to the Objective-C language, based in part on his master's work in modular programming."

**Background:** Born 1956, Council Bluffs, Iowa. Master's in Computer Science from UCLA specializing in Programming Languages and Systems. Worked at Bell Labs on Unix System V before joining NeXT in 1990, hired by Avie Tevanian.

#### Bertrand Serlet (CHM Catalog #102808913)

> "Serlet moved back to California and became a manager at NeXT, **leading the effort to create the Foundation framework**, factoring out fundamental data structures from the user interface-focused AppKit framework for the next release of OpenStep."

**Background:** Born 1960, France. PhD from University of Paris-Sud. Worked at INRIA and Xerox PARC before joining NeXT in 1989.

#### Steve Naroff (CHM Catalog #102717385)

> "Collaborated with **Bertrand Serlet and Blaine Garst** on adding protocols to Objective-C."

Naroff was the primary engineer adapting Objective-C for NeXT's needs, integrating it into GCC, but the memory management architecture was developed by the Garst/Serlet/Ozer team.

#### Avie Tevanian (CHM Catalog #102706885)

Tevanian led NeXT's OS group and hired Blaine Garst in 1990. His focus was primarily on the Mach kernel layer rather than the application frameworks, but he built the team that created the memory management system.

---

## Timeline

| Year | Event | Key People |
|------|-------|------------|
| 1984 | Objective-C created at Stepstone | Brad Cox, Tom Love |
| 1988 | NeXT licenses Objective-C | Steve Naroff adapts it |
| 1989 | Serlet joins NeXT, works on AppKit | Bertrand Serlet, Jean-Marie Hullot |
| 1990 | Garst joins NeXT, hired by Tevanian | Blaine Garst |
| 1990 | Protocols added to Objective-C | Garst + Serlet |
| 1990-93 | Reference counting added to runtime | Garst |
| 1993-94 | Foundation framework created (OpenStep) | Serlet (lead), Garst, Ali Ozer |
| 1995 | Autorelease patent filed | Garst, Ozer, Serlet, Matteson |
| 1997 | Apple acquires NeXT | Team moves to Apple |
| 2011 | ARC introduced in iOS 5 / OS X Lion | Blaine Garst (architect) |

---

## Individual Contributions

### Blaine Garst — The Reference Counting Architect

Garst appears to have been the **primary technical driver** of reference counting:

- Introduced reference counting to Objective-C's memory management system
- Described as "key advocate for reference-counted memory management"
- Co-inventor on autorelease pool patent
- Implemented Distributed Objects system at NeXT
- Later architected **ARC** at Apple (the automation of what he helped design manually)
- Introduced **Closures/Blocks** in Mac OS X 10.6 Snow Leopard

Quote from FOSDEM 2022:
> "Blaine is the reason Mac/iOS apps don't pause" — referring to ARC's efficiency compared to garbage collection.

### Bertrand Serlet — The Framework Leader

Serlet provided **architectural leadership** for the Foundation framework:

- Led the Foundation framework effort at NeXT
- Co-designed protocols with Garst
- Co-inventor on autorelease pool patent
- Later became SVP of Mac Software Engineering at Apple
- Oversaw Mac OS X development through Lion (10.7)

### Ali Ozer — The Implementation Engineer

Ozer was a **key implementer** of Foundation classes:

- Co-authored original Foundation framework classes
- Co-inventor on autorelease pool patent
- Worked alongside Garst and Serlet on OpenStep
- Later led AppKit development at Apple

---

## The Collaboration Model

From Blaine Garst's oral history, describing the NeXT work environment:

> "A very egoless environment... Steve Jobs' real talent was hiring good people. 'Cause he didn't invent most of that stuff but he sold it."

> "Small enough for cross-functional collaboration... a setting where he handled various responsibilities (kernel work, language development, etc.)"

The retain/release/autorelease system wasn't one person's invention — it emerged from collaborative work between engineers with complementary expertise:

- **Garst** brought programming language and systems expertise from Bell Labs
- **Serlet** brought framework architecture experience from Xerox PARC
- **Ozer** provided implementation depth in the Foundation classes

---

## Why This Matters

The memory management pattern designed at NeXT in the early 1990s has proven remarkably durable:

| Era | Technology | Underlying Model |
|-----|------------|------------------|
| 1994 | NeXTSTEP 3.3 Foundation Kit | Manual retain/release/autorelease |
| 2001 | Mac OS X Cocoa | Same model |
| 2008 | iPhone OS / iOS | Same model |
| 2011 | ARC (Automatic Reference Counting) | Compiler-automated retain/release |
| 2014 | Swift | ARC built into language |
| 2024 | Modern iOS/macOS | Still ARC (same primitives) |

The ownership rules documented in the 1994 Foundation Kit:

> "If you create an object, you alone are responsible for releasing it. If you didn't create the object, you don't own it and shouldn't release it."

...are still the mental model developers use today. The compiler just handles the bookkeeping automatically now.

---

## Conclusion

**Bertrand Serlet** was indeed a key architect of NeXT's memory management system, but as **leader of the Foundation framework effort** and **co-designer** rather than sole inventor.

**Blaine Garst** appears to have been the **primary technical architect** of reference counting specifically, while Serlet provided framework-level architecture and Ozer contributed implementation.

The system was a collaborative creation — three engineers (plus Matteson) are named on the patent, and oral histories consistently describe them working together on Foundation framework development.

This collaborative approach produced a memory management architecture so well-designed that it has remained fundamentally unchanged for three decades, simply gaining compiler automation in 2011.

---

## Sources

### Computer History Museum Oral Histories
- [Bertrand Serlet Oral History (Video)](https://www.computerhistory.org/collections/catalog/102808913)
- [Bertrand Serlet Oral History (PDF Transcript)](https://archive.computerhistory.org/resources/access/text/2024/12/102808912-05-01-acc.pdf)
- [Blaine Garst Oral History Part 1](https://www.computerhistory.org/collections/catalog/102717171)
- [Steve Naroff Oral History Part 1](https://www.computerhistory.org/collections/catalog/102717385)
- [Avie Tevanian Oral History Part 1](https://www.computerhistory.org/collections/catalog/102706885)
- [Avie Tevanian Oral History Part 2](https://www.computerhistory.org/collections/catalog/102740144)

### Patent Records
- [US Patent 7716450B2 — Transparent Local and Distributed Memory Management System](https://patents.google.com/patent/US7716450B2)

### Secondary Sources
- [FOSDEM 2022 — Blaine Garst Speaker Profile](https://archive.fosdem.org/2022/schedule/speaker/blaine_garst/)
- [Highlights of Avie Tevanian's Interview](https://www.copetti.org/writings/blog/avie-tevanian-interview/)

---

*This research was conducted while converting the NeXTSTEP 3.3 Developer Documentation to Markdown format for historical preservation.*
