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

### IOPCIDeviceDescription

**Inherits From:** IOEISADeviceDescription : IODeviceDescription : Object

**Declared In:** driverkit/i386/IOPCIDeviceDescription.h

#### Class Description

IOPCIDeviceDescription objects encapsulate information about IODirectDevices that run on PCI-compliant computers. Usually, you need only to pass around IOPCIDeviceDescriptions, without creating them, subclassing them, or sending messages to them. IOPCIDeviceDescriptions are created by the system and initialized from IOConfigTables.

This object encapsulates the PCI Configuration Space address of the device. This address contains three fields:

- Device number, ranging from 0 to 31
- Function number, ranging from 0 to 7
- Bus number, ranging from 0 to 255

#### Instance Variables

None declared.

#### Method Types

**Getting config address of PCI device**
- `– getPCIdevice:function:bus:`

#### Instance Methods

##### getPCIdevice:function:bus:

```objc
- (IOReturn)getPCIdevice:(unsigned char *)deviceNumber
                function:(unsigned char *)functionNumber
                     bus:(unsigned char *)busNumber
```

This method allows callers to get the PCI config address of the PCI device associated with this device description. If all goes well, the three parameters are filled in and `IO_R_SUCCESS` is returned. There are a variety of reasons that the address couldn't be known, in which case an appropriate code is returned and the parameters are left untouched. It is acceptable for any of the parameter pointers to be `nil`.

---

### IOPCMCIADeviceDescription

**Inherits From:** IOEISADeviceDescription : IODeviceDescription : Object

**Declared In:** driverkit/i386/IOPCMCIADeviceDescription.h

#### Class Description

IOPCMCIADeviceDescription objects encapsulate information about IODirectDevices that run on PCMCIA-compliant computers. Usually, you need only to pass around IOPCMCIADeviceDescriptions, without creating them, subclassing them, or sending messages to them. IOPCMCIADeviceDescriptions are created by the system and initialized from IOConfigTables.

#### Instance Variables

None declared.

#### Method Types

**Getting information about tuples**
- `– numTuples`
- `– tupleList`

#### Instance Methods

##### numTuples

```objc
- (unsigned)numTuples
```

Returns the number of items in the tuple list.

**See also:** `– tupleList`

##### tupleList

```objc
- (id *)tupleList
```

Returns the tuple list.

**See also:** `– numTuples`

---

### IOPCMCIATuple

**Inherits From:** Object

**Declared In:** driverkit/i386/IOPCMCIATuple.h

#### Class Description

IOPCMCIATuple objects encapsulate configuration information about IODirectDevices that run on PCMCIA-compliant computers. Data from a "tuple" is from information stored on the PCMCIA card; each tuple stores a separate piece of information. IOPCMCIADeviceDescription objects typically contain a list of IOPCMCIATuple objects, containing such configuration data as electrical requirements, I/O port ranges, and timing information.

Usually, you need only to pass around IOPCMCIATuple objects, without creating them, subclassing them, or sending messages to them. IOPCMCIATuples are created by the system.

#### Instance Variables

None declared.

#### Method Types

**Getting information from a tuple**
- `– code`
- `– data`
- `– length`

#### Instance Methods

##### code

```objc
- (unsigned char)code
```

Returns a code describing the contents of the tuple, as described in the PCMCIA standard.

**See also:** `– data`, `– length`

##### data

```objc
- (unsigned char *)data
```

Returns the tuple data, which is in machine readable form.

**See also:** `– code`, `– length`

##### length

```objc
- (unsigned)length
```

Returns the length of the tuple data in bytes.

**See also:** `– code`, `– data`

---

### IOSCSIController

**Inherits From:** IODirectDevice : IODevice : Object

**Conforms To:** IOSCSIControllerExported

**Declared In:** driverkit/IOSCSIController.h

#### Class Description

IOSCSIController is an abstract class for managing SCSI controllers. It provides a framework for making SCSI requests and providing standard statistics. It also provides an I/O thread.

##### Implementing a Subclass

To write a driver for a SCSI controller, you create a subclass of IOSCSIController. Your subclass must do the following:

- Implement `probe:` (as documented in IODevice) and `initFromDeviceDescription:`. These let your driver create instances of itself.
- Implement `executeRequest:buffer:client:` and `resetSCSIBus`.
- Implement timeouts, as described in "Implementing Timeouts," below.
- Implement `interruptOccurred`, as documented in IODirectDevice.

