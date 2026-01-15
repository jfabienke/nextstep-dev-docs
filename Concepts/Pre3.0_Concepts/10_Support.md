# 10. Support Objects and Functions

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

> **Note:** This information, although fundamentally correct, has not been updated for release 3.0. For up-to-date information on:
> - **Defaults** - `/NextLibrary/Documentation/NextDev/GeneralRef/ApB_Defaults`
> - **Exception Handling** - `/NextLibrary/Documentation/NextDev/Concepts/ExceptionHandling.rtf`
>
> For up-to-date information on the various classes mentioned below (Font, Storage, and so on), see the appropriate section of the NeXTSTEP General Reference manual (available online in `/NextLibrary/Documentation/NextDev/GeneralRef`).

## Overview

In addition to a program structure for applications that use the NeXT window system and a variety of preprogrammed user-interface objects, the Application Kit offers a number of other program support facilities. Some are implemented as class definitions and some as standard C functions and macros. All are designed to work well with the Kit's program structure and user-interface objects. They include:

- A set of functions for writing and reading data to streams
- A set of functions that allow you to save data structures, including objects, in an archive file and load them from the file into an application
- A system for specifying program defaults
- A Pasteboard object that supports cut, copy, and paste operations
- A Font and a FontManager object that help applications get information about a specific font
- List, Storage, and HashTable objects that act as general memory allocators
- A mechanism for handling errors

---

## Streams

A stream is a sequence of data into or out of a program. It acts as a channel, connecting an application with a source of data or a destination for data.

### Writing and Reading

#### Writing and Reading Characters

```c
NXPutc(stream, 'c');

unsigned char  aCharacter;
aCharacter = NXGetc(stream);

NXUngetc(stream);
aCharacter = NXGetc(stream);
```

#### Writing and Reading Bytes of Data

```c
NXRect  myRect;

NXSetRect(&myRect, 0.0, 0.0, 100.0, 200.0);
NXWrite(stream, &myRect, sizeof(NXRect));

NXRect  myRect;
NXRead(stream, &myRect, sizeof(NXRect));
```

#### Writing and Reading Formatted Data

```c
int    amt = 500;
char  *day = "Friday";

NXPrintf(stream, "Please send %d bucks before %s", amt, day);

int    numint;
float  numflo;
char   name[15];

NXScanf(stream,"%d%f%s", &numint, &numflo, name);
```

### Flushing and Seeking

```c
NXFlush(stream);

NXFill(stream);

NXSeek(stream, 0, NX_FROMSTART);
```

### Connecting Streams

#### Connecting to Memory

```c
NXStream  *stream;
stream = NXOpenMemory(NULL, 0, NX_WRITEONLY);

NXCloseMemory(stream, NX_FREEBUFFER);

const char  *home = NXHomeDirectory();
NXSaveToFile(stream, home);
```

#### Connecting to a File

```c
NXStream  *stream;
stream = NXMapFile("aPathname", NX_READONLY);

NXSaveToFile(stream, "aPathname");
NXCloseMemory(stream, NX_FREEBUFFER);

int        fileDescriptor;
NXStream  *stream;

fileDescriptor = fileno(stdin);
stream = NXOpenFile(fileDescriptor, NX_READONLY);

NXClose(stream);
```

#### Connecting to a Mach Port

```c
port_t     thePort;
NXStream  *outStream;

outStream = NXOpenPort(thePort, NX_WRITEONLY);

NXFlush(outStream);
NXClose(outStream);
```

---

## Archiving to a Typed Stream

Archiving is the process of preserving a data structure, especially an object, for later use.

### Archiving a Data Structure

Functions for archiving:

| Function | Data Type |
|---|---|
| `NXWriteType()` | Single specified type |
| `NXWriteTypes()` | Multiple specified types |
| `NXWriteArray()` | An array |
| `NXWritePoint()` | An NXPoint structure |
| `NXWriteSize()` | An NXSize structure |
| `NXWriteRect()` | An NXRect structure |
| `NXWriteObject()` | An id |
| `NXWriteObjectReference()` | An id |

