# Chapter 5: Driver Kit Reference

**Library:** Configure application API has no library; driver classes are contained in */System/Library/Frameworks/DriverKit.framework*.

**Header File Directory:** */System/Library/Frameworks/DriverKit.framework/Headers*

This chapter provides reference documentation for the Driver Kit classes, protocols, functions, types, and constants. The Driver Kit provides an object-oriented framework for creating device drivers in NeXTSTEP.

## Contents

This reference is organized into the following sections:

### [Classes](05_Reference/Classes.md)

Complete specifications for all 20 Driver Kit classes, including:

- **Device Management:** IODevice, IODirectDevice, IODeviceMaster, IODeviceInspector
- **Network Drivers:** IOEthernet, IONetwork, IOTokenRing, IONetbufQueue
- **Display Drivers:** IOFrameBufferDisplay, IOSVGADisplay, IODisplayInspector, IODisplay
- **SCSI Drivers:** IOSCSIController
- **Audio Drivers:** IOAudio
- **Device Descriptions:** IODeviceDescription, IOEISADeviceDescription, IOPCIDeviceDescription, IOPCMCIADeviceDescription, IOPCMCIATuple
- **Configuration:** IOConfigTable, IOAddressRanger

### [Functions](05_Reference/Functions.md)

Over 50 C functions and macros organized by category:

- **General-Purpose Functions:** Threading, timing, memory allocation, logging
- **Driver Debugging Module (DDM):** Debugging and tracing functions
- **Kernel-Only Functions:** Port conversion, VM tasks, physical memory mapping
- **Architecture-Specific Functions:** Intel I/O ports, interrupts, VGA registers, DMA
- **UNIX Device Switch Functions:** Block and character device switch tables

### [Protocols](05_Reference/Protocols.md)

Six protocols adopted by Driver Kit classes:

- **IOConfigurationInspector:** Configure application inspector interface
- **IOEventThread:** Event system I/O thread messaging
- **IONetworkDeviceMethods:** Network device driver requirements
- **IOScreenEvents:** Display cursor and brightness control
- **IOScreenRegistration:** Display driver registration
- **IOSCSIControllerExported:** SCSI controller communication

### [Types and Constants](05_Reference/Types.md)

Comprehensive reference for types, constants, and global variables:

- **Defined Types:** 30+ type definitions including DMA types, display types, device types, parameter types
- **Symbolic Constants:** Length constants, debugging constants, interrupt messages, IOReturn error codes
- **Global Variables:** IODDMMasks, IODMAStatusStrings

---

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
- Various device-specific subclasses for network, display, SCSI, audio, and other devices

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
