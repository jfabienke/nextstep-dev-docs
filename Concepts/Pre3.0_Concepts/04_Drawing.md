# 4. Drawing

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

> Changes made for the current release of NeXTstep affect the information presented in this chapter. For details see:
> - `/NextLibrary/Documentation/NextDev/ReleaseNotes/WindowServer.rtf`
> - `/NextLibrary/Documentation/NextDev/ReleaseNotes/AppKit.rtf`

## Overview

All of your program's visual output, whether sent to the laser printer or displayed on the screen, is generated through the PostScript language, a high-level, interpreted language with special facilities for drawing and handling graphic images, including text. The PostScript language has typically been used to produce high-quality images on the printed page. On the NeXT computer, it's also used for imaging on the screen. NeXT and Adobe Systems Incorporated have jointly developed the Display PostScript system, which refines the language and adapts it to the interactive requirements of the screen.

Because the NeXT computer creates both screen and printed images through the same language, the screen can display a document just as it will be printed. Except for differences in resolution, what you see is what you get.

Applications draw within windows—screen rectangles that can be moved, resized, and layered on top of each other—rather than directly on the screen. The NeXT window system is implemented as an extension to the Display PostScript system. At the most fundamental level, applications create and manage windows through PostScript operators. However, the Application Kit provides an object-oriented interface to the window system that most applications should use; the Kit handles basic window-management tasks for you.

NeXT has made other extensions to the Display PostScript system so that applications can better use the capabilities of the window system and MegaPixel Display. The principal additions permit compositing, drawing with partially transparent paint, and instance drawing:

- **Compositing** builds a new image by overlaying images that were previously drawn. It's analogous to a photographer printing a picture from two negatives, one placed on top of the other. However, on the NeXT computer there are even more ways that each of the images you start with can contribute to the composite. All image-transferring operations, including simply copying an image from one location to another, are accomplished through compositing. Compositing is the only way to copy on the NeXT computer.

- **Compositing with Transparency**: Compositing can achieve interesting effects when the initial images are drawn with partially transparent paint. In a typical compositing operation, paint that's partially transparent won't completely cover the image it's placed on top of; some of the other image will show through. The more transparent the paint is, the more of the other image you'll see. You can set the coverage of the paint you draw with—how transparent or opaque it is—just as you set its color or gray level.

- **Instance Drawing** puts temporary images on the screen, and removes them again, at little processing cost. When instance drawing is removed, the original images are automatically restored. Instance drawing is useful for temporarily highlighting an image, for showing an object being dragged from one location to another, and for other kinds of dynamic drawing.

All operators added to the Display PostScript system by NeXT are described in detail in the NeXTstep Reference, Volume 2. The Display PostScript system is documented in four publications by Adobe Systems Incorporated:
- PostScript Language Extensions for the Display PostScript System
- Client Library Reference Manual
- pswrap Reference Manual
- Perspective for Software Developers

The PostScript language itself is described in the PostScript Language Reference Manual by Adobe Systems Incorporated, published by Addison-Wesley. A beginning guide to the language and a series of example graphics programs are provided in the PostScript Language Tutorial and Cookbook, by the same author and publisher.

This chapter discusses drawing on the NeXT computer using the Display PostScript system and its extensions. In addition to transparency, compositing, and instance drawing, it describes how drawing works in the NeXT window system and on the MegaPixel Display.

Later chapters—Chapters 6 ("Program Structure"), 7 ("Program Dynamics"), and 9 ("User-Interface Objects")—discuss drawing in the broader context of the Application Kit. The Kit defines objects that draw the graphic elements of the NeXT user interface, including scrollers, switches, sliders, menus, and user-editable text; you don't need to write this PostScript code yourself. You can therefore focus your attention on drawing that's unique to your application. The drawing code you write will be integrated into the object-oriented program structure provided by the Kit, and will use the Display PostScript system and the extensions to it discussed in this chapter.

---

## Design Philosophy

When one imaging model is used for the printer and another for the screen, application programs must pursue two parallel lines of development. In addition to the extra work, it's nearly impossible to make screen and printed images match.

The NeXT computer avoids these problems by using a single imaging model, the PostScript language, for all drawing. The PostScript language is well-suited to this role because:

- It's device independent
- It's programmable
- It provides a complete two-dimensional imaging model
- It's a widely used standard on printers

Extensions to the PostScript language are the foundation for the NeXT window system. Drawing instructions and window management operations are both sent to the same interpreter. Because of this integration, you'll have fewer issues to worry about as you program your application.

Drawing on the screen has dynamic aspects that are missing when drawing for the printer:

- **Interactive**: Applications must follow the user's instructions and respond graphically to the user's actions
- **Changes over time**: Applications can repeatedly reuse the same area of a window by erasing and replacing what's displayed there
- **Multiple windows**: Instead of drawing one page at a time, applications present the user with a multi-windowed interface

These aspects present applications with both challenges and opportunities. One challenge is speed. To meet it, the Display PostScript system has been refined to respond quickly, without losing any of the original generality and power of the PostScript language. In a few cases, operators have been added so that common operations can be executed more efficiently.

Another challenge is programming simplicity. This challenge is met mainly by relieving applications of bothersome chores:

- Much of your program's drawing can be done through Application Kit objects that have the ability to draw themselves. Common drawing operations such as scrolling, resizing, clipping, and erasing are also handled through Application Kit objects. In addition, the Kit has facilities that make it relatively easy to use bitmaps and icons.

- The Window Server handles the dynamic behavior of windows. It moves and resizes them in response to user actions, without your program's intervention.

- The Window Server provides windows with backup buffers so that it can automatically save images when a window is hidden, and automatically restore them again when the window becomes visible once more.

