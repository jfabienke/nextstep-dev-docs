# 7. Program Dynamics

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

> Changes made for the current release of NeXTstep affect the information presented in this chapter. For details see:
> - `/NextLibrary/Documentation/NextDev/ReleaseNotes/AppKit.rtf`
> - `/NextLibrary/Documentation/NextDev/ReleaseNotes/AllocInitAndNew.rtf`

## Overview

The previous chapter, "Program Structure," described how a program based on the Application Kit is constructed from a small number of interconnected objects. This chapter shows how applications use that structure to carry out essential interactive activities:

- Responding to events
- Drawing on the screen (and on the printed page)
- Sending and receiving remote messages

Because events motivate almost all program activities, the chapter begins with a discussion of event handling in the Application Kit.

---

## Event Handling

NeXT applications are driven by the user's actions on the keyboard and mouse—that is, by events. The application receives an event from the Window Server, responds to it, then looks for the next event. If there is no next event, the application waits until the user does something and an event is received.

### The Main Event Loop

The Application object, NXApp, initiates the main event loop when it receives a run message:

```c
[NXApp run];
```

On each cycle of the loop, NXApp gets an event, analyzes it, and sends an appropriate message to initiate the application's response. It passes keyboard events to the key window and mouse events to the Window associated with the event in the event record. The Window, in turn, dispatches the event to one of its Views. Most of your program's activity will be in reaction to the messages NXApp and the application's Windows send out after receiving an event.

- Some of their event messages will reach objects that can respond directly.
- Some event messages will reach Control objects—Buttons, Sliders, Scrollers, TextFields, and the like—and will be translated into more specific action messages for other objects.

NXApp continues to get events out of the event queue and dispatch them until the event loop is broken. Typically, it's broken only when the application terminates. If the response to an event includes a `terminate:` message:

```c
[NXApp terminate:self];
```

NXApp closes all the application's windows, frees its objects, and exits the program.

**Note:** The `terminate:` method takes an argument only so that it can respond to an action message, usually an action message coming from the Quit command. It doesn't actually look at its argument, so it doesn't matter what value is passed.

Sometimes an application's response to an event is to set up a modal event loop that will get all subsequent events for a short period of time. For example, the response to a mouse-down event may be to set up a modal loop that will collect events until the user releases the mouse button and a mouse-up event is received. Modal loops are set up within the main event loop in response to an event or action message.

Most of the unique behavior of your application will be encoded in class definitions for objects that can respond to event and action messages.

---

## Setting Up Event-Handling Objects

An Objective-C program begins just as a C program does, by calling its `main()` function. In a program based on the Application Kit, `main()` is usually very short. Its job is to set up the Application object and other core objects your program needs at startup, then to turn over control of the program to them. It can be as short as just three or four lines of code:

```c
main()
{
    [Application new];
    setUp();
    [NXApp run];
    [NXApp free];
}
```

In this version of `main()`, the `new` method creates an Application object, NXApp, which receives a `run` message to begin getting events from the Window Server. When the `run` method quits, `free` cleans up after the application.

Most of the application's time is spent in the `run` method, getting and responding to events. But before a `run` message can be sent, the application must prepare itself for the events it's about to receive. It must:

- Create the Windows, Views, and other objects it needs to handle events at startup. It's possible to create new objects at any time while the program is running, but an initial set of core objects must be in place before the first event is processed.
- Initialize the objects so they're connected into a program framework.
- Present the application to the user by placing the initial display on-screen and designating a Window to serve as the initial key window. If the application allows users to edit text or graphics within the window, it should also designate a View to show the initial selection, and then set the selection.

### Using Interface Builder

In a typical `main()` shown above, all this code has been segregated into the `setUp()` function. For some applications, it might be appropriate to define a subclass of the Application class and include setup code in a redefined version of the `new` method:

```c
@implementation MyApplication : Application
{
}
+ new
{
    self = [super new];
    /* setup code goes here */
}
```

The `main()` function could then consist of just a single line of nested messages:

```c
main()
{
    [[[MyApplication new] run] free];
}
```

