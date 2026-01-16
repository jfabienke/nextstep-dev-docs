## Types and Constants

This section describes the types and constants defined by the Driver Kit.

### Defined Types

#### IOAddressRange

**Declared In:** driverkit/IODeviceInspector.h

**Synopsis:**

```c
typedef struct IOAddressRange {
    unsigned    start;
    unsigned    length;
} IOAddressRange;
```

**Description:** Used to describe address ranges.

---

#### IOCache

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
typedef enum {
    IO_CacheOff,
    IO_WriteThrough,
    IO_CopyBack
} IOCache;
```

**Description:** Used to specify caching. `IO_CacheOff` inhibits the cache.

---

#### IOChannelCommand

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
typedef unsigned int IOChannelCommand;
```

---

#### IOChannelDequeueOption

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
typedef unsigned int IOChannelDequeueOption;
```

---

#### IOChannelEnqueueOption

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
typedef unsigned int IOChannelEnqueueOption;
```

---

#### IOCharParameter

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
typedef char IOCharParameter[IO_MAX_PARAMETER_ARRAY_LENGTH];
```

**Description:** Standard type for a character parameter value, used by the get/set parameter functionality provided by IODevice and IODeviceMaster.

---

#### IODDMMsg

**Declared In:** driverkit/debuggingMsg.h

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

**Description:** The message format understood by the Driver Debugging Module. You don't usually have to use this message, as long as DDMViewer is sufficient for your needs.

---

#### IODescriptorCommand

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
typedef unsigned char IODescriptorCommand;
```

---

#### IODeviceNumber

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
typedef unsigned int IODeviceNumber;
```

---

#### IODeviceStyle

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
typedef enum {
    IO_DirectDevice,
    IO_IndirectDevice,
    IO_PseudoDevice
} IODeviceStyle;
```

**Description:** Returned by the `deviceStyle` method to specify whether the driver is a direct device driver (one that directly controls hardware), an indirect device driver (one that controls hardware using a direct device driver), or a pseudodriver (one that controls no hardware). The driver style determines how it's configured into the system.

---

#### IODisplayInfo

**Declared In:** bsd/dev/i386/displayDefs.h

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

**Description:** This structure describes a video display. Each linear mode supported by an IOFrameBufferDisplay has a corresponding IODisplayInfo. The structure's fields are:

- **width** – Width, in pixels
- **height** – Height, in pixels
- **totalWidth** – Width including undisplayed pixels
- **rowBytes** – The number of bytes to get from one scanline to next. To determine this value, determine how many 8-bit bytes each pixel occupies (rounding up to an integer) and multiply this by the value of totalWidth. For example, a color display mode that uses 15 bits per pixel and has a totalWidth of 1024 has a rowBytes value of 2048.
- **refreshRate** – Monitor refresh setting, in Hz
- **frameBuffer** – Pointer to origin of screen; untyped to force actual screen writes to be dependent on bitsPerPixel. The driver's `initFromDeviceDescription:` method should set this field to the value returned by `mapFrameBufferAtPhysicalAddress:length:`.
- **bitsPerPixel** – The memory space occupied by one pixel. 8-bit black and white display modes use the value `IO_8BitsPerPixel`, and "16-bit" color display modes that use 5 bits each for red, green, and blue use the value `IO_15BitsPerPixel`.
- **colorSpace** – Specifies the sample-encoding format. Typically, this value is either `IO_DISPLAY_ONEISWHITECOLORSPACE` (for monochrome modes) or `IO_DISPLAY_RGBCOLORSPACE` (for color modes).
- **flags** – Flags used to indicate special requirements or conditions to DPS. Currently, this should always be zero.
- **parameters** – Driver-specific parameters.

**Example:**

```c
static const IODisplayInfo MyModes[MY_NUM_MODES] = {
    { 1024, 768, 1024, 1024, 66, 0,
      IO_8BitsPerPixel, IO_DISPLAY_ONEISWHITECOLORSPACE, 0, 0 },
    { 1280, 1024, 2048, 2048, 68, 0,
      IO_8BitsPerPixel, IO_DISPLAY_ONEISWHITECOLORSPACE, 0, 0 },
    { 800, 600, 800, 1600, 72, 0,
      IO_15BitsPerPixel, IO_DISPLAY_RGBCOLORSPACE, 0, 0 },
    { 1024, 768, 1024, 2048, 72, 0,
      IO_15BitsPerPixel, IO_DISPLAY_RGBCOLORSPACE, 0, 0 }
};
```

---