To support standard statistics, you should implement `sumQueueLengths`, `maxQueueLength`, `numQueueSamples`, and `resetStats`, as described in "Supporting Standard Statistics," below.

##### Implementing Timeouts

To implement timeouts, you need to implement the `timeoutOccurred:` method (as documented in IODirectDevice) and make sure that your driver sends a timeout message whenever a request has taken too much time. To do the latter, your `executeRequest:buffer:client:` method should use `IOScheduleFunc()` to schedule a function; the method should then start I/O. If the I/O finishes before the function has executed, `executeRequest:buffer:client:` should unschedule the function. Otherwise, the function should send a timeout message (one with a msg_id field set to `IO_TIMEOUT_MSG`) to the instance's interrupt port. An example is below. Italicized text delineated in angle brackets is to be filled in with device-specific code.

In `executeRequest:buffer:client:`:

```objc
/* ...Construct a device-dependent command buffer "ccb"...
   Since the function we schedule won't be called from the I/O
   task, it must use msg_send_from_kernel. This means that we
   must convert the interrupt port from the I/O task space to one
   that's valid in the regular kernel space. We do this in
   initFromDeviceDescription: as follows:

   interruptPortKern = IOConvertPort([self interruptPort],
       IO_KernelIOTask, IO_Kernel);
*/
ccb->timeoutPort = interruptPortKern;
IOScheduleFunc(myTimeout, ccb, scsiRequest->timeoutLength);
/* ...Start the I/O and wait for it to finish... */
(void)IOUnscheduleFunc(myTimeout, ccb);
```

```objc
/* This method just logs a warning and sends a timeout message. */
static void myTimeout(void *arg)
{
    struct ccb *ccb = arg;
    msg_header_t msg;

    if(!ccb->in_use) {
        /* Race condition - this CCB got completed another way. */
        return;
    }

    msg.msg_remote_port = ccb->timeoutPort;
    msg.msg_id = IO_TIMEOUT_MSG;
    IOLog("mySCSIController timeout\n");
    (void)msg_send_from_kernel(&msg, MSG_OPTION_NONE, 0);
}
```

##### Supporting Standard Statistics

Subclasses of IOSCSIController can provide information used by the iostat command and any other statistics-gathering modules. To provide this information, the IOSCSIController must look at the number of requests in its queue of I/O requests, keeping track of the following:

- The total number of requests detected in the queue. The IOSCSIController should implement `sumQueueLengths` so that it returns this value.
- The highest number of requests in the queue at one time. This value should be returned by `maxQueueLength`.
- The number of times the driver has looked at the queue. The `numQueueSamples` method should return this value.

For example, assume the IOSCSIController has looked at its list of outstanding I/O requests three times, and found 1 request the first time, 5 the second, and 2 the third. At this point, `sumQueueLengths` should return 8, `maxQueueLength` should return 5, and `numQueueSamples` should return 3. The average number of requests in the list is `sumQueueLengths` divided by `numQueueSamples`.

The IOSCSIController should reset all these values to 0 whenever it receives a `resetStats` message.

#### Instance Variables

None declared.

#### Adopted Protocols

**IOSCSIControllerExported**
- `– allocateBufferOfLength:actualStart:actualLength:`
- `– executeRequest:buffer:client:`
- `– getDMAAlignment:`
- `– maxTransfer`
- `– releaseTarget:lun:forOwner:`
- `– reserveTarget:lun:forOwner:`
- `– resetSCSIBus`
- `– returnFromScStatus:`

#### Method Types

**Initializing a newly allocated IOSCSIController**
- `– initFromDeviceDescription:`

**Reserving target/lun pairs**
- `– numReserved`

**Getting and setting parameters**
- `– getIntValues:forParameter:count:`
- `– setIntValues:forParameter:count:`

**Collecting statistics**
- `– maxQueueLength`
- `– numQueueSamples`
- `– sumQueueLengths`
- `– resetStats`

#### Instance Methods

##### getIntValues:forParameter:count:

```objc
- (IOReturn)getIntValues:(unsigned int *)parameterArray
            forParameter:(IOParameterName)parameterName
                   count:(unsigned int *)count
```

