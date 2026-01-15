# 6. Program Structure

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

> Changes made for the current release of NeXTstep affect the information presented in this chapter. For details see:
> - `/NextLibrary/Documentation/NextDev/ReleaseNotes/AppKit.rtf`
> - `/NextLibrary/Documentation/NextDev/ReleaseNotes/AllocInitAndNew.rtf`
> - `/NextLibrary/Documentation/NextDev/ReleaseNotes/Performance.rtf`

## Overview

The simplest way to program an application for the NeXT computer is to take advantage of the facilities built into the three software kits: the Application Kit, Sound Kit, and Music Kit. The Application Kit provides a basic program structure for applications that draw on the screen and respond to events—for almost all NeXT applications, even those that mainly use the other kits. It implements the NeXT user interface and relieves you of many of the more tedious programming tasks common to most applications. When you use the Application Kit, you need to write only the code that makes your program distinct from a generic application.

### Application Kit Capabilities

Through the Application Kit, your program can:

- Open and maintain a connection to the Window Server
- Place windows on the screen
- Draw within the windows
- Process keyboard and mouse events
- Create buttons, menus, and the other graphical objects defined in the user interface
- Display editable text
- Print documents and on-screen displays
- Write data to and retrieve data from the pasteboard
- Manage off-screen bitmaps, including bitmaps for the cursor
- Communicate with cooperating applications

This chapter introduces the Application Kit and the program structure it defines. Chapter 7, "Program Dynamics," follows with a more detailed discussion of drawing and event handling within this structure. The Sound Kit and Music Kit are discussed in Sound, Music, and Signal Processing.

The three software kits provide an integrated object-oriented programming environment. They're coded in Objective-C, an object-oriented extension of the C language; programs that make use of the kits must also be written in Objective-C. However, through Interface Builder, you can develop much of your program graphically, on-screen, without writing any Objective-C code. Interface Builder constructs and initializes the user-interface objects for your application and sets up their connections to the rest of the program.

Chapter 8, "Interface Builder," assumes the program structure that's defined in the Application Kit and described in this and the following chapters. Chapter 9, "User-Interface Objects," has a more detailed discussion of Kit-defined objects that draw and respond to events; these are objects that you can program using Interface Builder. Chapter 10, "Support Objects and Functions," then presents the objects, functions, and macros that support the main functionality of your program.

If you're unfamiliar with object-oriented programming and the terminology and syntax of Objective-C, consult Chapter 3, "Object-Oriented Programming and Objective-C."

---

## Writing a Program with the Application Kit

Every application consists of a network of objects—many defined in the Application Kit, some defined in the application you write. The structure of the network and the kinds of messages that get passed from object to object are the subjects of this and the following chapter.

### Class Definitions

The Application Kit consists mainly of class definitions. It defines a separate class for each of the canonical control objects in the NeXT user interface. It also defines classes for panels and menus, for objects that permit users to enter and edit text, for an object that can manage the pasteboard for your application, for bitmaps and cursors, and for an object that oversees the entire application. All these classes, and others, are described in detail in this and the following chapters.

Like all Objective-C classes, the classes defined in the Application Kit are linked, through their superclasses, to an inheritance hierarchy that has the Object class at its root. Figure 6-1 shows the classes in the Application Kit inheritance hierarchy.

**Figure 6-1. Application Kit Inheritance Hierarchy**

This diagram shows, for example, that the Panel class has the Window class as its superclass, that the Window class has the Responder class as its superclass, and that the superclass of Responder is Object.

Each class inherits both instance variables (data structures) and methods (procedures) from all the classes above it in the hierarchy. The Panel class inherits from Window, Responder, and Object. This means that an instance of the Panel class can use not only Panel methods, but also methods defined in the Window, Responder, and Object classes. It also means that every Panel object has all the instance variables declared in the three classes it inherits from, in addition to any that might be declared in the Panel class itself.

Since the methods and instance variables that are defined in a class are inherited by its subclasses, a subclass can be viewed as a variant of the classes above it in the hierarchy. Inheritance makes Panel a kind of Window, and Window a kind of Responder. When we speak of a "Window object," we mean not only objects belonging directly to the Window class, but also any object that belongs to any class that inherits from Window. Panels, OpenPanels, FontPanels, Menus, and PopUpLists are all Windows.

### Using Kit-Defined Classes

You can make use of Application Kit classes in four ways:

1. **Instances** - You can create objects (instances) belonging to the classes defined in the Kit. Your application can include Button, Slider, and Menu instances, for example.