When you design your application using Interface Builder, you can do most of the work of setting up your application by selecting objects from palettes and editing them on-screen. Interface Builder lets you graphically lay out the Windows, Views, and other interface objects your application needs, initialize them, display them, and archive them in a file for later use. The file can then be inserted into the `__NIB` segment of the application executable:

```
cc ...  -segcreate __NIB myProject.nib myProject.nib
```

Here the `-segcreate` linker option copies the `myProject.nib` file into a section of the `__NIB` segment and assigns the section the same name as the file.

A message to NXApp to open this section takes the place of the `setUp()` function:

```c
main()
{
    [Application new];
    [NXApp loadNibSection:"myProject.nib" owner:NXApp];
    [NXApp run];
    [NXApp free];
}
```

In the example above, the objects that were archived in `myProject.nib` are loaded into memory and connected to the object that "owns" them, NXApp.

Archived objects can have owners other than NXApp:

```c
main()
{
    id  theHub;

    [Application new];
    theHub = [MyCoordinator new];
    [NXApp setDelegate:theHub];
    [NXApp loadNibSection:"newProject.nib" owner:theHub];
    [NXApp run];
    [NXApp free];
}
```

For this example, the programmer defined a class, `MyCoordinator`, to contain the basic algorithms of the application—its inner workings as opposed to its interface. After creating an Application object, `main()` creates an instance of the `MyCoordinator` class, makes it the delegate of the Application object, and connects it with the user-interface objects archived in `newProject.nib`. Whenever a Window object is loaded from the archive, it's automatically added to NXApp's list of windows; NXApp doesn't have to be named as the Window's owner.

---

## Event Masks

Each window has an event mask—a set of bits that determine which events the window will receive from the Window Server. By setting a window's event mask appropriately, you can limit the events your program must process to just those it needs to handle.

### Asking for Particular Events

An application should limit the events it receives from the Window Server to just those it's interested in. This saves processing time, reduces the amount of code you must write, and limits potential errors from handling unwanted events.

For most types of events, it's best to set the window's event mask at the outset so that it will receive all the events you ever want for it. It's generally not a good idea to change the mask in response to events, since the user might act between the time you send the message and the time the Window Server gets around to resetting the mask.

The principal exception to this rule arises when you want to receive mouse-dragged or mouse-moved events:

- Since these events are sent continuously, as long as the mouse is in motion, it doesn't matter that the Window Server won't begin sending them until it resets the event mask. Missing the first in a series of mouse-dragged or mouse-moved events is usually of little consequence.
- Since dispatching a continuous stream of events demands a lot of processing time, you should keep the mask for these two events set only for a limited period. Have your application set the Window's event mask to include mouse-dragged or mouse-moved events just before it's ready to respond to them and have it reset the mask when it's finished.

### Receiving Keyboard Events

The Application Kit guarantees that keyboard events are sent to the active application, regardless of whether its on-screen windows have event masks that accept keyboard events. This enables the application to respond to keyboard alternatives when it's active, even if it has no windows for the user to type in.

Although the event mask doesn't determine which application gets keyboard events, it does help determine which Window within the application will receive them:

- The Application Kit associates keyboard events with the current key window.
- For a Window to be the key window, it must have an event mask that accepts key-down events. The initial event mask for all Windows (except menus, miniwindows, and icons) includes key-down events, so, by default, they all are potential key windows.

Since the NeXT user interface requires every standard window to be the key window whenever it's the main window the user is working in, standard windows should keep key-down events in their event masks. If the window doesn't display typing, it will beep whenever it receives a key-down event.

However, if you have a panel that won't respond to keyboard events and shouldn't be marked as the key window, you must reset its mask to exclude key-down events:

```c
[myPanel removeFromEventMask:(NX_KEYDOWNMASK | NX_KEYUPMASK)];
```

A Panel should remove keyboard events from its event mask if it meets all three of these tests:

- It doesn't display typing.
- It's not an attention panel.
- It doesn't have a button that the user can operate from the keyboard by pressing Return.

### Receiving Timer and Cursor-Update Events

A Window's event mask never needs to include either timer events or cursor-update events.

