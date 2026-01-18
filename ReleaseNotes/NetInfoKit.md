# 3.3 Release Notes: NetInfo Kit

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

This file contains release notes for the 3.0 release of the NetInfo Kit. Some bugs have been fixed in the 3.1 release, but no new features have been introduced. There is no additional information specific to Release 3.3 or 3.2.

---

## Notes Specific to Release 3.0

### New Features

The NetInfo Kit interface is improved for the in Release 3.0. It's the first fully supported release of a minimal set of tools for use in administrative applications.

- The NetInfo Kit has been internationalized and now fully supports developement in foreign languages.
- The NetInfo Kit now supports defaults for panel size and starting directories. This should allow for easier customiztion of working environment.
- The NetInfo Kit API is more consistent with the naming conventions and standards of NEXTSTEP.

### New Objects

No new objects in Release 3.0.

### Known Bugs

The following are known bugs present in the NetInfo Kit.

- Return codes for the various panels are not documented. They are `EXIT_OK` (value 1), `EXIT_REMOVE` (value 2), `EXIT_CANCEL` (value 3). At this point `EXIT_REMOVE` is only present for historical (pre 1.0) value. `EXIT_OK` is returned when the action of the panel is selected (i.e. OK or Login...). `EXIT_CANCEL` is returned when the Cancel button is selected.

- Passing a `NULL` domain to the `NILoginPanel` may cause a crash on a derefenced pointer if and only if the default NetInfo style login is selected. If a delegate is chosen to do the authentication, a `NULL` value is legal.
