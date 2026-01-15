# 9. User-Interface Objects

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

> **Note:** This information, although fundamentally correct, has not been updated for release 3.0. For up-to-date information on the Text and Box classes, see the documentation in:
> ```
> /NextLibrary/Documentation/NextDev/GeneralRef/02_ApplicationKit/Classes
> ```

## Overview

The Application Kit consists mainly of class definitions for objects that respond to events or draw on the screen. Chapter 6, "Program Structure," and Chapter 7, "Program Dynamics," showed how these objects work together to ground your application in the NeXT window system and give it a unified program structure. Chapter 8, "Interface Builder," showed how to build a program from these objects graphically, writing only minimal amounts of Objective-C code.

This chapter discusses each of these objects in more detail, from the standpoint of the Application Kit, not Interface Builder. This information will be useful if you want to gain a full understanding of how these objects work, if you plan to define a subclass of any of them to change default behavior, or if you simply want to know the range of possibilities allowed by their kit definitions.

### Class Categories

All user-interface objects are Responders, and most are also Views of one sort or another. They fall roughly into five categories:

1. **The Application object** - Oversees the entire program. You'd generally customize this object to meet the needs of your program by defining an Application subclass.

2. **Control objects** - Views that implement the target-action paradigm and give users a way to control the program's activity.

3. **Window objects** - Manage the windows provided by the Window Server and the Views that are displayed within them.

4. **The Text object** - Gives programs a way to display text to users and users a way to enter, edit, and select text.

5. **Support Views** - Generally used in conjunction with other Views, either to provide scrolling and zooming support, or to provide a background and frame to a companion View.

For the most part, these are objects that you can use "off the shelf." You get an object that's ready to use just by creating and initializing an instance of a Kit class; you don't have to define your own subclass.

---

## The Text Class

The Text class is a subclass of View designed for the display and manipulation of text. An object of the Text class can:

- Display text in various fonts
- Control the format of lines and paragraphs
- Wrap lines within the boundaries you set
- Specify displayed text as either read-only or editable
- Let the user cut, copy, and paste text between windows in the same or different applications
- Write text to, or retrieve it from, the disk

A Text object can be used to implement notepads, static text displays, and electronic forms, to name a few possibilities.

### Text in Other Controls

Many applications will include a Text object only indirectly, through the use of a TextField or Form object. TextField and Form objects provide a simplified interface to the Text object. They initialize the Text object for you and convert its contents to the format you request: an integer, floating-point, or string value. Since it belongs to a subclass of Control, a TextField or Form object also lets you set an action message and a target. Furthermore, all TextField and Form objects in the same window share a single Text object, thus reducing the memory demands of your application.

TextField and Form give you access to a small but frequently used subset of the methods that the Text class implements. If the text-handling features provided by these Controls don't meet your needs, use a Text object directly. Because the Text class has a rich selection of features, you'll rarely need to create a Text subclass of your own.

### How a Text Object Works

A Text object is a View specifically designed for text display and editing. It draws in a flipped coordinate system; that is, the origin is located at the bounds rectangle's upper left corner. X-axis values increase from left to right and y-axis values increase from top to bottom.

Being a View, a Text object renders on the screen only the drawing that lies within its frame rectangle. However, the drawing of characters is confined to a generally smaller area, the **body rectangle**. This rectangle is inset from the edges of the frame rectangle by the width of the Text object's margins.

A Text object views its contents as a string of characters. Individual characters are identified by their character position, their numerical rank in the string starting from 0. As a Text object draws a line of text, it continually recalculates the accumulated width of all the characters drawn so far on that line. If the total width exceeds that available between the left and right margins, a portion of the text is wrapped onto the next line. Lines of text are generally wrapped on a word basis.

By default, the text a Text object displays can be edited by the user. However, there are two mechanisms you can use to protect the text from alteration. One lets you specify that the text is read-only, and the other lets another object within your program decide dynamically when the text can be edited and when it can't. This object is the Text object's delegate.

### Creating a Text Object

You create a Text object by sending a `newFrame:text:alignment:` message to the Text class:

```c
myText = [Text newFrame:&aRect text:"Brevity is the soul of wit."
    alignment:NX_LEFTALIGNED];
```

The Text class responds by creating a Text object. The object's frame rectangle, text, and text alignment are set by the method's three arguments:

| Argument | Permitted Value |
|---|---|
| `frame` | A pointer to an NXRect structure. This structure specifies the frame's size and the location of its lower left corner within the coordinate system of the Text object's superview. |
| `text` | A pointer to a null-terminated array of characters. These characters appear when the Text object is displayed. |
| `alignment` | A constant specifying the alignment of the text: `NX_LEFTALIGNED`, `NX_RIGHTALIGNED`, or `NX_CENTERED` |

You can also create Text objects by sending `new` or `newFrame:` messages:

```c
myText1 = [Text new];
myText2 = [Text newFrame:&aRect];
```

### Setting Text

The text the Text object displays can be set in a number of ways:

- Set at the time the object is created
- Set by a message from another object in the application
- Read into the Text object from a file
- Entered by the user

After a Text object has been created, another object in the application can reset the text:

```c
[myText setText:"O, for a horse with wings!"];
```

You can also set the Text object's text from a stream:

```c
NXStream  *input;
char      *theFile = "Sonnets";

input = NXMapFile(theFile, NX_READONLY);
[textOutlet readText:input];
NXCloseMemory(input, NX_FREEBUFFER);
```

`readText:` interprets the data in the stream as plain ASCII text. A similar method, `readRichText:`, is designed to read text encoded in Rich Text Format (RTF).

### Examining the Text

Your application can get a copy of all or part of the Text object's text:

```c
char  *textBuffer;
int    textLength;

textBuffer = malloc([text textLength]+1);
[text getSubstring:textBuffer start:0 length:[text textLength]];
```

### Writing Text to a File

A Text object can write its text out in either ASCII or RTF format. To write an ASCII version:

```c
int        fd;
NXStream  *stream;
char      *theFile = "Sonnets";

fd = open(theFile, O_CREAT | O_WRONLY | O_TRUNC, 0666);
stream = NXOpenFile(fd, NX_WRITEONLY);
[myText writeText:stream];
NXFlush(stream);
NXClose(stream);
close(fd);
```

### Text Layout

The format of a Text object's displayed text depends on several factors:

- The size of the Text object's frame rectangle
- The dimensions of the four margins
- The alignment of the text
- The vertical placement of the characters within the line
- The style of each paragraph

#### Frame Rectangle

A Text object can dynamically change the size of its frame rectangle in response to the addition or deletion of text. Control this with:

```c
[myText setVertResizable:YES];
[myText setHorizResizable:NO];
```

Set limits to the extent of resizing:

```c
NXSize maxSize = {100.0, 10000.0};
NXSize minSize = {100.0, 20.0};

[myText setMaxSize:&maxSize];
[myText setMinSize:&minSize];
```

#### Margins

Text layout is also determined by the width of a Text object's margins:

```c
NXCoord  lMgn, rMgn, tMgn, bMgn;

[myText getMarginLeft:&lMgn right:&rMgn top:&tMgn bottom:&bMgn];
[myText setMarginLeft:lMgn right:rMgn top:6.0 bottom:bMgn];
```

#### Text Alignment

By default, text is aligned with the left margin. Reset with:

```c
if ([myText alignment] != NX_CENTERED) {
    [myText setAlignment:NX_CENTERED];
    [myText display];
}
```

#### Line and Character Layout

Methods adjust the vertical placement of characters within a line and the height of the line itself:

```c
[myText setLineHeight:2.0 * [myText lineHeight]];
[myText setDescentLine:2.0 * [myText descentLine]];
```

#### Word and Character Wrapping

A Text object's default behavior is to word-wrap lines of text. Control wrapping:

```c
if([myText charWrap] == YES)
    [myText setCharWrap:NO];

[myText setNoWrap];
```

### The Selection

The selection is the portion of the text that an action, such as a command, will affect. The selection can have 0 width (an empty selection), in which case it's indicated by a blinking caret, or it can contain one or more characters.

#### Setting the Selection

The position and extent of the selection can be set programmatically using the `setSel::` method:

```c
[myText setSel:0:0];  // Move caret to start
[myText setSel:0:10]; // Select first 10 characters
```

#### Querying the Selection

```c
NXSelPt  selStart, selEnd;
[myText getSel:&selStart :&selEnd];
```

#### Modifying Selection Contents

```c
[myText replaceSel:"\n\nYours truly,\n"];
```

### Text Editing

The Text class provides two ways to control the editability of a Text object's text:

```c
if([myText isEditable] == YES)
    [myText setEditable:NO];
```

### The Delegate

A Text object can send notification messages to a delegate in response to user actions:

```c
[myText setDelegate:anObject];
anObject = [myText delegate];
```

#### Notification Messages

| Before Change | After Change |
|---|---|
| `textWillChange:` | `textDidChange:` |
| `textWillEnd:` | `textDidEnd:endChar:` |
| `textWillResize:` | `textDidResize:oldBounds:invalid:` |

#### Example Delegate Method

```c
- (BOOL) textWillChange:(id)textObject
{
    return(NO);  // NO = no objection to change
}
```

#### The Tag

You can assign a tag to identify a Text object to its delegate:

```c
[myText1 setTag:1];
[myText2 setTag:2];

- (BOOL) textWillChange:(id)textObject
{
    if([textObject getTag] == 1)
        return(YES);  // prevent change
    else
        return(NO);   // allow change
}
```

### Smart Cut and Paste

A Text object attempts to respect word spacing conventions when the user cuts words from or pastes them into the text. For example:

- Cutting "no" from "I see no objection" leaves "I see objection"
- Pasting "strong" between "no" and "objection" gives "no strong objection"

This feature only works on word selections (double-click to select).

### Cut, Copy, Paste, and Delete