Handles the two parameters specific to SCSI controllers--`IO_SCSI_CONTROLLER_STATS` and `IO_IS_A_SCSI_CONTROLLER`--and forwards the handling of all other parameters to super. The array of values returned for `IO_SCSI_CONTROLLER_STATS` is set to the numbers returned by `maxQueueLength`, `numQueueSamples`, and `sumQueueLengths`. No array is returned for `IO_IS_A_SCSI_CONTROLLER`; only `IO_R_SUCCESS` is returned, to indicate that this IODevice is indeed a SCSI controller.

**See also:** `– setIntValues:forParameter:count:`

##### initFromDeviceDescription:

```objc
- initFromDeviceDescription:deviceDescription
```

Initializes a new IOSCSIController instance. After invoking IODirectDevice's version of `initFromDeviceDescription:`, this method starts an I/O thread (with `startIOThread`) and sets its unit, name, and device kind. Each IOSCSIController has its own unit number. The first instance's unit is 0, the second is 1, and so on. The name is the concatenation of "sc" and the unit (for example, "sc0"), and the device kind is "sc".

This method also determines the alignment restrictions for the hardware, using the `getDMAAlignment:` method. The alignment restrictions are used by the method `allocateBufferOfLength:actualStart:actualLength:`.

This method returns `nil` and frees the instance if initialization failed; otherwise, it returns `self`.

You should implement this method to invoke IOSCSIController's version and to then perform any driver-dependent initialization, including initializing the hardware and (at the very end) invoking `registerDevice`.

##### maxQueueLength

```objc
- (unsigned int)maxQueueLength
```

Returns zero. Subclasses that support standard statistics should implement this method so that it returns the highest number of requests queued since this instance was initialized or `resetStats` was last called. See the class description for more information on supporting standard statistics.

##### numQueueSamples

```objc
- (unsigned int)numQueueSamples
```

Returns zero. Subclasses that support standard statistics should implement this method so that it returns the number of times the instance has collected information about its queue of I/O requests. This number should be reset to 0 when this instance is initialized and when `resetStats` is called. See the class description for more information on supporting standard statistics.

##### numReserved

```objc
- (unsigned int)numReserved
```

Returns the number of target/lun pairs that are reserved. Each pair corresponds to an active device on the SCSI bus that this instance controls.

**See also:** `– reserveTarget:lun:forOwner:` and `– releaseTarget:lun:forOwner:` (IOSCSIControllerExported protocol)

##### resetStats

```objc
- (void)resetStats
```

Does nothing. Subclasses that support standard statistics should implement this method so that it resets to zero the numbers that are returned by `maxQueueLength`, `numQueueSamples`, and `sumQueueLengths`. See the class description for more information on supporting standard statistics.

##### setIntValues:forParameter:count:

```objc
- (IOReturn)setIntValues:(unsigned int *)parameterArray
            forParameter:(IOParameterName)parameterName
                   count:(unsigned int)count
```

Handles the `IO_SCSI_CONTROLLER_STATS` parameter, forwarding the handling of all other parameters to super. The `IO_SCSI_CONTROLLER_STATS` parameter resets (using `resetStats`) the standard statistical data kept by this instance.

**See also:** `– getIntValues:forParameter:count:`

##### sumQueueLengths

```objc
- (unsigned int)sumQueueLengths
```

Returns zero. Subclasses that support standard statistics should implement this method so that it returns the total number of requests detected in its queue of I/O requests. This number should be reset to 0 when this instance is initialized and when `resetStats` is called. See the class description for more information on supporting standard statistics.

---

### IOSVGADisplay

**Inherits From:** IOFrameBufferDisplay : IODisplay : IODirectDevice : IODevice : Object

**Conforms To:** IOScreenEvents

**Declared In:** driverkit/IOSVGADisplay.h

#### Class Description

IOSVGADisplay is an abstract superclass of SVGA (Super VGA) display drivers. Unlike framebuffer devices, which have linear frame buffers, SVGA devices have segmented memories. The IOSVGADisplay class defines the protocol for segmented memories. Specifically, a mapping of segmented device memory is set up by the Window Server, and this mapping is maintained by invoking the `selectActiveSegment:` and `deselectActiveSegment:` methods in the IOSVGADisplay instance.

IOSVGADisplay supports standard 2-bit grayscale modes. More specifically, this means the `svgaMode` that is specified in the display's device-specific configuration table must be a 2-bit grayscale mode. The specific resolution and refresh rates are up to you.

##### Implementing a Subclass

