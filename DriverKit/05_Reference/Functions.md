## Functions

This section describes three types of functions and macros:

- **General-purpose functions** – to perform basic tasks
- **Driver Debugging Module (DDM) functions** – to help all drivers keep debugging information
- **Miscellaneous functions** – such as DMA alignment macros, functions that work only in the kernel, and functions specific to a particular machine architecture

Unless noted otherwise, all of the functions described in this section work in both user-level and kernel-level drivers.

### Other Functions Available to Drivers

Almost all Mach functions are available to kernel-level device drivers. If you don't find the appropriate functionality in a method or function, you can use a Mach function. For example, `port_allocate()` and `msg_send()` are used by many drivers.

**Note:** Instead of including the header file `mach/mach.h`, you must include `mach/mach_user_internal.h` and `mach/mach_interface.h`.

The `host_priv_self()` Mach function does not work in the kernel. You should use `IOHostPrivSelf()` instead.

### General-Purpose Functions

The general-purpose functions, defined in the header file `driverkit/generalFuncs.h`, provide a consistent interface for device drivers that may have to run in kernel space at one time (or in one configuration) and in user space at another time. Using these functions minimizes the work of porting between the two environments. All the Driver Kit classes, as well all NeXT kernel-level drivers that use the Driver Kit, were written using these functions so that they have one set of source files with minimal kernel and user mode differences.