Timer events are synthetic events, generated by the application when it needs them. Because they aren't sent across the connection from the Window Server, the event mask doesn't determine whether they can be received.

Cursor-update events signal that it's time to change the cursor image. Applications don't respond to these events directly; the change is made by the Application Kit.

---

## Selecting an Application, Window, and View

After the application's core objects have been set up, the event masks of its windows have been adjusted, and a `run` message sent, the Application object begins getting events from the Window Server. Most events are dispatched in messages to other objects.

Left mouse-down events also serve to select the application, the window, and even the View that will be the focus of future events. The selected object's event-handling status is designated by these terms:

- **Active application** - The application that's been selected to receive keyboard events, and to have visible menus and panels.
- **Key window** - The Window that's been selected to handle keyboard events for the application, and to be the primary recipient of action messages from menus and panels.
- **Main window** - The Window that's the principal focus of user actions. It's usually identical to the key window.
- **First responder** - The View that's been selected to have the first chance at responding to keyboard events and action messages sent to a Window.

### The First Responder

When the user clicks in a Text object, it's selected to receive subsequent events, especially keyboard events. The click also selects the insertion point where future typing will appear.

The object that's selected to be the focus of future events for a Window is the first responder. Each Window has its own first responder, which it returns when asked:

```c
id  handler;
handler = [myWindow firstResponder];
```

The first responder is typically a View object in the Window's view hierarchy, but it can be any Responder. At the outset, each Window is its own first responder. Because Windows generally can't respond to keyboard and mouse events, this is usually the same as having a nil first responder.

The first responder is a central actor in the handling of event and action messages. It receives:

- Keyboard event messages (`keyDown:`, `keyUp:`, and `flagsChanged:`).
- Action messages from Controls that don't have explicit targets of their own. This includes messages from menu commands that affect the current selection, such as the Cut, Copy, Paste, Bold, and Italic commands.
- Messages that notify the first responder when the Window becomes the key window and when it stops being the key window (`becomeKeyWindow` and `resignKeyWindow`).
- Mouse-moved event messages (`mouseMoved:`).

If the first responder can't respond to any of these messages, its next responder is given a chance to respond.

### Changing the First Responder

The Application Kit lets the user pick the first responder; the Window object alters its `firstResponder` instance variable on the basis of the left mouse-down events it receives.

Before making the View selected by a mouse-down event the first responder, the Window sends it an `acceptsFirstResponder` message to ask whether it accepts this role. By default, all Views—in fact, all Responders—answer NO, leaving the current first responder in place. An object can agree to be made the first responder simply by implementing an `acceptsFirstResponder` method that answers YES:

```c
- (BOOL)acceptsFirstResponder
{
    return YES;
}
```

Some objects may return YES under certain circumstances and NO under others. If a Text object displays editable or selectable text, it answers YES. If the text is neither editable nor selectable, it answers NO.

If the selected View returns YES, the Window attempts to make it the first responder through its `makeFirstResponder:` method:

```c
[self makeFirstResponder:selectedView];
```

To function as the initial first responder of a Window, a Text object needs not only to be made the first responder, it must also be assigned a selection. The Text class defines three methods that do both; they register a new selection and send the Window a `makeFirstResponder:` message:

- `selectAll:` - Selects all of the object's text
- `selectText:` - Does the same as selectAll:
- `setSel::` - Defines a range of text to select

```c
[windowText setSel:52 :191];
```

If the range of text selected is 0—if both arguments to `setSel::` are the same, or there's no text for `selectAll:` and `selectText:` to select—the selection is an insertion point.

### Notification

The `makeFirstResponder:` method first sends the current first responder a `resignFirstResponder` message to notify it that a change is about to be made; the new first responder is then notified with a `becomeFirstResponder` message. The default implementation for both these methods is simply to return `self`. An object can override the default to keep track of whether it's the current first responder, or to prevent the change from being made:

- If an object returns nil to a `resignFirstResponder` message, it refuses to be deactivated and remains the first responder. No `becomeFirstResponder` message is sent and `makeFirstResponder:` returns NULL.
- If an object returns nil to a `becomeFirstResponder` message, it refuses to be the new first responder. Since the current first responder has already resigned, the Window is made the first responder instead.

