## Protocols

### IOConfigurationInspector

**Adopted By:** IODeviceInspector class, IODisplayInspector class

**Declared In:** driverkit/IODeviceInspector.h

#### Protocol Description

The IOConfigurationInspector protocol is adopted by inspectors that are loaded into the Configure application. Each inspector lets the user inspect and set information about a device, such as a specific brand of Ethernet card. The inspector stores this information in an NXStringTable that is specified to the inspector with the `setTable:` method.

The default, customizable inspector implemented by the IODeviceInspector class is sufficient for many devices. However, if IODeviceInspector doesn't suit your configuration needs, you should implement your own inspector class that adopts the IOConfigurationInspector protocol. An example of adopting this protocol is under `/NextLibrary/Documentation/NextDev/Examples/DriverKit` in the DriverInspector directory.

#### Method Types

**Get the inspector's View**
- `– inspectionView`

**Notify that resources have changed**
- `– resourcesChanged:`

**Set the description table**
- `– setTable:`

#### Instance Methods

##### inspectionView

```objc
- (View *)inspectionView
```

Returns the View of the inspector.

##### resourcesChanged:

```objc
- resourcesChanged:(IOResources *)resources
```

The Configure application sends this message to all inspectors whenever an interrupt, DMA channel, I/O port, or memory range is chosen or dropped in any inspector. This method should check for conflicts and update the UI.

This message is sent as often as you might need it, including immediately after a `setTable:` and after your own changes. You are guaranteed to be deactivated before your current table is freed, but you will not receive a `setTable:nil`, so don't count on accessing or modifying the table except in response to a user action.

##### setTable:

```objc
- setTable:(NXStringTable *)anObject
```

Sets the NXStringTable describing the inspector's device to *anObject*. You should update the UI when `setTable:` gives you a table to inspect. Your object should keep a handle to the table. When the user makes changes, immediately update the table; do not use OK/Revert buttons.

---

### IOEventThread

**Adopted By:** The event system

**Declared In:** driverkit/eventProtocols.h

#### Protocol Description

The IOEventThread protocol provides access to the event system's I/O thread. You can obtain an IOEventThread-compliant object from IOEventSource's `owner` method.

#### Method Types

**Sending messages**
- `– sendIOThreadAsyncMsg:to:with:`
- `– sendIOThreadMsg:to:with:`

#### Instance Methods

##### sendIOThreadAsyncMsg:to:with:

```objc
- (IOReturn)sendIOThreadAsyncMsg:(id)instance
                              to:(SEL)selector
                            with:(id)data
```

From the event system's I/O thread, sends the message specified by *selector* to *instance*, with the argument *data*. This method doesn't wait for the selector method to be called, and doesn't detect whether *selector* is a valid method of *instance*. Returns `IO_R_IPC_FAILURE` if an error occurred; otherwise, returns `IO_R_SUCCESS`.

**See also:** `– sendIOThreadMsg:to:with:`

##### sendIOThreadMsg:to:with:

```objc
- (IOReturn)sendIOThreadMsg:(id)instance
                         to:(SEL)selector
                       with:(id)data
```

From the event system's I/O thread, sends the message specified by *selector* to *instance*, with the argument *data*. This method waits until the selector method has returned. Returns `IO_R_IPC_FAILURE` if the message couldn't be sent; otherwise, returns `IO_R_SUCCESS`.

**See also:** `– sendIOThreadAsyncMsg:to:with:`

---

### IONetworkDeviceMethods

**Adopted By:** IOEthernet, IOTokenRing

**Declared In:** driverkit/IONetwork.h

#### Protocol Description

This protocol must be implemented by network direct device drivers that use IONetwork to tie into the kernel network system. These methods are invoked by IONetwork objects in response to events in the network system.

**Note:** Network drivers must run at kernel level.

#### Method Types

**Creating netbufs**
- `– allocateNetbuf`

**Initializing the hardware**
- `– finishInitialization`

**Sending out a packet**
- `– outputPacket:address:`

**Performing control commands**
- `– performCommand:data:`

#### Instance Methods

