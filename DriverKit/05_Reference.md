# Chapter 5: Driver Kit Reference

**Library:** Configure application API has no library; driver classes are contained in */System/Library/Frameworks/DriverKit.framework*.

**Header File Directory:** */System/Library/Frameworks/DriverKit.framework/Headers*

This chapter provides reference documentation for the Driver Kit classes, protocols, functions, types, and constants. The Driver Kit provides an object-oriented framework for creating device drivers in NeXTSTEP.

## Class Overview

The Driver Kit classes are organized into two main categories:

**User-Level Classes:**
- IOAddressRanger
- IOConfigTable
- IODeviceDescription
- IODeviceInspector
- IODeviceMaster

**Driver Classes:**
- IODevice (abstract superclass)
- IODirectDevice (abstract subclass of IODevice)
- Various device-specific subclasses

---

## Classes

### IOAddressRanger

**Inherits From:** Object

**Declared In:** driverkit/IOAddressRanger.h

#### Class Description

IOAddressRanger is an object that provides a user interface for I/O port address ranges and memory-mapped address ranges. The Configure application uses IOAddressRanger to allow the user to select valid ranges that don't conflict with other devices.

The IOAddressRanger user interface consists of a matrix with two rows; the first row contains the starting address and the second row contains the length of the range. Plus and minus buttons to the right of each row let the user increase or decrease the value in that row. Pressing the plus or minus button once changes the corresponding value by one; pressing and holding the button changes the value continuously.

#### Instance Variables

None declared.

#### Method Types

**Checking for conflicts**
- `– checkRangesForConflicts:num:`

**Setting range values**
- `– setStartAddress:`
- `– setRangeLength:`

**Responding to user actions**
- `– plus:`
- `– minus:`

#### Instance Methods

##### checkRangesForConflicts:num:

```objc
- (BOOL)checkRangesForConflicts:(const IORange *)rangeList
                             num:(int)numRanges
```

Returns YES if the current values of startAddress and rangeLength don't conflict with any of the ranges in *rangeList*. Otherwise, returns NO. *numRanges* specifies the number of ranges in *rangeList*.

This method is used to verify that the user's selections don't conflict with other devices in the system.

##### minus:

```objc
- minus:sender
```

Decrements the value in the row corresponding to the minus button that was clicked. This method is invoked automatically when the user clicks a minus button.

##### plus:

```objc
- plus:sender
```

Increments the value in the row corresponding to the plus button that was clicked. This method is invoked automatically when the user clicks a plus button.

##### setRangeLength:

```objc
- setRangeLength:(unsigned int)length
```

Sets the range length to *length*. The range length determines the number of addresses in the range.

##### setStartAddress:

```objc
- setStartAddress:(unsigned int)address
```

Sets the starting address to *address*. This is the first address in the range.

---

### IOAudio

**Inherits From:** IODirectDevice : IODevice : Object

**Declared In:** driverkit/IOAudio.h

#### Class Description

IOAudio is an abstract class that defines the interface for sound card device drivers. Sound card drivers must be direct drivers that inherit from IOAudio and implement its required methods.

IOAudio provides DMA buffer management and interrupt handling support for sound cards. Subclasses must implement methods for resetting the hardware, starting and stopping DMA, handling interrupts, and reporting capabilities.

#### Instance Variables

None declared.

#### Method Types

**Required methods for subclasses**
- `– reset`
- `– startDMAForChannel:read:buffer:bufferSizeForInterrupts:`
- `– stopDMAForChannel:read:`
- `– interruptOccurredForInput:forOutput:`
- `– interruptClearFunc`
- `– channelCountLimit`

**Optional methods for subclasses**
- `– updateLimits`

#### Class Methods

##### deviceStyle

```objc
+ (IODeviceStyle)deviceStyle
```

Returns `IO_DirectDevice`, indicating that IOAudio drivers are direct drivers.

#### Instance Methods

##### channelCountLimit

```objc
- (unsigned int)channelCountLimit
```

**Required method.** Returns the maximum number of audio channels supported by this device. Must be implemented by subclasses.

##### interruptClearFunc

```objc
- (IOAudioInterruptClearFunc)interruptClearFunc
```

**Required method.** Returns a pointer to a function that clears the interrupt condition on the sound card. This function is called by the DMA handling code after an interrupt occurs. Must be implemented by subclasses.

The function should have the signature:

```objc
void interruptClearFunction(id device, void *interruptParam)
```

##### interruptOccurredForInput:forOutput:

```objc
- (void)interruptOccurredForInput:(BOOL *)serviceInput
                        forOutput:(BOOL *)serviceOutput
```

**Required method.** Called when an audio interrupt occurs. The subclass should examine the hardware to determine which channel(s) caused the interrupt and set *serviceInput* to YES if the input channel needs servicing and/or *serviceOutput* to YES if the output channel needs servicing. Must be implemented by subclasses.

##### reset

```objc
- (BOOL)reset
```

**Required method.** Resets the audio hardware to its default state. Returns YES if successful, NO otherwise. This method is called during initialization and when recovering from errors. Must be implemented by subclasses.

##### startDMAForChannel:read:buffer:bufferSizeForInterrupts:

```objc
- (BOOL)startDMAForChannel:(unsigned int)localChannel
                      read:(BOOL)isRead
                    buffer:(IODMABuffer)buffer
    bufferSizeForInterrupts:(unsigned int)bufferSize
```

**Required method.** Starts DMA for the specified channel. *localChannel* indicates which channel to start (0 for left, 1 for right, etc.). *isRead* is YES for input (recording), NO for output (playback). *buffer* contains the DMA buffer information. *bufferSize* specifies the interrupt frequency in bytes.

Returns YES if DMA was started successfully, NO otherwise. Must be implemented by subclasses.

##### stopDMAForChannel:read:

```objc
- (void)stopDMAForChannel:(unsigned int)localChannel
                     read:(BOOL)isRead
```

**Required method.** Stops DMA for the specified channel. *localChannel* indicates which channel to stop. *isRead* is YES for input, NO for output. Must be implemented by subclasses.

##### updateLimits

```objc
- updateLimits
```

**Optional method.** Called when audio parameters (such as sample rate or bit depth) have changed. Subclasses can override this method to update internal state when parameters change.

---

### IOConfigTable

**Inherits From:** Object

**Declared In:** driverkit/IOConfigTable.h

#### Class Description

IOConfigTable provides access to configuration information for device drivers. Configuration tables can be accessed from both kernel level (by drivers) and user level (by configuration utilities). Each driver instance has an associated IOConfigTable that contains its configuration parameters.

Configuration information is stored as key-value pairs, where keys are strings and values can be strings, integers, or arrays. The configuration system supports both boot-time configuration (from the system configuration file) and runtime configuration changes.

#### Instance Variables

None declared.

#### Method Types

**Creating instances**
- `+ newForDriver:unit:`
- `+ newFromSystemConfig`
- `+ tablesForBootDrivers`

**Accessing values**
- `– valueForStringKey:`
- `– intValueForStringKey:`

**Freeing**
- `– free`

#### Class Methods

##### newForDriver:unit:

```objc
+ newForDriver:(const char *)driverName
          unit:(int)unit
```

Creates and returns a new IOConfigTable for the specified driver and unit number. *driverName* should be the name of the driver class. *unit* specifies which instance of the driver (0 for the first instance, 1 for the second, etc.).

Returns `nil` if no configuration table exists for the specified driver and unit.

##### newFromSystemConfig

```objc
+ newFromSystemConfig
```

Creates and returns a new IOConfigTable containing the system-wide configuration parameters. These are global parameters that aren't associated with any specific driver.

##### tablesForBootDrivers

```objc
+ (List *)tablesForBootDrivers
```

Returns a List containing IOConfigTable instances for all drivers that should be loaded at boot time. This is typically used by the system during startup to determine which drivers to load.

#### Instance Methods

##### free

```objc
- free
```

Frees the IOConfigTable instance and releases its resources. Returns `nil`.

##### intValueForStringKey:

```objc
- (int)intValueForStringKey:(const char *)key
```

Returns the integer value associated with *key*. If *key* doesn't exist or its value isn't an integer, returns 0.

##### valueForStringKey:

```objc
- (const char *)valueForStringKey:(const char *)key
```

Returns the string value associated with *key*. If *key* doesn't exist, returns `NULL`.

---

### IODeviceDescription

**Inherits From:** Object

**Declared In:** driverkit/IODeviceDescription.h

#### Class Description

IODeviceDescription encapsulates information about a hardware device. The system creates IODeviceDescription instances during device probing and passes them to drivers during initialization. Each IODeviceDescription contains details about the device's hardware resources, such as interrupt lines, I/O ports, memory ranges, and DMA channels.

Architecture-specific subclasses of IODeviceDescription provide additional information specific to particular bus types:

- **IOEISADeviceDescription** – For EISA bus devices
- **IOPCIDeviceDescription** – For PCI bus devices
- **IOPCMCIADeviceDescription** – For PCMCIA devices

Drivers receive an IODeviceDescription in their `initFromDeviceDescription:` method and should extract the necessary hardware information from it.

#### Instance Variables

None declared.

#### Method Types

**Accessing device resources**
- `– interruptList`
- `– memoryRangeList`
- `– portRangeList`
- `– dmaChannelList`

**Accessing device information**
- `– devicePort`
- `– directDevice`
- `– configTable`

#### Instance Methods

##### configTable

```objc
- (IOConfigTable *)configTable
```

Returns the IOConfigTable associated with this device. The configuration table contains parameters that control the driver's behavior.

##### devicePort

```objc
- (port_t)devicePort
```

Returns the Mach port used for communicating with this device's driver. This is primarily used by user-level code to send messages to the driver.

##### directDevice

```objc
- directDevice
```

If this device description is for an indirect driver, returns the direct device (IODirectDevice instance) that provides hardware access. Returns `nil` for direct drivers.

##### dmaChannelList

```objc
- (unsigned int *)dmaChannelList
```

Returns a zero-terminated array of DMA channel numbers that are allocated to this device. Returns `NULL` if no DMA channels are allocated.

##### interruptList

```objc
- (unsigned int *)interruptList
```

Returns a zero-terminated array of interrupt line numbers (IRQs) that are allocated to this device. Returns `NULL` if no interrupts are allocated.

##### memoryRangeList

```objc
- (IORange *)memoryRangeList
```

Returns a zero-terminated array of IORange structures describing the memory-mapped address ranges allocated to this device. Each IORange contains a starting address and length. Returns `NULL` if no memory ranges are allocated.

##### portRangeList

```objc
- (IORange *)portRangeList
```

Returns a zero-terminated array of IORange structures describing the I/O port address ranges allocated to this device. Each IORange contains a starting port address and length. Returns `NULL` if no port ranges are allocated.

---

### IODeviceInspector

**Inherits From:** Object

**Conforms To:** IOConfigurationInspector (informal protocol)

**Declared In:** driverkit/IODeviceInspector.h

#### Class Description

IODeviceInspector is the default inspector for device drivers in the Configure application. When the user selects a device in Configure, an inspector panel is displayed that shows the device's configuration parameters. IODeviceInspector provides a standard interface that displays basic device information.

Drivers can provide custom inspectors by creating a subclass of IODeviceInspector. Custom inspectors can add an accessory view to display additional controls specific to the device type. The accessory view is typically loaded from a nib file and inserted into the standard inspector panel.

#### Instance Variables

```objc
id accessoryView;
```

**accessoryView** – The custom view to be inserted into the inspector panel.

#### Method Types

**Initialization**
- `– init`

**Setting configuration**
- `– setTable:`

**User actions**
- `– connectorChanged:`

#### Instance Methods

##### connectorChanged:

```objc
- connectorChanged:sender
```

Responds to changes in the connector selection. This method is invoked when the user changes the connector type in the inspector. Subclasses can override this method to update their accessory view when the connector changes.

##### init

```objc
- init
```

Initializes the IODeviceInspector instance. Subclasses should override this method to load their nib file and set up the accessory view:

```objc
- init
{
    [super init];
    [NXApp loadNibSection:"MyInspector.nib"
                    owner:self];
    return self;
}
```

##### setTable:

```objc
- setTable:(IOConfigTable *)configTable
```

Called when the inspector is about to be displayed. *configTable* contains the device's current configuration. Subclasses should override this method to initialize their accessory view with values from the configuration table:

```objc
- setTable:(IOConfigTable *)table
{
    const char *value;

    [super setTable:table];

    value = [table valueForStringKey:"MyParameter"];
    [myTextField setStringValue:value];

    return self;
}
```

---

### IODeviceMaster

**Inherits From:** Object

**Declared In:** driverkit/IODeviceMaster.h

#### Class Description

IODeviceMaster provides user-level applications with access to device driver instances. Applications use IODeviceMaster to look up devices by name, get and set driver parameters, and communicate with drivers.

IODeviceMaster acts as a proxy for kernel-level driver objects. Method calls on IODeviceMaster are converted to Mach messages and sent to the appropriate driver in the kernel. This allows user-level code to interact with drivers without requiring kernel access.

Typical usage:

```objc
IODeviceMaster *devMaster;
IOObjectNumber objectNumber;
IOString kind;
IOReturn ret;
unsigned int count;
char value[80];

devMaster = [IODeviceMaster new];

ret = [devMaster lookUpByDeviceName:"Sound"
                       objectNumber:&objectNumber
                         deviceKind:&kind];

if (ret == IO_R_SUCCESS) {
    count = 1;
    ret = [devMaster getCharValues:value
                      forParameter:"Volume"
                      objectNumber:objectNumber
                             count:&count];
}
```

#### Instance Variables

None declared.

#### Method Types

**Looking up devices**
- `– lookUpByDeviceName:objectNumber:deviceKind:`
- `– lookUpByObjectNumber:deviceKind:`

**Getting parameter values**
- `– getCharValues:forParameter:objectNumber:count:`
- `– getIntValues:forParameter:objectNumber:count:`