---

## Changing the Cursor

The cursor's appearance can be changed to provide feedback to the user about what actions are available in different areas of the window. The Window class maintains a set of cursor rectangles—rectangular areas within the window, each associated with a particular cursor image.

### Cursor Rectangles

A cursor rectangle associates a Cursor object with a particular rectangular area. When the cursor moves into a rectangle, it's automatically changed to the image associated with that rectangle. When it moves out of the rectangle back into an area that doesn't have a defined cursor, the cursor reverts to its default image.

Cursor rectangles are maintained by the Window in association with a View. When a cursor moves within a window, the Window checks whether the cursor is located within any of the rectangles registered for the Views and changes the cursor image accordingly. This is done automatically and requires no action from the application.

### When Cursor Rectangles Become Invalid

Cursor rectangles may become invalid when:

- A View that has cursor rectangles is moved, resized, or removed from the view hierarchy.
- The View's coordinate system is altered in some way.
- The Window's view hierarchy is rearranged.

For example, if a View is removed from the view hierarchy, some cursor rectangles might need to be removed with it. Scrolling a View that contains a cursor rectangle could relocate the rectangle. If the rectangle is scrolled completely out of view, it should be removed from the Window's record of cursor rectangles. If it's partly in view and partly out, it should be updated to reflect only the part that's visible.

It's therefore not enough for a View to register its tracking rectangles just once. It must be prepared to reregister them whenever the need arises.

### Registering Cursor Rectangles

Before getting an event in the main event loop (or in a modal loop for an attention panel), the Application Kit checks whether the key window's cursor rectangles remain valid. If any rectangles are invalid, they're discarded and `resetCursorRects` messages are initiated to reregister the correct rectangles.

Each View should define its own version of the `resetCursorRects` method. A `resetCursorRects` method should contain one or more `addCursorRect:cursor:` messages to register the View's cursor rectangles with the Window:

```c
- resetCursorRects
{
    [self addCursorRect:&bounds cursor:myImage];
    return self;
}
```

The Matrix, Form, and TextField classes use this mechanism to display an I-beam cursor over editable text. You don't have to register these cursor rectangles yourself.

The `removeCursorRect:cursor:` method removes a cursor rectangle from the Window's record and `discardCursorRects` removes all the cursor rectangles registered for the View:

```c
[myView removeCursorRect:&bounds cursor:NXIBeam];
[otherView discardCursorRects];
```

### Wait Cursors

The wait cursor informs users that the active application is busy and will therefore be unresponsive. Its disappearance lets users know that the application is again ready to receive events.

When the wait cursor is removed from the screen, the cursor should revert to whatever image is appropriate for its location and the current state of the application. This is done automatically when a `push` message is used to set the wait cursor image and a `pop` message is used to remove it. These messages should bracket the code that the user must wait for:

```c
[NXWait push];
/* code for a task that may take some time to execute */
[NXWait pop];
```

The wait cursor should remain on-screen for comparatively short periods of time. To inform users that the application will be busy for an extended period of time, don't rely on the wait cursor, but use an attention panel instead.

---

## Modal Event Loops

Part of an object's response to an event can be to get another event directly from the Application object:

```c
NXEvent  *myEvent;
myEvent = [NXApp getNextEvent:myMask];
```

This procedure short-circuits the main event loop, enabling an object to retrieve and respond to an expected event more quickly. When breaking into the main event loop, objects generally set up their own temporary, modal event loops for short periods of time.

`getNextEvent:` takes an argument, an event mask that limits the types of events it will return. The argument doesn't affect the events sent to the application by the Window Server—that is, it doesn't change any window's event mask. If the next event in the queue isn't one you want `getNextEvent:` to return, it skips over the event and continues checking until it finds one that matches the mask.

### Setting Up Modal Event Loops

Setting up a modal event loop should be limited to cases where you can be fairly certain that the next event belongs to a limited set. You'd most often do it in response to a `mouseDown:`, `keyDown:`, or `flagsChanged:` message:

- When a View receives a mouse-down event, a mouse-dragged or mouse-up event is likely to be the next event of interest. A mouse-up event breaks the loop, so the mode lasts only while the mouse button is held down.
- When a View receives a key-down event and there's reason to believe the user has started typing, a key-up event or another key-down event is likely to follow.
- When the application receives a flags-changed event indicating that the user has pressed a modifier key, it can enter a mode until another flags-changed event indicates that the key has been released.

Since the Text, TextField, and Form objects can handle the typing needs of most applications, you generally won't need to set up a modal loop to handle keyboard events.

### Coordinating Mouse Events

Mouse-down and mouse-up events often need to be coordinated:

- Views shouldn't normally respond to a mouse-up event unless they've also received the mouse-down event that preceded it.
- Views receiving a mouse-down event should generally wait until the mouse-up event before committing themselves to an irreversible action. Users are allowed to change their minds after pressing a mouse button. If they move the cursor from the View before releasing the button, the View shouldn't respond.

Coordinating mouse-down and mouse-up events is easier if the mouse-up event is received as part of the response to the mouse-down event.

### Mouse Event Example

The following example shows how a `mouseDown:` method can set up an event loop for mouse-dragged and mouse-up events:

```c
- mouseDown:(NXEvent *)thisEvent
{
    register int   inside;
    int            shouldLoop = YES;
    int            oldMask;
    NXEvent       *nextEvent;

    [self doMyOwnHighlight];
    oldMask = [window addToEventMask:NX_LMOUSEDRAGGEDMASK];

    while (shouldLoop) {
        nextEvent = [NXApp getNextEvent:(NX_LMOUSEUPMASK |
                                          NX_LMOUSEDRAGGEDMASK)];

        inside = NXMouseInRect([self convertPoint:
                                 &nextEvent->location
                                 fromView:nil],
                               &bounds,
                               [self isFlipped]);

        switch (nextEvent->type) {
            case NX_LMOUSEUP:
                shouldLoop = NO;
                if ( inside ) {
                    [self doMyOwnHighlight];
                    [self doMyOwnThing];
                }
                [self doMyOwnUnhighlight];
                break;
            case NX_LMOUSEDRAGGED:
                if ( inside )
                    [self doMyOwnHighlight];
                else
                    [self doMyOwnUnhighlight];
                break;
            default:
                break;
        }
    }
    [window setEventMask:oldMask];
    return(self);
}
```

This method first declares its local variables and highlights the View in response to the mouse-down event. It then resets the window's event mask to include left mouse-dragged events. Before changing the mask, it stores the window's old event mask in a local variable so that it can be reset later.

Next, the method enters an event loop, looking only for mouse-up and mouse-dragged events. As it receives each event, the location of the mouse is translated to the View's own coordinates and is tested against its bounds rectangle to see whether or not it lies inside the View.

The loop continues until the mouse button is released. If it's released while the mouse is pointing to the View, the View performs its `doMyOwnThing` method. In any case, the loop exits, leaving the Application object's event loop as the only one operating.

After the modal loop exits, the `mouseDown:` method resets the event mask to make sure that mouse-dragged events will no longer be sent to the Window.

---

## Getting and Peeking at Events

There are five methods that you can use to read events from the event queue. All five take an event mask specifying which events they're to return, and all five skip over any events in the queue that don't match the mask.

### Getting Events from the Queue

Some of the methods "get" the event by copying it to NXApp's `currentEvent` instance variable and removing it from the queue. `getNextEvent:` is one of those methods:

```c
NXEvent  *eventPtr;
eventPtr = [NXApp getNextEvent:(NX_KEYDOWNMASK | NX_KEYUPMASK)];
```

### Peeking at Events

Other methods simply "peek" at the event. They leave it in the queue but copy it to memory provided by the application:

```c
NXEvent  thisEvent, *eventPtr;
eventPtr = [NXApp peekNextEvent:NX_ALLEVENTS into:&thisEvent];
```

Peeking at an event is appropriate, for example, in modal event loops that handle key-down and key-up events. Since any other event type breaks the loop, the procedure that gets keyboard events must first peek at the next event to make sure it is, in fact, a keyboard event.