#### Archiving Arbitrary Data

Format characters:

| Character | Data Type |
|---|---|
| `c` | char |
| `s` | short |
| `i` | int |
| `f` | float |
| `d` | double |
| `@` | id |
| `*` | char * |
| `%` | NXAtom |
| `:` | SEL |
| `#` | class |

Example:

```c
float   aFloat = 3.0;
int     anInt = 5;
char   *aCharStar = "foo";

NXWriteTypes(typedStream, "fi*", &aFloat, &anInt, &aCharStar);
```

#### Archiving Objects

```c
NXWriteRootObject(typedStream, anId);
NXWriteObject(typedStream, anId);

char  *data;
int    length;

data = NXWriteRootObjectToBuffer(anId, &length);
```

### The write: Method

```c
- write:(NXTypedStream *) typedStream {
    [super write:typedStream];
    NXWriteObject(typedStream, subviews);
    // ... code for writing other instance variables
}
```

### Unarchiving a Data Structure

Functions for unarchiving:

| Function | Data Type |
|---|---|
| `NXReadType()` | Single specified type |
| `NXReadTypes()` | Multiple specified types |
| `NXReadArray()` | An array |
| `NXReadPoint()` | An NXPoint structure |
| `NXReadSize()` | An NXSize structure |
| `NXReadRect()` | An NXRect structure |
| `NXReadObject()` | An id |

Example:

```c
float  aFloat;
int    anInt;
char  *aCharStar;

NXReadTypes(typedStream, "fi*", &aFloat, &anInt, &aCharStar);
```

### The read: Method

```c
- read:(NXTypedStream *)typedStream
{
    [super read:typedStream];
    // ... code for reading instance variables
}
```

### Initializing Objects

After unarchiving, objects receive an `awake` message, then a `finishUnarchiving` message.

### Opening and Closing Typed Streams

```c
NXTypedStream  *typedStream;
typedStream = NXOpenTypedStreamForFile("yourPathname", NX_WRITEONLY);

NXCloseTypedStream();

id  someId;
someId = NXReadObjectFromBuffer(data, length);

NXFreeObjectBuffer(data, length);

NXOpenTypedStream(stream, NX_WRITEONLY);
```

---

## The Defaults System

Through the defaults system, applications can save and retrieve user preferences from the `.NextDefaults` database.

### Creating a Registration Table

```c
+ initialize
{
    static NXDefaultsVector WriteNowDefaults = {
        {"NXFont", "Helvetica"},
        {"NXFontSize", "12.0"},
        {NULL}
    };

    NXRegisterDefaults("WriteNow", WriteNowDefaults);
    return self;
}
```

### Reading Default Values

```c
char  *myDefaultFont;
myDefaultFont = NXGetDefaultValue("WriteNow", "NXFont");
```

For database-only queries:

```c
char *value = NXReadDefault("WriteNow", "NXFont");
```

### Writing Default Values

```c
NXWriteDefault("WriteNow", "NXFont", "Helvetica");

static NXDefaultsVector WriteNowDefaults = {
    {"NXFont", "Times"},
    {"NXFontSize", "12.0"},
    {NULL}
};
NXWriteDefaults("WriteNow", WriteNowDefaults);

NXSetDefault("WriteNow", "NXFont", "Helvetica");

NXUpdateDefault("WriteNow", "NXFont");
NXUpdateDefaults();

NXRemoveDefault("WriteNow", "NXFontSize");
```

### Command-Line Defaults

Specify values when launching:

```
Edit -WidthInChars 100 -HeightInChars 120 SomeFile.m &
```

### System Parameters

| Parameter | Initial Value |
|---|---|
| SystemAlert | Both |
| UnixExpert | NO |
| PublicWindowServer | NO |
| Printer | Local_Printer |
| SystemFont | Helvetica |
| BoldSystemFont | Helvetica-Bold |

