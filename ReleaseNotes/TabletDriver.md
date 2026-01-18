# 3.3 Release Notes: TabletDriver

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

This file contains information about the API for Tablet Support, which was introduced in Release 3.0. There is no additional information specific to Release 3.1, 3.2, or 3.3.

> **Note:** The tablet support this release note describes is provided only for NeXT computers, not for Intel-based computers.

---

## New API For Tablet Support

### New Features

The following extensions have been added to the NEXTSTEP input model to handle tablets and pressure-sensitivity:

- Pressure data is available in mouse events.
- Added support for suppressing event coalescing.

### Pressure Events

Pressure data is represented in the `NXEventData.mouse` structure (in `dpsclient/event.h`) by an eight-bit unsigned values ranging from 0-255.

```c
struct {                        /* For mouse events */
    short          reserved;
    short          eventNum;    /* unique identifier for this button */
    int            click;       /* click state of this event */
    unsigned char  pressure;    /* pressure value: 0=none, 255=full */
    char           reserved1;
    short          reserved2;
} mouse;
```

The above structure is passed along with mouse-moved and mouse-dragged events in the `event.data` field.

We have also added stylus proximity events as flags changed events (see `dpsclient/event.h` for details).

See the WindowServer release notes for an explanation of how to control pressure and event coalescing within an area of a window.
