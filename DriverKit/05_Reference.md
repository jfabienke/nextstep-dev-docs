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

## See Also

- Driver Kit Architecture documentation (Chapter 1)
- Designing a Driver documentation (Chapter 2)
- Support for Specific Devices documentation (Chapter 3)
- Building, Configuring, and Debugging Drivers documentation (Chapter 4)