These standard Edit menu commands are handled by Text objects:

```c
[myText delete:nil];   // Remove selected text
[myText copy:nil];     // Copy selection to pasteboard
[myText cut:nil];      // Copy and delete
[myText paste:nil];    // Insert pasteboard contents
```

### The Text's Font

Control fonts in a Text object:

```c
[myText setFont:[Font newFont:"Helvetica" size:12.0 style:0
    matrix:NX_FLIPPEDMATRIX]];
```

#### Modifying Font for Selection

```c
[myText setSelFontFamily:"Times-Roman"];
[myText setSelFontSize:14.0];
[myText setSelFontStyle:NX_BOLD];
```

#### Mono Font Setting

```c
[myText setMonoFont:YES];  // Ignore font info from pasteboard
```

### Drawing and the Text Class

A Text object, by default, is a transparent View. Control this:

```c
[myText setOpaque:YES];  // Force repainting of all pixels
```

Control gray values:

```c
[myText setBackgroundGray:NX_BLACK];
[myText setTextGray:NX_WHITE];
[myText setSelGray:NX_DKGRAY];
```

### Text Filters

The Text class supports character filter and text filter functions for processing input.

#### Character Filters

The Text class provides two built-in character filters:

```c
[myText setCharFilter:NXFieldFilter];    // Default
[myText setCharFilter:NXEditorFilter];   // More permissive
```

#### Text Filter Functions

To install a custom text filter:

```c
[myText setTextFilter:FilterText];
```

### Text in a Scrolling View

To accommodate large blocks of text, a Text object is commonly made a subview of a ScrollView:

```c
myText = [Text newFrame:&contentRect text:NULL alignment:NX_LEFTALIGNED];
[myText notifyAncestorWhenFrameChanged:YES];
[myText setVertResizable:YES];
[myText setHorizResizable:NO];
[theScrollView setDocView:myText];
```

### Archiving a Text Object

The `write:` and `read:` methods archive and restore Text objects. Note that some instance variables are not archived if you've assigned custom functions or tables.

### Rich Text Format Support

A Text object can encode and decode a subset of RTF formatting commands. Supported control words include:

- `\b` (bold), `\i` (italic)
- `\ql` (left align), `\qr` (right align), `\qc` (center)
- `\fn` (font), `\fsn` (font size)
- `\par` (paragraph), `\tab` (tab)

---

## The Box Class

A Box is a View that visually groups other Views. A typical Box displays a border and a title, often used to group choices that a user can make in a panel.

### Creating a Box Object

Create a new Box with:

```c
NXRect myRect;
NXSetRect(&myRect, 200.0, 200.0, 500.0, 500.0);
myBox = [Box newFrame:&myRect];
```

### The Border

A Box's border can be marked with a line, a bezel, a groove, or nothing at all. Change the border type:

```c
[myBox setBorderType:NX_BEZEL];
```

Border types:
- `NX_LINE`
- `NX_BEZEL`
- `NX_GROOVE`
- `NX_NOBORDER` (default is `NX_GROOVE`)

Query the border type:

```c
int  myBorderType;
myBorderType = [myBox borderType];
```

### The Title

Set the title position:

```c
[myBox setTitlePosition:NX_ABOVETOP];
```

Title positions:
- `NX_NOTITLE`
- `NX_ABOVETOP`
- `NX_ATTOP`
- `NX_BELOWATOP`
- `NX_ATBOTTOM`
- `NX_BELOWBOTTOM`
- `NX_ABOVEBOTTOM`

Change the title text:

```c
const char  *myTitle = "Find Options";
[myBox setTitle:myTitle];
```

Customize the title font:

```c
[myBox setFont:aNewFontObj];
```

Query title information:

```c
char  *myTitle = [myBox title];
int    myTitlePosition = [myBox titlePosition];
id     myFontObject = [myBox font];
```

### The Offsets

Adjust spacing between contents and border:

```c
[myBox setOffsets:40.0 :0.0];  // horizontal : vertical
```

Query offsets:

```c
NXSize  mySize;
[myBox getOffsets:&mySize];
```

### Box Contents and View Hierarchy

Add Views to the Box:

```c
[myBox addSubview:newSubview];
```

Replace the content view:

```c
[myBox setContentView:newContentView];
```

### Resizing the Box

Resize the Box to fit its contents:

```c
[myBox sizeToFit];
```

Set the content frame:

```c
NXRect myContentRectangle;
NXSetRect(&myContentRectangle, 100.0, 100.0, 300.0, 300.0);
[myBox setFrameFromContentFrame:&myContentRectangle];
```

Resize from outside in:

```c
[myBox sizeTo:300.0 :300.0];
```

---

## Summary

The Text and Box classes provide flexible, powerful tools for building NeXTSTEP user interfaces. Text objects handle all text display and editing needs, while Box objects provide visual organization of interface elements. Together with other Application Kit classes, they form the foundation of responsive, user-friendly applications.

For the most current information and additional Application Kit classes, consult the NeXTSTEP Reference manuals.