Buffering also makes it possible to draw into windows that never appear on-screen. The images that are cached in off-screen windows can then be copied to windows on-screen, using the same operators that transfer images between on-screen windows.

NeXT has adopted an advanced model for moving and combining images. Compositing and transparency make it possible for programs to explore new visual effects, such as slowly dissolving one image into another and building a final image out of several layers. With window buffering, they make animation fairly simple. NeXT compositing is fully compatible with color graphics, so you won't have to redesign your program for a color screen. Compositing is implemented as an extension to the PostScript language, but it's also possible to composite in Objective-C code using the Bitmap object of the Application Kit.

To help applications provide immediate feedback to users, NeXT has also augmented the PostScript language with instance drawing. Images that are drawn in this mode are temporary; when they're removed, the original image reappears.

---

## The Screen

The first part of this section briefly covers some background terminology. The parts that follow introduce drawing on the NeXT MegaPixel Display.

### Pixels, Halftones, and Rectangular Coordinates

Images on the screen are formed from tiny picture elements, or pixels, arranged in a rectangular grid of columns and rows that fill the entire surface of the screen. Because of their positions in the grid, pixels can be thought of as little square dots. Each pixel has a separate representation in screen memory and can be assigned an independent color value. Varying the color of the individual pixels makes it possible to render an almost unlimited variety of images on the screen.

Typically, a pixel on a monochrome computer screen is capable of showing just two colors: either black or white. A black line is rendered by turning all the pixels along its path black; a white circle has all the pixels within its radius turned white. Intermediate shades of gray are rendered by halftoning, turning some pixels black and others white. A mixed pattern of adjacent black and white pixels appears as a solid, uniformly colored gray area.

Locations on the screen are described using a standard rectangular (Cartesian) coordinate system. Points are identified by their position relative to a horizontal x-axis and a vertical y-axis. The origin, where the two axes meet, is (0, 0). A point 500 pixels to the right of the origin as measured along the x-axis and 200 pixels above the origin as measured along the y-axis is (500, 200). Negative coordinates locate points below the x-axis and to the left of the y-axis.

The resolution of an image is a function of pixel density. The more pixels there are in a given area (that is, the smaller they are), the more detailed and precise the image can be. Computer screens have a lower resolution (fewer pixels per inch) than do laser printers.

### The NeXT Computer Screen

The visible area on the MegaPixel Display screen holds nearly a million pixels, 1120 along each horizontal row and 832 in each vertical column. There are about 92 pixels per inch in each direction (91.80 per inch along the x-axis and 92.44 along the y-axis when the display is exactly 12.2 inches by 9 inches, but these dimensions may vary by 0.100 inch).

Pixels on the NeXT monochrome screen aren't limited to just black and white; each one can also display two shades of gray, for a total of four discrete colors:

- White
- Light gray
- Dark gray
- Black

This reduces the need for halftoning while increasing the number of halftone shades. Where halftones are used, there's less variation in the color of adjacent pixels, so the screen can show purer, less granular shades of gray. Transitions from one shade of gray to another in continuous-tone images are smoother. Contrasts between a shade of gray and either black or white are sharper, since the gray can be rendered with fewer, if any, black or white pixels.

Each pixel requires two bits of memory to distinguish the four colors. When needed, two additional bits are set aside to store the coverage of the pixel—how transparent or opaque it is. Halftoning produces intermediate "shades" of coverage, just as it produces intermediate shades of gray.

To avoid halftoning on the MegaPixel Display, you can set the gray level to a value that results in pure black, pure white, or a pure gray. These four C constants are defined to the required values:

- `NX_WHITE`
- `NX_LTGRAY`
- `NX_DKGRAY`
- `NX_BLACK`

In the PostScript language, values specified to three decimal places work:

| Value | Gray Level |
|-------|-----------|
| 1.0 | white |
| 0.667 | light gray |
| 0.333 | dark gray |
| 0.0 | black |

### Screen Coordinates

The entire screen can be thought of as the first (upper right) quadrant of a two-dimensional coordinate grid, with the origin in the lower left corner and the positive x-axis extending horizontally to the right and the positive y-axis extending vertically upward. A unit along either axis is equal to the distance across one pixel, approximately 1/92 inch.

Coordinates are specified as floating-point numbers. This permits you to move anywhere on the screen, not just from pixel to pixel. Coordinates that are integers—those that have no fractional part—lie between pixels in the screen coordinate system; fractional coordinates locate points somewhere within a pixel.

The screen coordinate system has just one function: to position windows on the screen. When your application creates a new window, it must specify the window's initial size and location in screen coordinates. For drawing in a window, it will use a different coordinate system, one that's specific to the window.

No matter where a window is located, its edges are always aligned on pixel boundaries. If you try to position a window using fractional coordinates, they will be adjusted to whole integers. Fractional coordinates can be freely used when drawing within a window, however.

The screen coordinate system extends beyond the screen in all directions. However, its range is not unlimited. No screen coordinate (no point within a window) can have a value greater than 16000 or less than -16000:

```
(-16000 ≤ x ≤ 16000, -16000 ≤ y ≤ 16000)
```

All windows must fit within the space bounded by 16000.

### Window Coordinates

The reference coordinate system for a window is known as the base coordinate system. It differs from the screen coordinate system in only two ways:

1. It applies only to a particular window; each window has its own base coordinate system
2. Its origin is at the lower left corner of the window, rather than the lower left corner of the screen. If the window moves, the origin and the entire coordinate system move with it. An image retains its position within a window's coordinate system no matter where the window is located

In PostScript language terms, the base coordinate system is the equivalent to a "default user space" for the window. It provides a starting place for programs that draw within a window, just as the default user space provides a starting place for programs that describe pages for a printer.

