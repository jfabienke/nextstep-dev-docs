# 3.3 Release Notes: The MIDI Driver

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

There are no new MIDI features or bug fixes in Releases 3.3, 3.2, or 3.1. This file contains release notes for the 3.0 release of the MIDI driver.

---

## Notes Specific to Release 3.0

### New Features

The MIDI driver has been rewritten for release 3.0. The new version has a simpler API, is more efficient, and supports new features such as synchronization to MIDI time code. See the MIDI driver documentation for API details.

The MIDI driver programming examples have been rewritten to use the new driver. They now support MIDI time code synchronization.

You no longer need to include `-lmidi` on your link line—the MIDI interface functions are now in `libsys`.

### Incompatible Changes

The old driver is shipped in 3.0 so that pre-3.0 applications will continue to work. However, 3.0 applications must convert to the new driver to compile and link.

The header files on `/usr/include/midi` have gone away. The new header files are the following:

- `/NextDeveloper/Headers/mididriver/midi_spec.h` contains definitions of MIDI op codes, controller numbers, and constants, as well as macros for disassembling MIDI commands.

- `/NextDeveloper/Headers/mididriver/midi_driver.h` contains the interface to the new MIDI driver.