**Setting parameter values**
- `– setCharValues:forParameter:objectNumber:count:`
- `– setIntValues:forParameter:objectNumber:count:`

#### Instance Methods

##### getCharValues:forParameter:objectNumber:count:

```objc
- (IOReturn)getCharValues:(unsigned char *)values
             forParameter:(IOParameterName)parameter
             objectNumber:(IOObjectNumber)objectNumber
                    count:(unsigned int *)count
```

Gets character array parameter values from the driver. *parameter* specifies which parameter to retrieve. *objectNumber* identifies the driver instance. On input, *count* specifies the maximum number of values to retrieve; on output, it contains the actual number retrieved. *values* must point to a buffer large enough to hold *count* values.

Returns `IO_R_SUCCESS` on success, or an error code on failure.

##### getIntValues:forParameter:objectNumber:count:

```objc
- (IOReturn)getIntValues:(unsigned int *)values
            forParameter:(IOParameterName)parameter
            objectNumber:(IOObjectNumber)objectNumber
                   count:(unsigned int *)count
```

Gets integer array parameter values from the driver. *parameter* specifies which parameter to retrieve. *objectNumber* identifies the driver instance. On input, *count* specifies the maximum number of values to retrieve; on output, it contains the actual number retrieved. *values* must point to a buffer large enough to hold *count* values.

Returns `IO_R_SUCCESS` on success, or an error code on failure.

##### lookUpByDeviceName:objectNumber:deviceKind:

```objc
- (IOReturn)lookUpByDeviceName:(IOString)deviceName
                  objectNumber:(IOObjectNumber *)objectNumber
                    deviceKind:(IOString *)kind
```

Looks up a device by its name. *deviceName* specifies the device name (such as "Sound" or "Ethernet0"). If successful, stores the device's object number in *objectNumber* and the device kind string in *kind*.

Returns `IO_R_SUCCESS` if the device was found, or an error code if it wasn't found or an error occurred.

##### lookUpByObjectNumber:deviceKind:

```objc
- (IOReturn)lookUpByObjectNumber:(IOObjectNumber)objectNumber
                      deviceKind:(IOString *)kind
```

Looks up a device by its object number. If successful, stores the device kind string in *kind*.

Returns `IO_R_SUCCESS` if the device was found, or an error code if it wasn't found or an error occurred.

##### setCharValues:forParameter:objectNumber:count:

```objc
- (IOReturn)setCharValues:(unsigned char *)values
             forParameter:(IOParameterName)parameter
             objectNumber:(IOObjectNumber)objectNumber
                    count:(unsigned int)count
```

Sets character array parameter values in the driver. *parameter* specifies which parameter to set. *objectNumber* identifies the driver instance. *count* specifies the number of values in *values*.

Returns `IO_R_SUCCESS` on success, or an error code on failure.

##### setIntValues:forParameter:objectNumber:count:

```objc
- (IOReturn)setIntValues:(unsigned int *)values
            forParameter:(IOParameterName)parameter
            objectNumber:(IOObjectNumber)objectNumber
                   count:(unsigned int)count
```

Sets integer array parameter values in the driver. *parameter* specifies which parameter to set. *objectNumber* identifies the driver instance. *count* specifies the number of values in *values*.

Returns `IO_R_SUCCESS` on success, or an error code on failure.

---

### IODevice

**Inherits From:** Object

**Declared In:** driverkit/IODevice.h

#### Class Description

IODevice is the abstract superclass of all device drivers in the Driver Kit. Every driver must inherit from IODevice (or one of its subclasses) and implement required methods for probing hardware and initializing driver instances.

There are two main categories of IODevice subclasses:

- **Direct drivers** (subclasses of IODirectDevice) – Access hardware directly, typically running in the kernel's I/O thread.
- **Indirect drivers** – Provide high-level services by using one or more direct drivers for actual hardware access.

All drivers must implement the following class methods:

- `+ (IODeviceStyle)deviceStyle` – Returns either `IO_DirectDevice` or `IO_IndirectDevice`
- `+ (BOOL)probe:` – Examines hardware to determine if the driver can handle it
- `- initFromDeviceDescription:` – Initializes a driver instance for a specific device

Direct drivers must register themselves during initialization by calling `registerDevice`, `setDeviceKind:`, `setLocation:`, and `setName:`.

Indirect drivers must also implement:

- `+ (Protocol **)requiredProtocols` – Returns protocols that direct drivers must implement

#### Instance Variables

None declared.

#### Method Types

**Device style**
- `+ deviceStyle`

**Probing hardware**
- `+ probe:`

**Initialization**
- `– initFromDeviceDescription:`

**Registration (direct drivers only)**
- `– registerDevice`
- `– setDeviceKind:`
- `– setLocation:`
- `– setName:`

**Parameter access**
- `– getCharValues:forParameter:count:`
- `– getIntValues:forParameter:count:`
- `– setCharValues:forParameter:count:`
- `– setIntValues:forParameter:count:`

**UNIX device support (direct drivers)**
- `– getDeviceNumber:blockDevice:`
- `+ assignDeviceNumber:blockDevice:`
- `+ unuseDeviceNumber:blockDevice:`
- `– entryForWhich:`

#### Class Methods

##### assignDeviceNumber:blockDevice:

```objc
+ (int)assignDeviceNumber:(int)major
             blockDevice:(BOOL)isBlock
```

Assigns a major device number for UNIX device nodes. *major* is the requested major number. *isBlock* is YES for block devices, NO for character devices. Returns the assigned major number on success, or -1 on failure.

This method is used by direct drivers that need to support UNIX-style device files in `/dev`.

##### deviceStyle

```objc
+ (IODeviceStyle)deviceStyle
```

**Required method.** Returns the device style: either `IO_DirectDevice` for direct drivers or `IO_IndirectDevice` for indirect drivers. Must be implemented by all subclasses.

##### probe:

```objc
+ (BOOL)probe:(IODeviceDescription *)deviceDescription
```

**Required method.** Examines the hardware described by *deviceDescription* to determine if this driver can handle it. Returns YES if the driver recognizes the hardware and can drive it, NO otherwise.

This method is called during device probing (at boot time or when new devices are detected). The driver should examine the hardware configuration but should not modify it or allocate resources. Resource allocation happens later in `initFromDeviceDescription:`.

Must be implemented by all subclasses.

##### requiredProtocols

```objc
+ (Protocol **)requiredProtocols
```

**Required for indirect drivers.** Returns a NULL-terminated array of Protocol pointers specifying which protocols direct drivers must implement for this indirect driver to use them. For example, a network protocol stack might require direct drivers to implement an IOEthernetProtocol.

Direct drivers don't implement this method.

##### unuseDeviceNumber:blockDevice:

```objc
+ (void)unuseDeviceNumber:(int)major
             blockDevice:(BOOL)isBlock
```

Releases a previously assigned major device number. *major* is the major number to release. *isBlock* is YES for block devices, NO for character devices.

Called when a driver is unloaded or no longer needs its device number.

#### Instance Methods

##### entryForWhich:

```objc
- (int (*)())entryForWhich:(IOBdevSwOps)operation
```

Returns a pointer to the function implementing the specified block device switch table operation. *operation* specifies which operation (open, close, strategy, etc.).

This method is used by the kernel to build the `bdevsw` table for block devices.

##### getCharValues:forParameter:count:

```objc
- (IOReturn)getCharValues:(unsigned char *)values
             forParameter:(IOParameterName)parameter
                    count:(unsigned int *)count
```

Gets character array parameter values. *parameter* specifies which parameter to retrieve. On input, *count* specifies the maximum number of values to retrieve; on output, it contains the actual number retrieved.

Drivers should override this method to support custom parameters. The default implementation returns `IO_R_UNSUPPORTED`.

##### getDeviceNumber:blockDevice:

```objc
- (int)getDeviceNumber:(int *)major
          blockDevice:(BOOL)isBlock
```

Gets the major device number for this driver's UNIX device nodes. Stores the major number in *major*. *isBlock* specifies whether to get the block device (YES) or character device (NO) major number.

Returns 0 on success, -1 on failure.

##### getIntValues:forParameter:count:

```objc
- (IOReturn)getIntValues:(unsigned int *)values
            forParameter:(IOParameterName)parameter
                   count:(unsigned int *)count
```

Gets integer array parameter values. *parameter* specifies which parameter to retrieve. On input, *count* specifies the maximum number of values to retrieve; on output, it contains the actual number retrieved.

Drivers should override this method to support custom parameters. The default implementation returns `IO_R_UNSUPPORTED`.

##### initFromDeviceDescription:

```objc
- initFromDeviceDescription:(IODeviceDescription *)deviceDescription
```

**Required method.** Initializes a new driver instance for the hardware described by *deviceDescription*. This method should allocate resources, configure the hardware, and register the device.

Direct drivers must call the following during initialization:

```objc
[self registerDevice];
[self setDeviceKind:"NetworkController"];
[self setLocation:"Slot 1"];
[self setName:"en0"];
```

Returns `self` on success, `nil` on failure.

Must be implemented by all subclasses.

##### registerDevice

```objc
- (IOReturn)registerDevice
```

Registers the device with the Driver Kit system. Direct drivers must call this method during initialization, before calling `setDeviceKind:`, `setLocation:`, or `setName:`.

Returns `IO_R_SUCCESS` on success, or an error code on failure.

##### setCharValues:forParameter:count:

```objc
- (IOReturn)setCharValues:(unsigned char *)values
             forParameter:(IOParameterName)parameter
                    count:(unsigned int)count
```

Sets character array parameter values. *parameter* specifies which parameter to set. *count* specifies the number of values in *values*.

Drivers should override this method to support custom parameters. The default implementation returns `IO_R_UNSUPPORTED`.

##### setDeviceKind:

```objc
- (void)setDeviceKind:(IOString)kind
```

Sets the device kind string. *kind* should be a descriptive string such as "NetworkController", "DiskController", "Sound", etc. This string is used by the Configure application to organize devices.

Must be called after `registerDevice` during initialization.

##### setIntValues:forParameter:count:

```objc
- (IOReturn)setIntValues:(unsigned int *)values
            forParameter:(IOParameterName)parameter
                   count:(unsigned int)count
```

Sets integer array parameter values. *parameter* specifies which parameter to set. *count* specifies the number of values in *values*.

Drivers should override this method to support custom parameters. The default implementation returns `IO_R_UNSUPPORTED`.

##### setLocation:

```objc
- (void)setLocation:(IOString)location
```

Sets the device location string. *location* should describe the physical location of the device, such as "Slot 2" or "Built-in".

Must be called after `registerDevice` during initialization.

##### setName:

```objc
- (void)setName:(IOString)name
```

Sets the device name. *name* should be a short, unique identifier for the device instance, such as "sd0", "en0", "Sound", etc.

Must be called after `registerDevice` during initialization.

---

### IODirectDevice

**Inherits From:** IODevice : Object

**Declared In:** driverkit/IODirectDevice.h
driverkit/architecture/directDevice.h
driverkit/architecture/IOPCIDirectDevice.h
driverkit/architecture/IOPCMCIADirectDevice.h

#### Class Description

IODirectDevice is a device-independent abstract class that is the superclass of all direct device driver classes. Most of the functionality of IODirectDevice is provided by device-dependent categories, which are described in detail below. IODirectDevice provides:

- An implementation of the `deviceStyle` IODevice class method, so IODirectDevice subclasses don't have to override it
- Methods for getting and setting IODirectDevice information, such as the interrupt port and the IODeviceDescription
- A default I/O thread that listens for messages to the interrupt port
- An efficient way to receive messages, to be used by drivers that provide their own I/O thread (see the `waitForInterrupt:` method description)

To use the default I/O thread, subclasses invoke one of the `startIOThread...` methods and implement one or more of the following methods:

- `interruptOccurred` or `interruptOccurredAt:`
- `timeoutOccurred`
- `commandRequestOccurred`
- `otherOccurred:`
- `receiveMsg`

Each of these methods is invoked when the I/O thread receives a corresponding Mach message on its interrupt port. For example, when the kernel sends an `IO_DEVICE_INTERRUPT_MSG` Mach message to the interrupt port, the I/O thread receives it and invokes `interruptOccurred`. The documentation for `startIOThread` describes in detail how the I/O thread listens for Mach messages and which methods it invokes in response to which Mach messages.

Interrupt messages are the only Mach messages that the kernel automatically sends. If you want to receive other types of Mach messages, your driver or some other module it works with must explicitly send them. For example, if you want your driver's `timeoutOccurred` method to be invoked by the I/O thread, you must ensure that your driver sends an `IO_TIMEOUT_MSG` at some point. Some classes, such as IOEthernet, have this functionality built in. Others, such as IOSCSIController, don't. See the IOSCSIController class description for an example of how to send a message.

##### ISA and EISA IODirectDevices

The IOEISADirectDevice category of IODirectDevice defined in the header file driverkit/i386/directDevice.h provides the following additional functionality for IODirectDevices that control hardware on ISA or EISA Intel-based computers:

- Reserving and releasing ranges of I/O ports
- Reserving, releasing, enabling, and disabling interrupts (also known as IRQs)
- A way of providing an interrupt handler, if interrupt messages aren't sufficient
- Mapping device memory into virtual memory
- Reserving and releasing DMA channels
- Starting DMA and dealing with DMA buffers
- Determining whether the computer has EISA slots

**Note:** The ISA/EISA category works for all hardware attached to ISA and EISA computers--ISA slots, EISA slots, VL-Bus, and so on. Remember that EISA computers can have ISA slots, but ISA computers don't have EISA slots.

I/O ports, interrupts, device memory ranges, and DMA channels are collectively known as resources.

##### PCI IODirectDevices

The IOPCIDirectDevice category of IODirectDevice defined in the header file driverkit/i386/IOPCIDirectDevice.h provides the following additional functionality for IODirectDevices that control hardware on PCI Intel-based computers:

- Indicating whether the PCI bus is enabled or not
- Reading and writing the device's configuration space