##### allocateNetbuf

```objc
- (netbuf_t)allocateNetbuf
```

This method creates and returns a netbuf to be used for an impending output.

This method doesn't always have to return a buffer. For example, you might want to limit the number of buffers your driver instance can allocate (say, 200 kilobytes worth) so that it won't use too much wired-down kernel memory. When this method fails to return a buffer, it should return `NULL`.

Here's an example of implementing `allocateNetbuf`:

```objc
#define my_HDR_SIZE    14
#define my_MTU        1500
#define my_MAX_PACKET  (my_HDR_SIZE + my_MTU)

- netbuf_t allocateNetbuf
{
    if (_numbufs == _maxNumbufs)
        return(NULL);
    else {
        _numbufs++;
        return(nb_alloc(my_MAX_PACKET));
    }
}
```

**See also:** `nb_alloc()` (NEXTSTEP Operating System Software)

##### finishInitialization

```objc
- (int)finishInitialization
```

This method should perform any initialization that hasn't already been done. For example, it should make sure its hardware is ready to run. You can specify what the integer return value (if any) should be.

If you implement this method, you need to check that `[self isRunning] == YES`.

##### outputPacket:address:

```objc
- (int)outputPacket:(netbuf_t)packet address:(void *)address
```

This method should deliver the specified packet to the given address. Its return value should be zero if no error occurred; otherwise, return an error number from the header file `sys/errno.h`.

If you implement this method, you need to check that `[self isRunning] == YES`. If so, insert the necessary hardware addresses into the packet and check it for minimum length requirements.

##### performCommand:data:

```objc
- (int)performCommand:(const char *)command data:(void *)data
```

This method performs arbitrary control operations; the character string *command* is used to select between these operations. Although you don't have to implement any operations, there are five standard operations. You can also define your own operations.

The standard commands are listed in the following table. The constant strings listed below are declared in the header file `net/netif.h` (under the bsd directory of `/NextDeveloper/Headers`).

| Command | Operation |
|---------|-----------|
| `IFCONTROL_SETFLAGS` | Request to have interface flags turned on or off. The data argument for this command is of type `union ifr_ifru` (which is declared in the header file `net/if.h`). |
| `IFCONTROL_SETADDR` | Set the address of the interface. |
| `IFCONTROL_GETADDR` | Get the address of the interface. |
| `IFCONTROL_AUTOADDR` | Automatically set the address of the interface. |
| `IFCONTROL_UNIXIOCTL` | Perform a UNIX `ioctl()` command. This is only for compatibility; `ioctl()` isn't a recommended interface for network drivers. The argument is of type `if_ioctl_t *`, where the `if_ioctl_t` structure contains the UNIX ioctl request (for example, `SIOCSIFADDR`) in the `ioctl_command` field and the ioctl data in the `ioctl_data` field. |

An example of implementing `performCommand:data:` follows:

```objc
- (int)performCommand:(const char *)command data:(void *)data
{
    int error = 0;

    if (strcmp(command, IFCONTROL_SETFLAGS) == 0)
        /* do nothing */;
    else
    if (strcmp(command, IFCONTROL_GETADDR) == 0)
        bcopy(&my_address, data, sizeof (my_address));
    else
        error = EINVAL;

    return (error);
}
```

---

### IOScreenEvents

**Adopted By:** IODisplay

**Declared In:** driverkit/eventProtocols.h

#### Protocol Description

The methods in this protocol are invoked by the event system, at the request of the Window Server or of pointer management software.

#### Method Types

**Manipulating the cursor**
- `– hideCursor:`
- `– moveCursor:frame:token:`
- `– showCursor:frame:token:`

**Get the device port**
- `– devicePort`

**Set screen brightness**
- `– setBrightness:token:`

#### Instance Methods

##### devicePort

```objc
- (port_t)devicePort
```

Returns the device port, which should be obtained from this instance's IODeviceDescription.

##### hideCursor:

```objc
- hideCursor:(int)token
```

Removes the cursor from the screen.

##### moveCursor:frame:token:

```objc
- moveCursor:(Point *)cursorLoc
       frame:(int)frame
       token:(int)token
```

Removes the cursor from the screen, moves it, and displays the cursor in its new position.

##### setBrightness:token:

```objc
- setBrightness:(int)level token:(int)token
```

Sets the brightness of the screen. Many devices (and thus many drivers) don't permit this operation.

**See also:** `– setBrightness:token:` (IOFrameBufferDisplay class)

##### showCursor:frame:token:

```objc
- showCursor:(Point *)cursorLocation
       frame:(int)frame
       token:(int)token
```

Displays the cursor at *cursorLocation*.

---

### IOScreenRegistration

**Adopted By:** The event system

**Declared In:** driverkit/eventProtocols.h

#### Protocol Description

Display drivers use the messages in the IOScreenRegistration protocol to register and unregister themselves with the event system. These methods are called by IODisplay in response to a `getIntValues:forParameter:count:` call that specifies the "IO_Framebuffer_Register" parameter.

You shouldn't need to invoke the methods in this protocol, because they're already invoked automatically by IOFrameBufferDisplay and IOSVGADisplay.

#### Instance Methods

##### registerScreen:bounds:shmem:size:

```objc
- (int)registerScreen:(id)instance
               bounds:(Bounds *)bounds
                shmem:(void **)address
                 size:(int *)num
```

Registers *instance* as a display driver. Returns a token that's used to refer to the display in other calls to the event system.

##### unregisterScreen:

```objc
- (void)unregisterScreen:(int)token
```

Unregisters the instance associated with *token* as a display driver.

---

### IOSCSIControllerExported

**Adopted By:** IOSCSIController class

**Declared In:** driverkit/scsiTypes.h

#### Protocol Description

Indirect device drivers for devices attached to SCSI controllers use the methods in this protocol to communicate with IOSCSIController.

#### Method Types

**Allocating well-aligned buffers**
- `– allocateBufferOfLength:actualStart:actualLength:`
- `– getDMAAlignment:`

**Requesting I/O**
- `– executeRequest:buffer:client:`
- `– maxTransfer`

**Reserving SCSI targets**
- `– reserveTarget:lun:forOwner:`
- `– releaseTarget:lun:forOwner:`

**Resetting the SCSI bus**
- `– resetSCSIBus`

**Getting the IOReturn equivalent of a sc_status_t value**
- `– returnFromScStatus:`

#### Instance Methods

##### allocateBufferOfLength:actualStart:actualLength:

```objc
- (void *)allocateBufferOfLength:(unsigned)length
                     actualStart:(void **)actualStart
                    actualLength:(unsigned *)actualLength
```

Allocates and returns a pointer to some well-aligned memory. Well-aligned memory is necessary for calls to `executeRequest:buffer:client:`. You should use *actualStart* and *actualLength* when freeing the memory, as follows:

```objc
dataBuffer = [_controller allocateBufferOfLength:block_size
                                     actualStart:&freePtr
                                    actualLength:&freeLength];
/* Use the buffer... */
IOFree(freePtr, freeLength);
```

Here's a typical use of this method:

```objc
IODMAAlignment    dmaAlign;
unsigned int      alignment, alignedLength, freeLength;
void             *alignedPtr = NULL;
unsigned int      maxLength; /* Max length of the current transfer */
/* . . . */
[_controller getDMAAlignment:&dmaAlign];
if(/* we're doing a write */)
    alignment = dmaAlign.writeLength;
else
    alignment = dmaAlign.readLength;

if(alignment > 1)
    alignedLength = IOAlign(unsigned int, maxLength, alignment);
else
    alignedLength = maxLength;

alignedPtr = [_controller allocateBufferOfLength:alignedLength
                                     actualStart:&freePtr
                                    actualLength:&freeLength];

/* If we're going to do a write, copy the data to alignedPtr.
   Set up the request and submit it, as described in the
   executeRequest:buffer:client: description. */
/* Do any post-I/O processing that's necessary. */

IOFree(freePtr, freeLength);
```

**See also:** `– getDMAAlignment:`