> **Note**: You can also establish a reference coordinate system with the same coordinate units as the default user space, 1/72 inch. However, this documentation assumes the base coordinate system for all windows. The base coordinate system is more convenient for drawing on the screen, since coordinate units fall on pixel boundaries. It's also the initial coordinate system established for all windows created through the Application Kit.

### Modifying the Coordinate System

You're not limited to the base coordinate system when you draw within a window. PostScript operators can radically transform the window's coordinates. The origin can be moved (by the `translate` operator), the x- and y-axes can be turned to point in any direction (by the `rotate` operator), and the size of a unit along either the x- or y-axis can be expanded or shrunk (by the `scale` operator). The base coordinate system is simply the reference system for any subsequent transformations by PostScript operators.

PostScript transformations apply only within a window. They don't alter the screen coordinate system, and therefore can't affect the size or location of the window itself.

When you draw, coordinates are expressed in the application's current coordinate system, the system reflecting the last coordinate transformations to have taken place within the current window. The PostScript interpreter keeps track of this system through the current transformation matrix (CTM) of the current graphics state.

### View Coordinates

Drawing is usually limited to areas that are smaller than the whole window. The Application Kit permits you to set up rectangular regions within a window and to move and resize them, much as windows themselves are moved and resized on the screen. The Kit provides each region—or "View," after the Objective-C class that defines them—with a coordinate system, which it makes the application's current coordinate system before you draw. This coordinate system is a transformation of the base coordinate system and has a more convenient origin, at the lower left corner of the area you're drawing in.

You can use Application Kit methods or PostScript operators, or both, to further modify these default coordinates.

### Borders and Content Areas

The initial size of a window is determined by parameters that specify its width (along the x-axis) and its height (along the y-axis) in the screen coordinate system. These two parameters define the window's initial content area, the area that's available for drawing. The window's initial location is set by parameters that specify the lower left corner of the content area.

Windows typically have a title bar above the content area and a border around both the content area and title bar. Often they also have a resize bar below the content area and inside the border. (Some windows may have a border but lack a title bar and resize bar; some lack even the border.)

The window border, title bar, and resize bar lie outside the requested window area. If you ask the Application Kit to create a titled window 100 pixels wide and 100 pixels high, the window border and title bar will surround the 100-by-100 square you asked for. The point where you locate the window will correspond to the lower left corner of the square, inside the border. The border, title bar, and content area all lie inside the window, and share the window's coordinate system.

### Printing Coordinates

When a screen image is printed by the NeXT 400 dpi Laser Printer, or by any other PostScript printer, one unit within the base or screen coordinate system (that is, one pixel) translates to 1/72 inch, equal to about one typographical point. Since a screen pixel is smaller, approximately 1/92 inch, the screen shows images at just over three-fourths their printed size. This reduction permits a full 8 1/2-by-11-inch page to be shown on the screen from top to bottom, without scaling the window's coordinate system.

This doesn't mean that an image must always be shown on the screen at three-fourths its printed size. It's possible, for example, to use the PostScript `scale` operator to "magnify" the image on the screen, then print the image without the scaled magnification. The screen version could actually be larger than the unmagnified printed version.

Laser printers produce images with many more pixels per inch than do screen displays. The NeXT laser printer prints either 300 or 400 pixels per inch, depending on the setting. Printed images, therefore, have a higher resolution and are portrayed in more detail than screen images.

For example, a vertical line with a specified width of one unit within the base coordinate system can be shown on the screen as a thin rectangle just one pixel wide. When the line is printed, it will look almost 28% larger and have a width of at least four printer pixels. Setting the width of this line to less than 1—to 0.25, for example—will have no effect on the dimensions of the line on-screen, but will make the line thinner when it's printed.

---

## Rectangles

It's often necessary to limit a PostScript operation, or drawing in general, to a particular area of the screen. These areas are most often specified as rectangles:

- A window is a rectangle in the screen coordinate system
- Several PostScript operators, such as the compositing operators, act on rectangles within windows
- The Application Kit uses rectangles extensively to designate particular areas—called "Views"—where drawing can occur inside windows

On the NeXT computer, a rectangle is a combination of a point and an extent. The point locates the rectangle by assigning it x and y values within a coordinate system. The extent specifies the size of the rectangle—its width as measured in a direction parallel to the x-axis and its height as measured in a direction parallel to the y-axis. The width and height are measured from the point that locates the rectangle.

PostScript operators take four parameters to specify a rectangle. An example is `rectfill`, which fills a rectangle with the current color:

```postscript
x y width height rectfill
```

In the Application Kit, rectangles are specified as C structures of type `NXRect`. The NeXT header file `event.h` (in the `dpsclient` subdirectory of `/usr/include`) defines the elements that make up an `NXRect` structure:

```c
typedef float   NXCoord;

typedef struct _NXPoint {
    NXCoord     x;
    NXCoord     y;
} NXPoint;

typedef struct _NXSize {
    NXCoord     width;
    NXCoord     height;
} NXSize;

typedef struct _NXRect {
    NXPoint     origin;
    NXSize      size;
} NXRect;
```

The values in an `NXSize` structure should never be negative; for the Application Kit, the width and height of a rectangle must be positive (or 0.0). This means that the point that locates a rectangle (origin) will have the smallest coordinate values of any point in the rectangle. The extent of a rectangle is measured in positive directions from its origin.

### Rectangle Geometry

From a pair of rectangles within the same coordinate system, it's possible to calculate a third rectangle, their union, the smallest rectangle that completely encloses them both.