Implementing an IOSVGADisplay subclass is almost identical to implementing an IOFrameBufferDisplay. The `enterLinearMode` method does not have to be implemented, but you must implement the following:

- `selectActiveSegment:` – Select a segment for device memory accesses.
- `deselectActiveSegment:` – De-select a segment.
- `segmentSize` – Return the segment size for this display in bytes.

The following methods may be implemented to take advantage of features provided by your device. For the most part, just return `NO` from these methods if your device does not support the feature. If it does, then return `YES` and do the right thing.

- `enterLinearMode` – Try to enter linear mode and return `YES` if successful; otherwise, return `NO`.
- `setTransferTable:count:` – Install new gamma correction values (return `NO` if not supported).

If you find that you need to implement a PostScript driver for your IOSVGADisplay subclass, you can implement your PostScript driver in C. Your C code must register a "drawFunc," which has the following form:

```objc
typedef void (*SVGADrawFunc)(
    id driver,
    unsigned char *seg,
    int segmentSize,
    int deltaRow,
    int deltaCol,
    int yStart,
    int yEnd,
    int xStart,
    int xEnd,
    int lineByteDiff);
```

The C routine that initializes your C PostScript driver should call `PSWSetSVGADrawFunc()` with the address of the C draw function to use.

When the Window Server issues a drawing operation to your driver, the `drawSVGARect:deltaY:deltaX:yFrom:yTo:xFrom:xTo:byteDelta:` method is called. If you haven't registered a C draw function, the default implementation is called; it simply calls `selectActiveSegment:` and `deselectActiveSegment:` once for each segment touched by the drawing operation. If you have registered a C draw function, the default implementation will call your C draw function, which gets to decide when to call `selectActiveSegment:` and `deselectActiveSegment:`.

You can find additional information and source code for an IOSVGADisplay subclass in the source code for the VGA driver. You must make sure that you leave your display in VGA text mode at boot time.

#### Instance Variables

None declared at this level.

#### Method Types

**Querying the device**
- `– segmentSize`

**Managing segments**
- `– selectActiveSegment:`
- `– deselectActiveSegment:`

**Drawing support**
- `– drawSVGARect:deltaY:deltaX:yFrom:yTo:xFrom:xTo:byteDelta:`

#### Instance Methods

##### deselectActiveSegment:

```objc
- (void)deselectActiveSegment:(int)segment
```

Invoked when a previously selected segment is no longer needed. Your subclass should implement this method to perform any hardware-specific operations needed to de-select the segment.

**See also:** `– selectActiveSegment:`

##### drawSVGARect:deltaY:deltaX:yFrom:yTo:xFrom:xTo:byteDelta:

```objc
- (void)drawSVGARect:(const void *)imageBase
              deltaY:(int)deltaRow
              deltaX:(int)deltaCol
               yFrom:(int)yStart
                 yTo:(int)yEnd
               xFrom:(int)xStart
                 xTo:(int)xEnd
           byteDelta:(int)lineByteDiff
```

Called when a drawing operation is performed. If your C PostScript driver has registered a C draw function, it will be called by the default implementation of this method. Otherwise, this method will loop over all segments involved in the drawing operation, call `selectActiveSegment:`, and, when done with the segment, call `deselectActiveSegment:`. See the class description for more information on registering a C draw function.

##### segmentSize

```objc
- (int)segmentSize
```

Returns the size, in bytes, of a segment. SVGA devices typically have segments of 64 KB.

**See also:** `– selectActiveSegment:` and `– deselectActiveSegment:`

##### selectActiveSegment:

```objc
- (void)selectActiveSegment:(int)segment
```

Invoked when a new segment needs to be selected for device memory access. Your subclass should implement this method to perform any hardware-specific operations needed to make the segment active.

The Window Server maintains a mapping to device memory that corresponds to a single segment. When the Window Server wants to access a different segment, this method is invoked to make that segment visible in the previously established memory mapping. The protocol is that the IOSVGADisplay instance manipulates the device so that when the Window Server accesses its mapped memory, it is really accessing the requested segment.

**See also:** `– deselectActiveSegment:`

---

### IOTokenRing

**Inherits From:** IONetwork : IODirectDevice : IODevice : Object

**Conforms To:** IONetworkDeviceMethods

**Declared In:** driverkit/IOTokenRing.h

#### Class Description