#### IODMAAlignment

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
typedef struct {
    unsigned readStart;
    unsigned writeStart;
    unsigned readLength;
    unsigned writeLength;
} IODMAAlignment;
```

**Description:** Used to specify DMA alignment. A field value of 0 means that alignment isn't restricted for values corresponding to the field.

---

#### IODMABuffer

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
typedef void *IODMABuffer;
```

**Description:** Used as a machine-independent type for a machine-dependent DMA buffer.

**See also:** IOEISADMABuffer

---

#### IODMADirection

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
typedef enum {
    IO_DMARead,
    IO_DMAWrite
} IODMADirection;
```

**Description:** Used to specify the direction of DMA. `IO_DMARead` indicates a transfer from the device into system memory; `IO_DMAWrite` indicates a transfer from system memory to the device.

---

#### IODMAStatus

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
typedef enum {
    IO_None,
    IO_Complete,
    IO_Running,
    IO_Underrun,
    IO_BusError,
    IO_BufferError,
} IODMAStatus;
```

**Description:** Used to specify machine-independent DMA channel status.

- `IO_None` – No appropriate status
- `IO_Complete` – DMA channel idle
- `IO_Running` – DMA channel running
- `IO_Underrun` – Underrun or overrun
- `IO_BusError` – Bus error
- `IO_BufferError` – DMA buffer error

---

#### IODMATransferMode

**Declared In:** driverkit/i386/directDevice.h

**Synopsis:**

```c
typedef enum {
    IO_Demand,
    IO_Single,
    IO_Block,
    IO_Cascade,
} IODMATransferMode;
```

**Description:** Used only in the `setTransferMode:forChannel:` method of the EISA/ISA category of IODirectDevice.

---

#### IOEISADMABuffer

**Declared In:** driverkit/i386/driverTypes.h

**Synopsis:**

```c
typedef void *IOEISADMABuffer;
```

**Description:** Used as a machine-dependent type for a DMA buffer.

---

#### IOEISADMATiming

**Declared In:** driverkit/i386/directDevice.h

**Synopsis:**

```c
typedef enum {
    IO_Compatible,
    IO_TypeA,
    IO_TypeB,
    IO_Burst,
} IOEISADMATiming;
```

**Description:** Used only in the `setDMATiming:forChannel:` method of the EISA/ISA category of IODirectDevice.

---

#### IOEISADMATransferWidth

**Declared In:** driverkit/i386/directDevice.h

**Synopsis:**

```c
typedef enum {
    IO_8Bit,
    IO_16BitWordCount,
    IO_16BitByteCount,
    IO_32Bit,
} IOEISADMATransferWidth;
```

**Description:** Used only in the `setDMATransferWidth:forChannel:` method of the EISA/ISA category of IODirectDevice.

---

#### IOEISAInterruptHandler

**Declared In:** driverkit/i386/driverTypes.h

**Synopsis:**

```c
typedef void (*IOEISAInterruptHandler)(void *identity,
                                       void *state,
                                       unsigned int arg);
```

---

#### IOEISAPortAddress

**Declared In:** driverkit/i386/driverTypes.h

**Synopsis:**

```c
typedef unsigned short IOEISAPortAddress;
```

---

#### IOEISAStopRegisterMode

**Declared In:** driverkit/i386/directDevice.h

**Synopsis:**

```c
typedef enum {
    IO_StopRegisterEnable,
    IO_StopRegisterDisable,
} IOEISAStopRegisterMode;
```

**Description:** Used only in the `setStopRegisterMode:forChannel:` method of the EISA/ISA category of IODirectDevice.

---

#### IOIncrementMode

**Declared In:** driverkit/i386/directDevice.h

**Synopsis:**

```c
typedef enum {
    IO_Increment,
    IO_Decrement,
} IOIncrementMode;
```

**Description:** Used only in the `setIncrementMode:forChannel:` method of the EISA/ISA category of IODirectDevice.

---

#### IOInterruptHandler

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
typedef void (*IOInterruptHandler)(void *identity,
                                   void *state,
                                   unsigned int arg);
```

---

#### IOInterruptMsg

**Declared In:** driverkit/interruptMsg.h

**Synopsis:**

```c
typedef struct {
    msg_header_t header;
} IOInterruptMsg;
```

**Description:** The format of the message sent by the kernel to a driver's interrupt handler.

---

#### IOIntParameter

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
typedef int IOIntParameter[IO_MAX_PARAMETER_ARRAY_LENGTH];
```

**Description:** Standard type for an integer parameter value, used by the get/set parameter functionality provided by IODevice and IODeviceMaster.

---

#### IOIPCSpace

**Declared In:** driverkit/kernelDriver.h

**Synopsis:**