It's often also possible to calculate a fourth rectangle, their intersection. Two rectangles are said to intersect if they have any area in common. Since rectangles within the same coordinate system have parallel sides, this area will also be a rectangle.

When passed pointers to two `NXRect` structures, `NXIntersectRect()` returns YES if the two rectangles intersect and NO if they don't.

```c
BOOL  overlap;
overlap = NXIntersectRect(&rect1, &rect2);
```

If the two rectangles intersect, you can use `NXIntersectionRect()` to calculate the area that overlaps. It will place the intersection in the structure specified by its second argument and return a pointer to the same structure.

```c
NXRect  *smallrect;
smallrect = NXIntersectionRect(&rect1, &rect2);
```

If the two rectangles don't overlap, `NXIntersectionRect()` returns a NULL pointer and doesn't modify either of the rectangles it's passed.

`NXUnionRect()` is similar to `NXIntersectionRect()`. It places the union of the two rectangles in the structure specified by its second argument and returns a pointer to the structure.

```c
NXRect  *bigrect;
bigrect = NXUnionRect(&rect1, &rect2);
```

`NXUnionRect()` and `NXIntersectionRect()` perform two of the most common calculations on rectangles. But there are also a number of other functions that modify `NXRect` structures in prescribed ways:

- `NXSetRect()` initializes an `NXRect` structure
- `NXIntegralRect()` alters a given rectangle so that it has no fractional coordinates. If the coordinate system hasn't been scaled or rotated, it guarantees that the sides of the rectangle will lie on pixel boundaries
- `NXDivideRect()` slices a rectangle in two. You can specify the size of the slice and the side from which it should be taken
- `NXInsetRect()` calculates a rectangle that's inset from the one given
- `NXOffsetRect()` moves a rectangle by specified offsets along the x- and y-axes

Another function, `NXPointInRect()`, is used to determine whether a particular point (such as the location of the cursor) lies within a particular rectangle.

```c
BOOL  inside;
inside = NXPointInRect(&point, &rect);
```

It returns YES if the point touches the rectangle, and NO if it doesn't.

All these functions are defined in the Application Kit. They're described in more detail in the NeXTstep Reference manuals.

### When a Rectangle Isn't Rectangular

The sides of a rectangle are always parallel to the x- and y-axes of its coordinate system. Since the coordinate axes are typically perpendicular to each other, the corners of a rectangle are, as expected, 90° angles.

It's possible, however, to modify the PostScript coordinate system so that the axes intersect at an angle other than 90°. A rectangle specified within such a coordinate system won't appear to be a rectangle at all. It will have the shape of a nonrectangular parallelogram; since its sides are parallel to the coordinate axes, they won't meet to form 90° corners.

The most straightforward way to produce a skewed coordinate system is with the following sequence of two PostScript transformations:

1. Scale the coordinate system unequally
2. Rotate the coordinate axes into the scaled coordinates

For example, the `rectfill` operator at (40.0, 40.0) with a width and height both equal to 100.0, when preceded by these transformations, produces a skewed shape:

```postscript
2.0 1.0 scale
30.0 rotate
40.0 40.0 100.0 100.0 rectfill
```

In an unscaled, unrotated coordinate system, it would appear as an upright square. The `scale` operator stretched the length of a horizontal unit (measured along the x-axis) to twice the length of a vertical unit (measured along the y-axis). The `rotate` operator then altered the position of the x- and y-axes, but not the horizontal and vertical orientation of the scaling.

Although skewed coordinates turn rectangles into parallelograms, NeXT documentation refers to any area specified by a point and an extent as a "rectangle," no matter what shape it has. When viewed from within a skewed coordinate system, a nonrectangular rectangle still has 90° angles; it's nonrectangular only when viewed from the outside.

---

## The Window System

The NeXT window system is grounded in extensions to the Display PostScript system. The extensions create windows, move and resize them, order them from front to back, direct drawing to them, associate events with them, remove them from the screen, and carry out other low-level window management tasks.

The Application Kit builds on these operators and provides an object-oriented programming interface to them. An application should create the windows it needs through the Kit. The Kit defines Window objects to manage windows, link them to an event-handling mechanism, and oversee the drawing that's done in them. Creating a Window object through the Kit produces a window to draw in and provides a structure that integrates the window into the application.

### Window Numbers

Applications typically use a number of different Window objects and keep track of them through their object ids. The Window Server keeps track of windows by assigning each one a unique window number as an identifier. The window number is an int guaranteed to be greater than 0.

No two windows, even if they belong to different applications, will be assigned the same window number. However, when a window is destroyed, the Window Server may reuse its number.

### Contexts and Graphics States

The Window Server can serve a large number of client applications; its PostScript interpreter interprets PostScript code concurrently for all running applications.

For each connection that it has to an application, the Window Server maintains an independent PostScript execution context. Every context has its own set of stacks, including an operand stack, graphics state stack, and dictionary stack. There are three built-in dictionaries in the dictionary stack. From top to bottom, they are `userdict`, `nextdict`, and `systemdict`. `userdict` is private to the context, while `nextdict` and `systemdict` are shared by all contexts. `nextdict` is a modifiable dictionary containing any dynamic information common to all applications, such as downloaded packages. `systemdict` is a read-only dictionary containing all the PostScript operators, both the standard ones and those implemented by NeXT.

### The Current Window

Each context has a current graphics state and graphics state stack. The current graphics state is a set of parameters that the PostScript interpreter uses when rendering images on the screen (or printed page). It includes parameters for such things as the current color, line width, clipping path, and dash pattern.

Three parameters are especially important to the window system:

1. The current device, where drawing is to be rendered
2. The current transformation matrix (CTM), which maps the coordinate system the application is currently using to the device
3. The current clipping path, which defines the area where images can be rendered