The PCI configuration space is memory available for configuration information for each device. A 256-byte portion is available for each device, addressed by the PCI anchor, which consists of three fields:

- Device number between 0 and 31
- Function number between 0 and 7
- Bus number between 0 and 255

Methods can either read or write the entire configuration space or access individual 32-bit pieces, accessing it by a register address--a byte address into the 256-byte portion.

##### PCMCIA IODirectDevices

The IOPCMCIADirectDevice category of IODirectDevice defined in the header file driverkit/i386/IOPCMCIADirectDevice.h provides the following additional functionality for IODirectDevices that control hardware on PCMCIA Intel-based computers:

- Mapping and unmapping attribute memory

Attribute memory resides on the PCMCIA card and contains tuples, i.e., configuration information that's stored on the card. To access attribute memory, you must map the memory using the mapping method; when you've completed your access, you must unmap it with the method provided. If you attempt to map the memory and it's already mapped, the mapping method returns failure status.

##### Local Equivalents of Resources

The ISA/EISA category refers to resources not by their actual numbers or addresses, but by their local equivalent. The local equivalent of a resource is the position (starting at 0) of that resource in the configuration list of all resources of that type.

For example, if a device is configured to have one DMA channel (DMA channel 6, for example), the local equivalent of that channel is 0. If a device is configured to have two DMA channels (specified in order as 4 and 6, for example), then channel 4 has the local equivalent of 0, and channel 6 has the local equivalent of 1.

Similarly, the first range of I/O ports in a device's configuration has the local equivalent of 0, the second range is 1, and so on.

The local equivalent is used in all ISA/EISA methods that refer to DMA channels, specific interrupts, I/O ports, and memory ranges. For example, to enable the first DMA channel in a device's configuration, a driver sends an `enableChannel:` message to self, specifying 0 as the channel.

See Chapter 4 and Chapter 5, "Configuration Keys" in "Other Features" for information on configuration files.

##### Implementing a Subclass

The IODirectDevice methods you must implement in a subclass depend on your driver's capabilities. To start with, you must implement all the methods that IODevice requires, except for `deviceStyle`, which is implemented by IODirectDevice. You must also implement `initFromDeviceDescription:` to perform any driver- or device-specific initialization.

If your device performs DMA, you must implement `startDMAForBuffer:channel:`.

If your device can interrupt, you generally need to implement either `interruptOccurred` (if your device uses only one interrupt) or `interruptOccurredAt:`. If your driver needs to handle some interrupts directly, instead of receiving interrupt notification by Mach messages, you must implement `getHandler:level:argument:forInterrupt:`.

If your driver uses other Mach messages, you might also need to implement `timeoutOccurred`, `commandRequestOccurred`, `otherOccurred:`, or `receiveMsg`.

Most drivers need an I/O thread, as discussed in Chapter 1. All Driver Kit subclasses of IODirectDevice (such as IOEthernet) provide an I/O thread for you, if necessary. However, if your class is a direct subclass of IODirectDevice, you need to provide your own I/O thread. You can do so by invoking one of the `startIOThread...` methods.

#### Instance Variables

None declared.

#### Method Types

**Architecture-Independent Methods:**

**Freeing instances**
- `– free`

**Registering the class**
- `+ deviceStyle`

**Getting and setting the interrupt port**
- `– attachInterruptPort`
- `– interruptPort`

**Handling messages to the interrupt port**
- `– commandRequestOccurred`
- `– interruptOccurred`
- `– interruptOccurredAt:`
- `– receiveMsg`
- `– timeoutOccurred`
- `– waitForInterrupt:`

**Running an I/O thread**
- `– startIOThread`
- `– startIOThreadWithPriority:`
- `– startIOThreadWithFixedPriority:`

**Getting and setting the IODeviceDescription**
- `– deviceDescription`
- `– setDeviceDescription:`

**ISA/EISA Architecture Methods:**

**Initializing instances**
- `– initFromDeviceDescription:`

**Reserving I/O ports**
- `– reservePortRange:`
- `– releasePortRange:`

**Dealing with interrupts**
- `– enableAllInterrupts`
- `– disableAllInterrupts`
- `– reserveInterrupt:`
- `– releaseInterrupt:`
- `– enableInterrupt:`
- `– disableInterrupt:`
- `– getHandler:level:argument:forInterrupt:`

**Mapping memory**
- `– mapMemoryRange:to:findSpace:cache:`
- `– unmapMemoryRange:from:`

**Dealing with DMA channels**
- `– enableChannel:`
- `– disableChannel:`
- `– reserveChannel:`
- `– releaseChannel:`

**Dealing with DMA buffers**
- `– startDMAForBuffer:channel:`
- `– createDMABufferFor:length:read:needsLowMemory:limitSize:`
- `– freeDMABuffer:`
- `– abortDMABuffer:`

**Setting the DMA mode**
- `– setTransferMode:forChannel:`
- `– setAutoinitialize:forChannel:`
- `– setIncrementMode:forChannel:`

**Using the EISA extended mode register**
- `– setDMATransferWidth:forChannel:`
- `– setDMATiming:forChannel:`
- `– setEOPAsOutput:forChannel:`
- `– setStopRegisterMode:forChannel:`

**Getting a DMA channel's status**
- `– currentAddressForChannel:`
- `– currentCountForChannel:`
- `– getDMATransferWidth:forChannel:`
- `– isDMADone:`

**Optional DMA locking**
- `– reserveDMALock`
- `– releaseDMALock`

**Getting information about EISA slots**
- `– isEISAPresent`
- `– getEISAId:forSlot:`

**PCI Architecture Methods:**

**Determining if PCI bus support is enabled**
- `+ isPCIPresent`
- `– isPCIPresent`

**Reading and writing the entire configuration space**
- `+ getPCIConfigSpace:withDeviceDescription:`
- `+ setPCIConfigSpace:withDeviceDescription:`
- `– getPCIConfigSpace:withDeviceDescription:`
- `– setPCIConfigSpace:withDeviceDescription:`

**Reading and writing the configuration space**
- `+ getPCIConfigData:atRegister:withDeviceDescription:`
- `+ setPCIConfigData:atRegister:withDeviceDescription:`
- `– getPCIConfigData:atRegister:withDeviceDescription:`
- `– setPCIConfigData:atRegister:withDeviceDescription:`

**PCMCIA Architecture Methods:**

**Managing attribute memory**
- `– mapAttributeMemoryTo:findSpace:`
- `– unmapAttributeMemory:`

#### Class Methods

##### deviceStyle

```objc
+ (IODeviceStyle)deviceStyle
```

Reports the basic style of driver as `IO_DirectDevice`. Because IODirectDevice implements this method, its subclasses don't have to.

**See also:** `+ deviceStyle` (IODevice)

##### isPCIPresent (PCI)

```objc
+ (BOOL)isPCIPresent
```

Returns YES if PCI Bus support is enabled. Returns NO otherwise.

##### getPCIConfigSpace:withDeviceDescription: (PCI)

```objc
+ (IOReturn)getPCIConfigSpace:(IOPCIConfigSpace *)configurationSpace
         withDeviceDescription:description
```

Reads the device's entire configuration space using the IOPCIDeviceDescription *description*. Returns `IO_R_SUCCESS` if successful. If this method fails, the driver should make no assumptions about the state of the data returned in the IOPCIConfigSpace struct.

##### setPCIConfigSpace:withDeviceDescription: (PCI)

```objc
+ (IOReturn)setPCIConfigSpace:(IOPCIConfigSpace *)configurationSpace
         withDeviceDescription:description
```

Writes the device's entire configuration space using the IOPCIDeviceDescription *description*. Returns `IO_R_SUCCESS` if successful. If this method fails, the driver should make no assumptions about the state of the device's configuration space.

##### getPCIConfigData:atRegister:withDeviceDescription: (PCI)

```objc
+ (IOReturn)getPCIConfigData:(unsigned long *)data
                  atRegister:(unsigned char)address
        withDeviceDescription:description
```

Reads from the device's configuration space at the byte address *address* using the IOPCIDeviceDescription *description*. All accesses are 32 bits wide and *address* must be aligned as such.

##### setPCIConfigData:atRegister:withDeviceDescription: (PCI)

```objc
+ (IOReturn)setPCIConfigData:(unsigned long)data
                  atRegister:(unsigned char)address
        withDeviceDescription:description
```

Writes to the device's configuration space at the byte address *address* using the IOPCIDeviceDescription *description*. All accesses are 32 bits wide and *address* must be aligned as such.

#### Instance Methods (Architecture-Independent)

##### attachInterruptPort

```objc
- (IOReturn)attachInterruptPort
```

Creates the interrupt port, if none exists already, and requests that the interrupt port receive all interrupt messages for the device's reserved interrupts. This method is invoked whenever an interrupt is enabled. Returns `IO_R_SUCCESS` if successful; otherwise, returns `IO_R_NOT_ATTACHED`.

**See also:** `– interruptPort`, `– enableAllInterrupts`

##### commandRequestOccurred

```objc
- (void)commandRequestOccurred
```

Does nothing; subclasses can implement this method if desired. This method is invoked by the default I/O thread (implemented by `startIOThread...`) whenever it receives a bodyless message with ID `IO_COMMAND_MSG`. The part of a driver that handles user requests can use this message to notify the I/O thread that it should execute a command that's been placed in global data.

**See also:** `– startIOThread`

##### deviceDescription

```objc
- deviceDescription
```

Returns the IODeviceDescription associated with this instance.

**See also:** `– setDeviceDescription:`

##### free

```objc
- free
```

Deallocates the IODirectDevice's memory and its interrupt port, if one exists. Returns `nil`.

##### interruptOccurred

```objc
- (void)interruptOccurred
```

Invokes `interruptOccurredAt:` with an argument of zero. This method is invoked by the default I/O thread (implemented by `startIOThread...`) whenever it receives a bodyless Mach message with the ID `IO_DEVICE_INTERRUPT_MSG`. Subclasses that support only one interrupt should implement this method so that it processes the hardware interrupt, as described in Chapters 1 and 2.

**See also:** `– interruptOccurredAt:`, `– startIOThread`

##### interruptOccurredAt:

```objc
- (void)interruptOccurredAt:(int)localInterrupt
```

Does nothing; subclasses that need to handle interrupts should implement this method so that it processes the hardware interrupt, as described in Chapter 1. This method is invoked by the default I/O thread (implemented by `startIOThread...`) whenever it receives a bodyless Mach message with an ID between `IO_DEVICE_INTERRUPT_MSG_FIRST` and `IO_DEVICE_INTERRUPT_MSG_LAST` (excluding `IO_DEVICE_INTERRUPT_MSG`).

**See also:** `– interruptOccurred`, `– startIOThread`

##### interruptPort

```objc
- (port_t)interruptPort
```

Returns the Mach port on which the IODirectDevice should receive interrupt messages. The returned `port_t` is in the context of the kernel I/O task.

**See also:** `– attachInterruptPort`

##### otherOccurred:

```objc
- (void)otherOccurred:(int)msgID
```

Does nothing; subclasses can implement this method if desired. This method is invoked by the default I/O thread (implemented by `startIOThread...`) whenever it receives a bodyless message with an unrecognized ID. The ID is given in *msgID*.

**See also:** `– receiveMsg`, `– startIOThread`

##### receiveMsg

```objc
- (void)receiveMsg
```

Dequeues the next Mach message from the interrupt port and throws it away; subclasses can implement this method if desired to handle custom messages. This method is invoked by the default I/O thread (implemented by `startIOThread...`) whenever it tries to receive a message that has a body. To implement this message, you need to call `msg_receive()` on the interrupt port. In this sample implementation, fill in the italicized text between angle brackets with device-specific code:

```objc
- (void)receiveMsg
{
    IOReturn        result;
    port_t          inPort;
    MyMsg           myMsg;
    kern_return_t   result;

    inPort = [self interruptPort];
    if (inPort == PORT_NULL) {
        /* React to having no interrupt port. */
        return;
    }

    myMsg.header.msg_size = sizeof(myMsg);
    myMsg.header.msg_local_port = inPort;

    result = msg_receive(&myMsg.header, (msg_option_t)RCV_TIMEOUT, 0);

    if (result != RCV_SUCCESS) {
        IOLog("%s receiveMsg: msg_receive returns %d\n", result);
        return;
    }
    else {
        switch (myMsg.header.msg_id) {
            case MyMsg1:
                [self handleMsg1];
                break;

            case MyMsg2:
                [self handleMsg2];
                break;
            /* ... */
        }
    }
}
```

**See also:** `– otherOccurred:`, `– startIOThread`

##### setDeviceDescription:

```objc
- (void)setDeviceDescription:deviceDescription
```

Records *deviceDescription* as the IODeviceDescription associated with this instance. ISA/EISA-architecture devices don't need to invoke this method because `initFromDeviceDescription:` already does so.

**See also:** `– deviceDescription`

##### startIOThread

```objc
- (IOReturn)startIOThread
```

Invokes `attachInterruptPort` and, if attaching the interrupt port was successful, forks a thread to serve as the instance's I/O thread. This thread, which is appropriate for most drivers, sits in an endless loop that does the following:

1. Waits for a Mach message on the interrupt port by invoking `waitForInterrupt:`
2. If the message couldn't be dequeued because it was too large, invokes `receiveMsg` so that the subclass can dequeue and handle the message itself
3. If the message is dequeued successfully, invokes one of five methods, depending on the message ID:

| Message ID | Method Invoked |
|------------|----------------|
| `IO_TIMEOUT_MSG` | `timeoutOccurred` |
| `IO_COMMAND_MSG` | `commandRequestOccurred` |
| `IO_DEVICE_INTERRUPT_MSG` | `interruptOccurred` |
| `IO_DEVICE_INTERRUPT_MSG_FIRST` to `IO_DEVICE_INTERRUPT_MSG_LAST` | `interruptOccurredAt:` |
| (anything else) | `otherOccurred:` |