```c
typedef enum {
    IO_Kernel,
    IO_KernelIOTask,
    IO_CurrentTask
} IOIPCSpace;
```

**Description:** Used only by the `IOConvertPort()` function to specify which space to convert the port from and to.

---

#### IONamedValue

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
typedef struct {
    int value;
    const char *name;
} IONamedValue;
```

**Description:** Map between constants or enumerations and text description.

---

#### IOObjectNumber

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
typedef unsigned int IOObjectNumber;
```

---

#### IOParameterName

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
typedef char IOParameterName[IO_MAX_PARAMETER_NAME_LENGTH];
```

**Description:** Standard type for a parameter name, used by the get/set parameter functionality provided by IODevice and IODeviceMaster.

---

#### IORange

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
typedef struct range {
    unsigned int start;
    unsigned int size;
} IORange;
```

**Description:** Indicates a range of values. Used for memory regions, port regions, and so on.

---

#### IOReturn

**Declared In:** driverkit/return.h

**Synopsis:**

```c
typedef int IOReturn;
```

**Description:** IOReturn values are returned by many Driver Kit classes.

---

#### IOSCSIRequest

**Declared In:** driverkit/scsiTypes.h

**Synopsis:**

```c
typedef struct {
    unsigned char target;
    unsigned char lun;
    cdb_t cdb;
    BOOL read;
    int maxTransfer;
    int timeoutLength;
    unsigned disconnect:1;
    unsigned pad:31;
    sc_status_t driverStatus;
    unsigned char scsiStatus;
    int bytesTransferred;
    ns_time_t totalTime;
    ns_time_t latentTime;
    esense_reply_t senseData;
} IOSCSIRequest;
```

**Description:** Used in the IOSCSIController protocol's `executeRequest:buffer:client:` method.

---

#### IOString

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
typedef char IOString[IO_STRING_LENGTH];
```

**Description:** Standard type for an ASCII name, such as a device's name or type.

---

#### IOSwitchFunc

**Declared In:** driverkit/devsw.h

**Synopsis:**

```c
typedef int (*IOSwitchFunc)();
```

**Description:** Used by `IOAddToBdevsw()` and `IOAddToCdevsw()` to specify UNIX-style entry points into a driver.

---

#### IOThread

**Declared In:** driverkit/generalFuncs.h

**Synopsis:**

```c
typedef void *IOThread;
```

**Description:** An opaque type used by the general-purpose functions to represent a thread.

---

#### IOThreadFunc

**Declared In:** driverkit/generalFuncs.h

**Synopsis:**

```c
typedef void (*IOThreadFunc)(void *arg);
```

**Description:** Used by the general-purpose functions to specify the function that a thread should execute.

---

### Symbolic Constants

#### Length Constants

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
IO_STRING_LENGTH
IO_MAX_PARAMETER_NAME_LENGTH
IO_MAX_PARAMETER_ARRAY_LENGTH
```

**Description:** These constants are used to determine the maximum length of the following types:

- `IO_STRING_LENGTH` – IOString
- `IO_MAX_PARAMETER_NAME_LENGTH` – IOParameterName
- `IO_MAX_PARAMETER_ARRAY_LENGTH` – IOIntParameter, IOCharParameter

---

#### Debugging String Length

**Declared In:** driverkit/debuggingMsg.h

**Synopsis:**

```c
IO_DDM_STRING_LENGTH
```

**Description:** The length of the string field in an IODebuggingMsg.

---

#### Debugging Messages

**Declared In:** driverkit/debuggingMsg.h

**Synopsis:**

| Constant | Meaning |
|----------|---------|
| `IO_DDM_MSG_BASE` | The lowest ID an IODebuggingMsg can have |
| `IO_LOCK_DDM_MSG` | Lock the Driver Debugging Module (DDM) |
| `IO_UNLOCK_DDM_MSG` | Unlock the DDM |
| `IO_GET_DDM_ENTRY_MSG` | Get an entry from the DDM |
| `IO_SET_DDM_MASK_MSG` | Set the debugging mask for the DDM |
| `IO_CLEAR_DDM_MSG` | Clear all entries from the DDM |

**Description:** Values for the `header.msg_id` field of an IODebuggingMsg.

---

#### Return Values from the DDM

**Declared In:** driverkit/debuggingMsg.h

**Synopsis:**

| Constant | Meaning |
|----------|---------|
| `IO_DDM_SUCCESS` | The message was received and understood |
| `IO_NO_DDM_BUFFER` | The DDM has no entry at the specified offset |
| `IO_BAD_DDM_INDEX` | The specified index isn't valid |

**Description:** Values for the status field of an IODebuggingMsg.

---

#### DDM Masks

