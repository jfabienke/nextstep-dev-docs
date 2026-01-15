# Building Portable NEXTSTEP Applications

The operating system, client processes, development tools, and software libraries that comprise NEXTSTEP are standard for all computers on which NEXTSTEP runs. This means that, in general, when you port your NEXTSTEP application to a new computer, you won't have to redesign your code to achieve expected behavior. All the pieces are there, and as they work on one machine, so will they work on every other.

If you follow the NEXTSTEP user interface guidelines and avoid hard-wired data values, then your application will probably be portable: It should run properly on all configurations of a given architecture and should need only to be recompiled to run on a new architecture. But few applications are perfect. Yours might fall prey to the differences between computers, requiring a bit of fine-tuning before it will work with a new configuration or on a new architecture. It's anticipated that all such necessary changes will be of the type that generalize your code—you should rarely need to "special-case" your code to adapt to a particular computer.

This paper describes some of the differences between computers that can run NEXTSTEP and suggests ways to avoid configuration- or architecture-specific code that could make your application non-portable. It's divided into two parts:

1.  The first part discusses differences between hardware configurations, such as differences in screen size and color capability, or between types of keyboards.
2.  The second part deals with differences in data representation between computer architectures.

---

## Hardware Considerations

Every computer on which NEXTSTEP runs will certainly possess the three hardware pieces that have come to be regarded as obligatory: a screen, a keyboard, and a mouse. However, the attributes of these devices aren't the same on all computers. The sound capabilities of computers also vary widely.

### The Screen

Not all screens are the same size, nor do they provide the same color support. Therefore, a portable application shouldn't depend on a particular screen size or color capability.

-   **Placement**: Always place windows relative to the edges of the screen rather than in absolute positions. Interface Builder's Size Inspector can be used to set the position of a window relative to the edges of a screen.
-   **Minimum Sizes**: Keep minimum window sizes reasonable (screens are usually at least 640x480 pixels).
-   **Automatic Adjustment**: The **Window** class guarantees that windows are displayed such that the title bar and resize bar remain on-screen.
-   **Color and Grayscale**: Use the **NXImage** class for bitmapped images; it automatically uses the most appropriate representation. Use the **View** method `shouldDrawColor` to choose appropriate grayscale equivalents.

If you need more control, use the `NXScreen` structure (declared in `appkit/screens.h`).

### The Mouse

You should never depend on having a two-button mouse. The NeXT user interface guidelines urge you to ignore the second button on a NeXT mouse. At the very least, ensure that all second-button operations can be performed through some other method (e.g., menu items, or Shift/Alternate key modifiers).

The `NXEventSystemInfo()` function can be used to query the type of pointing device, though it doesn't specifically provide the number of mouse buttons.

### The Keyboard

For portability, you should never use the `keyCode` field in an event, as it is keyboard-dependent. Instead, use the **character code** (a combination of `key.charSet` and `key.charCode`). While some precision may be lost (e.g., the "1" key vs. the number pad "1"), you gain portability. You can check for a key on the numeric keypad by masking the event record's flags field with `NX_NUMERICPADMASK`.

### Sound

The sound capabilities of different computers vary considerably. The inability to play a sound should never cause your application to crash; always check return codes from sound functions.

---

## Data Representation Considerations

When recompiling for a new architecture, you must consider differences in how data is represented: datum size, byte alignment, byte order, and argument passing.

### Datum Size

Never assume the size of data types. Use the `sizeof` operator to programmatically discover the size of a datum. This applies to structures and unions as well, as alignment restrictions can change their sizes.

### Byte Alignment

Some computers demand that the starting address of a value fall on a particular boundary (e.g., 4-byte values on 4-byte boundaries).
-   **Avoid Casting**: Never cast a data pointer to a larger type and then write to it (e.g., casting a `char *` to an `int *` on a misaligned address will crash on many architectures).
-   **Allocation**: If you redefine memory allocation functions like `malloc()`, you must ensure they return appropriately aligned memory.

### Byte Order (Endian-ness)

Multi-byte data can be "big-endian" (most significant byte first) or "little-endian" (least significant byte first).
-   **Internal Data**: Not a problem for data that stays in memory during a single run.
-   **External Data**: Essential when reading/writing to files.
-   **Detection**: Use preprocessor macros `__BIG_ENDIAN__` and `__LITTLE_ENDIAN__` for conditional code.

### Datum Format

Never rely on the internal format of a datum. Always use field names for structures and unions. Bitfields are particularly variable and should only be used for internal data.

### Argument Passing

When designing functions with a variable number of arguments, use the `stdarg` macros (`<stdarg.h>`) rather than assuming arguments are passed contiguously on the stack.

---

## External Data

Data written on a little-endian machine will be swapped when read on a big-endian machine.
-   **Typed Streams**: Use **typed streams** (`NXTypedStream`) for reading and writing multi-byte data. They automatically handle byte-order differences.
-   **Exceptions**: One-byte data (like ASCII strings) can be read/written through standard functions for efficiency.

### Reading and Writing Structure Bitfields

Neither the compiler nor typed streams resolve the order of contiguous bitfields to match endian-ness.

#### Approach 1: Modify the Read and Write Routines
Expand bitfields into full-byte values during archival:

```objc
- write:(NXTypedStream *)stream
{
    [super write:stream];
    unsigned char canWalkHolder = dogFlags.canWalk;
    unsigned char canTalkHolder = dogFlags.canTalk;
    unsigned int whiskerCountHolder = dogFlags.whiskerCount;

    NXWriteTypes(stream, "*sCCI", name, &age, &canWalkHolder,
                 &canTalkHolder, &whiskerCountHolder);
    return self;
}
```

#### Approach 2: Redefine the Structure
Redefine the structure using preprocessor macros:

```objc
struct _dogFlags {
#ifdef __BIG_ENDIAN__
    unsigned int canWalk:1;
    unsigned int canTalk:1;
    unsigned int whiskerCount:10;
    unsigned int PAD:20;
#else
    unsigned int PAD:20;
    unsigned int whiskerCount:10;
    unsigned int canTalk:1;
    unsigned int canWalk:1;
#endif
} dogFlags;
```
Ensure the total size matches a standard unsigned integer type (8, 16, or 32 bits) and treat it as that type during read/write.

### Reading Existing Files

NEXTSTEP provides byte-swapping functions in `<architecture/byte_order.h>`:
-   `NXSwapShort()`, `NXSwapInt()`, `NXSwapLong()`
-   `NXSwapBigIntToHost()`, `NXSwapHostToLittleInt()`, etc.
-   Floating point swaps use `NXSwappedFloat` and `NXSwappedDouble` types to avoid CPU modification of invalid values.

#### Determining Endian-ness
-   **Magic Numbers**: Use a non-symmetric magic number (e.g., `0x50ab40cd`) at the start of a file. If you read it and it doesn't match, swap it. If it matches after swapping, you know the file is from an opposite-endian machine.

---

## Internal Data

-   **Access by Name**: Always refer to structure elements by name, never by pointer offsets.
-   **Pointer Types**: Be scrupulous with pointer types. Use `void *` for unknown types.
-   **Avoid Recasting**: Never write the "wrong" type of data by recasting a pointer to a mismatched alignment.

### Memory-mapped Data

To make mapped files portable:
1.  Use a magic number to record endian-ness.
2.  **Pad data elements** so they lie on natural alignment boundaries (e.g., 8-byte boundaries for doubles).
3.  Avoid bitfields in mapped files.

---
*Release 3.3 Copyright ©1995 NeXT Computer, Inc. All Rights Reserved.*