Returns the value returned by `attachInterruptPort`.

**See also:** `– startIOThreadWithFixedPriority:`, `– startIOThreadWithPriority:`

##### startIOThreadWithFixedPriority:

```objc
- (IOReturn)startIOThreadWithFixedPriority:(int)priority
```

The same as `startIOThreadWithPriority:`, except that the I/O thread's priority never lessens due to aging. This method lets you do performance tuning by disabling priority aging.

For more information about scheduling policies and priorities, see Chapter 1 of the NEXTSTEP Operating System Software manual.

**See also:** `– startIOThread`, `– startIOThreadWithPriority:`

##### startIOThreadWithPriority:

```objc
- (IOReturn)startIOThreadWithPriority:(int)priority
```

The same as `startIOThread`, except that the I/O thread runs at the specified priority. This method lets you do performance tuning by raising or lowering the thread's scheduling priority. By default, kernel I/O threads start with a priority equal to the maximum user priority (currently 18).

For more information about priorities, see Chapter 1 of the NEXTSTEP Operating System Software manual.

**See also:** `– startIOThread`, `– startIOThreadWithFixedPriority:`

##### timeoutOccurred

```objc
- (void)timeoutOccurred
```

Does nothing; subclasses that support timeouts can implement this method. See the IOEthernet class for an example of implementing this method as part of timeout support. This method is invoked by the default I/O thread (implemented by `startIOThread...`) whenever it receives a bodyless Mach message with an ID of `IO_TIMEOUT_MSG`. See the IOSCSIController class for an example of sending Mach messages.

**See also:** `– startIOThread`

##### waitForInterrupt:

```objc
- (IOReturn)waitForInterrupt:(int *)msgID
```

Listens to the interrupt port until it detects a Mach message; dequeues the message if possible. This method should be invoked by the I/O thread whenever the thread needs to listen to the interrupt port. The default I/O thread provided by IODirectDevice invokes this message as described under `startIOThread`.

If the interrupt port hasn't been set, this message returns `IO_R_NO_INTERRUPT`. If the message has a body, this method leaves the message on the queue and returns `IO_R_MSG_TOO_LARGE`. If the message couldn't be dequeued due to another reason, this method returns `IO_R_IPC_FAILURE` and logs an error message.

If a message is already on the queue when this method is invoked, this method dequeues the message and then attempts to give up the processor before returning. Without this precaution, a thread with many messages queued could prevent other kernel threads from being executed.

If this method successfully detects and dequeues a message, it sets *msgId* to the message's ID and returns `IO_R_SUCCESS`.

**See also:** `– startIOThread`

#### Instance Methods (ISA/EISA Architecture)

##### initFromDeviceDescription:

```objc
- initFromDeviceDescription:deviceDescription
```

Initializes and returns the IODirectDevice instance. Records *deviceDescription* as the IODeviceDescription corresponding to this IODirectDevice. Reserves all the interrupts, DMA channels, and I/O ports specified in *deviceDescription*. If any resources can't be reserved, releases all resources and returns `nil`.

This method must be invoked before any methods that require local equivalents of resources can be used. For example, `mapMemoryRange:...` requires that you specify the local equivalent of a memory range. However, IODirectDevices don't know what memory ranges they can use until `initFromDeviceDescription:` has been invoked. This means, for example, that subclass implementations of `initFromDeviceDescription:` must invoke the superclass's implementation of `initFromDeviceDescription:` before they can map any memory ranges or do anything else that requires access to resources.

##### reservePortRange:

```objc
- (IOReturn)reservePortRange:(unsigned int)localPortRange
```

Releases the range of I/O ports corresponding to *localPortRange* and returns `IO_R_SUCCESS`.

You don't normally have to invoke this method, since `initFromDeviceDescription:` reserves all the device's I/O ports.

**See also:** `– releasePortRange:`

##### releasePortRange:

```objc
- (void)releasePortRange:(unsigned int)localPortRange
```

Releases the range of I/O ports corresponding to *localPortRange*.

**See also:** `– reservePortRange:`

##### enableAllInterrupts

```objc
- (IOReturn)enableAllInterrupts
```

Creates and attaches an interrupt port, if one isn't already attached, and enables all interrupts associated with this IODirectDevice. Returns `IO_R_NO_INTERRUPT` if the interrupt port couldn't be attached; otherwise, returns `IO_R_SUCCESS`.

Because this method uses a local equivalent of a resource, it can't be invoked until after this category's implementation of `initFromDeviceDescription:` is invoked.

**See also:** `– attachInterruptPort`, `– disableAllInterrupts`, `– enableInterrupt:`

##### disableAllInterrupts

```objc
- (void)disableAllInterrupts
```

Disables all interrupts associated with this IODirectDevice, so that no interrupts can be generated by the hardware. Returns `IO_R_NO_INTERRUPT` if no interrupt port is attached; otherwise, returns `IO_R_SUCCESS`.

**Note:** Even after invoking `disableAllInterrupts:` successfully, your driver may still receive interrupt messages for interrupts that occurred before they were disabled.

Because this method uses a local equivalent of a resource, it can't be invoked until after this category's implementation of `initFromDeviceDescription:` is invoked.

**See also:** `– enableAllInterrupts`, `– disableInterrupt:`

##### reserveInterrupt:

```objc
- (IOReturn)reserveInterrupt:(unsigned int)localInterrupt
```

Reserves the interrupt corresponding to *localInterrupt* so that no other device can use it. Returns `IO_R_NOT_ATTACHED` if *localInterrupt* doesn't correspond to an interrupt or if another device has already reserved the interrupt. Otherwise, returns `IO_R_SUCCESS`.

You don't normally have to invoke this method, since `initFromDeviceDescription:` reserves all the device's interrupts.

**See also:** `– releaseInterrupt:`

##### releaseInterrupt:

```objc
- (void)releaseInterrupt:(unsigned int)localInterrupt
```

Releases the interrupt corresponding to *localInterrupt* so that another device can use the interrupt.

**See also:** `– reserveInterrupt:`

##### enableInterrupt:

```objc
- (IOReturn)enableInterrupt:(unsigned int)localInterrupt
```

Invokes `attachInterruptPort` and, if `attachInterruptPort` succeeds, enables the interrupt corresponding to *localInterrupt* and returns `IO_R_SUCCESS`. If `attachInterruptPort` doesn't succeed, returns `IO_R_NOT_ATTACHED`.

Because this method uses a local equivalent of a resource, it can't be invoked until after this category's implementation of `initFromDeviceDescription:` is invoked.

**See also:** `– disableInterrupt:`, `– enableAllInterrupts`

##### disableInterrupt:

```objc
- (void)disableInterrupt:(unsigned int)localInterrupt
```

Disables the interrupt corresponding to *localInterrupt*.

Because this method uses a local equivalent of a resource, it can't be invoked until after this category's implementation of `initFromDeviceDescription:` is invoked.

**See also:** `– disableAllInterrupts`, `– enableInterrupt:`

##### getHandler:level:argument:forInterrupt:

```objc
- (BOOL)getHandler:(IOInterruptHandler *)handler
             level:(unsigned int *)ipl
          argument:(unsigned int *)arg
      forInterrupt:(unsigned int)localInterrupt
```

Does nothing and returns NO. Subclasses can implement this method to specify a function to directly handle the interrupt specified by *localInterrupt*. This method is invoked every time an interrupt is enabled.

If this method returns YES, interrupts from the device result directly in a call to *handler*, with the driver-dependent argument *arg*, at interrupt level *ipl*. Otherwise, interrupts result in a Mach message to the instance's interrupt port.

If you implement this method, you should use interrupt level 3 (`IPLDEVICE`, as defined in kernserv/i386/spl.h) unless a higher interrupt level is absolutely necessary. Using interrupt levels greater than 3 requires great care and a good grasp of NeXT kernel internals.

**Note:** The interrupt level is different from the interrupt number (which is also known as the IRQ). The kernel handles interrupts on each of the 15 IRQs at an interrupt level between 0 and 7; the default is 3. The interrupt level determines which devices can interrupt; specifically, only devices with an interrupt level higher than the current interrupt level can interrupt. For example, a device that interrupts using IRQ 9 might have a direct interrupt handler that runs at interrupt level 3. While this interrupt handler is running, other devices with handlers that run at interrupt level 3 can't interrupt the CPU.

Here's a typical implementation of this method:

```objc
- (BOOL)getHandler:(IOEISAInterruptHandler *)handler
             level:(unsigned int *)ipl
          argument:(unsigned int *)arg
      forInterrupt:(unsigned int)localInterrupt
{
    *handler = myIntHandler;
    *ipl = IPLDEVICE;
    *arg = 0;
    return YES;
}
```

In the example above, `myIntHandler` is the function that handles the interrupt. It might be implemented as follows:

```objc
static void myIntHandler(void *identity, void *state, unsigned int arg)
{
    /* ... Do what we must at interrupt level ... */
    if (/* I/O thread doesn't need to know about this interrupt */)
        return;

    /* Forward this to the I/O thread for further handling. */
    IOSendInterrupt(identity, state, IO_DEVICE_INTERRUPT_MSG);
}
```

**See also:** `IOSendInterrupt()`

##### mapMemoryRange:to:findSpace:cache:

```objc
- (IOReturn)mapMemoryRange:(unsigned int)localMemoryRange
                        to:(vm_address_t *)destinationAddress
                 findSpace:(BOOL)findSpace
                     cache:(IOCache)caching
```

Maps the device memory corresponding to *localMemoryRange* into the calling task's address space. *localMemoryRange* is the local range number in the device description.

If *findSpace* is TRUE, this method ignores the *destinationAddress* and determines where the mapped memory should go, returning the value in *destinationAddress*. If *findSpace* is FALSE, this method truncates *destinationAddress* to the nearest page boundary, maps the memory to the truncated address, and returns the truncated address.

The *caching* argument determines how the memory is cached. Usually, it should be `IO_WriteThrough`. However, if caching seems to be causing problems, try using `IO_CacheOff` instead.

If *localMemoryRange* doesn't correspond to one of this device's memory ranges, `IO_R_INVALID_ARG` is returned. There must also be more than one I/O port range associated with the device (i.e., `[deviceDescription numPortRanges] > 1`); otherwise `IO_R_INVALID_ARG` is returned. If the mapping couldn't be performed for another reason, `IO_R_NO_SPACE` is returned. If the mapping was successful, returns `IO_R_SUCCESS`.

Because this method uses a local equivalent of a resource, it can't be invoked until after this category's implementation of `initFromDeviceDescription:` is invoked.

**See also:** `– unmapMemoryRange:from:`

##### unmapMemoryRange:from:

```objc
- (void)unmapMemoryRange:(unsigned int)localMemoryRange
                    from:(vm_address_t)address
```

Unmaps the device memory corresponding to *localMemoryRange* from the calling task's address space. The value of *address* must be the same as the value returned by the *destinationAddress* argument of `mapMemoryRange:to:findSpace:cache:` for the same *localMemoryRange*.

**See also:** `– mapMemoryRange:to:findSpace:cache:`

##### enableChannel:

```objc
- (IOReturn)enableChannel:(unsigned int)localChannel
```

Enables transfers on the DMA channel corresponding to *localChannel*. Returns `IO_R_NOT_ATTACHED` if *localChannel* doesn't correspond to a DMA channel or if the DMA channel isn't reserved by this device. Otherwise, returns `IO_R_SUCCESS`.

Because this method uses a local equivalent of a resource, it can't be invoked until after this category's implementation of `initFromDeviceDescription:` is invoked.

**See also:** `– disableChannel:`, `– startDMAForBuffer:channel:`

##### disableChannel:

```objc
- (void)disableChannel:(unsigned int)localChannel
```

If the DMA channel corresponding to *localChannel* is reserved by this device, this method disables the channel. You typically disable the channel just before changing its setting. You need to invoke `enableChannel:` once the channel is set up so that transfers can occur.

Because this method uses a local equivalent of a resource, it can't be invoked until after this category's implementation of `initFromDeviceDescription:` is invoked.

**See also:** `– enableChannel:`

##### reserveChannel:

```objc
- (IOReturn)reserveChannel:(unsigned int)localChannel
```

Reserves the DMA channel corresponding to *localChannel* so that no other device can use the channel. Returns `IO_R_NOT_ATTACHED` if *localChannel* doesn't correspond to a DMA channel or if the DMA channel is reserved by another device. Otherwise, returns `IO_R_SUCCESS`.

You don't normally have to invoke this method, since `initFromDeviceDescription:` reserves all the device's DMA channels.

**See also:** `– releaseChannel:`

##### releaseChannel:

```objc
- (void)releaseChannel:(unsigned int)localChannel
```

Releases the DMA channel corresponding to *localChannel* so that another device can use the channel.

**See also:** `– reserveChannel:`

##### startDMAForBuffer:channel:

```objc
- (IOReturn)startDMAForBuffer:(IOEISADMABuffer)buffer
                      channel:(unsigned int)localChannel
```

Begins DMA with *buffer* on the DMA channel specified by *localChannel*, and returns `IO_R_SUCCESS`. DMA isn't started if *localChannel* doesn't correspond to a DMA channel (in which case `IO_R_INVALID_ARG` is returned), if the DMA channel isn't assigned, or if no DMA frames could be allocated (`IO_R_NO_FRAMES` is returned).

Because this method uses a local equivalent of a resource, it can't be invoked until after this category's implementation of `initFromDeviceDescription:` is invoked.

##### createDMABufferFor:length:read:needsLowMemory:limitSize:

```objc
- (IOEISADMABuffer)createDMABufferFor:(unsigned int *)physicalAddress
                               length:(unsigned int)numBytes
                                 read:(BOOL)isRead
                       needsLowMemory:(BOOL)lowerMem
                            limitSize:(BOOL)limitSize
```