2. **Subclasses** - You can define new subclasses of Application Kit classes, then create instances of the subclasses for your application. Through its superclass, each class you define will inherit methods and instance variables from the Kit. The instance variables and methods you add in the subclass definition serve to adapt generic Kit capabilities to the specific needs of your application.

3. **Categories** - You can add new methods to a Kit class by putting them in a category definition that extends the original definition of the class.

4. **Delegates** - You can define your own objects to act on behalf of objects that inherit from the Application Kit. Kit objects handle most of the work themselves, but can delegate responsibility for some of the messages they receive to other objects.

#### Instances

In many cases, creating an instance of an Application Kit class is all that's needed. Class definitions in the Kit give you a good deal of freedom to adjust an instance to the needs of your application. A Button, for example, can be shaped, titled, associated with a keystroke, and assigned an action of your choosing, simply by initializing its instance variables. (For a Button, you'd usually do this through Interface Builder.)

#### Subclasses

Since your application will do some things no other application will do, it will probably need to define some objects of its own. In most cases, these objects will be adaptations of objects defined in the Application Kit. For example, the Panel class defines a kind of window that behaves as a panel should according to the user-interface guidelines. If you need a panel with a particular display and a special relationship to the other windows in your application, you can define it as a subclass of Panel.

The main reason for defining subclasses of Kit classes is to design objects that draw and respond to events in ways that are specific to your application. Generally, you'd define subclasses of View.

A View is an object that has an area and a coordinate system for drawing within a window. It can be selected to receive keyboard and mouse events, and can be scrolled and automatically displayed. If you need an object with these features, you'd simply define a subclass of View and implement methods that draw and respond to events as you'd like. You wouldn't have to re-implement the methods that manage the object's coordinate system, display it on the screen, permit it to be scrolled, or select it to receive mouse and keyboard events. The subclass inherits all the methods and instance variables of the View class; you need to implement only the features you want to add or change.

You might also choose to define subclasses of specific kinds of Views. For example, the Button class defines one- and two-state buttons. If you want a button that can display three or more states, you can subclass Button to add the functionality that you need.

#### Categories

In simple cases, where the only modification you need to make to a Kit-defined class is the addition of some methods, you can add them in a category definition rather than in a new subclass. Categories extend the definition of an existing class to encompass new methods. They're discussed under "Adding to a Class" in Chapter 3.

#### Delegates

Some Application Kit objects permit other objects to intervene and control some of their actions. Because the Kit object delegates responsibility for its behavior to the controlling object, the controlling object is known as a delegate. A delegate does only as much as you program it to do (in its class definition). One delegate can serve a number of different client objects.

By centralizing application-specific code in the delegate, you can adapt the behavior of Application Kit objects without defining Kit subclasses. The delegate can be a subclass of an Application Kit class, but usually it's just a subclass of the generic Object class.

The section on "The Extended Framework," later in this chapter, has more information on delegates and the messages they can receive.

---

## Structuring an Application

When it's launched, an application creates a set of objects for itself, displays itself on the screen and waits for events from the Window Server. Whenever it receives an event, it initiates a message to the responsible object, then waits for the next event. The object that receives the message will initiate some messages of its own, so several objects are likely to be involved in the application's response to the event.

The response to an event always includes a visible reaction on-screen, if only to let the user know the event was received. Applications draw both to present the user with a display that can prompt for events and to show the results of those events. The user, the application, and the Window Server participate in a continuous cycle of action and response.

This cycle is driven by the user's actions. From the point of view of an application, it's driven by events. Between events, applications can respond to three other kinds of input:

- **Remote messages** - Messages received from other applications. For example, an application might receive a message asking it to supply some data in text form to a word processor, or it might get a message from the Workspace Manager requesting it to open another file. Remote messages are handled just like messages initiated within the application itself.

- **Timed entries** - Procedures that are to be called periodically and have been registered with the `DPSAddTimedEntry()` function.

- **Data received at a monitored port or file** - For example, an application might monitor changes to a central database (perhaps of messages written to the console). The monitoring procedure is registered using the `DPSAddPort()` or `DPSAddFD()` function.

Most everything that an application does is, directly or indirectly, a response to an event, to a remote message (which is itself prompted by an event in another application), to a timed entry (which is usually registered because of an event), or to data received at a monitored port or file (which changes due to user activity elsewhere).

### Application Requirements

To respond to events and remote messages, every application needs:

- **An Application object** - To get events from the Window Server
- **Window objects** - To provide areas on-screen where the application can draw
- **View objects** - To draw within the Windows and to handle keyboard and mouse events associated with windows
- **A Listener** - To receive remote messages, and a Speaker, to send them