IOTokenRing is an abstract superclass for Token Ring network drivers. Some Token Ring devices can handle the Token Ring packet framing for the driver; others require the driver to format the Token Ring packets.

If you're writing a Token Ring driver, the following conventions apply to reading and receiving Token Ring packets:

**Receiving packets:** If the device handles Token Ring framing, your receive method should parse the Token Ring header and feed the network layer only the network layer data. If the device doesn't handle Token Ring framing, then you should use the `isInputPacket:freeWhenDone:` method that is declared by IONetwork. This method will handle the Token Ring framing for you.

**Sending packets:** When sending a packet, you need to decide which IONetwork method to call to get the data from. If your device can't handle Token Ring framing, use `getInputPacket:`. If the device handles Token Ring framing, you need to deal with an IONetwork `tokenHeader_t` struct, and you should use the non-Token Ring version of `getInputPacket:`.

The `tokenHeader_t` struct contains four fields: `macHdr`, `dsap`, `ssap`, and `ctrl`. You must use the information in this struct to format the Token Ring packet appropriately for the device.

IOTokenRing defines the following public types and constants:

```objc
typedef struct {
    u_char dsap;
    u_char ssap;
    u_char ctrl;
    u_char macHdr[32];
} tokenHeader_t;

#define IO_IS_BROADCAST(addr) \
    ((addr)[0] & (unsigned char)0x80)

typedef enum {
    IO_TR_SPEED_4,      // 4 Mbps
    IO_TR_SPEED_16      // 16 Mbps
} IOTokenRingSpeed;
```

The ring speed constants are used by the `ringSpeed` method.

##### Implementing a Subclass

Your subclass must implement the following methods:

- `resetAndEnable:` – Resets the device, optionally enabling it to work.
- `getHardwareAddress:` – Returns the device's hardware address.
- `ringSpeed` – Returns the speed at which the device is running.

It's also a good idea to implement a `setRunning:` method and enable or disable interrupts as appropriate.

In your subclass, you must call your superclass's implementation of `initFromDeviceDescription:`. The IOTokenRing implementation does the following:

1. Allocates an IONetwork instance by calling `attachToNetworkWithAddress:`
2. Registers the instance with that IONetwork object

If you want to support user-level configuration or provide debugging support, you should implement these methods from the IODevice class: `getIntValues:forParameter:count:` and `setIntValues:forParameter:count:`.

In general, implementing a Token Ring driver is similar to implementing an Ethernet driver. See the IOEthernet class specification for more information.

#### Instance Variables

```objc
IOTokenRingSpeed ringSpeed;
```

**ringSpeed** – The current ring speed (4 Mbps or 16 Mbps).

#### Method Types

**Initializing the driver**
- `– initFromDeviceDescription:`

**Accessing hardware addresses**
- `– getHardwareAddress:`

**Ring control**
- `– ringSpeed`
- `– resetAndEnable:`

#### Instance Methods

##### getHardwareAddress:

```objc
- (void)getHardwareAddress:(enet_addr_t *)address
```

Returns by reference the hardware Token Ring address for the device. (This is the "source address" that the device uses when it sends a packet.) This method is called by the IOTokenRing implementation of the `initFromDeviceDescription:` method. Your subclass must implement this method.

**See also:** `– initFromDeviceDescription:`

##### initFromDeviceDescription:

```objc
- initFromDeviceDescription:(IODeviceDescription *)devDesc
```

This implementation of the IODirectDevice method performs additional initialization for Token Ring devices: It calls `attachToNetworkWithAddress:` with the device's hardware address (as returned by `getHardwareAddress:`) and then registers the IOTokenRing instance with the returned IONetwork object. If an error occurs during this process, the implementation returns `nil`; otherwise, it returns `self`.

Your subclass should invoke the superclass implementation of this method. You can perform other initialization before or after invoking `super`'s implementation, depending on your driver's needs.

**See also:** `– attachToNetworkWithAddress:` (IONetwork), `– getHardwareAddress:`

##### resetAndEnable:

```objc
- (void)resetAndEnable:(BOOL)enable
```

Resets the device. If *enable* is `YES`, this method should also perform all the work necessary to open the Token Ring device and join the ring. If *enable* is `NO`, this method should disable the device so that it can't send or receive packets and also cause it to leave the ring. Your subclass must implement this method.

##### ringSpeed

```objc
- (IOTokenRingSpeed)ringSpeed
```