Returns a DMA buffer for the contents of physical memory starting at *physicalAddress* and continuing for *numBytes* bytes. You should specify YES for *isRead* if the data will be read from the device; if the data will be written to the device, specify NO. *lowerMem* should be YES if the transfer must be from or to the first 16MB of physical memory (as required by some ISA devices); otherwise, it should be NO. To limit the size of the transfer to 64KB, specify *limitSize* as YES; otherwise, *limitSize* should be NO.

This method changes the physical address if necessary to accommodate the ISA bus. When the physical address is changed, the data is copied to the new physical address (if the transfer is a write), and the new physical address is returned in *physicalAddress*.

Returns NULL if kernel memory for the buffer couldn't be allocated.

**See also:** `– freeDMABuffer:`

##### freeDMABuffer:

```objc
- (void)freeDMABuffer:(IOEISADMABuffer)buffer
```

Completes the transfer associated with *buffer* and frees the buffer. *buffer* should be a value returned by `createDMABufferFor:...`. If `createDMABufferFor:...` changed the physical address and the transfer is a read, this method moves the data from the new physical address to the old one. In other words, any data that's read appears at the address passed to `createDMABufferFor:...` in the *physicalAddress* argument, not at the address returned in *physicalAddress*.

**See also:** `– abortDMABuffer:`, `– createDMABufferFor:length:read:needsLowMemory:limitSize:`

##### abortDMABuffer:

```objc
- (void)abortDMABuffer:(IOEISADMABuffer)buffer
```

Frees the memory allocated to *buffer*. If a read transfer is in progress, the data read is lost.

**See also:** `– freeDMABuffer:`

##### setTransferMode:forChannel:

```objc
- (IOReturn)setTransferMode:(IODMATransferMode)mode
                 forChannel:(unsigned int)localChannel
```

Sets the specified channel's transfer mode to *mode*. The new transfer mode stays in effect until this method is invoked again or the computer is rebooted.

If *localChannel* doesn't correspond to a DMA channel, this method does nothing and returns `IO_R_INVALID_ARG`. If the DMA channel isn't reserved by this device, this method does nothing and returns `IO_R_NOT_ATTACHED`. Otherwise, this method returns `IO_R_SUCCESS`.

Because this method uses a local equivalent of a resource, it can't be invoked until after this category's implementation of `initFromDeviceDescription:` is invoked.

**See also:** `– setAutoinitialize:forChannel:`, `– setIncrementMode:forChannel:`

##### setAutoinitialize:forChannel:

```objc
- (IOReturn)setAutoinitialize:(BOOL)flag
                   forChannel:(unsigned int)localChannel
```

Sets the specified channel's autoinitialize DMA mode to on if *flag* is YES; otherwise, sets it off. The new autoinitialize mode stays in effect until this method is invoked again or the computer is rebooted. By default, autoinitialize mode is disabled.

If *localChannel* doesn't correspond to a DMA channel, this method does nothing and returns `IO_R_INVALID_ARG`. If the DMA channel isn't reserved by this device, this method does nothing and returns `IO_R_NOT_ATTACHED`. Otherwise, this method returns `IO_R_SUCCESS`.

Because this method uses a local equivalent of a resource, it can't be invoked until after this category's implementation of `initFromDeviceDescription:` is invoked.

**See also:** `– setIncrementMode:forChannel:`, `– setTransferMode:forChannel:`

##### setIncrementMode:forChannel:

```objc
- (IOReturn)setIncrementMode:(IOIncrementMode)mode
                  forChannel:(unsigned int)localChannel
```

This method lets the driver specify how the start address and length of its DMA buffers should be interpreted. By default, the increment mode is `IO_Increment`, so each DMA buffer is interpreted so that if the start address is *n* and the length is *m*, the data in addresses *n* through *n + m − 1* are transferred. By setting the increment mode to `IO_Decrement`, however, the driver specifies that the affected addresses should be *n* through *n − m + 1*. The new increment mode is in effect until this method is invoked again or until the computer is rebooted.

If *localChannel* doesn't correspond to a DMA channel, this method does nothing and returns `IO_R_INVALID_ARG`. If the DMA channel isn't reserved by this device, this method does nothing and returns `IO_R_NOT_ATTACHED`. Otherwise, this method returns `IO_R_SUCCESS`.

**Note:** `IO_Decrement` mode is not currently supported.

Because this method uses a local equivalent of a resource, it can't be invoked until after this category's implementation of `initFromDeviceDescription:` is invoked.

**See also:** `– setAutoinitialize:forChannel:`, `– setTransferMode:forChannel:`

##### setDMATransferWidth:forChannel:

```objc
- (IOReturn)setDMATransferWidth:(IOEISADMATransferWidth)width
                     forChannel:(unsigned int)localChannel
```

Makes the specified channel use the specified width for DMA transfers. The width can be 8-bit (`IO_8Bit`), 16-bit (`IO_16BitByteCount`), or 32-bit (`IO_32Bit`). The 16-bit mode requires byte counting, not word counting (which is unsupported). This method is valid only on EISA systems.

If the system is ISA-based, this method does nothing and returns `IO_R_UNSUPPORTED`. If *localChannel* doesn't correspond to a DMA channel, this method does nothing and returns `IO_R_INVALID_ARG`. If the DMA channel isn't reserved by this device, this method does nothing and returns `IO_R_NOT_ATTACHED`. Otherwise, this method returns `IO_R_SUCCESS`.

Because this method uses a local equivalent of a resource, it can't be invoked until after this category's implementation of `initFromDeviceDescription:` is invoked.

##### setDMATiming:forChannel:

```objc
- (IOReturn)setDMATiming:(IOEISADMATiming)timing
              forChannel:(unsigned int)localChannel
```

Makes the specified channel use the specified DMA bus cycle--ISA-compatible (`IO_Compatible`), Type A (`IO_TypeA`), Type B (`IO_TypeB`), or burst (`IO_Burst`), which is also known as Type C. This method is valid only on EISA systems.

If the system is ISA-based, this method does nothing and returns `IO_R_UNSUPPORTED`. If *localChannel* doesn't correspond to a DMA channel, this method does nothing and returns `IO_R_INVALID_ARG`. If the DMA channel isn't reserved by this device, this method does nothing and returns `IO_R_NOT_ATTACHED`. Otherwise, this method returns `IO_R_SUCCESS`.

Because this method uses a local equivalent of a resource, it can't be invoked until after this category's implementation of `initFromDeviceDescription:` is invoked.

##### setEOPAsOutput:forChannel:

```objc
- (IOReturn)setEOPAsOutput:(BOOL)flag
                forChannel:(unsigned int)localChannel
```

Selects whether the specified channel's EOP pin is an output signal (the default) or an input signal. This method is valid only on EISA systems.

If the system is ISA-based, this method does nothing and returns `IO_R_UNSUPPORTED`. If *localChannel* doesn't correspond to a DMA channel, this method does nothing and returns `IO_R_INVALID_ARG`. If the DMA channel isn't reserved by this device, this method does nothing and returns `IO_R_NOT_ATTACHED`. Otherwise, this method returns `IO_R_SUCCESS`.

Because this method uses a local equivalent of a resource, it can't be invoked until after this category's implementation of `initFromDeviceDescription:` is invoked.

##### setStopRegisterMode:forChannel:

```objc
- (IOReturn)setStopRegisterMode:(IOEISAStopRegisterMode)mode
                     forChannel:(unsigned int)localChannel
```

Enables or disables the specified channel's Stop register. By default, the Stop register is disabled. You can enable it by specifying *mode* to be `IO_StopRegisterEnable`. This method is valid only on EISA systems.

**Note:** Enabling the Stop register isn't currently supported.

If the system is ISA-based or *mode* is `IO_StopRegisterEnable`, this method does nothing and returns `IO_R_UNSUPPORTED`. If *localChannel* doesn't correspond to a DMA channel, this method does nothing and returns `IO_R_INVALID_ARG`. If the DMA channel isn't reserved by this device, this method does nothing and returns `IO_R_NOT_ATTACHED`. Otherwise, this method returns `IO_R_SUCCESS`.

Because this method uses a local equivalent of a resource, it can't be invoked until after this category's implementation of `initFromDeviceDescription:` is invoked.

##### currentAddressForChannel:

```objc
- (unsigned int)currentAddressForChannel:(unsigned int)localChannel
```

Returns the physical address currently in the address register of the specified DMA channel. This method can be invoked at any time--even when DMA is in progress. This method is often used along with autoinitialize mode. It's also used to help diagnose errors when a device or channel aborts a DMA transfer.

Because this method uses a local equivalent of a resource, it can't be invoked until after this category's implementation of `initFromDeviceDescription:` is invoked.

**See also:** `– currentCountForChannel:`, `– setAutoinitialize:forChannel:`

##### currentCountForChannel:

```objc
- (unsigned int)currentCountForChannel:(unsigned int)localChannel
```

Returns the number of bytes remaining to be transferred on the specified channel. The maximum number returned is equal to the length of the DMA buffer currently being handled by the channel. This method is often used along with autoinitialize mode. It's also used to help diagnose errors when a device or channel aborts a DMA transfer.

Because this method uses a local equivalent of a resource, it can't be invoked until after this category's implementation of `initFromDeviceDescription:` is invoked.

**See also:** `– currentAddressForChannel:`, `– setAutoinitialize:forChannel:`

##### getDMATransferWidth:forChannel:

```objc
- (IOReturn)getDMATransferWidth:(IOEISADMATransferWidth *)width
                     forChannel:(unsigned int)localChannel
```

Returns in *width* the width currently used for DMA transfers on the specified channel. The width can be 8-bit (`IO_8Bit`), 16-bit (`IO_16BitByteCount`), or 32-bit (`IO_32Bit`). On EISA systems, you can set the width using `setDMATransferWidth:forChannel:`.

If *localChannel* doesn't correspond to a DMA channel, this method does nothing and returns `IO_R_INVALID_ARG`. If the DMA channel isn't reserved by this device, this method does nothing and returns `IO_R_NOT_ATTACHED`. Otherwise, this method returns `IO_R_SUCCESS`.

Because this method uses a local equivalent of a resource, it can't be invoked until after this category's implementation of `initFromDeviceDescription:` is invoked.

**See also:** `– setDMATransferWidth:forChannel:`

##### isDMADone:

```objc
- (BOOL)isDMADone:(unsigned int)localChannel
```

Returns YES if DMA has completed on the specified channel; otherwise, returns NO. If *localChannel* doesn't correspond to a DMA channel, this method does nothing and returns `IO_R_INVALID_ARG`.

Because this method uses a local equivalent of a resource, it can't be invoked until after this category's implementation of `initFromDeviceDescription:` is invoked.

##### reserveDMALock

```objc
- (void)reserveDMALock
```

Reserves the lock associated with DMA. See `releaseDMALock` for information on DMA locking.

##### releaseDMALock

```objc
- (void)releaseDMALock
```

Releases the lock associated with DMA. This method panics if this IODirectDevice doesn't hold the DMA lock.

Most drivers don't need to use DMA locking. However, the floppy drive (and possibly other devices) tends to have DMA underruns when the bus is saturated. As a result, the floppy driver and drivers for devices that tend to saturate the bus use DMA locking to avoid performing I/O at the same time. DMA locking is ignored by all other device drivers.

You don't have to use DMA locking unless your device is having DMA underruns or is causing another device to have underruns. Sometimes these underruns occur on ISA computers, but not EISA ones. If your device is causing the floppy drive to have underruns, you'll see the following error on the console while your device is performing I/O:

```
fd0: DMA Over/underrun
```

**See also:** `– reserveDMALock`

##### isEISAPresent

```objc
- (BOOL)isEISAPresent
```

Returns YES if the computer conforms to the EISA specification; otherwise, returns NO.

**See also:** `– getEISAId:forSlot:`

##### getEISAId:forSlot:

```objc
- (BOOL)getEISAId:(unsigned int *)id
          forSlot:(int)slotNumber
```

Returns in *id* the EISA id for the specified slot. Returns YES if the slot is a valid EISA slot; otherwise, returns NO. You can use this method to loop through the computer's slots, testing each slot for whether it contains a particular card. For example, the following code is executed in the QVision display driver's `initFromDeviceDescription:` method to determine whether QVision hardware is present in the system:

```objc
adapter = UnknownAdapter;
for (slot = 1; slot <= 0xF; slot++) {
    if ([self getEISAId:&product_id forSlot:slot] == YES) {
        switch (product_id) {
            case QVISION_EISA_ID:
                adapter = QVisionAdapter;
                break;
            case ORION_EISA_ID:
                adapter = OrionAdapter;
                break;
            case ORION12_EISA_ID:
                adapter = Orion12Adapter;
                break;
            case QVISION_ISA_ID:
            case ORION_ISA_ID:
            case ORION12_ISA_ID:
                IOLog("%s: Sorry, ISA cards are not supported.\n",
                    [self name]);
                break;
        }
        break;
    }
}
```

**See also:** `– isEISAPresent`

#### Instance Methods (PCI Architecture)

##### isPCIPresent

```objc
- (BOOL)isPCIPresent
```

Returns YES if PCI Bus support is enabled. Returns NO otherwise.

##### getPCIConfigSpace:

```objc
- (IOReturn)getPCIConfigSpace:(IOPCIConfigSpace *)configurationSpace
```

Reads the device's entire configuration space. Returns `IO_R_SUCCESS` if successful. If this method fails, the driver should make no assumptions about the state of the data returned in the IOPCIConfigSpace struct.

##### setPCIConfigSpace:

```objc
- (IOReturn)setPCIConfigSpace:(IOPCIConfigSpace *)configurationSpace
```

Writes the device's entire configuration space. Returns `IO_R_SUCCESS` if successful. If this method fails, the driver should make no assumptions about the state of the device's configuration space.

##### getPCIConfigData:atRegister:

```objc
- (IOReturn)getPCIConfigData:(unsigned long *)data
                  atRegister:(unsigned char)address
```

Reads from the device's configuration space at the byte address *address*. All accesses are 32 bits wide and *address* must be aligned as such.