When drawing on the screen, the device is a window. The device of the current graphics state is the current window. PostScript painting operators, such as `stroke`, `fill`, and `show`, draw directly in the current window. You don't need the `showpage` or `copypage` operators to make images visible.

When the current graphics state is set to a new window device, the other two parameters also change:

- The current transformation matrix is initialized to the window's base coordinate system. (Thereafter, the `initmatrix` operator can be used to reestablishes this coordinate system.)
- The current clipping path is set to a path around the window. (The `initclip` operator reestablishes this clipping path.)

These changes ensure that drawing is confined to the current window and is in a coordinate system specific to the window.

### Changing Graphics States

As users shift their attention from place to place on the screen, applications are required to shift the drawing focus from window to window and graphics state to graphics state. There can be only one current graphics state, and therefore only one current window, for each execution context.

The graphics state stack saves former graphics states that might later be restored. The `gsave` operator pushes the current graphics state on the stack and `grestore` replaces the current graphics state with one from the stack.

Because applications typically draw in many windows, it's possible for each graphics state on the stack to point to a different window device.

The stack saves graphics states in a particular order, but as users shift their attention on-screen, an application can be required to switch between graphics states in an order that the stack can't predict. Therefore, to simplify the task of switching between graphics states, the Display PostScript system permits graphic states to be saved not only on the stack, but also in individual graphics state objects. The `gstate` operator creates a new graphics state object and copies the current graphics state to it:

```postscript
/foo gstate def
```

A graphics state object is not stored on the stack; the `gstate` operator allocates memory for it. Applications therefore tend not to create many graphics state objects for themselves, but reuse the ones they do create.

The `currentgstate` operator copies the current graphics state to a graphics state object, and the `setgstate` operator copies the graphics state recorded in a graphics state object to the current graphics state.

At minimum, the Application Kit keeps one graphics state object for each window. The object identifies the window (as the device) and records its base coordinate system. Shifting the drawing focus from one window to another is a matter of first setting the current graphics state from the desired window's graphics state object and then altering parameters of the current graphics state as needed. Altering these parameters doesn't affect the graphics state object.

Additional graphics state objects can be assigned to facilitate shifting back and forth between particular graphics states within a window. For example, one object could be assigned to a text display and another to the scroller that scrolls the display. Views created through the Application Kit should be assigned graphic state objects using Kit methods rather than PostScript operators.

### Window Buffering

Windows often overlap when there's more than one on the screen. When a group of windows are stacked together, a mouse click within one of them usually brings it to the front and its previously obscured contents become visible. Whether the contents are automatically made visible by the Window Server or must be redrawn by the application depends on whether the window has a backup buffer. The buffer stores pixel values for the portions of a window that aren't shown on the screen. When those portions become visible again, they can be copied from the buffer by the Window Server rather than be redrawn by the application.

So that you can pick the buffering scheme that's best for the needs of your application, three distinct types are provided:

#### Nonretained

A nonretained window has no buffer. All drawing is done directly on-screen—that is, into the memory dedicated to holding screen pixel values. If part of the window gets covered by another window, the memory for that part of the screen changes and a portion of the covered window's contents are lost. If later the window is uncovered, any drawing in that part will be replaced by the background color.

#### Retained

A retained window has a buffer equal to the size of the window. Drawing is done directly on-screen—directly into screen memory—for all visible portions of the window, and into the buffer for all portions of the window that are covered and not visible. If a visible portion of the window becomes obscured, the contents of that portion are saved in the buffer. If the obscured portion of the window is later revealed, the contents of the buffer are copied on-screen.

#### Buffered

Like a retained window, a buffered window has a buffer large enough to hold its entire contents. However, the buffer isn't only backup storage, it also serves as a true input buffer. All drawing is first done in the buffer, then is copied to the screen through an explicit call to the NeXT PostScript `flushgraphics` operator. Drawing done through the Application Kit's display methods is automatically flushed.

When a covered portion of a retained or buffered window becomes visible, the newly uncovered portion is automatically refreshed from the buffer. However, an uncovered portion of a nonretained window needs to be redrawn from scratch.

The NeXT header file `dpsNeXT.h` (in `/usr/include/dpsclient`) defines three C constants that you can use to specify the buffering type when creating a window through Application Kit methods:

- `NX_NONRETAINED`
- `NX_RETAINED`
- `NX_BUFFERED`

#### Choosing a Buffering Type

Because it combines the simplicity of drawing directly on the screen with the security of a backup buffer, a retained window is usually preferred over a nonretained window. Nonretained windows are appropriate for transitory images that you don't need to save, or for windows you can guarantee will never be covered. However, in a multi-application environment, this guarantee is a hard one to give.

A buffered window is appropriate when you're drawing images that take some time to appear, and you don't want users to watch them being rendered on-screen. With a buffered window, an elaborate picture can be displayed first in the buffer, and when it's complete be flushed instantaneously to the screen.

Buffered windows also yield smoother transitions between images when you're repeatedly drawing in the same area of the window. This prevents visible flicker as images are erased and redrawn.

### The Screen List

The Window Server keeps track of the current front-to-back order of windows through a screen list. The list is rearranged every time the user brings a window to the front or sends one to the back. Applications reorder windows on-screen by reordering them in the list.

A window can be placed at the top of the screen list—that is, in the frontmost position—at the bottom of the list, or in any position above or below another window. Every window has a unique position in the list; even if two windows don't overlap on the screen, one of them is listed in front of the other. Windows can also be left off the list, and thus kept off the screen.

When you position a window in the list, you must specify two parameters:

1. Whether it's to be placed above or below another window, or left off the list entirely
2. What the window number of the other window is. If this number is 0, the window you're ordering will be placed at the very top or bottom of the list. If the window you're ordering is to be left off the list, this second parameter is ignored

The NeXT header file `dpsNeXT.h` (in `/usr/include/dpsclient`) defines three C constants that can be used to order windows:

- `NX_ABOVE`
- `NX_BELOW`
- `NX_OUT`

#### Window Tiers

The Application Kit divides the screen list into six tiers. The top five tiers keep special types of windows from being buried under other windows when they're on-screen. The tiers are:

1. Pop-up and pull-down lists
2. Attention panels
3. The main menu and attached submenus
4. Other menus
5. Docked icons
6. All other windows

### Off-Screen Windows

Windows are created off-screen. Applications must place them in the screen list to make them visible. If a window is buffered or retained, it's a good idea to draw in it before moving it on-screen. This prevents the user from seeing images rendered in stages.

Windows that are kept permanently off-screen can be useful for caching images that you want to composite to on-screen windows.

An off-screen window is entirely invisible; it has no screen memory. All drawing goes directly into the window's buffer. Drawing in an off-screen nonretained window is pointless; since it has no buffer, there's nowhere for the drawing to go. Flushing an off-screen buffered window is similarly pointless.

You can put a window in the screen list, and take it out again, with Application Kit methods. A window disappears from the screen when it's removed from the list, and is restored to the screen when it's put back in the list.

It's also possible to locate windows off-screen yet keep them in the screen list. Because the screen coordinate system extends far beyond the screen along both the x- and the y-axis, you can assign coordinates that prevent a window from being visible. An ordinary-sized window located at (5000.0, 5000.0) would not show up on the screen, for example.

For two reasons, it's preferable to keep windows off-screen by removing them from the screen list rather than by assigning them off-screen coordinates:

1. Events—user actions on the keyboard and mouse—can be associated with any window in the screen list, whether its coordinates place it on the screen or off. However, windows not in the list don't get events. Since events are usually associated with an on-screen display (a slider or button, for example) and require visual feedback, they shouldn't normally be associated with windows the user can't see.

2. Windows that aren't in the screen list can be assigned on-screen coordinates. This simplifies subsequent handling; placing the window on-screen involves no coordinate adjustments and no effort to remember where the window was located.

### The Background Color

When a new window is created, the pixels within it are initialized to a background color, usually white. You can choose a color other than white by combining the effects of a few PostScript operators.

Through the `setexposurecolor` operator, you can determine the color that's displayed when new areas of the window are exposed. This operator sets the color that's shown in areas added to a window when it's resized larger. It also sets the color that's used when covered areas of a nonretained window are exposed.

This code sets the background color of the current window to 50% gray:

```postscript
0.5 setgray         % Set a new gray value
setexposurecolor    % Set the background color to new gray value
```

To make the initial color of a nonretained window match the color set with this operator, create the window off-screen, set the background color, then move the window on-screen.

You can set the background color of a retained or buffered window by a combination of the `fill` operator (for the initial surface area of the window) and the `setexposurecolor` operator (for any future surface area if the window is resized).

---

## Compositing and Transparency

Compositing is a NeXT extension to the Display PostScript system that enables separately rendered images to be combined into a final image. It encompasses a wide range of imaging capabilities:

- It provides the means for simply copying an image "as is" from one place to another
- It lets you add two images together so that both appear in the composite superimposed on each other
- It defines a number of operations that take advantage of transparency in one or both of the images that are combined. When they're composited, the transparency of one image can let parts of the other show through

Compositing can be used for copying within the same window on the screen, as during scrolling, or for taking an image rendered in one window and transferring it to another. Images are often stored in off-screen windows and composited into windows on-screen as they're needed.

When images are partially transparent, they can be composited so that transparent sections of one determine what you'll see of the other. Each compositing operation uses transparency in a different way.

### Transparent Paint

On the NeXT computer, you can set a coverage parameter in the current graphics state in addition to the color parameter:

- The `setalpha` operator sets the current coverage to an alpha value in a range from 0 (completely transparent) to 1 (completely opaque)

```postscript
0.333 setalpha
```

The default alpha value is 1. To use transparency, you must explicitly set a lower value; `currentalpha` returns the current setting.

- The `setgray` operator sets the current color to a value in a range from 0 (black) to 1 (white)

```postscript
0.5 setgray
```

Together, the alpha value and color value determine the kind of paint that's used when rendering an image. The color value determines the color of the paint; the alpha value determines how opaque or transparent it is.

The color is still fully affected by the alpha value, so a white pixel (true color = 1) that's one-third transparent (coverage = 0.667) will store a color value reflecting this transparency.

What paint with a gray value and an alpha value actually looks like depends on the background it's applied against. The more transparent the paint is, the more of the background you'll see. If completely transparent paint is used, images drawn with it are invisible.

The NeXT MegaPixel Display adopts the PostScript imaging model: A white background is assumed. Areas painted with completely transparent paint appear white on-screen; areas painted with partially transparent grays appear as lighter grays.

### Bitmaps

A bitmap is binary data that describes the pixel values for an image. It's an external data representation, perhaps kept in a file or in memory allocated by your program, as opposed to the representations used internally by the device.

A bitmap is the most efficient way of rendering a complicated image in a window. The `NXImageBitmap()` function displays a bitmap image, and `NXReadBitmap()` produces a bitmap from an existing image.

Bitmaps cover a rectangular area and describe all the pixels within the rectangle. Because the MegaPixel Display requires two bits of memory for each component of a pixel, a bitmap also assigns two bits per pixel for each component. Bitmaps always have a color component and may or may not also have a coverage component.

