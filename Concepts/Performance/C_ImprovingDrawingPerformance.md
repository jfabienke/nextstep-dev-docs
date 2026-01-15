# Improving Drawing Performance

In most applications, a large chunk of time is consumed by drawing. Indeed, the optimization of those sections of your application that perform drawing should probably be one of your first priorities, since often relatively small changes to your code can result in dramatic improvements in the real and perceived performance of your application. This discussion is broken down into three topics: General rules to follow in a client-server world, efficient focusing, and specific techniques for optimizing the performance of your drawing code.

---

## General Rules in a Client-Server World

NEXTSTEP® is based on a client-server model, which has certain implications when you're writing your application, as discussed below.

### Write Your Program in Objective C, not in PostScript

Underlying the design of NEXTSTEP is the assumption that the center of gravity for a NEXTSTEP application should be on the application side rather than the Server side, that is, applications in the NEXTSTEP environment should be written in Objective C®, and not in PostScript. This was based on performance considerations, as well as on the view that given the characteristics of the respective languages and the state of the respective development and debugging environments, it was much easier to write and debug applications in Objective C than it was to do the same things in PostScript code.

The bulk of your algorithms and data structures should be written in Objective C and should avoid maintaining state in the Window Server. This point about state is important for a number of reasons:

- If your application maintains state or other data structures in the Server, you must either provide PostScript procedures to manipulate that state data, or you need to make calls to the Server whenever your algorithms on the application side need to access or modify that data. On the other hand, if you maintain two copies of the state data, one on the application side and one on the Server side, you'll need to ensure that they're always in synch. In either case, you'll require communication with the Window Server, and that implies interprocess communication. You're far better off to avoid the problem entirely by not maintaining state information in the Server, and doing all the work on the application side. If you have PostScript wraps that require state information, pass the state information as parameters to the wraps.
- Another side benefit of avoiding a lot of functions and state information in the Server comes when you print. As you know, printing occurs in a different PostScript context than that of your running application. As a result, if you have any PostScript procedures or state variables that are in the Server, you must ensure that they're downloaded to the printing context as well. While this certainly can be done, it's often easier to avoid the problem altogether.

It should be noted that the above discussion shouldn't be construed as an argument against using wraps. In fact, replacing calls to multiple single-operator C functions with a single call to a wrap may result in a performance gain. Adobe recommends that while single-operator C functions are easy to use, wraps should be used for all but the simplest drawing. However, avoid using wraps for any operation that can be done entirely within the client application. It almost always pays to perform calculations on the application side and pass the results as parameters, as opposed to performing the calculations in the Server. Similarly, pass required state information as parameters to the wrap. Note also that there's little or no performance gain from downloading a wrap that contains a procedure definition and invoking the downloaded procedure from another wrap when needed, as opposed to downloading the procedure each time it's to be executed. Last, and perhaps most important, is being smart about the drawing your application does and your choice of techniques to perform that drawing. This will probably have a much more dramatic effect on overall performance than the decision whether to use single-operator C functions rather than custom wraps.

### Avoid Round Trips

Any PostScript function or wrap that has a return value in its parameter list will cause a round trip to the Window Server, and should be avoided if possible. The data an application sends to the Server is buffered to minimize communication overhead and context switches. Executing wraps that return values defeats this buffering, since the data must be flushed to the Server before an answer can be returned. This is always going to be slower than inspecting a variable in your application, or calling a method or function within your application. Thus, be judicious when making calls to the Server; consider whether by caching data or state on the application side you can avoid making the call altogether.

### Use the Application Kit's Font Mechanism

The Application Kit provides a number of classes (**Font**, **FontManager**, and **FontPanel**) that can perform many font operations without interacting with the Server, thereby reducing the number of round trips. Wherever possible, you should use the font mechanism built into the Application Kit rather than interact directly with the Server. For example, the font mechanism caches font metric information on the application side. Hence, you can query the **Font** object for font metrics without involving communication with the Window Server. In fact, it's almost twice as fast to use the `getStringWidth:` method provided by the **Font** class than to use `PSstringwidth()` to get the width of strings before drawing.

### Avoid Unnecessary Calls

Below is an example of making a call to the Server that's silly and inefficient. In this case you're better off just setting the gray, rather than querying the Server first for the current gray.

```objc
float currentGray;
PScurrentgray(&currentGray);
if (currentGray != desiredGray)
    PSsetgray(desiredGray);
```

In general, you should avoid making unnecessary calls to the Window Server. Try to structure your drawing code so that you avoid redundant or unnecessary calls to `PSsetgray()`, `PSsetlinewidth()`, `PSgsave()`, and so on.