### Event Queue Methods

All five methods that read events from the queue return a pointer to the event. The pointer returned by `getNextEvent:` is the same one that `currentEvent` returns:

```c
eventPtr = [NXApp currentEvent];
```

These methods differ in one other respect: If there is no event in the queue (at least none matching the mask passed to the method), `getNextEvent:` waits for one, but `peekNextEvent:into:` doesn't wait. It immediately returns a NULL pointer.

A third method, `peekAndGetNextEvent:`, gets events like `getNextEvent:`, but, like `peekNextEvent:into:`, returns immediately if there's no matching event in the queue:

```c
eventPtr = [NXApp peekAndGetNextEvent:NX_ALLEVENTS];
```

There are also "get" and "peek" methods that let you specify how long they should wait for the next event:

```c
NXEvent  thisEvent, *eventPtr;
eventPtr = [NXApp getNextEvent:NX_APPDEFINEDMASK
                      waitFor:10.0
                    threshold:NX_BASETHRESHOLD];
eventPtr = [NXApp peekNextEvent:NX_ALLEVENTS
                           into:&thisEvent
                       waitFor:10.0
                    threshold:NX_BASETHRESHOLD];
```

The time is specified in seconds. The last argument to both of these methods is a priority threshold.

---

## Scheduling

While your application is responding to an event, it can't be doing anything else. Between events, however, it can turn to other tasks. It could:

- Execute a timed entry, if one is due.
- Respond to a message from another application, if one has been received.
- Read data from a file descriptor, if there's any data to read.

Whenever the application is ready to get (or peek at) another event, it can call timed entries, procedures to handle messages received at a port, and procedures to read data received at a file descriptor.

### Priority Levels and Thresholds

Whether or not a procedure is scheduled depends largely on its priority level. The priority is an integer between 0 and 30, with 30 as the highest possible priority and 0 as the lowest.

Whenever an application gets (or peeks at) the next event, it specifies a priority threshold. Procedures with priorities lower than the threshold are screened out; all those with equal or higher priorities are checked at least once to see whether they should be called before the "get" or "peek" method returns.

The Application Kit makes use of three priority thresholds and defines a constant for each:

| Threshold | Constant |
|---|---|
| 10 | `NX_MODALRESPTHRESHOLD` |
| 5 | `NX_RUNMODALTHRESHOLD` |
| 1 | `NX_BASETHRESHOLD` |

The main event loop gets events at the lower priority threshold of 1; it's very permissive about what procedures can be called between events. But when a Control, Text object, or other Responder sets up a modal event loop to get its own events, it gets (or peeks at) them at the higher priority threshold of 10.

---

## Using Timer Events

A modal event loop sometimes needs to react even when no event has been received from the Window Server. The absence of an event may indicate that the user is still in the midst of an action, one that generated an earlier event but hasn't produced any new ones. Typically, this occurs while the user is keeping the mouse stationary and holding a mouse button down in order to:

- Press an object (such as a button) that has a repeating or continuous action.
- Automatically scroll the contents of a View after dragging outside it.

Because the mouse isn't moving and the mouse button isn't going up or down, these actions don't generate events. Nevertheless, the modal event loop must continue to respond to the action as if events were being received.

### Generating Timer Events

The `NXBeginTimer()` function starts up a timed entry that puts timer events in the event queue at specified intervals:

```c
NXTrackingTimer  myTimer;
NXBeginTimer(&myTimer, 0.1, 0.05);
```

The first argument to `NXBeginTimer()` is a pointer to an `NXTrackingTimer` structure. This structure is for the internal use of the timed entry; you don't have to initialize it. If you pass a NULL pointer, memory will be allocated for the structure.

`NXEndTimer()` stops the flow of timer events:

```c
NXEndTimer(&myTimer);
```

`NXBeginTimer()` returns a pointer to the `NXTrackingTimer` structure it uses, so even if you pass it a NULL pointer, you'll have access to the pointer required by `NXEndTimer()`.

---

## Modal Windows

Sometimes an application needs to set up a modal event loop at the Window level, one step removed from the objects that are actually responding to the events. This can be done through the `runModalFor:` method:

```c
[NXApp runModalFor:myWindow];
```

The Window, myWindow, should be an attention panel, and it must have an event mask that accepts key-down events (so that it can become the key window).

The `runModalFor:` method puts the panel on-screen, in front of even the main menu, and makes it the key window. It then sets up an event loop that filters the events the Application object receives from the Window Server. It distributes mouse events only if they're associated with the panel; other mouse events are removed from the queue and don't generate event messages. This means that the user can't use the mouse to select any other window in the application (though it can be used to move windows and activate another application).

In this way the panel can command the user's attention until some condition is met—usually the user clicking on one of the panel's buttons.

### Breaking the Modal Loop

The modal loop that `runModalFor:` begins can be terminated by sending the Application object a `stopModal` message:

```c
[NXApp stopModal];
```

It can also be terminated with an `abortModal` message:

```c
[NXApp abortModal];
```

If the response to an event includes a `stopModal` message, the Window's event loop will be broken. The `runModalFor:` method won't attempt to get another event, but it will finish responding to the current one. In contrast, an `abortModal:` message breaks the event loop immediately. Without returning, it raises an exception that causes the `runModalFor:` method to return at once.

Use `abortModal` rather than `stopModal` to break the loop from within code that executes between events—such as a timed entry or a method that responds to a remote message.

### Return Codes

It's often necessary to know why a Window's modal loop has ended. By using a `stopModal:` method (with a colon), you can pass a return code that identifies the reason why the modal loop is being terminated:

```c
[NXApp stopModal:2];
```

This integer is passed to and returned by the `runModalFor:` method:

```c
int  why;
why = [NXApp runModalFor:myWindow];
```

When the modal loop is terminated by an `abortModal` message, `runModalFor:` returns an integer identified by the `NX_RUNABORTED` constant. When `stopModal` (without a colon) terminates the loop, `runModalFor:` returns `NX_RUNSTOPPED`.

Only the `stopModal:` method (with a colon) permits you to specify your own return codes. The code should be an integer above -1000; integers less than -999 are reserved by NeXT.

---

## Modal Sessions

On occasion, applications need to carry out time-consuming operations that are not responsive to events—an extensive calculation, perhaps, or moving a large amount of data from one location to another. From the point of view of the user interface, these operations can be divided into three categories:

1. Those that prevent the user from doing anything else, but that might be interrupted, aborted, or otherwise controlled by the user.
2. Those that prevent the user from doing anything else and cannot be interrupted or aborted.
3. Those that shouldn't prevent the user from carrying out other tasks.

Operations in the third category should be placed in a separate Mach thread and performed in the background. Those in the first two categories require you to run an attention panel while carrying out the operation. For the first category, the panel would give the user some control over the operation—notably the ability to terminate it. For the second category, the panel would let the user know what was happening, but would offer no opportunity to abort it.

### Bracketing Operations in Sessions

The `runModalFor:` method can't run this type of panel. Since the modal event loop it sets up controls all the application's activities, it would be impossible to carry out the concurrent operation.

Instead, the application must carry out the operation within a modal session bracketed by `beginModalSession:for:` and `endModalSession:` messages to the Application object. While in the session, a modal event loop—similar to the one set up by `runModalFor:`—is run repeatedly for short periods of time. The rest of the time is available for the concurrent operation.

A modal session is identified by an `NXModalSession` structure. A pointer to the structure is passed as an argument to all three methods mentioned above. The structure isn't one that you need to initialize. If you pass a null pointer to `beginModalSession:for:`, it will create the structure for you and return a pointer that you can pass to `runModalSession:` and `endModalSession:`.

It's more efficient to declare the structure as a local variable and avoid the necessity of allocating memory for it.

---

## Acknowledgments

This chapter covers the fundamental mechanisms of the NeXT Application Kit for handling events, managing windows, and coordinating user interaction. Through these mechanisms, your application can respond to user actions, provide visual feedback, and maintain responsive modal interactions even during long-running operations.

For more detailed information on specific methods and classes, consult the NeXTstep Reference manuals and the Interface Builder documentation.