### Premultiplication

Color values are stored as "premultiplied" values, reflecting the effect of the coverage component. The intended or "true" color (as determined by `setgray`) is multiplied by the coverage before being stored. So a white pixel (true color = 1) that's one-third transparent (coverage = 0.667) will store a color value of 0.667 (1 × 0.667).

There are two reasons for this adjustment:

1. It's a required part of all compositing operations. Doing the multiplication when pixel values are first stored—"premultiplying"—means that it can be avoided during compositing. This makes compositing more efficient.

2. It results in a value that more accurately reflects the pixel's actual color contribution.

In bitmaps, color values must be premultiplied "toward black," meaning that a pixel's color value can never be greater than its coverage value.

Although a black background is assumed for the external data representation stored in bitmaps, the internal representations on the MegaPixel Display use the opposite convention. Internally, color values are premultiplied towards white rather than black.

### Compositing Operators

The three compositing operators are `composite`, `compositerect`, and `dissolve`.

#### composite

The `composite` operator takes a list of eight operands; the first four specify the rectangle of the source image:

```postscript
srcx srcy width height srcgstate destx desty op composite
```

The fifth operand, `srcgstate`, names a graphics state that specifies both the window device where the source rectangle is located and the coordinate system it's defined in.

The next two operands, `destx` and `desty`, locate the destination image in the current window and current coordinate system. Pixels in the destination image are paired one-to-one with pixels in the source image. The destination rectangle will be the same size and orientation as the source rectangle, regardless of the current coordinate system.

The final operand to `composite` specifies the type of compositing operation. There are several to choose from:

| Operator | Operator | Operator |
|----------|----------|----------|
| Copy | Sover | Dover |
| Clear | Sin | Din |
| PlusD | Sout | Dout |
| PlusL | Satop | Datop |
| Xor | | |

The `dpsNeXT.h` header file defines matching constants that can be used in Objective-C code:

| Constant | Constant | Constant |
|----------|----------|----------|
| NX_COPY | NX_SOVER | NX_DOVER |
| NX_CLEAR | NX_SIN | NX_DIN |
| NX_PLUSD | NX_SOUT | NX_DOUT |
| NX_PLUSL | NX_SATOP | NX_DATOP |
| NX_XOR | | |

The operation called "Copy" is one of the most basic; it simply replaces the destination image with the source image:

```postscript
myGState setgstate    % make myGState the current graphics state
0 0 200 200           % the source rectangle
myGState              % the source graphics state
0 10                  % location of the destination rectangle
Copy composite        % copy source to destination
```

#### compositerect

When the source of a compositing operation is a constant color, it's convenient to use the `compositerect` operator instead of `composite`.

```postscript
destx desty width height op compositerect
```

`compositerect` is like `composite`, except that there's no real source image; its first four operands define the destination rectangle directly in the current window. `compositerect`'s effect on the destination is as if there were a source image filled as specified by the color and alpha parameters in the current graphics state.

For example, you could erase a rectangular area to white by setting the current color to 1 and then calling `compositerect` to perform a Copy operation:

```postscript
1 setalpha
1 setgray
0 0 100 100 Copy compositerect
```

#### Types of Compositing Operations

| Operation | Description |
|-----------|-------------|
| **Copy** | Simplest and most common. Ignores the destination image and replaces it with the source image; every pixel in the source rectangle is copied to the destination |
| **Clear** | Ignores both the destination and source. Turns the destination rectangle completely transparent |
| **PlusD** / **PlusL** | Add the source and destination images together. PlusD adds colors to become darker; PlusL adds them to become lighter |
| **Sover** / **Dover** | One image is placed over the other. Transparency in the image on top lets the image underneath show through |
| **Sin** / **Din** | One image is displayed wherever, and to the extent that, the other image is opaque |
| **Sout** / **Dout** | One image is displayed wherever, and to the extent that, the other image is transparent |
| **Satop** / **Datop** | One image is placed on top of the other, with the composite adopting the transparency of the image underneath |
| **Xor** | Each image is visible only where (and to the extent that) it's opaque and the other image is transparent |

#### Dissolving

The `dissolve` operator blends two images together. Typically, it's called over and over again in a loop so that one image (the source) can appear to slowly replace the other (the destination).

```postscript
srcx srcy width height srcgstate destx desty delta dissolve
```

When `delta` is 0, only the destination image is in the composite; when it's 1, only the source image is present. To gradually replace one image with another, `dissolve` should be called in a loop with delta increasing slowly from 0 to 1 with each call.

#### Highlighting

On the NeXT MegaPixel Display, highlighting is usually accomplished by changing white pixels to light gray and light gray pixels to white. There's an additional compositing operation called "Highlight" that's used only with the `compositerect` operator:

```postscript
10 10 50 50 Highlight compositerect
```

On the MegaPixel Display, Highlight turns every white pixel in the destination rectangle to light gray and every light gray pixel to white, regardless of the pixel's coverage value.

The `NXHighlightRect()` function is the most direct way to highlight in Objective-C code:

```c
NXRect  rect = {{10.0, 10.0}, {50.0, 50.0}};
NXHighlightRect(&rect);
```

---

## Instance Drawing

A screen-oriented user interface often does highly interactive drawing as a means of providing feedback to the user, typically in response to the user dragging with the mouse. Examples include stretching out a rectangle, moving an object around on the screen, and highlighting objects as they're selected.

Instance drawing is temporary drawing done within an on-screen window. Rendered images appear directly on-screen and aren't saved in the window's buffer. They can be easily removed and replaced with the original image.

