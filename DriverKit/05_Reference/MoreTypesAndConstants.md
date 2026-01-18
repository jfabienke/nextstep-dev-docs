# More Types and Constants

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

*Note: This document contains supplemental type definitions and internal development notes for the NeXTSTEP Driver Kit.*

---

## Defined Types

### IOAddressRange
**Declared In:** `driverkit/IODeviceInspector.h`
**Synopsis:**
```c
typedef struct IOAddressRange {
    unsigned start;
    unsigned length;
} IOAddressRange;
```
**Description:** Used to describe address ranges.

### IOCache
**Declared In:** `driverkit/driverTypes.h`
**Synopsis:**
```c
typedef enum {
    IO_CacheOff,
    IO_WriteThrough,
    IO_CopyBack
} IOCache;
```
**Description:** Used to specify caching. `IO_CacheOff` inhibits the cache.

### IODDMMsg
**Declared In:** `driverkit/debuggingMsg.h`
**Synopsis:**
```c
typedef struct {
    msg_header_t header;
    msg_type_t argType;
    unsigned index;
    unsigned maskValue;
    unsigned status;
    unsigned timestampHighInt;
    unsigned timestampLowInt;
    int cpuNumber;
    msg_type_t stringType;
    char string[IO_DDM_STRING_LENGTH];
} IODDMMsg;
```
**Description:** The message format understood by the Driver Debugging Module.

### IODisplayInfo
**Declared In:** `bsd/dev/i386/displayDefs.h` (Note: to be moved to `driverkit/`)
**Synopsis:**
```c
typedef struct {
    int width;
    int height;
    int totalWidth;
    int rowBytes;
    int refreshRate;
    void *frameBuffer;
    IOBitsPerPixel bitsPerPixel;
    IOColorSpace colorSpace;
    unsigned int flags;
    void *parameters;
} IODisplayInfo;
```
**Description:** Describes a video display mode.
- **rowBytes:** Number of bytes from one scanline to next. (e.g., 1024 width at 15bpp = 2048 rowBytes).
- **bitsPerPixel:** `IO_8BitsPerPixel` (8-bit grayscale) or `IO_15BitsPerPixel` (16-bit RGB).
- **colorSpace:** `IO_DISPLAY_ONEISWHITECOLORSPACE` (monochrome) or `IO_DISPLAY_RGBCOLORSPACE` (color).

---

## Symbolic Constants

### Interrupt Messages
**Declared In:** `driverkit/interruptMsg.h`

| Constant | Meaning |
| :--- | :--- |
| `IO_INTERRUPT_MSG_ID_BASE` | Lowest ID an IOInterruptMsg can have |
| `IO_TIMEOUT_MSG` | Timeout notification |
| `IO_COMMAND_MSG` | Manual command request |
| `IO_DEVICE_INTERRUPT_MSG` | Sent by kernel on hardware interrupt |
| `IO_DMA_INTERRUPT_MSG` | DMA completion notification |

---

## Unused Constants
**Declared In:** `driverkit/driverTypes.h`

*Note: These constants are primarily for NeXTbus/M68k heritage and are not used by drivers for Intel-based computers.*

```c
IO_CC_START_READ
IO_CC_START_WRITE
IO_CC_ABORT
IO_CC_ENABLE_DEVICE_INTERRUPTS
IO_CC_DISABLE_DEVICE_INTERRUPTS
IO_CC_ENABLE_INTERRUPTS
IO_CC_DISABLE_INTERRUPTS
IO_CC_CONNECT_FRAME_LOOP
IO_CC_DISCONNECT_FRAME_LOOP
IO_CDO_DONE
IO_CDO_ALL
IO_CDO_ENABLE_INTERRUPTS
IO_CDO_ENABLE_INTERRUPTS_IF_EMPTY
IO_CEO_END_OF_RECORD
IO_CEO_DESCRIPTOR_INTERRUPT
IO_CEO_ENABLE_INTERRUPTS
IO_CEO_DESCRIPTOR_COMMAND
IO_CEO_ENABLE_CHANNEL
IO_MAX_BOARD_SIZE
IO_MAX_NRW_SLOT_SIZE
IO_MAX_SLOT_SIZE
IO_NATIVE_SLOT_ID
IO_NO_CHANNEL
IO_NULL_SLOT_ID
IO_NULL_DEVICE_TYPE
IO_NULL_DEVICE_INDEX
IO_NULL_DMA_ID
IO_SLOT_DEVICE_TYPE
```

---

## Global Variables

### IODDMMasks
**Declared In:** `driverkit/debugging.h`
**Synopsis:** `unsigned int IODDMMasks[IO_NUM_DDM_MASKS]`
**Description:** Bitmask used to filter Driver Debugging Module events.

### IODMAStatusStrings
**Declared In:** `driverkit/driverTypes.h`
**Synopsis:** `const IONamedValue IODMAStatusStrings[]`
**Description:** Used with `IOFindNameForValue()` to convert `IODMAStatus` values into strings.