Returns one of the following values:

- `IO_TR_SPEED_4` – The device is running at 4 Mbps.
- `IO_TR_SPEED_16` – The device is running at 16 Mbps.

Your subclass must implement this method. IOTokenRing has a *ringSpeed* instance variable that you can use to store the ring speed.

---

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

## Other Features

### Auto Detection of Devices

EISA- and PCI-compliant systems can support automatic detection of devices, referred to as the auto detect feature. When auto detect is supported, the system can determine which devices are connected to the bus and the location of the devices. Devices are easier to configure because less information is required in the `.table` files for the driver. Auto detect is nondestructive: It doesn't change the state of any device.

Auto detect determines which devices are connected to a bus and their bus location. Each device identifies itself with an auto detect ID and indicates its location with an anchor.

#### Auto Detect IDs and Anchors

Each device driver identifies itself by an auto detect ID string, which is a list of identifiers that can be used to detect the devices that can be controlled by the driver. The meaning of the identifiers is different for each bus type.

For the EISA, PCI and Plug and Play bus types, this ID is expressed as a 32-bit hexadecimal number containing the vendor ID and the device ID for the device. For the EISA bus and for Plug and Play, the device ID is in the lower 16 bits, and the vendor ID is in the upper 16 bits. For the PCI bus, the vendor ID is in the lower 16 bits, and the device ID is in the upper 16 bits. The "Auto Detect IDs" key should contain IDs for all the devices that can be controlled by the device driver. There is currently no provision for matching "don't care" bits in the ID, although that may be added in the future.

When your driver is configured in the system, the configuration software will scan the bus for devices that match your auto detect IDs. When it finds a device, it will create a device description for your driver with a value for the "Location" key that allows you to locate your device on the bus. This key is known as the "anchor" for your device and is different for each bus type.

For the EISA bus, the anchor is a slot number between 0 and 15. The value of the "Location" key is `"Slot <n>"`, where `<n>` is your slot number.

For the PCI bus, the anchor is a three-part identifier containing the bus number, the device number, and the function number for your device. The bus number can be between 0 and 255, the device number can be between 0 and 31, and the function number can be between 0 and 7. The syntax of the "Location" key is `"Dev:<d> Func:<f> Bus:<b>"`, where `<b>` is the bus number, `<d>` is the device number, and `<f>` is the function number.

Plug and Play support does not currently define an anchor for the card. Instead, the resources assigned in your configuration table, such as base I/O address, IRQ level, and DMA channels, are programmed into your device using the Plug and Play control registers. In the future, an anchor will be assigned so you can use new Driver Kit methods to control resources in more detail.

#### Auto Detect Process

The driver bundle's `Default.table` has two key/value pairs of interest for auto detection: "Bus Type" and "Auto Detect IDs". The first tells which bus the driver supports. The second lists the auto detect IDs of all the supported devices for this driver, expressed in the 32-bit hexadecimal number format.

Auto detection is used at two times: During installation and when you use the Configure application.

During initial installation, the auto detect software scans each bus and obtains from each device its auto detect ID and its anchor in the form that the bus uses. It adds the "Location" key to your driver's device description in memory.

**Note:** A computer may have more than one bus, and the buses may be different types.

When you use Configure to add a driver to your system, it looks at every file with a `.table` extension (with the exception of `Instancen.table` files) in each configuration bundle, trying to match bus types and auto detect IDs. It first examines the "Bus Type" and then the "Auto Detect IDs" key/value pairs and generates a candidate list of drivers for each device found. There may be more than one candidate driver for a device. In that case, the user is presented with a list of drivers for the device and asked to pick one. After the user chooses, the `.table` file is copied to an `Instancen.table` with this line appended:

```
"Location" = "anchor"
```

where *anchor* is the anchor in the format appropriate for the bus.

There are cases where "Location" is blank. Each bus-specific category of IODirectDevice (IOEISADirectDevice, IOPCIDirectDevice, and so on) and IODeviceDescription subclass (IOPCMCIADeviceDescription and so on) provide methods for extracting this information, such as `getPCIDevice:function:bus:` and `getEISASlotNumber`.

---

## See Also

- Driver Kit Architecture documentation (Chapter 1)
- Designing a Driver documentation (Chapter 2)
- Support for Specific Devices documentation (Chapter 3)
- Building, Configuring, and Debugging Drivers documentation (Chapter 4)