The `setinstance` operator turns instance drawing mode on and off. While the mode is on, all drawing in the current window is treated as instance drawing rather than as part of the window's permanent display. The mode setting is stored as a parameter in the current graphics state. All images produced in instance drawing mode are removed by the `newinstance` operator. `newinstance` restores the original image.

In addition, there's a `hideinstance` operator for restoring the original image within a specified rectangle.

---

## Sending PostScript Code to the Window Server

Your application must send PostScript code to the NeXT Window Server to draw on the screen. Communication with the Window Server is made possible through a bidirectional connection between the application process and the Window Server process.

Programs based on the Application Kit establish this connection automatically at startup. If your program doesn't use the Application Kit, you must establish a connection to the Window Server through direct calls to functions defined in the library `libNeXT.a`.

Once a connection is established, information passes across it in both directions until the connection is closed when the user quits the application. For transmission efficiency, the information sent to the Window Server consists primarily of binary-encoded PostScript code.

### Using pswrap

For most programmers, `pswrap` provides the best way of sending PostScript code to the Window Server. `pswrap` is a program that creates a C function to correspond to a sequence of PostScript code. When your application is run, a call to the C function sends a binary-encoded version of the PostScript code to the Window Server.

Each function definition begins with `defineps` and ends with `endps`. The definition includes the C function name, any arguments the function requires, and a listing of the PostScript code the function represents.

For example, you could define a function that draws a square on the screen:

```c
defineps drawSquare(float x)
newpath
100.0 100.0 moveto
0.0 50.0 rlineto
50.0 0.0 rlineto
0.0 -50.0 rlineto
closepath
x setgray
stroke
endps
```

Elsewhere in your C source code, you can call this function in the same manner as you would any C function:

```c
drawSquare(0.5);
```

### Using Single-Operator Functions

The easiest way to send one or two lines of PostScript code to the Window Server is by using a set of C functions that have a one-to-one correspondence with the PostScript operators. These "single-operator" functions have the same name as the corresponding PostScript operator, but begin with the prefix "PS" or "DPS".

The functions that have the "PS" prefix act on the current PostScript context; you must specify the PostScript context for the "DPS" functions.

An example of a single-operator function is `PSsetgray()`, the C function that corresponds to the PostScript `setgray` operator. A rewrite of the above example using single-operator functions:

```c
PSnewpath();
PSmoveto(100.0, 100.0);
PSrlineto(0.0, 50.0);
PSrlineto(50.0, 0.0);
PSrlineto(0.0, -50.0);
PSclosepath();
PSsetgray(0.5);
PSstroke();
```

### Connection Buffering

For efficiency, data passed from the application to the Window Server is first accumulated in a buffer. Data returned from the Server is similarly buffered. To ensure that all data in the application's buffer is sent across the connection, the buffer must be flushed.

The buffer on the application side is flushed automatically at the following times:

- Whenever the application tries to get events
- After a timed entry executes

This buffer can also be flushed explicitly with a call to `DPSFlush()`.

---

## Imaging Conventions

In the PostScript language, all visible forms have an outline delineating the area to be colored. This is true both of filled figures and of simple lines.

There are two steps to drawing a filled figure:

1. Construct a path around the area that's to be colored
2. Fill the area outlined by the path

Drawing a line—whether it's open (like an arc) or closed (like a full circle)—also takes two steps:

1. Construct the path where you want the line located
2. Stroke the path to color the line

Stroking fills an outline that's constructed along either side of the path using the line width, line cap, and line join parameters of the current graphics state. One-half the line width falls on one side of the path and one-half on the other.

### The General Rule

An outlined figure selects all the pixels that it overlaps. This includes all the pixels its outline completely surrounds and any pixel the outline enters, even if most of the pixel actually lies outside the area the outline encloses.

If an outline lies on a pixel boundary but doesn't enclose or enter the pixel, the pixel isn't treated as if it were inside the outline. The pixel and the outlined figure don't overlap.

### Outlines with No Area

When, to draw a very thin line, you set the PostScript line width variable to 0, the resulting outline is identical to the path; it has no width at all.

#### Points

A single point is shown by turning on a single pixel. If the point lies in the middle of a pixel, that pixel is the one turned on. If the point lies between pixels, just one of the pixels is chosen based on these rules:

- If the point lies on the vertical boundary between pixels, the pixel on the right is chosen
- If the point lies on the horizontal boundary between pixels, the one below is chosen
- If the point lies on the corner of four pixels, the one on the lower right is chosen

#### Zero-Width Lines

Zero-width lines are rendered by turning on the fewest possible number of pixels to show a connected line:

- If the difference between the two x coordinates is greater than the difference between the two y coordinates (the line is more horizontal than vertical), exactly one pixel will be turned on in each vertical column between the two endpoints
- If the difference between the two y coordinates is greater than the difference between the two x coordinates (the line is more vertical than horizontal), exactly one pixel will be turned on in each horizontal row
- If the differences between the y coordinates and the x coordinates are the same (the line is drawn at a 45° angle), exactly one pixel will be turned on in each column and in each row

### Half-Open Shapes

All shapes, whether pixels or outlined figures, are considered "half-open": Only about half the points on the edge of the shape are included in it.

For straight edges, the rules are:

- If the edge of a shape faces exactly upward or at all leftward, the points on the edge belong to the shape. If the edge faces exactly downward or at all rightward, the points on the edge don't belong to the shape
- A point at the corner between two straight edges belongs to the shape if both of the edges do, or if just one edge does and the corner is concave

### Clipping

A PostScript clipping path outlines the area where painting operators (such as `show`, `fill`, and `stroke`) can render an image. Like any other outline, it selects pixels according to the general rule and other conventions.

Only pixels selected by both the clipping path and an outlined figure can actually be used to display the figure.