##### executeRequest:buffer:client:

```objc
- (sc_status_t)executeRequest:(IOSCSIRequest *)scsiRequest
                       buffer:(void *)buffer
                       client:(vm_task_t)client
```

Executes the specified request. Indirect devices invoke this method whenever they need the IOSCSIController to perform I/O.

Subclasses of IOSCSIController must implement this method. A typical implementation of this method consists of the following:

1. Using `IOScheduleFunc()` to schedule a timeout function to be called after `scsiRequest->timeoutLength` time has elapsed without I/O completion
2. Sending the command descriptor block (CDB) specified in *scsiRequest* to the controller
3. When the I/O has completed, unscheduling the timeout function

This method should return `scsiRequest->driverStatus`, which should be set by the part of the driver that detected I/O completion or timeout.

Indirect devices use this method as shown below:

```objc
void             *alignedPtr = NULL;
unsigned int      alignedLength;
IOSCSIRequest     request;
cdb_t             cdb;

/* . . . */
if (/* we're going to be doing DMA */) {
    /* Ensure we have a well-aligned buffer that starts at alignedPtr
       and continues for alignedLength bytes.  See the
       allocateBuffer: description for one way of doing this. */
} else {
    alignedLength = 0;
    alignedPtr = 0;
}

bzero(&request, sizeof(request));
request.target = [self target];
request.lun    = [self lun];
request.read = /* YES if this is a read; NO otherwise */;
request.maxTransfer = alignedLength;
request.timeoutLength = /* some timeout length, in seconds */;
request.disconnect = /* 1 if allowed to disconnect; otherwise 0 */;
request.cdb = cdb;
/* Set up the cdb (command descriptor block) field. The type of this
   field, cdb_t, is defined and described in the header file
   bsd/dev/scsireg.h. */

rtn = [_controller executeRequest:&request
                           buffer:alignedPtr
                           client:IOVmTaskSelf()];
```

##### getDMAAlignment:

```objc
- (void)getDMAAlignment:(IODMAAlignment *)alignment
```

Returns the DMA alignment requirements for the current architecture. IOSCSIController subclasses can override this method to specify any device-specific alignment requirements. See the description of `allocateBufferOfLength:actualStart:actualLength:` for an example of using this method.

**See also:** `– allocateBufferOfLength:actualStart:actualLength:`

##### maxTransfer

```objc
- (unsigned)maxTransfer
```

Returns the maximum number of bytes per DMA transfer. This is the maximum transfer that can be requested in a call to `executeRequest:buffer:client:`.

##### releaseTarget:lun:forOwner:

```objc
- (void)releaseTarget:(unsigned char)target
                  lun:(unsigned char)lun
             forOwner:owner
```

Releases the specified target/lun pair. If *owner* hasn't reserved the pair, this method uses `IOLog` to print an error message.

**See also:** `– reserveTarget:lun:forOwner:`

##### reserveTarget:lun:forOwner:

```objc
- (int)reserveTarget:(unsigned char)target
                 lun:(unsigned char)lun
            forOwner:owner
```

Reserves the specified target/lun pair, if it isn't already reserved. This method is invoked by a client (for example, a SCSIDisk instance) to mark a particular target/lun as being in use by that client. Usually, this happens at `probe:` time; however, the SCSIGeneric driver uses this method at other times.

This method returns a nonzero value if the target/lun pair is already reserved. Otherwise, it returns zero.

**See also:** `– releaseTarget:lun:forOwner:`

##### resetSCSIBus

```objc
- (sc_status_t)resetSCSIBus
```

Resets the SCSI bus. Subclasses of IOSCSIController must implement this method so that it resets the SCSI bus. The `sc_status_t` enumerated type is defined and described in the header file `bsd/dev/scsireg.h`.

##### returnFromScStatus:

```objc
- (IOReturn)returnFromScStatus:(sc_status_t)sc_status
```

Returns the IOReturn value corresponding to the specified `sc_status_t` value. The `sc_status_t` enumerated type is defined and described in the header file `bsd/dev/scsireg.h`.

---