##### setPCIConfigData:atRegister:

```objc
- (IOReturn)setPCIConfigData:(unsigned long)data
                  atRegister:(unsigned char)address
```

Writes to the device's configuration space at the byte address *address*. All accesses are 32 bits wide and *address* must be aligned as such.

#### Instance Methods (PCMCIA Architecture)

##### mapAttributeMemoryTo:findSpace:

```objc
- (IOReturn)mapAttributeMemoryTo:(vm_address_t *)destinationAddress
                       findSpace:(BOOL)findSpace
```

Maps attribute memory to *destinationAddress* in *findSpace*.

**See also:** `– unmapAttributeMemory:`

##### unmapAttributeMemory:

```objc
- (void)unmapAttributeMemory
```

Unmaps attribute memory.

**See also:** `– mapAttributeMemoryTo:findSpace:`

---

### IODisplayInspector

**Inherits From:** IODeviceInspector : Object

**Conforms To:** IOConfigurationInspector

**Declared In:** driverkit/IODisplayInspector.h

#### Class Description

This class provides inspectors used by the Configure application for all displays. It provides an accessory View to IODeviceInspector that displays the current display mode and has a button. When the button is clicked, the IODisplayInspector puts up a panel that lets the user select the display mode for the device.

The panel shows all display modes specified for the "DisplayModes" key in the driver bundle's Language.lproj/Localizable.strings file. The mode that's selected is placed in the device's Instancen.table file as the value of the "Display Mode" key.

#### Instance Variables

```objc
id displayAccessoryHolder;
id displayMode;
id panel;
id modes;
id okButton;
id selectButton;
id modeText;
IODisplayMode *modeRecs;
unsigned int modeCount;
```

**displayAccessoryHolder** – The View where the display inspector's own accessory View (as opposed to the IODeviceInspector's accessory View) is placed

**displayMode** – The accessory View provided to the IODeviceInspector

**panel** – The Select Display Mode panel

**modes** – The DBTableView where valid display modes are listed and can be selected

**okButton** – The OK button in panel

**selectButton** – The Select button in displayMode

**modeText** – The text in displayMode that shows the current display mode

**modeRecs** – An array of IODisplayModes, initialized during `setTable:` with the modes specified in the device's Default.table

**modeCount** – The number of IODisplayModes in modeRecs

#### Method Types

**Initializing the IODisplayInspector**
- `– init`

**Setting attributes**
- `– setAccessoryView:`
- `– setTable:`

**Displaying the Select Display Mode panel**
- `– runPanel:`
- `– panelDone:`

**Target and Action methods**
- `– cancel:`
- `– doubleClicked:`

#### Instance Methods

##### cancel:

```objc
- cancel:sender
```

Exits the Select Display Modes panel without changing the current display mode. Returns `self`.

**See also:** `– runPanel:`

##### doubleClicked:

```objc
- doubleClicked:sender
```

Clicks the OK button in the Select Display Modes panel. This method is invoked when the user double-clicks an item in the display modes DBTableView. Returns `self`.

**See also:** `– panelDone:`

##### init

```objc
- init
```

Initializes and returns a newly allocated IODisplayInspector. Returns `nil` and frees itself if an error occurs.

##### panelDone:

```objc
- panelDone:sender
```

Dismisses the Select Display Modes panel. This method is invoked when the user clicks the panel's OK button. Returns `self`.

**See also:** `– runPanel:`

##### runPanel:

```objc
- runPanel:sender
```

Runs the Select Display Modes panel in a modal loop. Before displaying the panel, this method reads the supported display modes from the driver bundle's Localizable.strings file, puts the modes in the panel's DBTableView, and selects the current mode. When the user clicks the Cancel or OK button the loop is broken, the panel is hidden, and, if the button was OK, the new display mode is written to the driver's configuration table. Returns `self`.

**See also:** `– cancel:`, `– panelDone:`

##### setAccessoryView:

```objc
- setAccessoryView:aView
```

Sets the IODisplayInspector's accessory View to *aView*. Because IODisplayInspector's inspector View is implemented as IODeviceInspector's accessory View, *aView* is an accessory View within an accessory View. Use this method to add a device-specific View to the inspector. Returns `self`.

##### setTable:

```objc
- setTable:(NXStringTable *)instanceTable
```

Specifies *instanceTable* as the configuration table associated with this device and uses the value of *instanceTable's* "Display Mode" key to initialize the display modes for this device. The data in *instanceTable* is written out to its corresponding file (Instancen.table) when the user saves the configuration.

The Configure application invokes this method whenever the user selects this device for inspection. Returns `self`.

---

### IOEISADeviceDescription

**Inherits From:** IODeviceDescription : Object

**Declared In:** driverkit/i386/IOEISADeviceDescription.h

#### Class Description

IOEISADeviceDescriptions encapsulate information about IODirectDevices that run on ISA- and EISA-compliant computers. Usually, you need only to pass around IOEISADeviceDescription objects, without creating them, subclassing them, or sending messages to them. IOEISADeviceDescriptions are created by the system and initialized from IOConfigTables. They are then passed to the `probe:` method to instantiate the driver, using the device description.

#### Instance Variables

None declared.

#### Method Types

**Getting and setting the list of DMA channels**
- `– channel`
- `– channelList`
- `– numChannels`
- `– setChannelList:num:`

**Getting and setting the list of I/O port ranges**
- `– portRangeList`
- `– numPortRanges`
- `– setPortRangeList:num:`

**Getting the EISA slot number and ID**
- `– getEISASlotNumber`
- `– getEISASlotID`

#### Instance Methods

##### channel

```objc
- (unsigned int)channel
```

Returns the first DMA channel associated with this device. The return value is undefined if this device has no DMA channels associated with it.

**See also:** `– channelList`, `– numChannels`, `– setChannelList:num:`

##### channelList

```objc
- (unsigned int *)channelList
```

Returns all the DMA channels associated with this device. You can get the number of items in the returned array by invoking `numChannels`. You should never free the data returned by this method.

**See also:** `– channel`, `– numChannels`, `– setChannelList:num:`

##### getEISASlotID

```objc
- (IOReturn)getEISASlotID:(unsigned long *)slotID
```

Returns the EISA slot identifier for the device. In this identifier, the device ID is in the lower 16 bits, and the vendor ID is in the upper 16 bits.

##### getEISASlotNumber

```objc
- (IOReturn)getEISASlotNumber:(unsigned int *)slotNumber
```

Returns the EISA slot number for the device.

##### numChannels

```objc
- (unsigned int)numChannels
```

Returns the total number of DMA channels associated with this device. This number is determined from the IOConfigTable from which this IOEISADeviceDescription is initialized.

**See also:** `– channel`, `– channelList`, `– setChannelList:num:`

##### numPortRanges

```objc
- (unsigned int)numPortRanges
```

Returns the total number of I/O port ranges associated with this device.

##### portRangeList

```objc
- (IORange *)portRangeList
```

Returns all the I/O port ranges associated with this device. You can get the number of items in the returned array by invoking `numPortRanges`. You should never free the data returned by this method.

**See also:** `– numPortRanges`, `– setPortRangeList:num:`

##### setChannelList:num:

```objc
- (IOReturn)setChannelList:(unsigned int *)aList
                       num:(unsigned int)numChannels
```

Sets the array and number of DMA channels associated with this device. You shouldn't normally invoke this method, since it overrides the normal configuration scheme (which is documented in Chapter 4).

**See also:** `– channel`, `– channelList`, `– numChannels`

##### setPortRangeList:num:

```objc
- (IOReturn)setPortRangeList:(IORange *)aList
                         num:(unsigned int)numPortRanges
```

Sets the array and number of I/O port ranges associated with this device. You shouldn't normally invoke this method, since it overrides the normal configuration scheme (which is documented in Chapter 4).

**See also:** `– numPortRanges`, `– portRangeList`

---

### IOEthernet

**Inherits From:** IODirectDevice : IODevice : Object

**Conforms To:** IONetworkDeviceMethods

**Declared In:** driverkit/IOEthernet.h

#### Class Description

IOEthernet is an abstract class for controlling Ethernet devices. It provides a framework for sending and receiving packets, handling interrupts, and setting and detecting transmission timeouts. It also provides an IONetwork instance that connects the driver with the kernel networking subsystem, as well as an I/O thread from which most of the IOEthernet instance methods are invoked.

IOEthernet includes support for multicast mode and promiscuous mode. It doesn't currently provide gdb support for non-NeXT drivers. (gdb support enables the kernel running the IOEthernet driver to be debugged over the network.)

IOEthernet's multicast support consists mainly of keeping a list of the multicast addresses at which multicast packets should be received and providing methods for configuring multicast addresses. Depending on the hardware's capability, you can either implement `enableMulticastMode` and `disableMulticastMode` to allow and disallow receptions of all multicast packets or implement `addMulticastAddress:` and `removeMulticastAddress:` to configure the hardware for individual addresses.

Most hardware implementations don't guarantee filtering based on individual addresses. For this reason, the `isUnwantedMulticastPacket:` method exists to indicate packets that aren't bound for an address in the list of valid multicast addresses. A subclass of IOEthernet written for hardware that implements partial or no filtering based upon individual addresses should invoke this method each time it receives a multicast packet to determine whether it should be discarded or not.

To write an Ethernet driver, you create a subclass of IOEthernet.

##### Implementing a Subclass

Your subclass of IOEthernet must do the following:

- Implement `probe:` and `initFromDeviceDescription:`. The implementation of `probe:` should allocate an instance and invoke `initFromDeviceDescription:`. See the IODevice specification for more information on implementing `probe:`.
- Implement `transmit:`, `resetAndEnable:`, `interruptOccurred`, and `timeoutOccurred`. These methods perform the real work of the driver. `interruptOccurred` is invoked from the I/O thread whenever the Ethernet hardware interrupts. See the EISA/ISA method descriptions in the IODirectDevice specification for more information on `interruptOccurred` and `timeoutOccurred`.

If your subclass supports multicast mode, you must implement either `enableMulticastMode` and `disableMulticastMode` or `addMulticastAddress:` and `removeMulticastAddress:`.

If your subclass supports promiscuous mode, you must implement `enablePromiscuousMode` and `disablePromiscuousMode`.

##### IONetworkDeviceMethods Protocol Implementation

In IOEthernet's implementation, `finishInitialization` invokes `resetAndEnable:YES` if `[self isRunning] == YES`.

#### Instance Variables

None declared.

#### Method Types

**Creating and destroying IOEthernet instances**
- `– free`
- `– initFromDeviceDescription:`
- `– attachToNetworkWithAddress:`

**Handling interrupts**
- `– interruptOccurred` (IODirectDevice)

**Transmitting packets**
- `– transmit:`
- `– performLoopback:`

**Setting and handling hardware timeouts**
- `– setRelativeTimeout:`
- `– relativeTimeout`
- `– clearTimeout`
- `– timeoutOccurred` (IODirectDevice)

**Setting and getting the state of the hardware**
- `– isRunning`
- `– resetAndEnable`

**Supporting multicast**
- `– enableMulticastMode`
- `– disableMulticastMode`
- `– addMulticastAddress:`
- `– removeMulticastAddress:`
- `– isUnwantedMulticastPacket:`

**Supporting promiscuity**
- `– disablePromiscuousMode`
- `– enablePromiscuousMode`

#### Adopted Protocols

**IONetworkDeviceMethods**
- `– allocateNetbuf`
- `– finishInitialization`
- `– outputPacket:address:`
- `– performCommand:data:`

#### Instance Methods

##### addMulticastAddress:

```objc
- (void)addMulticastAddress:(enet_addr_t *)address
```

Does nothing. Subclasses that support multicast mode can implement this method so that it notifies the hardware that it should receive packets sent to *address*. Some subclasses that support multicast don't implement this method because their hardware doesn't provide filtering based upon individual multicast addresses. Instead, they inspect all multicast packets, using `isUnwantedMulticastPacket:` to weed out packets to unwanted multicast addresses. This method, followed by `enableMulticastMode`, is invoked in the I/O thread every time a new multicast address is added to the list that IOEthernet maintains.

**See also:** `– enableMulticastMode`, `– isUnwantedMulticastPacket:`, `– removeMulticastAddress:`

##### attachToNetworkWithAddress:

```objc
- (IONetwork *)attachToNetworkWithAddress:(enet_addr_t)address
```

Creates an IONetwork instance and attaches to the network subsystem by sending the IONetwork an `initForNetworkDevice:...` message. Before returning, this method logs a message including the ethernet address. Returns the IONetwork instance just created.

You invoke this method at the end of your implementation of `initFromDeviceDescription:`. You must invoke `resetAndEnable:NO` before invoking this method, as described under `initFromDeviceDescription:`.

##### clearTimeout

```objc
- (void)clearTimeout
```

If a transmission timeout is scheduled, unschedules the timeout. This method is normally invoked from a subclass's implementation of `interruptOccurred`.

**See also:** `– setRelativeTimeout:`, `– relativeTimeout`, `– timeoutOccurred`

##### disableMulticastMode

```objc
- (void)disableMulticastMode
```

Does nothing. Subclasses that support multicast mode and implement `enableMulticastMode` should implement this method so that it disables the hardware's support for multicast mode. This method is invoked in the I/O thread when the last multicast address has been removed from the list that IOEthernet maintains.

**See also:** `– enableMulticastMode`

##### disablePromiscuousMode

```objc
- (void)disablePromiscuousMode
```

Does nothing. Subclasses that support promiscuous mode must implement this method so that it disables the hardware's support for promiscuous mode. This method is invoked in the I/O thread by the networking subsystem.

**See also:** `– enablePromiscuousMode`

##### enableMulticastMode

```objc
- (BOOL)enableMulticastMode
```

Does nothing and returns YES. Subclasses that support multicast mode can implement this method so that it enables the hardware's support for multicast mode. Every time a new multicast address is added to the list that IOEthernet maintains, `addMulticastAddress:` and this method are invoked in the I/O thread.