**Warning:** Before using any of the general-purpose functions, each user-level driver must call `IOInitGeneralFuncs()`. (Kernel-level drivers don't need to call it.)

#### Thread Functions

These functions provide the functionality of the C-thread functions in a uniform way in both user and kernel space.

- `IOForkThread()`
- `IOSuspendThread()`
- `IOResumeThread()`
- `IOExitThread()`

#### Timer Functions

- `IOSleep()`
- `IODelay()`
- `IOScheduleFunc()`
- `IOUnscheduleFunc()`
- `IOGetTimestamp()`

#### Memory Allocation and Copying Functions

- `IOCopyMemory()`
- `IOMalloc()`
- `IOFree()`

#### Miscellaneous General-Purpose Functions

- `IOInitGeneralFuncs()`
- `IOFindNameForValue()`
- `IOFindValueForName()`
- `IOLog()`
- `IOPanic()`

### Driver Debugging Module (DDM) Functions

See the "Adding Debugging Code" section in Chapter 2 for information on using the DDM.

- `IOAddDDMEntry()`
- `IOClearDDM()`
- `IOCopyString()`
- `IODEBUG()`
- `IOGetDDMEntry()`
- `IOGetDDMMask()`
- `IOInitDDM()`
- `IONsTimeFromDDMMsg()`
- `IOSetDDMMask()`

### Miscellaneous Functions

#### Kernel-Only Functions

The function `IOConvertPort()` is necessary for some kernel-level drivers—and not for user-level drivers—because kernel-level drivers can execute in more than one task. The first thread of a kernel-level driver executes in the loadable kernel server's task, any threads that the driver creates execute in the kernel I/O task, and network drivers and drivers with UNIX entry points (at some stage) can execute in the context of an unknown task.

`IOGetObjectForDeviceName()` provides to kernel-level drivers some of the functionality provided to user-level programs by IODeviceMaster. Similarly, `IOHostPrivSelf()` is used by some kernel-level drivers that need the information normally returned by `host_priv_self()` (which is one of the few Mach functions that doesn't work in the kernel).

The function `IOVmTaskSelf()` supplies a `vm_task_t` for Mach function calls that expect one for the kernel; this is necessary because `vm_task_t` and `task_t` aren't the same in the kernel (as they are at user level). `IOVmTaskCurrent()` supplies a `vm_task_t` that's needed by some UNIX-style drivers. Finally, `IOVmTaskForBuf()` supplies a `vm_task_t` for the unknown task that is requesting UNIX-style I/O.

- `IOConvertPort()`
- `IOGetObjectForDeviceName()`
- `IOHostPrivSelf()`
- `IOPhysicalFromVirtual()`
- `IOSetUNIXError()`
- `IOVmTaskCurrent()`
- `IOVmTaskForBuf()`
- `IOVmTaskSelf()`

#### DMA Alignment Macros

- `IOAlign()`
- `IOIsAligned()`

#### Architecture-Specific Functions

The following functions are used by some Intel drivers to read and write I/O ports:

- `inb()`
- `inw()`
- `inl()`
- `outb()`
- `outw()`
- `outl()`

Some Intel drivers use the following functions to help handle interrupts:

- `IODisableInterrupt()`
- `IOEnableInterrupt()`
- `IOSendInterrupt()`

Some Intel devices require memory in the low 16 MB:

- `IOMallocLow()`
- `IOFreeLow()`

Intel display drivers often use the following functions to read and write VGA registers:

- `IOReadRegister()`
- `IOReadModifyWriteRegister()`
- `IOWriteRegister()`

Some Intel kernel drivers need to map physical addresses:

- `IOMapPhysicalIntoIOTask()`
- `IOUnmapPhysicalFromIOTask()`

#### UNIX Device Switch Functions

- `IOAddToBdevsw()`
- `IOAddToCdevsw()`
- `IOAddToVfssw()`
- `IORemoveFromBdevsw()`
- `IORemoveFromCdevsw()`
- `IORemoveFromVfssw()`

---

### Function Specifications

#### inb(), inw(), inl(), outb(), outw(), outl()

**Summary:** Read or write data to an I/O port

**Declared In:** driverkit/i386/ioPorts.h

**Synopsis:**

```c
unsigned char inb(unsigned int address)
unsigned short inw(unsigned int address)
unsigned long inl(unsigned int address)
void outb(unsigned int address, unsigned char data)
void outw(unsigned int address, unsigned short data)
void outl(unsigned int address, unsigned long data)
```

**Description:**

These inline functions let drivers read and write I/O ports on Intel-based computers. Use `inb()` to read a byte at the I/O port *address*. Use `inw()` to read the two bytes at *address* and *address* + 1, and `inl()` to read four bytes starting at *address*. To write a byte, use `outb()`; to write two bytes (to *address* and *address* + 1), use `outw()`; to write four bytes, use `outl()`.

These functions have nothing to do with main memory; they work only for the 64 kilobytes of I/O address space on an Intel-based computer. These functions use the special machine instructions that are necessary for reading and writing data from and to the I/O space.

**Note:** These functions work only at kernel level and only on Intel-based computers.

**Example:**

```c
temp_cr = inb(base+CR);  /* get current CR value */
```

---

#### IOAddDDMEntry()

**Summary:** Add one entry to the Driver Debugging Module

**Declared In:** driverkit/debugging.h

**Synopsis:**

```c
void IOAddDDMEntry(char *format, int arg1, int arg2, int arg3,
                   int arg4, int arg5)
```

**Description:**

This is the exported function that is used to add events to the DDM's circular buffer. However, drivers typically don't use this directly; instead, they should use macros that call `IOAddDDMEntry()` conditionally based on the current state of debugging flags. See the description of `IODEBUG()` for examples.

**Note:** The last 5 arguments to this function are typed above as int, but they are really untyped and could be any 32-bit quantity. They are stored in the debugging log as int but are eventually evaluated as arguments to `sprintf()`, so they could be int, char, short, or pointers to a string. See `IOCopyString()`, later in this section, for information on passing string pointers to `IOAddDDMEntry()`.

**See also:** `IODEBUG()`

---

#### IOAddToBdevsw(), IOAddToCdevsw(), IOAddToVfssw()

**Summary:** Add UNIX-style entry points to a device switch table

**Declared In:** driverkit/devsw.h

**Synopsis:**

```c
int IOAddToBdevsw(IOSwitchFunc openFunc,
                  IOSwitchFunc closeFunc,
                  IOSwitchFunc strategyFunc,
                  IOSwitchFunc dumpFunc,
                  IOSwitchFunc psizeFunc,
                  BOOL isTape)

int IOAddToCdevsw(IOSwitchFunc openFunc,
                  IOSwitchFunc closeFunc,
                  IOSwitchFunc readFunc,
                  IOSwitchFunc writeFunc,
                  IOSwitchFunc ioctlFunc,
                  IOSwitchFunc stopFunc,
                  IOSwitchFunc resetFunc,
                  IOSwitchFunc selectFunc,
                  IOSwitchFunc mmapFunc,
                  IOSwitchFunc getcFunc,
                  IOSwitchFunc putcFunc)

int IOAddToVfssw(const char *vfsswName,
                 const struct vfsops *vfsswOps)
```

**Description:**

These functions find a free row in a device switch table and add the specified entry points. Each function returns the major number (equivalent to the row number) for the device, or -1 if the device couldn't be added to the table.

**Note:** You should use IODevice's `addToBdevsw...` and `addToCdevsw...` methods instead of `IOAddToBdevsw()` and `IOAddToCdevsw()`, whenever possible.

**See also:** `IORemoveFromBdevsw()`, `IORemoveFromCdevsw()`, `IORemoveFromVfssw()`

---

#### IOAlign()

**Summary:** Truncate an address so that it's aligned to a buffer size

**Declared In:** driverkit/align.h

**Synopsis:**

```c
type IOAlign(type, address, bufferSize)
```

**Description:**

This macro truncates *address* to a multiple of *bufferSize*.

**See also:** `IOIsAligned()`

---

#### IOClearDDM()

**Summary:** Clear the Driver Debugging Module's entries

**Declared In:** driverkit/debugging.h

**Synopsis:**

```c
void IOClearDDM()
```

**Description:**

This function empties the DDM's circular buffer.

---

#### IOConvertPort()

**Summary:** Convert a port name from one IPC space to another

**Declared In:** driverkit/kernelDriver.h

**Synopsis:**

```c
port_t IOConvertPort(port_t port, IOIPCSpace from, IOIPCSpace to)
```

**Description:**

This function lets a kernel driver convert a port name (*port*) so that the port can be used in a different IPC space. Three types of conversion are supported:

- From the current task's IPC space to the kernel I/O task's space
- From the kernel's IPC space to the kernel I/O task's space
- From the kernel I/O task's IPC space to kernel's IPC space

The arguments *from* and *to* should each be specified as one of the following: `IO_Kernel`, `IO_KernelIOTask`, or `IO_CurrentTask`. For example, the following code converts a port name from the current task's name to the name used by the kernel I/O task.

```c
ioTaskPort = IOConvertPort(aPort, IO_CurrentTask, IO_KernelIOTask);
```

**Note:** This function works only in kernel-level drivers.

**Return:** Returns the port's name in the *to* space. Specifying an invalid conversion results in a return value of `PORT_NULL`.

---

#### IOCopyMemory()

**Summary:** Copy memory using the specified transfer width

**Declared In:** driverkit/generalFuncs.h

**Synopsis:**

```c
void IOCopyMemory(void *from, void *to, unsigned int numBytes,
                  unsigned int bytesPerTransfer)
```

**Description:**

Copies memory 1, 2, or 4 bytes at a time (as specified by *bytesPerTransfer*) until *numBytes* bytes starting at *from* have been copied to *to*. The *from* and *to* buffers must not overlap.

This function is useful when devices have mapped memory that can be accessed in only 8-bit or 16-bit quantities. In these situations, `bcopy()` isn't appropriate, since it assumes 32-bit access to all memory involved.

If *from* is not aligned on a *bytesPerTransfer* boundary, `IOCopyMemory()` performs 8-bit transfers until it has reached a *bytesPerTransfer* boundary. Similarly, if the end of the *from* buffer extends past a *bytesPerTransfer* boundary, the remaining memory is copied 8 bits at a time.

---

#### IOCopyString()

**Summary:** Return a copy of the specified string

**Declared In:** driverkit/debugging.h

**Synopsis:**

```c
const char *IOCopyString(const char *instring)
```

**Description:**

This function is required when you want to use a pointer to a string whose existence is transitory as an argument. The reason for this is that the string won't be read until the Driver Debugging Module's buffer is examined, which could be a long time (minutes or more) after the call to `IOAddDDMEntry()`. By then, the string pointer passed to `IOAddDDMEntry()` might no longer point to a useful string.

**Warning:** The string returned by this function is created with `IOMalloc()` and is never freed. Use this function with discretion.

---

#### IODEBUG()

**Summary:** Conditionally add one entry to the Driver Debugging Module

**Declared In:** driverkit/debugging.h

**Synopsis:**

```c
void IODEBUG(int index, int mask, char *format, int arg1, int arg2,
             int arg3, int arg4, int arg5)
```

**Description:**

This macro is used to add entries to the DDM's circular buffer. The entry is added only if both of the following are true:

- The C preprocessor flag `DDM_DEBUG` is defined.
- A bitwise and operation performed on *mask* and `IODDMMasks[index]` results in a nonzero result.

`IODEBUG()` is typically used to define other macros specific to a driver, as shown in the following example.

**Example:**

```c
#define MY_INDEX      0

#define MY_INPUT       0x00000001
#define MY_OUTPUT      0x00000002
#define MY_OTHER       0x00000004

#define logInput(x, a, b, c, d, e) \
    IODEBUG(MY_INDEX, MY_INPUT, x, a, b, c, d, e)

#define logOutput(x, a, b, c, d, e) \
    IODEBUG(MY_INDEX, MY_OUTPUT, x, a, b, c, d, e)

#define logOther(x, a, b, c, d, e) \
    IODEBUG(MY_INDEX, MY_OTHER, x, a, b, c, d, e)

. . .
IODDMMasks[MY_INDEX] = MY_INPUT | MY_OUTPUT;
. . .
logInput("Input error %d: %s\n", error,
         IOFindNameForValue(error, &errorList));
```

---

#### IODelay()

**Summary:** Wait (without blocking) for the indicated number of microseconds

**Declared In:** driverkit/generalFuncs.h

**Synopsis:**

```c
void IODelay(unsigned int microseconds)
```

**Description:**

This is a quick, nonblocking version of `IOSleep()`.

**Note:** This function guarantees a minimum "spin" delay in the user-level version; due to thread scheduling, the call to `IODelay()` could take much longer than the indicated time. This should not be a problem with properly designed user-level drivers as this is a common real-time constraint on all user-level code.

---

#### IODisableInterrupt()

**Summary:** Prevent interrupt messages from being sent

**Declared In:** driverkit/IODirectDevice.h

**Synopsis:**

```c
void IODisableInterrupt(void *identity)
```

**Description:**

This function allows handlers of non-shared interrupts to indicate that the interrupt should be left disabled on return from the interrupt handler.

The *identity* argument should be set to the value that the interrupt handler received in its own arguments.

**Note:** `IODisableInterrupt()` must be called inside a special interrupt handler function. It can't be called from any other context.

**See also:** `IOEnableInterrupt()`, `IOSendInterrupt()`

---

#### IOEnableInterrupt()

**Summary:** Allow interrupt messages to be sent

**Declared In:** driverkit/IODirectDevice.h

**Synopsis:**

```c
void IOEnableInterrupt(void *identity)
```

**Description:**

This function allows interrupt handlers to indicate that the interrupt should be reenabled on return from the interrupt handler. You should only re-enable the interrupt after removing the source of the interrupt—by clearing the interrupt status register on the device, or by using whatever mechanism is necessary for the hardware your driver controls.

The *identity* argument should be set to the value that the interrupt handler received in its own arguments.

**Note:** `IOEnableInterrupt()` must be called inside a special interrupt handler function. It can't be called from any other context.

**See also:** `IODisableInterrupt()`, `IOSendInterrupt()`

---

#### IOExitThread()

**Summary:** Terminate the execution of the current thread

**Declared In:** driverkit/generalFuncs.h

**Synopsis:**

```c
volatile void IOExitThread()
```

**Description:**

This function terminates the execution of the current (calling) thread. Note that there's no way for one thread to kill another thread other than by sending some kind of message to the soon-to-be-terminated thread instructing it to kill itself.

**Note:** In the user-level implementation, the main C thread (the first thread in the task) doesn't exit until all other C threads in the task have exited.

---

#### IOFindNameForValue(), IOFindValueForName()

**Summary:** Convert an integer to a string, or vice versa, using an IONamedValues array

**Declared In:** driverkit/generalFuncs.h

**Synopsis:**

```c
const char *IOFindNameForValue(int value, const IONamedValues *array)

IOReturn IOFindValueForName(const char *string,
                           const IONamedValue *array,
                           int *value)
```

**Description:**

These functions are the primary use of the `IONamedValues` data type, which maps integer values to strings. `IOFindNameForValue()` maps a given integer value to a string, given a pointer to an array of `IONamedValues`. `IOFindValueForName()` maps a given string into an integer, returning the integer in *value*.

One typical use for `IOFindNameForValue()` is to map integer return values into error strings. IODevice's `IOStringFromReturn:` method performs this function. A subclass that defines additional `IOReturn` values should override this method and call `[super IOReturnToString:]` if the specified value does not match one of the class-specific IOReturns.

**Return:** `IOFindNameForValue()` returns the string corresponding to *value*, or a string indicating that *value* is undefined if the integer wasn't found. `IOFindValueForName()` returns `IO_R_SUCCESS` if it finds the specified string; otherwise, it returns `IO_R_INVALIDARG`.

---

#### IOForkThread()

**Summary:** Start a new thread

**Declared In:** driverkit/generalFuncs.h

**Synopsis:**

```c
IOThread IOForkThread(IOThreadFunc function, void *arg)
```

**Description:**

This function causes a new thread to be started up. For kernel-level drivers, the new thread is in the IOTask's address space; for user-level drivers, the thread is in the current task. The thread begins execution at *function*, which is passed *arg* as its argument.

---

#### IOFree()

**Summary:** Free memory allocated by IOMalloc()

**Declared In:** driverkit/generalFuncs.h

**Synopsis:**

```c
void IOFree(void *var, int numBytes)
```

**Description:**

This function frees memory allocated by `IOMalloc()`.

**Note:** You must use the same value for *numBytes* as you used for the call to `IOMalloc()` that allocated the memory you're now freeing.

---

#### IOFreeLow()

**Summary:** Free memory allocated by IOMallocLow()

**Declared In:** driverkit/i386/kernelDriver.h

**Synopsis:**

```c
void IOFreeLow(void *var, int numBytes)
```

**Description:**

This function frees memory allocated by `IOMallocLow()`.

**Note:** This function works only in kernel-level drivers.

---

#### IOGetDDMEntry()

**Summary:** Obtain an entry from the Driver Debugging Module

**Declared In:** driverkit/debugging.h

**Synopsis:**

```c
int IOGetDDMEntry(int entry, int outStringSize, char *outString,
                  ns_time_t *timestamp, int *cpuNumber)
```

**Description:**

Returns in *outString* an entry from the DDM. The *entry* argument should indicate which entry to return, counting backwards from the most recent entry. The *timestamp* argument is set to a value indicating the time at which the entry was logged. The *cpuNumber* argument is set to the number of the CPU that the retrieved entry is associated with.

**Return:** Returns a nonzero value if the specified entry doesn't exist. Otherwise, returns zero.

---

#### IOGetDDMMask()

**Summary:** Returns the specified bitmask word

**Declared In:** driverkit/debugging.h

**Synopsis:**

```c
unsigned IOGetDDMMask(int index)
```

**Description:**

This is typically not used by drivers; it provides a procedural means of obtaining a specified bitmask value. For performance reasons, the macros that filter and call `IOAddDDMEntry()` typically read the index words directly (the `IODDMMasks` array is a global variable).

---

#### IOGetObjectForDeviceName()

**Summary:** Obtain the id of a kernel device, given its name

**Declared In:** driverkit/kernelDriver.h

**Synopsis:**

```c
IOReturn IOGetObjectForDeviceName(IOString deviceName, id *deviceId)
```

**Description:**

This function provides a simple mapping of device names to objects. Since this is valid only at kernel level, no security mechanism is provided; any kernel code can get the id of any kernel IODevice.

**Note:** This function works only in kernel-level drivers.

**Return:** Returns `IO_DR_NOT_ATTACHED` if *deviceName* isn't found; otherwise returns `IO_R_SUCCESS`.

---

#### IOGetTimestamp()

**Summary:** Obtains a microsecond-accurate current timestamp

**Declared In:** driverkit/generalFuncs.h

**Synopsis:**

```c
void IOGetTimestamp(ns_time_t *nsp)
```

**Description:**

This function obtains a quick, microsecond-accurate, system-wide timestamp.

---

#### IOHostPrivSelf()

**Summary:** Returns the kernel I/O task's version of the privileged host port

**Declared In:** driverkit/kernelDriver.h

**Synopsis:**

```c
port_t IOHostPrivSelf()
```

**Description:**

This function is necessary because the Mach function `host_priv_self()` doesn't work at kernel level.

**Note:** This function works only in kernel-level drivers. In user-level drivers, use `host_priv_self()` instead.

---

#### IOInitDDM()

**Summary:** Initialize the Driver Debugging Module

**Declared In:** driverkit/debugging.h

**Synopsis:**

```c
/* Kernel level: */
void IOInitDDM(int numBufs)

/* User level: */
void IOInitDDM(int numBufs, char *serverPortName)
```

**Description:**

This function must be called once by your driver before calling any other DDM functions.

---

#### IOInitGeneralFuncs()

**Summary:** Initialize the general-purpose functions

**Declared In:** driverkit/generalFuncs.h

**Synopsis:**

```c
void IOInitGeneralFuncs()
```

**Description:**

Each user-level driver must call `IOInitGeneralFuncs()` once before calling any other functions declared in the `driverkit/generalFuncs.h` header file.

**Note:** Kernel-level drivers don't need to call this function, because it's automatically called by the kernel.

---

#### IOIsAligned()

**Summary:** Determine whether an address is aligned

**Declared In:** driverkit/align.h

**Synopsis:**

```c
unsigned int IOIsAligned(address, bufferSize)
```

**Description:**

This macro returns a nonzero value if *address* is a multiple of *bufferSize*; otherwise, it returns 0.

---

#### IOLog()

**Summary:** Adds a string to the system log

**Declared In:** driverkit/generalFuncs.h

**Synopsis:**

```c
void IOLog(const char *format, ...)
```

**Description:**

This is the Driver Kit's substitute for `printf()`; its implementation is similar to `syslog()`. `IOLog()` logs the string to `/usr/adm/messages` by default; you can specify another destination in the configuration file `/etc/syslog.conf`. The arguments are stdargs, just as for `printf()`. This function doesn't block on single-processor systems. It runs at level `LOG_ERR` and its facility is `kern`.

**See also:** `printf(3)` UNIX manual page, `syslog(3)` UNIX manual page

---

#### IOMalloc()

**Summary:** Standard memory allocator

**Declared In:** driverkit/generalFuncs.h

**Synopsis:**

```c
void *IOMalloc(int numBytes)
```

**Description:**

This function causes *numBytes* bytes of memory to be allocated; a pointer to the memory is returned. No guarantees exist as to the alignment or the physical contiguity of the allocated memory, but when `IOMalloc()` is called at kernel-level, the allocated memory is guaranteed to be wired down. Memory allocated with `IOMalloc()` should be freed with `IOFree()`.

**Warning:** If no memory is available, `IOMalloc()` blocks until it can obtain memory. For this reason, you shouldn't call `IOMalloc()` from a direct interrupt handler.

Drivers that can control (directly or indirectly) disks, network cards, or other devices used by a file system can run into a deadlock situation if they use `IOMalloc()` during I/O. This deadlock can occur when the pageout daemon attempts to free memory by moving pages out to disk. When the pageout daemon requests this I/O and the driver uses `IOMalloc()` to request more memory than is available, `IOMalloc()` blocks. The result is deadlock: the driver can't perform the I/O until memory is freed, and the memory can't be freed by the pageout daemon until the I/O happens. In general, a driver can avoid this deadlock by not allocating large amounts of memory during I/O. For example, allocating less than 100 bytes is safe, but allocating 8K bytes is very unsafe.

---

#### IOMallocLow()

**Summary:** Allocates memory in the low 16MB of the computer's memory range

**Declared In:** driverkit/i386/kernelDriver.h

**Synopsis:**

```c
void *IOMallocLow(int numBytes)
```

**Description:**

This function acts like `IOMalloc()`, except that the allocated range of memory is guaranteed to be in the low 16MB of system memory and to be physically contiguous. This function is provided because some cards for Intel-based computers must be mapped to low memory. Memory allocated with `IOMallocLow()` should be freed with `IOFreeLow()`.

**Note:** This function works only in kernel-level drivers running on Intel-based computers.

---

#### IOMapPhysicalIntoIOTask()

**Summary:** Map a physical address range into your IOTask's address space

**Declared In:** driverkit/kernelDriver.h

**Synopsis:**

```c
IOReturn IOMapPhysicalIntoIOTask(unsigned physicalAddress,
                                unsigned length,
                                vm_address_t *virtualAddress)
```

**Description:**

This function maps a range of physical memory into your IOTask. It returns the virtual address at which the range is mapped in the *virtualAddress* argument.

**Note:** This function works only in kernel-level drivers.

**Return:** Returns an error if the specified physical range could not be mapped; otherwise, returns `IO_R_SUCCESS`.

**See also:** `IOUnmapPhysicalFromIOTask()`

---

#### IONsTimeFromDDMMsg()

**Summary:** Extracts the time from a Driver Debugging Module message

**Declared In:** driverkit/debuggingMsg.h

**Synopsis:**

```c
ns_time_t IONsTimeFromDDMMsg(IODDMMsg *msg)
```

**Description:**

This inline function combines the `timestampHighInt` and `timestampLowInt` fields from *msg* and returns the result.

---

#### IOPanic()

**Summary:** Panic or dump memory after logging a string to the console

**Declared In:** driverkit/generalFuncs.h

**Synopsis:**

```c
void IOPanic(const char *reason)
```

**Description:**

The *reason* argument is logged to the console, after which either a kernel panic (if in kernel space) or a memory dump (if in user space) occurs.

**Note:** Use of this function is an extreme measure. Use `IOPanic()` only when continued execution may cause system corruption.

---

#### IOPhysicalFromVirtual()

**Summary:** Find the physical address corresponding to a virtual address

**Declared In:** driverkit/kernelDriver.h

**Synopsis:**

```c
IOReturn IOPhysicalFromVirtual(vm_task_t task,
                              vm_address_t virtualAddress,
                              unsigned int *physicalAddress)
```

**Description:**

This function gets the physical address (if any) that corresponds to *virtualAddress*. It returns `IO_R_INVALID_ARG` if no physical address corresponds to *virtualAddress*. On success, it returns `IO_R_SUCCESS`. If *virtualAddress* is in the current task, then the *task* argument should be set to `IOVmTaskSelf()`. This function will never block. Use this function only to find the physical address of wired down memory since the physical address of unwired down memory might change over time.

**Note:** This function is available only at kernel level. This function shouldn't be used in a custom interrupt handler—it can't run at the interrupt level.

---

#### IOReadRegister(), IOWriteRegister(), IOReadModifyWriteRegister()

**Summary:** Read or write values of display registers

**Declared In:** driverkit/i386/displayRegisters.h

**Synopsis:**

```c
unsigned char IOReadRegister(IOEISAPortAddress port,
                            unsigned char index)

void IOWriteRegister(IOEISAPortAddress port,
                    unsigned char index,
                    unsigned char value)

void IOReadModifyWriteRegister(IOEISAPortAddress port,
                              unsigned char index,
                              unsigned char protect,
                              unsigned char value)
```

**Description:**

These inline functions perform operations commonly used to read or write display registers. `IOReadRegister()` reads and returns the value of the register specified by *port* and *index*. `IOWriteRegister()` writes *value* to the register specified by *port* and *index*. `IOReadModifyWriteRegister()` reads the specified register, zeroes every bit that isn't set in the *protect* mask, sets every bit that's set in *value*, and sets the register to the new value. When the *protect* mask is zero, the effect is to set the register to *value*.

**Note:** These functions are supported only on Intel-based computers.

---

#### IORemoveFromBdevsw(), IORemoveFromCdevsw(), IORemoveFromVfssw()

**Summary:** Remove UNIX-style entry points from a device switch table

**Declared In:** driverkit/devsw.h

**Synopsis:**

```c
void IORemoveFromBdevsw(int bdevswNumber)
void IORemoveFromCdevsw(int cdevswNumber)
void IORemoveFromVfssw(int vfsswNumber)
```

**Description:**

These functions remove a device from a device switch table, replacing it with a null entry.

**Note:** You should use IODevice's `removeFromBdevsw` and `removeFromCdevsw` methods instead of `IORemoveFromBdevsw()` and `IORemoveFromCdevsw()`, whenever possible.

**See also:** `IOAddToBdevsw()`, `IOAddToCdevsw()`, `IOAddToVfssw()`

---

#### IOResumeThread()

**Summary:** Resume the execution of a thread suspended with IOSuspendThread()

**Declared In:** driverkit/generalFuncs.h

**Synopsis:**

```c
void IOResumeThread(IOThread thread)
```

**Description:**

This function causes the execution of a suspended thread to continue.

---

#### IOScheduleFunc()

**Summary:** Arrange for the specified function to be called at a certain time in the future

**Declared In:** driverkit/generalFuncs.h

**Synopsis:**

```c
void IOScheduleFunc(IOThreadFunc function, void *arg, int seconds)
```

**Description:**

This function causes *function* to be called in *seconds* seconds, with *arg* as *function*'s argument. The call to *function* occurs in the context of the caller's task, but in a thread that is unique to the Driver Kit. The call to *function* can be cancelled with `IOUnscheduleFunc()`.

**Note:** The kernel version of `IOScheduleFunc()` performs the callback in the kernel task's context, not the I/O Task context. One consequence is that *function* can't send Mach messages with `msg_send()`; it needs to use `msg_send_from_kernel()` instead, as described in Chapter 2.

---

#### IOSendInterrupt()

**Summary:** Arrange for an interrupt message to be sent

**Declared In:** driverkit/IODirectDevice.h

**Synopsis:**

```c
void IOSendInterrupt(void *identity, void *state, unsigned int msgId)
```

**Description:**

This function is useful if you need to handle interrupts directly—for example, because of a timing constraint in the hardware—but don't wish to give up the advantages of interrupt notification by messages. To handle interrupts directly, you must implement the `getHandler:level:argument:forInterrupt:` message of IODirectDevice.

The *msgId* argument specifies the message ID of the interrupt message that will be sent. This should be `IO_DEVICE_INTERRUPT_MSG` unless the driver's documentation specifies otherwise. The *identity* and *state* arguments should be set to the values that the interrupt handler received in its own arguments. For example:

```c
static void myInterruptHandler(void *identity, void *state,
                              unsigned int arg)
{
    /* handle the interrupt */
    IOSendInterrupt(identity, state, IO_DEVICE_INTERRUPT_MSG);
}
```

**See also:** `IODisableInterrupt()`, `IOEnableInterrupt()`

---

#### IOSetDDMMask()

**Summary:** Set specified bitmask word to specified value

**Declared In:** driverkit/debugging.h

**Synopsis:**

```c
void IOSetDDMMask(int index, unsigned int bitmask)
```

**Description:**

This is typically used by individual user-level drivers at initialization time, if then. Subsequently, it is usually used only by the Driver Debugging Module's server thread to change the current bitmask value.

The *index* argument is an index into `IODDMMasks`, which is an array of `unsigned int`. Each entry of the array contains 32 mask bits.

---

#### IOSetUNIXError()

**Summary:** Explicitly return an error value from a UNIX-style driver

**Declared In:** driverkit/kernelDriver.h

**Synopsis:**

```c
void IOSetUNIXError(int errno)
```

**Description:**

Most UNIX-style drivers don't need to use this function. However, those that explicitly set the caller's errno can use this function to do so. This function is used when the caller executes as a result of a UNIX-style entry point.

**Note:** This function works only in kernel-level drivers.

---

#### IOSleep()

**Summary:** Sleep for indicated number of milliseconds

**Declared In:** driverkit/generalFuncs.h

**Synopsis:**

```c
void IOSleep(unsigned int milliseconds)
```

**Description:**

This function causes the caller to block for the indicated number of milliseconds.

---

#### IOSuspendThread()

**Summary:** Suspend the execution of a thread started with IOForkThread()

**Declared In:** driverkit/generalFuncs.h

**Synopsis:**

```c
void IOSuspendThread(IOThread thread)
```

**Description:**

This function causes the execution of a running thread to pause. The thread can be resumed with `IOResumeThread()`.

---

#### IOUnmapPhysicalFromIOTask()

**Summary:** Unmap a physical address range from your IOTask's address space

**Declared In:** driverkit/kernelDriver.h

**Synopsis:**

```c
IOReturn IOUnmapPhysicalFromIOTask(vm_address_t virtualAddress,
                                  unsigned length)
```

**Description:**

This function unmaps a range of memory that was mapped with `IOMapPhysicalIntoIOTask()`. You should use this to destroy a mapping when you no longer need to use it.

**Note:** This function works only in kernel-level drivers.

**Return:** Returns an error if the specified virtual range was not mapped by `IOMapPhysicalIntoIOTask()`; otherwise, returns `IO_R_SUCCESS`.

**See also:** `IOMapPhysicalIntoIOTask()`

---

#### IOUnscheduleFunc()

**Summary:** Cancel a request made with IOScheduleFunc()

**Declared In:** driverkit/generalFuncs.h

**Synopsis:**

```c
void IOUnscheduleFunc(IOThreadFunc function, void *arg)
```

**Description:**

This function removes a request made using `IOScheduleFunc()` from the current list of pending requests. An error will be logged to the console if the specified *function*/*arg* pair is not currently registered.

---

#### IOVmTaskCurrent()

**Summary:** Returns the vm_task_t of the current task

**Declared In:** driverkit/kernelDriver.h

**Synopsis:**

```c
vm_task_t IOVmTaskCurrent()
```

**Description:**

Returns the `vm_task_t` for the current task. The only reason to use this function is to perform DMA to user space memory transfers in a UNIX-style driver.

**Note:** This function works only in kernel-level drivers.

**See also:** `IOVmTaskSelf()`

---

#### IOVmTaskForBuf()

**Summary:** Returns the vm_task_t associated with a buf structure

**Declared In:** driverkit/kernelDriver.h

**Synopsis:**

```c
vm_task_t IOVmTaskForBuf(struct buf *buffer)
```

**Description:**

Block drivers use this function to determine the task for which they're doing I/O. The value returned by this function is used in calls to `IOPhysicalFromVirtual()`, which returns an address that's used in IODirectDevice's `createDMABufferFor:...` method.

**Note:** This function works only in kernel-level drivers.

---

#### IOVmTaskSelf()

**Summary:** Obtain the vm_task_t of the kernel

**Declared In:** driverkit/kernelDriver.h

**Synopsis:**

```c
vm_task_t IOVmTaskSelf()
```

**Description:**

This function is used to obtain the kernel's `vm_task_t`, which is the `vm_task_t` for memory allocated with `IOMalloc()`. This function is required because the type definition of `vm_task_t` at kernel level is different from that of `vm_task_t` at user level.

**Note:** This function works only in kernel-level drivers.

---

