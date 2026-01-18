# 8. Network Modules

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

Loadable kernel servers that serve as network modules use special functions and interfaces in addition to the ones available to all loadable kernel servers. This chapter discusses how to write network modules. The special functions that network modules can use are described in detail in Chapter 10, "Kernel Support Functions," under the section "Network Functions."

The NeXT Mach kernel supports the following types of network modules:

- **Network device drivers**: A network device driver sends and receives packets to and from some network media.
- **Protocol handlers**: On input, a protocol handler receives packets from network device drivers and forwards the data to the interested programs. On output, the protocol handler takes data from programs, puts the data into packets, and sends these packets to the appropriate network device driver.
- **Packet sniffers**: A packet sniffer examines input packets for diagnostic purposes.

If you're familiar with UNIX 4.3BSD networking primitives, you'll find many similarities to what's described in this chapter. The biggest difference is that a common programming interface like the socket mechanism isn't defined. While sockets work well for TCP/IP, they don't generalize well to other protocols. If you're writing a protocol handler and want to open it up to programmers, you must define your own interface for communication between user programs and your protocol handler.

This chapter first gives an overview of NeXT networking support, and then discusses the objects you'll use in your network module. The next section has details on the routines that you should implement. The chapter ends with notes on implementing specific interfaces.

---

## Overview

Here's a simplified view of what happens when a network packet is received by a NeXT computer:

1. The packet is received by the appropriate network device driver, which puts the packet into a data structure called a **netbuf** (netbufs are discussed in the next section).
2. The driver calls the dispatcher (by calling `if_handle_input()`).
3. The dispatcher polls all registered packet sniffers and protocol handlers until it finds a protocol handler that accepts the packet.
4. If the protocol handler is an IP (Internet Protocol) handler, it sends the packet up to the kernel by calling `inet_queue()`.

When a packet is sent out onto the network, the following events happen:

1. The output function of the protocol handler is called. One of the arguments is a netbuf containing the packet to be sent. (This netbuf must have been previously allocated by the network device driver; how netbufs are allocated is described later in this chapter.)
2. The protocol handler calls the appropriate network device driver's output function, passing it the netbuf containing the packet.
3. The device driver puts the packet out onto the network.

Note that there's one extra step in the case of the input packet: A dispatcher is called. This happens because a network device driver doesn't know what its associated protocol handler is, but the protocol handler knows which driver to call. The dispatcher doesn't query the modules in any particular order, except that it queries all packet sniffers before querying any protocol handlers.

---

## Network Objects

The NeXT kernel includes two abstractions especially for network modules:

- **Network buffers**, known as **netbufs**
- **Network interfaces**, known as **netifs**

### Network Buffers (Netbufs)

The NeXT kernel uses netbufs for dealing with network packet buffers. Netbufs are an interface to an abstract sequence of bytes that can be read and written. The sequence has an original starting point and ending point, but these can be changed. An input network packet typically has its starting point advanced as the various headers are pulled off. Similarly, an output packet has its starting point retreated as headers are inserted.

Operating beyond the range of the original starting and ending points isn't currently detected as an error. This means that an outgoing netbuf should be copied into a larger netbuf if the information being added to its top requires more bytes than are available between the current ending point and the original starting point.

### Network Interfaces (Netifs)

Netifs are used to handle the installation and usage of network modules. Remember that a network module is one of three things: a network device driver, a protocol handler, or a packet sniffer.

Each network module initializes and installs its netif (thus registering itself) by calling `if_attach()`. A network device driver should immediately register itself by calling `if_attach()` at load time. Protocol handlers and packet sniffers, on the other hand, don't have to register themselves until their services are required. They determine whether to register themselves in a callback function that they supply as an argument to the function `if_registervirtual()`. This callback function is called once for each network device driver; it should call `if_attach()` if the module isn't already registered and it wants to receive input packets from the specified driver.

---

## Functions Implemented in the Network Module