### Global Parameters

| Parameter | Initial Value |
|---|---|
| NXFont | Helvetica |
| NXFontSize | 12 |
| NXMenuX | 1.0 |
| NXMenuY | 1000000.0 |
| NXFixedPitchFont | Ohlfs |
| NXFixedPitchFontSize | 10 |
| NXPaperType | Letter |
| NXMargins | 72 72 90 90 |

---

## The Pasteboard

The pasteboard is the principal means by which users can move data within and between applications.

### Using the Pasteboard

```c
id  myPboard;
myPboard = [NXApp pasteboard];
```

### Standard Data Types

| Data Type | Global Variable |
|---|---|
| Plain ASCII text | `NXAsciiPboard` |
| Rich Text Format (RTF) | `NXRTFPboard` |
| Encapsulated PostScript (EPS) | `NXPostScriptPboard` |
| Tag Image File Format (TIFF) | `NXTIFFPboard` |
| NeXT sound data | `NXSoundPboard` |

### Copying Data to Pasteboard

```c
- copy:sender
{
    id         pb = [NXApp pasteboard];
    NXStream  *stream;
    char      *data;
    int        length;

    [pb declareTypes:&NXPostScriptPboard num:1 owner:self];
    stream = NXOpenMemory(NULL, 0, NX_WRITEONLY);
    [self copyPSCodeInside:NULL to:stream];
    NXGetMemoryBuffer(stream, &data, &length, &maxLength);
    [pb writeType:NXPostScriptPboard data:data length:length];
    NXCloseMemory(stream, NX_FREEBUFFER);
    return self;
}
```

### Reading Data from Pasteboard

```c
- paste:sender
{
    id         pb = [NXApp pasteboard];
    char     **type;
    char      *data;
    int        length;
    NXStream  *stream;

    for(type = [pb types]; *type; type++)
        if(!strcmp(*type, NXPostScriptPboard))
            break;

    if(*type) {
        [pb readType:NXPostScriptPboard data:&data length:&length];
        stream = NXOpenMemory(data, length, NX_READONLY);
        // parse PostScript data
        NXCloseMemory(stream, NX_FREEBUFFER);
    }
    return self;
}
```

### Using Typed Streams with Pasteboard

```c
- copy:sender
{
    const char  *const types[1] = {"PrivateTypes"};
    id           pb = [NXApp pasteboard];
    char        *data;
    int          length;

    [pb declareTypes:types num:1 owner:self];
    data = NXWriteRootObjectToBuffer(SelectionList, &length);
    [pb writeType:types[0] data:data length:length];
    NXFreeObjectBuffer(data, length);
    return self;
}

- paste:sender
{
    char  **type;
    id      pb = [NXApp pasteboard];
    char   *data;
    int     length;
    id      pasteList;

    for(type = [pb types]; *type; type++) {
        if(!strcmp(*type,"PrivateTypes"))
            break;
    }
    if(*type) {
        [pb readType:*type data:&data length:&length];
        pasteList = NXReadObjectFromBuffer(data, length);
        NXFreeObjectBuffer(data, length);
    }
    return self;
}
```

### Standard Menu Integration

Interface Builder provides Cut, Copy, and Paste commands in the Edit menu that automatically send `cut:`, `copy:`, and `paste:` messages to the first responder through the responder chain.

---

## Exception Handling

For up-to-date information on exception handling, see the documentation in:

```
/NextLibrary/Documentation/NextDev/Concepts/ExceptionHandling.rtf
```

---

## Summary

The Application Kit provides comprehensive support for application development through streams for data I/O, archiving for persistence, a defaults system for user preferences, the pasteboard for cut/copy/paste operations, and exception handling for error management. These facilities work together with the basic application structure and user-interface objects to provide a complete development platform for NeXT applications.

For detailed information on all methods and functions, consult the NeXTSTEP Reference manuals.