The Application Kit creates a default Listener and Speaker for every application. You must create the Application object, Windows, and Views for yourself. The Application object also oversees the application's Windows, and is usually expected to respond to the remote messages received by the Listener. Windows oversee their Views. All the objects are designed to work together in a coherent program structure.

---

## Responding to Kit-Generated Messages

As it receives events from the Window Server and reacts to the instructions in your program, the Application Kit initiates a variety of messages. Very often, the message is designed to give the objects in your program the opportunity to do their own work in their own way. To take advantage of this opportunity, you need only implement the appropriate method in a class definition.

### Kit-Defined Methods for Application Customization

| If You Want to | Write Your Own Version of |
|---|---|
| Respond to events and subevents | `keyDown:`, `keyUp:`, `flagsChanged:`, `mouseDown:`, `mouseUp:`, `mouseDragged:`, `mouseEntered:`, `mouseExited:`, `mouseMoved:`, `windowMoved:`, `windowExposed:`, `windowResized:`, `applicationDefined:`, `powerOff:` |
| Determine which events to receive | `acceptsFirstResponder`, `acceptsFirstMouse` |
| Reflect change of event-handling status | `becomeActiveApp`, `resignActiveApp`, `becomeKeyWindow`, `resignKeyWindow`, `becomeMainWindow`, `resignMainWindow`, `becomeFirstResponder`, `resignFirstResponder` |
| Respond to a keyboard alternative | `performKeyEquivalent:`, `commandKey:` |
| Draw an object on the screen | `drawSelf::` |
| Open a file | `validateFilename`, `openFile:ok:`, `appAcceptsAnotherFile`, `appOpenFile:type:` |
| Archive and unarchive an object | `read:`, `write:`, `finishUnarchiving`, `awake` |
| Change the cursor | `resetCursorRects` |
| React when the user logs out | `powerOffIn:andSave:`, `appPowerOff:` |
| Control text displays | `textWillChange:`, `textDidChange:`, `textWillEnd:`, `textDidEnd:endChar:`, `textWillResize:`, `textDidResize:oldBounds:invalid:`, `textWillSetSel:toFont:`, `textWillConvert:fromFont:toFont:`, `textWillStartReadingRichText:`, `textWillReadRichText:stream:atPosition:`, `textWillFinishReadingRichText:`, `textDidlRead:paperSize:`, `textWillWrite:paperSize:`, `textWillWriteRichText:stream:forRun:atPosition:emitDefaultRichText:`, `text:isEmpty:` |
| Control a window | `windowWillClose:`, `windowWillReturnFieldEditor:toObject:`, `windowWillMiniaturize:toMiniwindow:`, `windowDidMiniaturize:`, `windowDidDeminiaturize:`, `windowWillResize:toSize:`, `windowDidResize:`, `windowDidExpose:`, `windowDidMove:`, `windowDidBecomeKey:`, `windowDidResignKey:`, `windowDidBecomeMain:`, `windowDidResignMain:`, `windowDidUpdate:` |
| Manage the application | `appDidHide:`, `appDidUnhide:`, `appDidInit:`, `appDidBecomeActive:`, `appDidResignActive:`, `appDidUpdate:` |
| Update a menu item | A method with a name of your own choosing |
| Respond to user actions on a control | A method with a name of your own choosing |

Although you may write your own versions of these methods, you never send a message to have an object perform them. The message is initiated by the Application Kit as part of its procedures for handling an event, or in reaction to another message your program sends.

There's no penalty for failing to write your own versions of these methods. In a few cases (such as `textWillEnd:`), the message won't be sent unless the receiving object has a method that can respond. In other cases (such as `mouseDown:` and `drawSelf::`), the Kit defines a default version of the method that the receiving object can inherit and use.