Besides a callback function, your network module needs to supply certain functions so that other modules can call it. When your network module calls `if_attach()`, you must specify the locations of five functions:

- **Initialization function** — Does any initialization that's required to change the module's state to "on."
- **Input function** — Receives packets from lower layers and either consumes them or passes them on to other modules.
- **Output function** — Sends packets from higher layers.
- **Getbuf function** — Provides netbufs for higher layers to use in impending sends.
- **Control function** — Provides any necessary operations the above functions don't.

**Note:** You should specify null to `if_attach()` for any unimplemented function.

### Callback Function

A callback function is required in protocol handlers and packet sniffers, but isn't appropriate in network device drivers. It must have the following syntax:

```c
void callback_func(void *private, netif_t realif)
```

The purpose of the callback function is to determine whether its network module is interested in a particular device driver and, if necessary, to register its module (using `if_attach()`). The callback function is called once for each current and future network device driver, so it can keep information about more than one network device driver.

The callback function is specified in the network module's call to `if_registervirtual()`. The `private` argument is the data that was specified in the call to `if_registervirtual()`. The `realif` argument is a pointer to the network device driver for which this function is being called.

**Example Callback Function:**

```c
static void myhandler_attach(void *private, netif_t rifp)
{
    netif_t ifp;
    const char *name;
    int unit;
    void *ifprivate;

    if (strcmp(if_type(rifp), IFTYPE_ETHERNET) != 0) {
        return;
    }

    ifprivate = (void *)kalloc(sizeof(myhandler_private_t));
    name = MYNAME;
    unit = MYUNIT;
    ifp = if_attach(NULL, myhandler_input, myhandler_output,
                    myhandler_getbuf, myhandler_control, name, unit, IFTYPE_IP,
                    MYMTU, IFF_BROADCAST, NETIFCLASS_VIRTUAL, ifprivate);

    ((myhandler_private_t *)if_private(ifp))->rifp = rifp;

    if_control(rifp, IFCONTROL_GETADDR, MYHANDLER_ADDRP(ifp));

    if (verbose) {
        printf("IP protocol enabled for interface %s%d, type \"%s\"\n", 
               name, unit, MYDRIVER_TYPE);
    }
    return;
}

void myhandler_config(void)
{
    if_registervirtual(myhandler_attach, NULL);
}
```

### Initialization Function

An initialization function is not required but is often found in network device drivers. It must have the following syntax:

```c
int init_func(netif_t netif)
```

**Example:**

```c
int mydriver_init(netif_t netif)
{
    unsigned unit = if_unit(netif);
    register struct mydriver_data_t *is = &mydriver_data[unit];

    if (is->is_flags & HW_RUNNING)
        return;

    is->is_flags |= HW_RUNNING;
    /* Initialize software structures and the hardware. */
    return;
}
```

### Input Function

An input function is required in protocol handlers and packet sniffers, but not in network device drivers. It must have the following syntax:

```c
int input_func(netif_t netif, netif_t realnetif, netbuf_t packet, void *extra)
```

This function should return zero if it accepts the packet, or `EAFNOSUPPORT` to allow other modules to receive it. Packet sniffers should always return `EAFNOSUPPORT`.

### Output Function

All network modules except packet sniffers must have an output function:

```c
int output_func(netif_t netif, netbuf_t packet, void *address)
```

### Getbuf Function

A getbuf function is required in all modules except packet sniffers:

```c
netbuf_t getbuf_func(netif_t netif)
```

### Control Function

The control function isn't required, but it's useful in all three kinds of network modules:

```c
int control_func(netif_t netif, const char *command, void *data)
```

---

## Notes for Specific Interfaces

### Ethernet Interfaces

Network device drivers that implement the 10-megabit-per-second Ethernet protocol should register their type as `IFTYPE_ETHERNET`.

### TCP/IP Interfaces

IP protocol handlers can hand over their input packets to the kernel for processing by calling `inet_queue()`. IP protocol handlers should specify their type as "Internet Protocol" when they call `if_attach()`.