### Boxcarring

The Display PostScript language has added a number of imaging operations that don't alter the underlying imaging model, but which cover frequently performed operations and provide highly optimized execution. These operations include:

- **user paths**: A number of operators are provided that allow you to download a very compact representation of a path. The operator builds a special kind of path from it and performs some operation like stroke or fill on the path.
- **rectangles**: A number of operators are provided to build a path made up of one or more rectangles, and perform some operation on the resulting path (fill, stroke, or clip). The operators can take either a single rectangle or an encoded array of rectangles. If you're doing any rectangle operations, you're almost always better off using these operators than `moveto`, `lineto`, and so on. Note that the Application Kit provides a number of functions (`NXRectClip()`, `NXRectClipList()`, `NXRectFill()`, `NXRectFillList()`, `NXEraseRect()`, and so on) that use the underlying rectangle operators provided by the Display PostScript language. You should use them whenever performing rectangle operations.
- **text drawing**: A number of operators are provided that take an encoded array of coordinates and a character string, and use the coordinates to place and show the characters in the character string (e.g., `xyshow`).

We call the use of these operators "boxcarring" because they allow you to download one large chunk of data to the Server where it's operated on efficiently in bulk, instead of sending many smaller chunks of data that are processed individually. In fact, these operators can be several times faster than the equivalent single-operator C functions or wraps.

Needless to say, you should become familiar with these operators and use them wherever appropriate. Note also that the rectangle, `xyshow` and user path operators above are simulated in the default print packages, so you can use them when printing without modification.

---

## Efficient Focusing and gstate Objects

Before an application draws in a view, the view must become the focus for drawing; otherwise the drawing will appear in some other view that was previously the focus. When you draw within the context of `drawSelf::`, focusing is performed by the view's display method. If you draw outside the context of `drawSelf::`, you manage focusing explicitly by invoking `lockFocus` and `unlockFocus`.

Ensuring that your view has the focus before drawing is a necessary step in drawing, but you should avoid unnecessary focusing. When the view is focused on, an appropriate transform matrix must be built to map the view's coordinate system back to the window's coordinate system, an appropriate clipping path must be built, and a `PSgsave()` is performed. While care has been taken to make this process as efficient as possible, it nonetheless takes time and is worth paying attention to. Here are some suggestions:

- The fastest focus is the one you don't need to do. Try to structure your code so that you aren't doing unnecessary focusing. For example, if you're drawing within the context of a modal loop, put the `lockFocus` and `unlockFocus` outside the loop.
- Turn off clipping if not needed with `setClipping:`. Building the clipping path is usually the slowest part of focusing, so you can usually save a good deal of time by turning clipping off for the view if you don't need it. You don't need clipping if you're sure you'll never attempt to draw outside the bounds of your view.
- Use **gstate** objects.

If you have a view that's repeatedly being focused on (for example, the gauge on an instrument panel of a flight simulator), you should consider allocating a **gstate** object for the view by using **View**'s `allocateGState:` method. A **gstate** object is a PostScript object (not an Objective C object) that contains all the information contained in a graphics state, and is a Display PostScript extension designed to make it much more efficient to switch graphics states.

The very first time a `lockFocus` or `display` is sent to a view for which a **gstate** object has been allocated, the view machinery focuses on the view in the normal manner, has the Window Server take a snapshot of the graphics state, and then sticks it in a **gstate** object. Thereafter, whenever focusing occurs, the view machinery first checks to ensure the **gstate** object is still valid (that is, the coordinate system of the view or its superviews has not been permanently changed) and, assuming it is, has the Window Server copy the **gstate** to the current graphics state.

**Note:** **View** has a handy method called `initGState` that's called automatically whenever the **gstate** is created or modified. You can override `initGState` to initialize other aspects of the graphics state, such as line width or gray level.

The use of **gstate** objects effectively avoids all the work normally associated with focusing, and subsequently reduces the time required for focusing. However, they don't come for free, and thus should be used judiciously. Since a **gstate** object contains all the information contained in a graphics state, including the current clipping path and current path, they consume hundreds of bytes within the Window Server at a minimum, and can be much larger depending on the nature of the clippath and currentpath.

If your application repeatedly focuses on and draws in a view, it may be a good candidate for a **gstate** object. On the other hand, if you have a view that your application focuses on infrequently, it may not make sense to allocate a **gstate** object for it, regardless of the amount of drawing you actually do in that view. Allocating a **gstate** affects only the focusing time, as it has no effect on the drawing time once the graphics state has been set up.

---
*Release 3.3 Copyright ©1995 NeXT Computer, Inc. All Rights Reserved.*