**See also:** `– disableMulticastMode`

##### enablePromiscuousMode

```objc
- (BOOL)enablePromiscuousMode
```

Does nothing and returns YES. Subclasses that support promiscuous mode must implement this method so that it enables the hardware's support for promiscuous mode. This method is invoked in the I/O thread by the networking subsystem.

**See also:** `– enablePromiscuousMode`

##### free

```objc
- free
```

Frees the IOEthernet instance and returns `nil`.

##### initFromDeviceDescription:

```objc
- initFromDeviceDescription:(IODeviceDescription *)deviceDescription
```

Initializes a newly allocated IOEthernet instance. This includes invoking `initFromDeviceDescription:` on super; invoking `startIOThread`; setting the name, kind, and unit of this instance; and invoking `registerDevice`.

Subclasses of IOEthernet should implement this method so that it invokes `[super initFromDeviceDescription:]` and then performs any device-specific initialization. The subclass implementation should invoke `resetAndEnable:NO` and should finish by invoking `attachToNetworkWithAddress:`. An example of a subclass implementation of this method is below. Italicized text delineated in angle brackets is to be filled in with device-specific code.

```objc
- initFromDeviceDescription:(IODeviceDescription *)devDesc
{
    IOEISADeviceDescription *deviceDescription =
        (IOEISADeviceDescription *)devDesc;
    IORange *io;

    if ([super initFromDeviceDescription:devDesc] == nil)
        return nil;

    /* Perform any 1-time hardware initialization. */

    /* NOTE: This implementation of resetAndEnable: sets myAddress. */
    [self resetAndEnable:NO];  // Finish initializing the hardware

    /* Perform any additional software initialization. */

    network = [self attachToNetworkWithAddress:myAddress];
    return self;
}
```

Returns `self` if the instance was successfully initialized; otherwise, frees itself and returns `nil`.

##### isRunning

```objc
- (BOOL)isRunning
```

Returns YES if the hardware is currently capable of communication with other stations in the network; otherwise, returns NO.

**See also:** `– setRunning:`

##### isUnwantedMulticastPacket:

```objc
- (BOOL)isUnwantedMulticastPacket:(ether_header_t *)header
```

Determines whether the specified packet is to a multicast address that this device shouldn't listen to. Returns YES if the packet should be dropped; otherwise, returns NO.

**See also:** `– addMulticastAddress:`

##### performLoopback:

```objc
- (void)performLoopback:(netbuf_t)packet
```