**Note:** When defining a class, you're free to override any inherited method. However, most methods do what you need them to do, so there's no reason to override them (just as there's no reason to override functions in the standard C library). The difference between most methods and the methods in the list above is that the listed methods are designed to be overridden; most methods are not.

---

## Principal Application Kit Classes

Application Kit objects are designed to work together. Through them, and through your own subclass adaptations of them, your program assumes a framework for dealing with events, drawing on the screen, and managing user-interface objects.

The inheritance hierarchy of Application Kit classes shows that most Application Kit classes inherit from the Responder class, and almost all Responders also inherit from Window or View. The basic structure of a complete Application Kit program can be outlined by looking at just these three classes and the other Responder subclass, Application. Although other Kit classes—notably Panel, Menu, Listener, and Speaker—play important roles, the fundamental framework of an application is defined by Responder and its three subclasses.

Two other Kit classes, Control and Cell, make a vital enough contribution to program design to also be considered principal classes.

Responder is an abstract superclass; programs use instances of its subclasses rather than of Responder itself. Objects that inherit from Responder can, as its name implies, respond to events. Those that inherit from View can also draw on the screen. Since the mouse requires a graphical object for the cursor to point to, Views are the principal handlers of mouse events. Since typing must be displayed on-screen, Views also handle keyboard events. Most of the objects in your program will likely be Views of one sort or another.

Control is the abstract superclass for View objects that fit the control-action and target-selection paradigms of the user interface. The main attribute that Control imparts to its subclasses is the ability to translate events into application-specific messages for other objects. Each of the controls described in the user interface—buttons, sliders, text fields, and scrollers—is implemented as a separate Control subclass.

The Cell class defines an object that is very much like a View; it can draw and respond to events. But Cells lack the View superstructure that assigns the object an actual location within a window and permits it to receive events. Cells therefore must serve at the pleasure of Views. A View gets events for its Cells and tells them where and when to draw. Cells are a way of dividing a View into semi-autonomous regions, just as Views divide up the content area of a window. Most Control objects use a Cell to handle their drawing tasks.

Each Window object corresponds to a separate window provided by the Window Server. Every program will have at least one Window object, not only to open an area on the screen for the application to draw in, but also to supervise drawing and event handling among the View objects that are displayed within the window. Each Window typically has a number of Views arranged hierarchically inside it. Window's subclass, Panel, defines an object that can fill the role of a panel in the user interface. Panel's main subclass, Menu, defines objects that serve as menus.

Every program must also include one, and only one, Application object to act as its contact with the Window Server. The Application object supervises the entire program, receiving events from the Server and dispatching them to the appropriate Window objects. It manages the Windows in the application much as each Window object manages its Views.

---

## Responder

Responder is the abstract superclass that most other Application Kit classes inherit from. Those that don't inherit from Responder principally act as support for those that do. The objects that inherit from Responder respond to events; they receive the messages that are initiated by the user's actions on the keyboard and mouse.

Responders participate in a linked list of event-handling objects called a responder chain. If an object in the chain can't handle a message that's sent to it, the message is passed on to its next responder. If the next responder can't handle it either, the message continues to be passed up the chain from object to object in search of a Responder that can. This allows event handling to be consolidated in an object that collects messages originally intended for other Responders. The chain is unidirectional; messages are passed in one direction only.

The Responder class defines the elements essential to the responder chain: a `nextResponder` instance variable and the methods for passing messages from one object to another.

Only two types of message are passed along the chain. Both types are generated in direct response to the user's actions:

1. **Event Messages** - Messages that announce an event, such as messages to perform a `keyDown:` or `mouseExited:` method. This type of message is discussed under "Event Messages" in the "Event Handling" section of the next chapter.

2. **Action Messages** - Messages that announce a user action on a Control object, such as a Button or a Slider. These messages are discussed under "Action Messages" in the "Event Handling" section of the next chapter.

Although Responder's contribution to event handling is crucial, it's also quite small. Its subclasses—especially Application, View, Window, and Control—add much more specific event-handling capabilities.

---

## View

The View class provides a structure for drawing on the screen and for handling mouse and keyboard events. All the graphical objects defined in the NeXT user interface inherit from View. Views draw scrollers and buttons, display text, and even draw the borders and title bars of windows. The graphical objects you design must also be Views.

The drawing a View object places on the screen can be thought of as a visual representation of the object itself. In many cases, View subclasses are named for the objects they draw. A Button, for example, draws an image of a button that the cursor can point to and the user can click or press. We speak both of the Button object "drawing" and of it "being drawn" on the screen. In a sense, it draws itself.

Although all View objects are also Responders, you can define a View that draws but doesn't respond to events. Any events it happens to receive will be passed to another object through the responder chain.

### The View Hierarchy

Every View object is associated with the particular window where it's displayed. All the Views within a window are linked together in a view hierarchy. Each View has another View as its superview and may be the superview for any number of subviews.

The view hierarchy benefits drawing in two ways:

1. It permits a View object to be constructed out of other Views. For example, a graphical keypad might be a View with separate subviews for each key. A spreadsheet could use a different View for each data field.

2. It also permits each View object to have its own coordinate system for drawing. Views are positioned within the coordinates of their superviews, so when a View object is moved or its coordinate system is transformed, all its subviews are moved and transformed with it. Since a View draws within its own coordinate system, its drawing instructions can remain constant no matter where it or its superview moves on the screen.

The view hierarchy is not the same as the inheritance hierarchy. The inheritance hierarchy is an arrangement of classes; the view hierarchy is an arrangement of objects. They don't parallel each other. You could create an instance of the Box class that had another instance of Box as its superview, for example, and several ScrollViews and Controls as subviews.

The inheritance hierarchy is fixed at compile time. The view hierarchy is dynamic; it can be rearranged as the program runs. A View can be moved from window to window and be installed as a subview first of one superview then of another.

Three instance variables locate a View object within its view hierarchy:

- **`window`** - The Window object where the View will appear
- **`superview`** - The object that's immediately above the View in the hierarchy
- **`subviews`** - A list of all the objects that are immediately below the View in the hierarchy

A View's superview and all the Views above the superview in the view hierarchy are sometimes referred to as the View's ancestors. A View's subviews and all the Views below its subviews are known as its descendants.

### The Frame Rectangle

The location and dimensions of a View object are provided by the instance variable `frame`. `frame` is an `NXRect`, a structure that defines the essential features of a rectangle, its coordinates and size:

```c
typedef float  NXCoord;

typedef struct _NXPoint {
    NXCoord    x, y;
} NXPoint;

typedef struct _NXSize {
    NXCoord     width, height;
} NXSize;

typedef struct _NXRect {
    NXPoint    origin;
    NXSize     size;
} NXRect;
```

This structure was discussed in Chapter 4, "Drawing." The width and height variables specify the dimensions of the rectangle within the superview's coordinate system; they cannot be negative. The x and y variables locate one corner of the rectangle, also within the superview's coordinates. The width and height are measured from this corner along the positive x- and y-axes, so the corner is the one with the smallest x and y values in the superview's coordinate system.

If the superview's coordinate system has the positive x-axis extending rightward and the positive y-axis extending upward in the usual manner, the x and y variables specify the lower left corner. If the superview's coordinates have been rotated or flipped, the corner may not be at the lower left visually.

`NXCoord`, `NXPoint`, and `NXSize` are defined in the NeXT header file `dpsclient/event.h`. The `NXRect` structure itself is defined in `appkit/graphics.h`; it's used throughout the Application Kit to specify the size and location of rectangles.

A View object doesn't have to draw a rectangle—it could be a circle, for example, a stick figure, or a line of text—but it draws only inside the area specified by its frame instance variable. This area is the View's frame rectangle. A default clipping path is constructed around the edge of the rectangle before the object is displayed. You can specify a more restricted clipping path for the View, but you can't extend it to include any area falling outside its frame rectangle.

The frame rectangle, therefore, defines the boundaries of the View, the tablet on which it can draw. A View object can be thought of as simply a rectangular opening into the window. `frame` specifies the location and dimensions of this opening.

### Drawing Coordinates

Each View has its own coordinate system for drawing. Before a View draws, its coordinate system is made the current coordinate system for the application.

As a default, a View's coordinate system is the same as its superview's, except that:

- The point recorded in a View's frame instance variable becomes the origin (0.0, 0.0) of its drawing coordinates.
- If a View is rotated, its coordinate system is rotated with it; the x- and y-axes stay parallel to the sides of the View's frame rectangle.

A View can also adopt a different coordinate system by modifying the system it gets by default. Modifications are made with methods that parallel the standard PostScript transformations:

- The coordinate origin can be translated to a point other than `frame.origin`.
- The size of a unit along either the x- or y-axis can be scaled to a dimension other than the one used by the superview.
- The coordinate axes can be rotated around the origin so that they're no longer aligned with the sides of the frame rectangle.

These modifications alter the reference coordinate system that the View uses for drawing, and may affect the appearance of what's drawn, but they don't change the area where the drawing appears—that is, they don't affect the View's frame rectangle.

Besides the default coordinate system and transformations of it, a View can opt for either of two alternative coordinate systems:

- It can keep its superview's coordinate system.
- It can flip its coordinate system so that the origin is in the upper left corner and the positive y-axis extends downward. Flipped coordinates are especially useful for Views that draw text in the normal manner for English (starting at the upper left of the page and proceeding to the lower right).

### The Bounds Rectangle

The `frame` instance variable locates and sizes a View within its superview's coordinate system, but is of little use when the View comes to draw within its own coordinate system. To draw efficiently, a View often needs to know where it's located and how large it is within its own coordinates; that is, it needs to have its frame rectangle translated into its own coordinate system. The `bounds` instance variable provides this information.

A View's bounds rectangle is guaranteed to be the smallest rectangle that completely encloses the View and is expressed in the View's own drawing coordinates. It will usually specify exactly the same area on-screen as the frame rectangle, though stated in a different coordinate system. If a View uses the default coordinate system, `bounds` will be identical to `frame`, except that `bounds.origin` will be (0.0, 0.0).

Views typically use the bounds rectangle to ensure that they don't uselessly attempt to draw images that will never be rendered on the screen. By checking whether the images they wish to draw lie within their bounds, they can avoid sending the Window Server drawing instructions for images that lie outside the View and will consequently be clipped.

Although programs read the `bounds` instance variable, they never set it. `bounds` is automatically updated by methods that change the frame rectangle and by methods that alter the View's coordinate system.

### The Visible Rectangle

The drawing that a View does is clipped not only to its own frame rectangle, but also to every ancestor View above it in the view hierarchy. Therefore, the bounds rectangle by itself is a reliable guide only for Views that aren't scrolled and stay entirely within the frame rectangles of all their ancestors. For Views that fail this test, the View class provides a more reliable indicator of where to draw than bounds alone—the visible rectangle.

The visible rectangle is the intersection of the View's bounds with the visible portions of all its ancestor Views' bounds. It describes the area where the View can actually draw. For Views in a scroll view, the visible rectangle excludes the portions of the View that lie outside the scroll view's clip rectangle.

---

## Window

Every program must have one, and only one, Application object. The program must also have at least one Window object, though a typical application will have several. The Window class provides methods for creating windows, positioning them on-screen, and controlling their display.

### Setting Up a Window

A new window is created with one of two class methods, both named `new`:

```
+ new;
+ newContent:newStyle:backing:buttonMask:defer:
```

The first form creates a Window with default settings. The second form allows you to specify options for the Window, including its size and location on screen, the elements it displays (such as title bar and borders), the type of backing surface (described below), and whether creation of the Window Server's window should be deferred.

Typically, you'll use the second form:

```c
NXRect  myFrame;

NXSetRect(&myFrame, 100.0, 100.0, 500.0, 400.0);
myWindow = [[Window alloc]
    newContent:&myFrame
    style:NX_TITLEDSTYLE
    backing:NX_RETAINED
    buttonMask:NX_MINIATURIZEBUTTONMASK | NX_CLOSEBUTTONMASK
    defer:NO];
```

The first argument defines the content rectangle of the Window, the area where the Window's Views will draw. It's passed as a pointer to an NXRect structure with coordinates and size for the rectangle. The Window Server adjusts the rectangle, if necessary, to keep the window on-screen.

The style argument specifies whether the window has a title bar, resize box, or other decorative elements:

- `NX_TITLEDSTYLE` - A titled window with a title bar and appropriate borders
- `NX_PLAINSTYLE` - A window with no decorative elements
- `NX_NOTITLEDSTYLE` - A titled window without a title bar

The backing argument specifies what kind of backing surface the window has:

- `NX_RETAINED` - The Window Server maintains an off-screen buffer
- `NX_NONRETAINED` - The Window Server doesn't maintain a buffer; the window must be redrawn when exposed
- `NX_BUFFERED` - The Window Server maintains a buffer similar to retained, but with some optimizations for animation

The buttonMask argument specifies which window control buttons appear in the title bar. Standard buttons are created with mask constants:

- `NX_CLOSEBUTTONMASK` - Close button
- `NX_MINIATURIZEBUTTONMASK` - Miniaturize button
- `NX_ZOOMBUTTONMASK` - Zoom button
- `NX_TOOLBARBUTTONMASK` - Toolbar button

The last argument, `defer`, specifies whether the actual window from the Window Server should be created immediately (NO) or deferred until the window is placed on-screen (YES).

### Window Configuration

Once created, a Window can be configured with these methods:

#### Document Editing Status

```c
[myWindow setDocEdited:YES];
```

The message above has myWindow display the close button that indicates unsaved changes, if it isn't already displaying it. The same message with NO as the argument would have myWindow display the standard close button.

```c
BOOL  lastSet;
lastSet = [myWindow isDocEdited];
```

The `isDocEdited` method returns the last value set by `setDocEdited:`. NO is the default value.

#### Background Color

A window can be assigned a background color with the `setBackgroundGray:` method:

```c
[myWindow setBackgroundGray:NX_WHITE];
```

The default background color is light gray (NX_LTGRAY). `backgroundGray` returns the current background color:

```c
float  shade;
shade = [myWindow backgroundGray];
```

Whenever the Window is displayed, its content area is filled with the background color before any of its Views draw, but only if its content view hasn't been registered as an opaque View.

#### Deferred and One-Shot Windows

Each Window object is associated with a window provided by the Window Server. The Window Server usually provides the window when the Window object is created. However, when you create a Window, you have the option of not producing a window for it right away. If you pass YES as the last argument to `newContent:style:backing:buttonMask:defer:`, the Window object will be created but it won't be associated with a window until one of three methods puts it on-screen: `orderFront:`, `makeKeyAndOrderFront:`, or `orderWindow:relativeTo:`.

Deferring the creation of a Window object's window makes sense in two situations:

- If a window might never be used, deferring it saves the memory that would otherwise be allocated to it.
- If the application is large, deferring some of its windows will reduce the time required to launch it.

Once a window is associated with a Window object, the association usually lasts until the Window object is freed. Freeing the object also frees its window. However, you can arrange for the window to be freed whenever it's taken out of the screen list, by sending the object a `setOneShot:` message:

```c
[myWindow setOneShot:YES];
```

A new window will be produced for the Window object each time it's returned to the screen.

```c
BOOL  willFree;
willFree = [myWindow isOneShot];
```

Windows that the user might never bring to the screen, or might put on-screen just once, are prime candidates for being deferred and made one-shot.

#### Hiding Panels

By default, when the active application deactivates, the Application Kit hides its panels. However, you can choose to override this default and leave a panel on-screen even when its application isn't active:

```c
[myPanel setHideOnDeactivate:NO];
```

The `doesHideOnDeactivate` method returns the value set by the last `setHideOnDeactivate:` message:

```c
BOOL  willHide;
willHide = [myPanel doesHideOnDeactivate];
```

### Window Status

Although users generally select the key window and main window, there's a method that lets the application do it:

```c
[myWindow makeKeyWindow];
```

This method is most often used to designate the initial key window when the application first starts up. When a standard window is made the key window, it also becomes the main window.

The `isKeyWindow` and `isMainWindow` methods return the status of the receiving object:

```c
BOOL  nowKey, nowMain;

nowKey = [myWindow isKeyWindow];
nowMain = [myWindow isMainWindow];
```

The Application object can identify the current key window and main window:

```c
id  keyWin, mainWin;

keyWin = [NXApp keyWindow];
mainWin = [NXApp mainWindow];
```

These methods return nil if the key window or main window isn't in the current application.

### Physical Management

There are three ways that an application can manipulate the window associated with a Window object:

1. Move the window
2. Resize it
3. Reorder it in (or out of) the screen list

#### Moving a Window

The `moveTo::` method repositions a window within the screen coordinate system:

```c
[myWindow moveTo:x :y];
```

Its arguments, (x, y), give the new location of the lower left corner of the Window's frame rectangle in screen coordinates.

Another method, `moveTopLeftTo::`, can be used to reposition a window by its top left corner:

```c
[myWindow moveTopLeftTo:x :y];
```

`moveTopLeftTo::` is the same as `moveTo::`, except that it positions the top left, rather than the lower left, corner of the window.

The `center` method positions a window so that it's centered in the top two-thirds of the screen. That's where attention panels should come up so that users can't miss or ignore them.

```c
[myPanel center];
```

#### Resizing a Window

The `placeWindow:` method resizes and repositions a window within the screen coordinate system:

```c
NXRect  newRect;

NXSetRect(&newRect, 300.0, 500.0, 700.0, 600.0);
[myWindow placeWindow:&newRect];
```

The `sizeWindow::` method keeps the lower left corner of the window constant while adjusting its width and height:

```c
[myWindow sizeWindow:width :height];
```

#### Reordering a Window

The `orderWindow:relativeTo:` method alters the position of a window in the Window Server's screen list. The first argument specifies whether the receiving Window is to be placed above another window in the list, placed below another window, or left entirely off the list:

- `NX_ABOVE` - Place above another window
- `NX_BELOW` - Place below another window
- `NX_OUT` - Take off screen

The Window class defines three methods as shortcuts for common reordering cases:

```c
[myWindow orderOut:nil];
[myWindow orderFront:nil];
[myWindow orderBack:nil];
```

The `orderOut:` method removes the receiving Window from the screen list. The `orderFront:` method puts the receiving Window at the front of its tier in the screen list. `orderBack:` puts a window at the back of its tier, but in front of the workspace window.

A fourth method, `makeKeyAndOrderFront:`, combines two operations that often go together:

```c
[myWindow makeKeyAndOrderFront:nil]
```

### Getting Information about a Window

#### Frame and Content Rectangles

The `getFrame:` method takes a pointer to a rectangle (NXRect *), which it initializes with the location and size of the Window in screen coordinates:

```c
NXRect  myFrame;
[myWindow getFrame:&myFrame];
```

`getFrame:` provides the Window's frame rectangle. Given the frame rectangle, the `getContentRect:forFrameRect:style:` class method can find the content rectangle:

```c
NXRect  myContent;
[Window getContentRect:&myContent
    forFrameRect:&myFrame
    style:NX_TITLEDSTYLE];
```

#### Window Numbers

The `windowNum` method returns the receiver's window number:

```c
int  which;
which = [myWindow windowNum];
```

If the receiving Window object isn't currently associated with a window, this method returns 0. The Application object can find the id of the Window object associated with a window when given its window number:

```c
id  theWindow;
theWindow = [NXApp findWindow:theNum];
```

#### Screen List Information

A Window can return whether it's currently in the screen list:

```c
BOOL  inList;
inList = [myWindow isVisible];
```

The `isVisible` method returns YES even if the window is completely covered by other windows. This method merely tests whether the window is somewhere in the screen list.

The Application object can identify the frontmost window in the list:

```c
id  topWin;
topWin = [NXApp frontWindow];
```

NXApp can also provide the ordering of all the application's windows in the screen list:

```c
int  *listPtr, howMany;
[NXApp getScreenList:&listPtr count:&howMany];
```

---

## Environmental Information

Applications run in an environment that's determined in part by the hardware, in part by the Mach operating system, the Window Server, and the Application Kit, and in part by choices made by the user. The Kit defines methods and functions that supply the environmental information an application might require:

### Screen and Host Information

```c
NXSize  theScreen;
[NXApp getScreenSize:&theScreen];
```

The `getScreenSize:` method supplies the size of the screen where the application's windows are displayed. You should use this method rather than assume a fixed screen size.

```c
const char  *where;
where = [NXApp hostName];
```

The `hostName` method provides the name of the machine where the application is running.

```c
const char  *me;
me = [NXApp appName];
```

Application's `appName` method provides the name of the application. The name is the one assigned while compiling the program or, if no name was assigned, the name passed in the argv array.

### User Information

```c
const char  *who;
who = NXUserName();
```

The name of the current user is returned by the `NXUserName()` function.

```c
const char  *domicile;
domicile = NXHomeDirectory();
```

The user's home directory is returned by `NXHomeDirectory()`.

When the Workspace Manager launches an application, it sets the application's current working directory to the user's home directory. The standard UNIX function `getwd()` returns the current working directory, and `chdir()` changes it.

---

## Command-Line Arguments

Information entered on the command line when launching a C (or Objective-C) program is passed to the `main()` function as a pair of conventional arguments, `argc` and `argv`. To make command-line arguments available from any point within an application, the loader writes them to a well-defined location and the Application Kit provides these global variables to refer to them:

```c
int     NXArgc;
char  **NXArgv;
```

It's therefore unnecessary for the `main()` function to deal with command-line arguments itself.

When the Workspace Manager launches an application, there's no command line where users can specify program options and parameter values. An application can record user choices in the defaults database for use the next time the application is launched.

Nevertheless, the Workspace Manager uses command-line arguments to pass useful information to the applications it launches. Information passed on the command line as pairs of argument strings in the format:

```
-parameter value
```

can be read by the functions that prepare a table of default parameter values for an application.

### Debugging Arguments

Arguments that you can pass on the command line when debugging your application:

| Parameter | Value |
|---|---|
| `NXShowPS` | If not NULL, all PostScript code sent to the Window Server, and all values returned from the PostScript interpreter to the application, will be written (in ASCII rather than binary form) to the standard error stream. |
| `NXShowAllWindows` | If not NULL, all windows will be placed on-screen, including those that normally stay off-screen. |

### Workspace Manager Arguments

The arguments passed by the Workspace Manager are read and stripped from `NXArgv` when the Application class is initialized. They define these public parameters:

| Parameter | Value |
|---|---|
| `NXOpen` | The full pathname of the file the application is to open. |
| `NXOpenTemp` | The full pathname of a temporary file the application is to open. Temporary files should be deleted before the application quits. |
| `NXHost` | The name of the host machine. The Workspace Manager provides this information only when it's run remotely. |
| `NXAutoLaunch` | A value other than 0 if the application was launched by the Workspace Manager at login. |

When an application is launched by the Workspace Manager, `NXArgv[0]` is guaranteed to contain the full pathname of the application's executable file. By stripping the last element from the pathname, an application can obtain the path for the file package where auxiliary files essential to the application are located.

---

## Segments

The Mach object file format allows you to create segments in the executable file and store whatever information you need there. The software kits take advantage of this facility to keep various kinds of information—archived objects, sound files, icons, and other images—in close association with the code that requires it.