**Declared In:** driverkit/debugging.h

**Synopsis:**

```c
IO_NUM_DDM_MASKS
```

**Description:** This constant specifies the number of masks used by the Driver Debugging Module.

---

#### Interrupt Messages

**Declared In:** driverkit/interruptMsg.h

**Synopsis:**

| Constant | Meaning |
|----------|---------|
| `IO_INTERRUPT_MSG_ID_BASE` | The lowest ID an IOInterruptMsg can have |
| `IO_TIMEOUT_MSG` | Timeout message |
| `IO_COMMAND_MSG` | Command message |
| `IO_DEVICE_INTERRUPT_MSG` | Sent by the kernel when an interrupt occurs |
| `IO_DMA_INTERRUPT_MSG` | DMA interrupt message |
| `IO_FIRST_UNRESERVED_INTERRUPT_MSG` | First unreserved interrupt message ID |

**Description:** Values for the `header.msg_id` field of an IOInterruptMsg.

---

#### IOReturn Constants

**Declared In:** driverkit/return.h

**Synopsis:**

| Constant | Meaning |
|----------|---------|
| `IO_R_SUCCESS` | No error occurred |
| `IO_R_NO_MEMORY` | Couldn't allocate memory |
| `IO_R_RESOURCE` | Resource shortage |
| `IO_R_VM_FAILURE` | Miscellaneous virtual memory failure |
| `IO_R_INTERNAL` | Internal library error |
| `IO_R_RLD` | Error in loading a relocatable file |
| `IO_R_IPC_FAILURE` | Error during IPC |
| `IO_R_NO_CHANNELS` | No DMA channels are available |
| `IO_R_NO_SPACE` | No address space is available for mapping |
| `IO_R_NO_DEVICE` | No such device |
| `IO_R_PRIVILEGE` | Privilege/access violation |
| `IO_R_INVALID_ARG` | Invalid argument |
| `IO_R_BAD_MSG_ID` | Bad message ID |
| `IO_R_UNSUPPORTED` | Unsupported function |
| `IO_R_INVALID` | Should never be seen |
| `IO_R_LOCKED_READ` | Device is read locked |
| `IO_R_LOCKED_WRITE` | Device is write locked |
| `IO_R_EXCLUSIVE_ACCESS` | Device is exclusive access and is already open |
| `IO_R_CANT_LOCK` | Can't acquire requested lock |
| `IO_R_NOT_OPEN` | Device not open |
| `IO_R_OPEN` | Device is still open |
| `IO_R_NOT_READABLE` | Reading not supported |
| `IO_R_NOT_WRITABLE` | Writing not supported |
| `IO_R_IO` | General I/O error |
| `IO_R_BUSY` | Device is busy |
| `IO_R_NOT_READY` | Device isn't ready |
| `IO_R_OFFLINE` | Device is off line |
| `IO_R_ALIGN` | DMA alignment error |
| `IO_R_MEDIA` | Media error |
| `IO_R_DMA` | DMA failure |
| `IO_R_TIMEOUT` | I/O timeout |
| `IO_R_NOT_ATTACHED` | The device or channel isn't attached |
| `IO_R_PORT_EXISTS` | The device port already exists |
| `IO_R_CANT_WIRE` | Can't wire down physical memory |
| `IO_R_NO_INTERRUPT` | No interrupt port is attached |
| `IO_R_NO_FRAMES` | No DMA is enqueued |

**Description:** Values for IOReturns.

---

#### IODevice Parameter Names

**Declared In:** driverkit/IODevice.h

**Synopsis:**

| Constant | Meaning |
|----------|---------|
| `IO_CLASS_NAME` | The value returned by `+ name` |
| `IO_DEVICE_NAME` | The value returned by `– name` |
| `IO_DEVICE_KIND` | The value returned by `– deviceKind` |
| `IO_UNIT` | The value returned by `– unit` |

---

#### Null Constants

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
#define NULL 0
#define IO_NULL_VM_TASK    ((vm_task_t)0)
```

**Description:** Standard null values, used in various places.

---

### Global Variables

#### IODDMMasks

**Declared In:** driverkit/debugging.h

**Synopsis:**

```c
unsigned int IODDMMasks[IO_NUM_DDM_MASKS];
```

**Description:** The bitmask used to filter storing of debugging events. See the discussion of the Driver Debugging Module in Chapter 2 for more information.

---

#### IODMAStatusStrings

**Declared In:** driverkit/driverTypes.h

**Synopsis:**

```c
const IONamedValue IODMAStatusStrings[];
```

**Description:** Used as an argument to `IOFindNameForValue()` to convert an IODMAStatus value into an error string.

---