Determines whether the outgoing packet should be received by this device (because it's a broadcast packet, for example, or a multicast packet for an enabled address). If so, simulates reception by sending a copy of *packet* to the protocol stack. You should invoke this method in your `transmit:` method if your hardware device can't receive its own packets.

##### relativeTimeout

```objc
- (unsigned int)relativeTimeout
```

Returns the number of milliseconds until a transmission timeout will occur. If no transmission timeout is currently scheduled, this method returns zero.

**See also:** `– clearTimeout`, `– setRelativeTimeout:`, `– timeoutOccurred`

##### removeMulticastAddress:

```objc
- (void)removeMulticastAddress:(enet_addr_t *)address
```

Does nothing. Subclasses that support multicast mode can implement this method so that it notifies the hardware that it should stop listening for packets sent to *address*.

**See also:** `– addMulticastAddress:`, `– disableMulticastMode`

##### resetAndEnable:

```objc
- (BOOL)resetAndEnable:(BOOL)enable
```

Does nothing and returns YES. Subclasses of IOEthernet must implement this method so that it resets and initializes the hardware. Interrupts should be enabled if *enable* is YES; otherwise, they should be left disabled. In either case, this method should invoke `setRunning:` to record the basic state of the device.

This method should return YES if it encounters no errors (no matter what the value of *enable* is); if it encounters errors, it should return NO. For example, the result from `resetAndEnable:NO` should be YES if the reset is successful.

The only time this method is invoked, with the exception of any invocations from your IOEthernet subclass implementation, is during initialization. Specifically, `resetAndEnable:YES` is invoked once in the I/O thread after `attachToNetworkWithAddress:` is invoked.

**See also:** `– setRunning:`

##### setRelativeTimeout:

```objc
- (void)setRelativeTimeout:(unsigned int)timeout
```

Schedules a timeout to occur in *timeout* milliseconds. This method is generally invoked by the IOEthernet's `transmit:` method. When *timeout* milliseconds pass without the timeout being cleared (with `clearTimeout`), the method `timeoutOccurred` is invoked.

**See also:** `– clearTimeout`, `– relativeTimeout`, `– timeoutOccurred`

##### setRunning:

```objc
- (void)setRunning:(BOOL)running
```

Sets whether the hardware is on line. The value of *running* should be YES to indicate that the hardware is on line; otherwise, it should be NO. This method is invoked only by methods in IOEthernet subclasses--not by IOEthernet's own method implementations. You should invoke this method in your implementation of `resetAndEnable:`.

**See also:** `– isRunning`

##### transmit:

```objc
- (void)transmit:(netbuf_t)packet
```

Does nothing except free *packet*, using the `nb_free()` function. This method is invoked by the kernel networking subsystem when the hardware should transmit a packet.

Subclasses of IOEthernet must implement this method. To determine the number of bytes of data to be transmitted, use the `nb_size()` function. To get a pointer to the data, use `nb_map()`. After getting the information you need from *packet*, you must free it with `nb_free()`. Just before transmitting the packet, you can set a timeout with `setRelativeTimeout:`. If your hardware can't receive packets it transmits, you must invoke `performLoopback:` in your implementation of this method.

This method can be invoked in many contexts, not just from the I/O thread (or from the I/O task). For example, `transmit:` and `interruptOccurred` can run at the same time, so any common structures they both use must be protected with locks.

---

### IOFrameBufferDisplay

**Inherits From:** IODisplay : IODirectDevice : IODevice : Object

**Conforms To:** IOScreenEvents

**Declared In:** driverkit/IOFrameBufferDisplay.h

#### Class Description

IOFrameBufferDisplay is an abstract class for managing display cards that support linear-mode frame buffers. IOFrameBufferDisplay's close interaction with the window server and event system means that your driver needs to do very little.

**Note:** If your display adapter doesn't allow you to linearly address the entire frame buffer at once, use the IOSVGADisplay class instead.

IOFrameBufferDisplay currently supports the following bit depths:

- 2-bit grayscale
- 8-bit grayscale
- 8-bit color
- 16-bit RGB (5-5-5 or 4-4-4--both with 4096 colors)
- 24-bit RGB (8-8-8)

Most of the work in writing a IOFrameBufferDisplay driver is determining how to put the hardware into an advanced mode in which the frame buffer is linearly addressable. Some drivers support several advanced modes, which the user chooses between using the Configure application. The IODisplayInfo specification describes how to specify the modes your driver supports.

When specifying your driver's memory ranges in its default configuration table, you must first specify the addresses of the linear frame buffer, and then the addresses 0xa0000-0xbffff and 0xc0000-0xcffff.

```
"Memory Maps" = "0x7e00000-0x7ffffff 0xa0000-0xbffff 0xc0000-0xcffff";
```

See the IODisplayInfo specification for information on display-specific configuration keys.

##### Implementing a Subclass

In your subclass of IOFrameBufferDisplay, you must implement the following methods:

- `initFromDeviceDescription:`
- `enterLinearMode`
- `revertToVGAMode`

You might also need to implement two more methods:

- If the hardware supports setting brightness, you must implement `setBrightness:`.
- To support multiple gamma correction tables, implement `setTransferTable:count:`.

#### Instance Variables

None declared.

#### Method Types

**Creating and initializing IOFrameBufferDisplays**
- `+ probe:`
- `– initFromDeviceDescription:`

**Getting and setting parameters**
- `– getIntValues:forParameter:count:`
- `– setCharValues:forParameter:count:`
- `– setIntValues:forParameter:count:`

**Handling the cursor**
- `– hideCursor:`
- `– moveCursor:frame:token:`
- `– showCursor:frame:token:`

**Setting screen brightness**
- `– setBrightness:token:`

**Setting the gamma correction table**
- `– setTransferTable:count:`

**Mapping the frame buffer**
- `– mapFrameBufferAtPhysicalAddress:length:`

**Choosing display modes**
- `– enterLinearMode`
- `– revertToVGAMode`
- `– selectMode:count:`
- `– selectMode:count:valid:`

#### Class Methods

##### probe:

```objc
+ (BOOL)probe:deviceDescription
```

Without checking for the presence of hardware, allocates and initializes an IOFrameBufferDisplay. You shouldn't reimplement this method.

If the initialization (done with `initFromDeviceDescription:`) is unsuccessful, this method returns NO. Otherwise, this method sets the device kind to "Linear Framebuffer", invokes `registerDevice`, and returns YES.

**See also:** `– initFromDeviceDescription:`

#### Instance Methods

##### enterLinearMode

```objc
- (void)enterLinearMode
```

Implemented by subclasses to put the display into linear frame buffer mode. This method is invoked by the system when appropriate, such as when the window server starts running.

**See also:** `– revertToVGAMode`

##### getIntValues:forParameter:count:

```objc
- (IOReturn)getIntValues:(unsigned int *)parameterArray
            forParameter:(IOParameterName)parameterName
                   count:(unsigned int *)count
```

Handles NeXT-internal parameters specific to IOFrameBufferDisplays; forwards the handling of all other parameters to super.

**See also:** `– getIntValues:forParameter:count:` (IODevice)

##### hideCursor:

```objc
- hideCursor:(int)token
```

Implements this method, as described in the IOScreenEvents protocol specification. You should never need to invoke or implement this method.

##### initFromDeviceDescription:

```objc
- initFromDeviceDescription:deviceDescription
```

Invokes `initFromDeviceDescription:` on super. If successful, sets the unit number and the name (to "Display" followed by the unit number). Frees itself if initialization was unsuccessful.

Subclasses must implement this method so that it performs all initialization necessary to set up the device and the driver. This includes setting the IODisplayInfo structure (as described in the IODisplay class description) and invoking `mapFrameBufferAtPhysicalAddress:length:`. If possible, this method should also check the hardware to see if it matches the IOConfigTable. If the hardware doesn't match, the driver should do what it can to ensure that the display is still usable.

**See also:** `+ probe:`

##### mapFrameBufferAtPhysicalAddress:length:

```objc
- (vm_address_t)mapFrameBufferAtPhysicalAddress:(unsigned int)address
                                         length:(int)numBytes
```

Maps the physical memory for this instance into virtual memory for use by the device driver. If *address* is 0, this method maps the physical memory corresponding to local memory range 0, and *numBytes* is ignored. If *address* is not 0, the reserved resources are overridden--*address* is used as the physical memory address and *numBytes* is used as the length. The mapped memory range is cached as specified in the IODisplayInfo for this instance.

**Note:** When overriding reserved resources, you can't map memory outside of the memory range reserved for the device. However, you can map a subset of the memory range.

You should invoke this method during initialization.

Returns the virtual address that corresponds to *address*. If the memory mapping failed, this method logs an error message and returns NULL.

**See also:** `– initFromDeviceDescription:`

##### moveCursor:frame:token:

```objc
- moveCursor:(Point *)cursorLoc
       frame:(int)frame
       token:(int)token
```

Implements this method, as described in the IOScreenEvents protocol specification. You should never need to invoke or implement this method.

##### revertToVGAMode

```objc
- (void)revertToVGAMode
```

Implemented by subclasses to remove the display from whatever advanced mode it's in and enter a mode in which it can be used as a standard VGA device.

**See also:** `– enterLinearMode`

##### selectMode:count:

```objc
- (int)selectMode:(const IODisplayInfo *)modeList
            count:(int)count
```

Invokes `selectMode:count:valid:`, specifying 0 for the last argument.

##### selectMode:count:valid:

```objc
- (int)selectMode:(const IODisplayInfo *)modeList
            count:(int)count
            valid:(const BOOL *)isValid
```

Determines which IODisplayInfo in the driver-supplied *modeList* matches the value of the "Display Mode" key in the device's IOConfigTable. Drivers that support multiple advanced modes should invoke this method during initialization. When the driver receives a `enterLinearMode` message, it should enter the mode selected by this method. If this method doesn't find a valid mode, the driver should determine a mode that will work.

The "Display Mode" key is a configuration key that can be used by drivers to support multiple modes--for example, both 8-bit gray and 16-bit RGB. IODisplayInfo is defined in the header file driverkit/displayDefs.h.

The *modeList* argument should contain a IODisplayInfo for each advanced mode the driver supports. The *count* argument should specify the number of IODisplayInfos in *modeList*. *isValid* should either be 0 (in which case it's ignored) or an array that corresponds to the *modeList*. If `isValid[1]` is NO, for example, then this method ignores the IODisplayInfo pointed to by `modeList[1]`.

If this method finds a match, it returns the index of the matching IODisplayInfo in *modeList*. If the "Display Mode" key is missing or its value is improperly formatted, or if a corresponding IODisplayInfo isn't found, this method returns -1.

See the IODisplay class description for information on display modes and the IODisplayInfo type.

##### setBrightness:token:

```objc
- setBrightness:(int)level
          token:(int)token
```

Checks whether *level* is between `EV_SCREEN_MIN_BRIGHTNESS` and `EV_SCREEN_MAX_BRIGHTNESS` (inclusive). If not, this method logs an error message. Subclasses that support brightness changes should override this method and implement it as described in the IOScreenEvents protocol specification.

Returns `self`.

##### setCharValues:forParameter:count:

```objc
- (IOReturn)setCharValues:(unsigned char *)parameterArray
             forParameter:(IOParameterName)parameterName
                    count:(unsigned int)count
```

Handles NeXT-internal parameters specific to IOFrameBufferDisplays; forwards the handling of all other parameters to super.

**See also:** `– setCharValues:forParameter:count:` (IODevice)

##### setIntValues:forParameter:count:

```objc
- (IOReturn)setIntValues:(unsigned int *)parameterArray
            forParameter:(IOParameterName)parameterName
                   count:(unsigned int)count
```

Handles NeXT-internal parameters specific to IOFrameBufferDisplays; forwards the handling of all other parameters to super.

**See also:** `– setIntValues:forParameter:count:` (IODevice)

##### setTransferTable:count:

```objc
- setTransferTable:(const unsigned int *)table
             count:(int)numEntries
```

Specifies new gamma correction values to be used by the hardware. The default implement does nothing but return `self`. Subclasses that support multiple gamma correction transforms must override this method so that it sets the hardware to reflect the values in *table*.

Gamma correction is necessary because displays respond nonlinearly to linear ranges of voltage. For example, consider a pixel that can have red, green, and blue values between 0 and 15. This pixel's brightness when the values are (7, 0, 0) might be more than 7/15 its brightness when the values are (15, 0, 0). Gamma correction lets the hardware adjust the voltage of the beam--for example, using 6.5/15 of maximum voltage instead of 7/15, so that the pixel isn't too bright.

Each entry in *table* specifies the gamma correction (a value scaled to be between 0 and 255, inclusive) for the corresponding pixel component values. For example, for RGB color modes, `table[7]` specifies the gamma corrections for a red value of 7, a green value of 7, and a blue value of 7 (using one byte of the entry per component). If a pixel's value is (0, 5, 15), for example, the hardware should use the red gamma correction from `table[0]`, the green gamma correction from `table[5]`, and the blue gamma correction from `table[15]`. Which bytes you use from each table entry depends on whether the transfer table is for a color or black-and-white mode; you can determine the mode from the value of *numEntries*.

When *numEntries* is `IO_2BPP_TRANSFER_TABLE_SIZE` or `IO_8BPP_TRANSFER_TABLE_SIZE` (as defined in the header file driverkit/displayDefs.h), the table is for a black-and-white display. In this case, each table entry has only one meaningful byte: the least significant byte.

When *numEntries* is `IO_12BPP_TRANSFER_TABLE_SIZE`, `IO_15BPP_TRANSFER_TABLE_SIZE`, or `IO_24BPP_TRANSFER_TABLE_SIZE`, the table is for an RGB display, and each entry has three meaningful bytes. The most significant byte holds the red gamma correction, the next most significant byte holds the green gamma correction, and the next holds the blue gamma correction. The least significant byte holds no information.

(See the original specification for detailed code examples of implementing gamma correction.)

Gamma correction transforms are set using the setframebuffertransfer PostScript operator. The Window Server uses the functions specified in setframebuffertransfer to fill the values used in *table*. It then passes the values down the display system so that eventually the `setTransferTable:count:` message is invoked.

**Note:** The default transfer table cannot be specified using NetInfo, despite the claims of the setframebuffertransfer documentation.

**See also:** setframebuffertransfer PostScript Operator (NEXTSTEP General Reference)

##### showCursor:frame:token:

```objc
- showCursor:(Point *)cursorLoc
       frame:(int)frame
       token:(int)token
```

Implements this method, as described in the IOScreenEvents protocol specification. You should never need to invoke or implement this method.

---

### IONetbufQueue

**Inherits From:** Object

**Declared In:** driverkit/IONetbufQueue.h

#### Class Description

IONetbufQueue is used by network device drivers to store packets until they're transmitted. IONetbufQueue is a first-in first-out (FIFO) queue.

#### Instance Variables

None declared.

#### Method Types

**Creating and initializing instances**
- `– initWithMaxCount:`

**Adding and removing netbufs from the queue**
- `– enqueue:`
- `– dequeue`

**Getting the size of the queue**
- `– count`
- `– maxCount`

#### Instance Methods

##### count

```objc
- (unsigned int)count
```

Returns the number of netbufs in the IONetbufQueue.

**See also:** `– maxCount`

##### dequeue

```objc
- (netbuf_t)dequeue
```

Removes and returns the netbuf that has been in the queue the longest. Returns NULL if no netbufs are in the queue.

##### enqueue:

```objc
- (void)enqueue:(netbuf_t)netbuf
```

Adds the specified netbuf to the queue, unless the queue already has reached its maximum length. If the queue is at its maximum length, the netbuf is freed.

**See also:** `– count`, `– maxCount`

##### initWithMaxCount:

```objc
- initWithMaxCount:(unsigned int)maxCount
```

Initializes and returns a newly allocated IONetbufQueue. The maximum number of netbufs in the queue is set to *maxCount*.

##### maxCount

```objc
- (unsigned int)maxCount
```

Returns the maximum number of netbufs that can be in the IONetbufQueue. This number is set at initialization time.

**See also:** `– maxCount`, `– initWithMaxCount:`

---

### IONetwork

**Inherits From:** Object

**Declared In:** driverkit/IONetwork.h

#### Class Description

The IONetwork class connects direct drivers, such as Ethernet drivers, into the kernel network interface. One IONetwork object is associated with each instance of a network direct driver. In the future, support may be added for indirect network drivers, as well.

Network direct drivers must implement the IONetworkDeviceMethods protocol, so that the IONetwork can send them messages.

**Note:** Network drivers must run in the kernel.

See the IOEthernet specification for information on how to write Ethernet drivers, and the IOTokenRing specification for information on writing Token Ring drivers. See Chapter 8, "Network Modules," in NEXTSTEP Operating System Software for more information about network drivers.

#### Instance Variables

None declared.

#### Method Types

**Initializing an IONetwork instance**
- `– initForNetworkDevice:name:unit:type:maxTransferUnit:flags:`
- `– finishInitialization`

**Passing packets from the driver up to the protocol stack**
- `– handleInputPacket:extra:`

**Outputting a packet**
- `– outputPacket:address:`

**Performing a command**
- `– performCommand:data:`

**Allocating a network buffer**
- `– allocateNetbuf`

**Keeping statistics**
- `– collisions`
- `– incrementCollisions`
- `– incrementCollisionsBy:`
- `– incrementInputErrors`
- `– incrementInputErrorsBy:`
- `– incrementInputPackets`
- `– incrementInputPacketsBy:`
- `– incrementOutputErrors`
- `– incrementOutputErrorsBy:`
- `– incrementOutputPackets`
- `– incrementOutputPacketsBy:`
- `– inputErrors`
- `– inputPackets`
- `– outputErrors`
- `– outputPackets`

#### Instance Methods

##### allocateNetbuf

```objc
- (netbuf_t)allocateNetbuf
```

This method creates and returns a netbuf to be used for an impending output.

This method doesn't always have to return a buffer. For example, you might want to limit the number of buffers your driver instance can allocate (say, 200 kilobytes worth) so that it won't use too much wired-down kernel memory. When this method fails to return a buffer, it should return NULL.

Here's an example of implementing allocateNetbuf:

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

**See also:** nb_alloc() (NEXTSTEP Operating System Software)

##### collisions

```objc
- (unsigned int)collisions
```

Returns the total number of network packet collisions that have been detected since boot time.

##### finishInitialization

```objc
- (int)finishInitialization
```

This method should perform any initialization that hasn't already been done. For example, it should make sure its hardware is ready to run. You can specify what the integer return value (if any) should be.

If you implement this method, you need to check that `[self isRunning] == YES`.

##### handleInputPacket:extra:

```objc
- (int)handleInputPacket:(netbuf_t)packet
                   extra:(void *)extra
```

Increments the number of input packets and passes *packet* to the kernel for processing. The kernel dispatches the packet to the appropriate protocol handler.

A network device driver should invoke this method after it's processed a newly received packet. The value of *extra* should be zero, unless the protocol handler requires another value. For instance, token ring drivers need to return a valid pointer to a token ring header. This method returns `EAFNOSUPPORT` if no protocol handler accepts the packet; otherwise, it returns zero.

##### incrementCollisions

```objc
- (void)incrementCollisions
```

Increments by one the total number of network packet collisions that have been detected since boot time.

##### incrementCollisionsBy:

```objc
- (void)incrementCollisionsBy:(unsigned int)increment
```

Increments by *increment* the total number of network packet collisions that have been detected since boot time.

##### incrementInputErrors

```objc
- (void)incrementInputErrors
```

Increments by one the total number of packet input errors that have been detected since boot time.

##### incrementInputErrorsBy:

```objc
- (void)incrementInputErrorsBy:(unsigned int)increment
```

Increments by *increment* the total number of packet input errors that have been detected since boot time.

##### incrementInputPackets

```objc
- (void)incrementInputPackets
```

Increments by one the total number of packets that have been received by the computer since boot time. You usually don't need to invoke this method because `handleInputPacket:extra:` does so for you.

##### incrementInputPacketsBy:

```objc
- (void)incrementInputPacketsBy:(unsigned int)increment
```

Increments by *increment* the total number of packets that have been received by the computer since boot time.

##### incrementOutputErrors

```objc
- (void)incrementOutputErrors
```

Increments by one the total number of packet output errors that have been detected since boot time.

##### incrementOutputErrorsBy:

```objc
- (void)incrementOutputErrorsBy:(unsigned int)increment
```

Increments by *increment* the total number of packet output errors that have been detected since boot time.

##### incrementOutputPackets

```objc
- (void)incrementOutputPackets
```

Increments by one the total number of packets that have been transmitted by the computer since boot time.

##### incrementOutputPacketsBy:

```objc
- (void)incrementOutputPacketsBy:(unsigned int)increment
```

Increments by *increment* the total number of packets that have been transmitted by the computer since boot time.

##### initForNetworkDevice:name:unit:type:maxTransferUnit:flags:

```objc
- initForNetworkDevice:device
                  name:(const char *)name
                  unit:(unsigned int)unit
                  type:(const char *)type
     maxTransferUnit:(unsigned int)mtu
                 flags:(unsigned int)flags
```

Initializes and returns the IONetwork instance associated with the specified direct device driver *device*. This method connects *device* into the kernel's networking subsystem. It's typically called from a network driver's implementation of `initFromDeviceDescription`. You shouldn't invoke `initForNetworkDevice:...` directly. IOEthernet and IOTokenRing invoke this method on behalf of their subclasses and return an IONetwork object in their respective `attachToNetworkWithAddress:` methods.

The *name* argument should be set to a constant string that names this type of network device. For example, Ethernet drivers are named "en", and Token Ring drivers are named "tr". The *unit* is an integer greater than or equal to zero that's unique for *name*. For example, the first instance of an Ethernet driver is unit 0, the second is unit 1, and so on.

The *type* is a constant string that describes this module. For example, Ethernet drivers supply the constant `IFTYPE_ETHERNET` (which is defined in net/etherdefs.h to be "10MB Ethernet").

The *mtu* is the maximum amount of data your module can send or receive. For example, Ethernet drivers use the value `ETHERMTU`, which is defined in the header file net/etherdefs.h as 1500.

Finally, *flags* defines the initial flags for the interface. Possible values are:

- **IFF_UP:** If true, this interface is working.
- **IFF_BROADCAST:** If true, this interface supports broadcast.
- **IFF_LOOPBACK:** If true, this interface is local only.
- **IFF_POINTTOPOINT:** If true, this is a point-to-point interface.

##### inputErrors

```objc
- (unsigned int)inputErrors
```

Returns the total number of packet input errors that have been detected since boot time.

##### inputPackets

```objc
- (unsigned int)inputPackets
```

Returns the total number of packets that have been received by the computer since boot time.

##### outputErrors

```objc
- (unsigned int)outputErrors
```

Returns the total number of packet output errors that have been detected since boot time.

##### outputPacket:address:

```objc
- (int)outputPacket:(netbuf_t)packet
            address:(void *)address
```

This method should deliver the specified packet to the given address. Its return value should be zero if no error occurred; otherwise, return an error number from the header file sys/errno.h.

If you implement this method, you need to check that `[self isRunning] == YES`. If so, insert the necessary hardware addresses into the packet and check it for minimum length requirements.

##### outputPackets

```objc
- (unsigned int)outputPackets
```

Returns the total number of packets that have been transmitted by the computer since boot time.

##### performCommand:data:

```objc
- (int)performCommand:(const char *)command
                 data:(void *)data
```

This method performs arbitrary control operations; the character string *command* is used to select between these operations. Although you don't have to implement any operations, there are five standard operations. You can also define your own operations.

The standard commands are listed in the following table. The constant strings listed below are declared in the header file net/netif.h (under the bsd directory of /NextDeveloper/Headers).

| Command | Operation |
|---------|-----------|
| `IFCONTROL_SETFLAGS` | Request to have interface flags turned on or off. The *data* argument for this command is of type union ifr_ifru (which is declared in the header file net/if.h). |
| `IFCONTROL_SETADDR` | Set the address of the interface. |
| `IFCONTROL_GETADDR` | Get the address of the interface. |
| `IFCONTROL_AUTOADDR` | Automatically set the address of the interface. |
| `IFCONTROL_UNIXIOCTL` | Perform a UNIX ioctl() command. This is only for compatibility; ioctl() isn't a recommended interface for network drivers. The argument is of type if_ioctl_t *, where the if_ioctl_t structure contains the UNIX ioctl request (for example, SIOCSIFADDR) in the ioctl_command field and the ioctl data in the ioctl_data field. |

An example of implementing performCommand:data: follows.

```objc
- (int)performCommand:(const char *)command data:(void *)data
{
    int error = 0;

    if (strcmp(command, IFCONTROL_SETFLAGS) == 0)
        /* do nothing */;
    else
    if (strcmp(command, IFCONTROL_GETADDR) == 0)
        bcopy(&my_address, data, sizeof(my_address));
    else
        error = EINVAL;

    return (error);
}
```

---

## See Also

- Driver Kit Architecture documentation (Chapter 1)
- Designing a Driver documentation (Chapter 2)
- Support for Specific Devices documentation (Chapter 3)
- Building, Configuring, and Debugging Drivers documentation (Chapter 4)
