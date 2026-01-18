# 3.3 Release Notes: Application Kit

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

This file contains developer release notes for the 3.3, 3.2, 3.1, and 3.0 releases of the Application Kit. Items specific to or introduced in Release 3.2 are listed first, followed by the 3.1 and 3.0 notes. Some user-visible changes to the application architecture have been moved to `/NextLibrary/Documentation/NextAdmin/ReleaseNotes/AppBehavior.rtf`.

---

## Notes Specific to Release 3.2

### Known Bugs

#### NXRunAlertPanel

In all 3.X releases `NXRunAlertPanel` incorrectly tries to localize the arguments it receives. If one of these arguments is not found as a key in the localization string table, the system retains a reference to that pointer and uses it as the localized value. This results in the correct display, but if that argument is later freed or modified, the application gets hashtable error messages or crashes. The workaround is to be sure that you call this function with strings that you know will not change. Use either literal strings or make copies of dynamically created strings that may change later.

#### Text Delegate Methods

Before the Text object sends the delegate method `textWillStartReadingRichText:`, it checks to see if its delegate responds to it. Unfortunately, the text object checks to see if the delegate responds to a method named `textWillStartReadingRichText:text:runs:`; thus, the delegate method, even though it might be implemented, is never invoked. The same bug exists with `textWillFinishReadingRichText:`.

If you would like to respond to these delegate methods, implement the following stub methods in your delegate:

```objc
- textWillFinishReadingRichText:obj text:(void *)text runs:(void *)runs
- textWillStartReadingRichText:obj text:(void *)text runs:(void *)runs
```

These methods will never be invoked; so all they need to do is return self.

This bug exists in the 3.0 and 3.1 releases as well.

#### NXHelpPanel

If additional help is located in a directory other than `YourProject/English.lproj/Help`, and that help is to be loaded with the NXHelpPanel method, `addSupplement:inPath:`, the help directory must not be compressed (i.e. do not invoke the `compresshelp` utility on it).

#### NXJournaler

Earlier versions of the documentation implied that journal recording could be made non-abortable by or'ing in the `NX_NONABORTABLEMASK` flag when calling the `setEventStatus:soundStatus:eventStream:soundfile` method. This was incorrect. In earlier versions, if this flag was included journal recording would not work. In the 3.2 version, this flag will simply be ignored if the request is for journal recording. If the request is for journal playback, the flag will be accepted and will work as it has in the past.

---

## Notes Specific to Release 3.1

### New Methods

#### ignoreModifierKeysWhileDragging

This method has been added to the NXDraggingSource informal protocol. (This protocol is implemented by the object that supplies an image that can be dragged from one window to another, the source object passed as an argument in the `dragImage:...` message that initiates a dragging session.) If `ignoreModifierKeysWhileDragging` is implemented to return YES, the modifier keys held down by the user will not cause the cursor to change (to the link or copy cursor, for example), nor will they be used when deciding whether to allow the operation.

An application can use this method when implementing the dragging of private data types (e.g., patterns). The benefit is that the cursor will not switch to the copy or link cursor when the user applies the modifier keys, but the operation can still succeed and the application can apply its own semantics to the use of the modifiers. This method existed undocumented in 3.0, and is invoked under that release as well.

This method should never return YES when the user drags a file (when the data is `NXFilenamePboardType`), since files can be dragged between applications. The modifier keys have well-defined meanings in the user interface for dragging files.

#### Removable Media Methods

These methods have been added to the NXWorkspaceRequestProtocol formal protocol, which is implemented by the object returned by the NXApp's `workspace` method. When an application is ready to let the user access files that might be on a removable disk (for example, when it puts up a custom Open panel), it can use these methods to get a list of currently mounted removable devices, and mount new devices if necessary.

```objc
- (BOOL)getMountedRemovableMedia:(char **)mountedMediaPathnames
- (BOOL)mountNewRemovableMedia:(char **)newlyMountedPathnames
- (void)checkForRemovableMedia
```

`getMountedRemovableMedia:` fills in `mountedMediaPathnames` with a zero-terminated tab-separated list of full pathnames to all currently mounted removable media. It returns YES if it was successful in filling in `mountedMediaPathnames` and NO otherwise.

On systems that don't send an interrupt or other notification that a disk has been inserted, an application must invoke either `mountNewRemovableMedia:` or `checkForRemovableMedia` before invoking `getMountedRemovableMedia:`. Both of these methods cause the Workspace Manager to poll the drives to see if a disk is present. If a disk has been inserted but not yet been mounted, these methods will cause the Workspace Manager to mount it. `mountNewRemovableMedia:` waits until the new disk has been mounted and then fills in `newlyMountedPathnames` with a zero-terminated tab-separated list of full pathnames to all newly mounted disks. It returns YES if it was successful in filling in `newlyMountedPathnames`, and NO otherwise. `checkForRemovableMedia` does not wait until the new disk is mounted; instead, it asks the Workspace Manager to mount the disk asynchronously and returns immediately.

These methods are new in 3.1, and your application will crash if you try to use them on a 3.0 system. Therefore, before you send them you should test whether the object returned by `[NXApp workspace]` responds to these methods with `respondsTo:`.

### Semantic Changes

#### Secure Ports

In 3.0, a change was made so that, unless the user enabled the Public Window Server preference, the ports of Listener objects were registered securely (i.e., under modified names). This allowed applications launched by the user on the same machine to communicate with each other, but prevented other machines on the network from looking up their ports. Because this caused problems for some existing applications that need to rendezvous over the network, the Listener class has been changed in 3.1 to register ports securely only for applications built under Release 3.0 or later. Applications built under Releases 1 or 2 will have their ports registered under the plain port name given the Listener.

Users who wish to force pre-3.0 applications to register their ports securely may set the `NXSecureAppListenerPorts` default to YES. This default may be set for particular applications or for all applications by specifying "GLOBAL" as the application name.

If a 3.0 application wishes to publish a port under a nonsecure name for intermachine use, it should override the Listener's `checkInAs:` method and publish the port under an unprotected name using `netname_check_in()`.

#### NXColors

Release 3.1 continues the change in direction for NXColors begun in Release 3.0. Under 2.0, all colors were device dependent, and there were no named colors (such as PANTONE). Under 3.0 and 3.1, NXColors can basically be in one of three flavors:

- Device dependent, such as CMYK,
- Device independent, such as RGB, HSB, or grayscale, and
- Named, such as PANTONE.

Colors in the last category come with lookup tables to provide the ability to generate the correct color (CMYK, RGB, or whatever) on a given device.

This change means that colors that look the same on a device (such as the screen) might look different on another device (such as the printer). Thus, freely converting an NXColor from one type to another without regard to how the user chose it (from the color panel) might produce undesirable results on certain devices, or cause the application to discard certain aspects of the color (its device dependence or independence, its name, etc).

In line with this change in direction, Release 3.1 makes these specific modification:

- In 3.1, `NXEqualColor()` will return YES if and only if the two colors will produce the same results on any printer. This means that a color created from CMYK components is not equal to a color created from RGB components, as the former is device dependent and the latter is device independent. Similarly, a PANTONE color is never equal to a color created from CMYK or RGB components.

- Under 3.0, functions such as `NXConvertColorToCMYK()` and `NXYellowComponent()` reported the CMYK components of PANTONE colors by simply converting the screen color (specified in RGB) to CMYK. The results were not satisfying to users, as they lacked the K component and the CMY components were meaningless.

Under 3.1, when an NXColor is asked for its CMYK color components (if it doesn't record them directly), it will first attempt to get or compute them before resorting to the simple-minded conversion between its own color components and the desired ones. This means that if a PANTONE (or other named) color is asked for its CMYK components, the values will be obtained from the DefaultCMYK device table. This feature can be disabled by setting the value of the `NXSmartNamedColorConversion` default to NO.

Under 3.1, developers may wish to test their applications with the `NXCMYKAdjust` default enabled (as described above), and assure that the application behaves reasonably both on the screen and on the printer. This will help verify that the application uses NXColors and the Color panel correctly. Note that when CMYK adjustment is enabled, the algorithms that convert between RGB and CMYK components of a color also change; they return values that match the way the colors appear on the screen. This could be an area of trouble in some applications.

### Other Changes

#### New Utility

A new utility called `compresshelp` has been released in the `/usr/bin` directory. If you are using the NeXT Help System, and have one or more help directories in your project, this utility can be used to compress the files in those directories into one file called `DirectoryName.store`. In the case of the default Help directory (named "Help"), a file called `Help.store` will be created. When Help is invoked for your application, the Help system will first look for a Help directory and then for a `Help.store` file. By default a `Help.store` file, and not a Help directory will be placed in your `*.app`.

The `.store` file created by `compresshelp` will contain only files found in the Help directory which have a `.rtf`, `.rtfd`, `.tiff`, or `.eps` extension. `compresshelp` will warn you about files it has skipped.

By default `compresshelp` will create a compressed output file named `DirectoryName.store`. However if the `-o OutputFileName` option is specified, the compressed help will be placed in a file named `OutputFileName.store`.

Typically you will not need to invoke this utility directly, as it is invoked automatically by the Makefiles supplied by Project Builder for default help directory, named Help. If you have other help directories, and you want them to be compressed, you will have to explicitly invoke this utility in your `Makefile.postamble`.

#### Trapping Illegal Floating Point Operations

A new default named `NXTrapIllegalFloatingPointOps` has been added to allow catching illegal IEEE floating point operations. These operations include dividing 0 by 0, invalid comparisons to NaN, and converting out-of-range floating point numbers to integral values. Running with this default set to YES enables the trap:

```bash
appname.app/appname -NXTrapIllegalFloatingPointOps YES
```

If your application performs an illegal operation, a floating exception will be raised and the application will crash, allowing you to debug the problem. Given that the results of most invalid floating point operations are machine-dependent, using this default may help you chase down floating-point related problems encountered while porting between architectures. Don't leave this default enabled except when debugging.

### Bug Fixes

The following bugs are among those that have been fixed in the 3.1 version of the Application Kit:

| Reference | Problem |
|-----------|---------|
| 20657 | When the user changed the update mode of a data link to "continuous," an out-of-date link would not immediately update. |
| | **Description:** Users expect "continuous" data links to be "in sync" with the source data. In 3.1, when the update mode is changed to "continuous," the system immediately checks whether the data is current, and if not, updates it. |
| 29821 | Filter services weren't executed with the full path. |
| | **Description:** The Application Kit invoked filter services using only the application name, not the full pathname. Some filter services expected the full path name. In 3.1, the Kit executes all filter services with the full pathname. |
| 29866 | Data links with a "continuous" update mode would not update when changes made to the source document were reverted. |
| | **Description:** A "continuous" data link should ask the source document for new data when changes made to the document are undone. In Release 3.1, it does. |
| 30009 | The frames that outline linked data in response to the Show Links command were not drawn correctly in scaled coordinates. |
| | **Description:** Corrected in 3.1. |
| 30234 | `fileOperationCompleted:` methods were not being invoked. |
| | **Description:** When requested to perform a file operation, the Workspace Manager might choose to do it asynchronously. If so, the Application object's delegate is supposed to receive a `fileOperationCompleted:` message notifying it that the operation was accomplished. In 3.0, the message wasn't getting through. |
| 30393 | Some NXCachedImageRep objects would generate an error when archived. |
| | **Description:** The `NXSizeBitmap()` function was incorrectly calculating the size of some images. |
| 30694 | The data links mechanism strips the `/private` prefix on pathnames, sometimes resulting in invalid paths. |
| | **Description:** Removing `/private` from a pathname sometimes produces a bogus path, typically where the directory is `/private/tmp`. In 3.1, `/private` is stripped only where the result is a valid and equivalent path. |
| 30807 | It wasn't possible to read images from more than one Display PostScript context. |
| | **Description:** After reading (getting bitmap data for) a rendered image—for example, by invoking NXBitmapImageRep's `initData:fromRect:` method—in one context, an application could not then read an image in a different context. The problem has been corrected. |
| 31024 | `draggedImage:beganAt:` notifications were received before the dragged image was on-screen. |
| | **Description:** In 3.0, there was a race condition between `draggedImage:beganAt:` messages and the dragged image appearing on-screen. This race made it difficult for `draggedImage:beganAt:` methods to erase underneath the dragged image without an annoying flash. In 3.1, you are guaranteed that the dragged image is on-screen before the message is sent, and thus you can erase under the image without flashing. |
| 31049 | NXDataLink's `initLinkedToSource:...` method would, on rare occasions, cause a crash. |
| | **Description:** The cause of the crash has been removed. |
| 32870 | `windowWillMove:` sender is not the Window. |
| | **Description:** In 3.0, the window delegate message `windowWillMove:` would pass a view from the Window as the sender parameter instead of the Window itself. It has been corrected in 3.1 to pass the window. Apps compiled under 3.0 that worked around this bug will not be affected. |

---

## Notes Specific to Release 3.0

These notes were included with the Release 3.0 version of the Application Kit. Sections that are no longer relevant have been marked with an italicized comment.

### Known Problems

Known problems in the Application Kit:

- The command line program `genstrings` will crash if the specified output file cannot be created.

### Incompatible Changes Since 2.1

Incompatible changes made to the Application Kit since Release 2.1:

- The global variable `NXSelectionPboard` has been renamed to `NXGeneralPboard`.

- The following Speaker/Listener messages have been obsoleted and replaced with improved API described below under "Interapplication Image Dragging" and "Workspace Protocol".

```objc
- (int)registerWindow:(int)windowNum toPort:(port_t)aPort;
- (int)unregisterWindow:(int)windowNum;
- (int)icon...; (the set of messages received during dragging)
- (int)launchProgram:(const char *)name ok:(int *)flag;
- (int)getFileInfoFor:(char *)fullPath app:(char **)appname
    type:(char **)type ilk:(int *)ilk ok:(int *)flag;
- (int)getFileIconFor:(char *)fullPath TIFF:(char **)tiff
    TIFFLength:(int *)length ok:(int *)flag;
- (int)unmounting:(const char *)fullPath ok:(int *)flag;
- (int)powerOffIn:(int)ms andSave:(int)aFlag;
- (int)extendPowerOffBy:(int)requestedMs actual:(int *)actualMs;
```

- The function `PSsetpattern()` no longer takes an argument. This change is only visible to programs recompiled under 3.0, and will not affect existing applications built for a 2.x release.

- The constants `NX_RESIZEBUTTONMASK` and `NX_ALLBUTTONS` have been removed for 3.0. In place of using `NX_RESIZEBUTTONMASK`, you should create a window with style `NX_RESIZEBARSTYLE`. In place of `NX_ALLBUTTONS` you should use `(NX_CLOSEBUTTONMASK|NX_MINIATURIZEBUTTONMASK)`.

- PrintInfo has been changed to use the NXPrinter object for specific information about the printer that is being used. See the description of NXPrinter above. Thus, the following methods have been removed from the public API:

```objc
- setManualFeed:(BOOL)flag;
- (BOOL)isManualFeed;
- setPrinterName:(const char *)aString;
- (const char *)printerName;
- setPrinterType:(const char *)aString;
- (const char *)printerType;
- setPrinterHost:(const char *)aString;
- (const char *)printerHost;
- setResolution:(int)anInt;
- (int)resolution;
```

- Because of the addition of `updateFromPrintInfo` and `finalWritePrintInfo` (see below), the following methods have been removed from PrintPanel API:

```objc
- readPrintInfo;
- writePrintInfo;
```

- The following methods have been removed from the NXColorPanel API:

```objc
+ new
+ newColorMask:
+ newContent:style:backing:buttonMask:defer:
+ newContent:style:backing:buttonMask:defer:colorMask:
- updateCustomColorList
- (int)colorMask;
- setColorMask:(int)colorMask
```

The only initializer for this class is now:

```objc
+ sharedInstance:(BOOL)create
```

The `setColorMask:` method was always a no-op. The initial color mask can now only be set before the ColorPanel is instantiated, using the `setPickerMask:` method (see below).

- The following constants have been renamed in the NXColorPanel API:

```
NX_CUSTOMCOLORMODE    →    NX_COLORLISTMODE
NX_BEGINMODE          →    NX_WHEELMODE
NX_CUSTOMCOLORMODEMASK    →    NX_COLORLISTMODEMASK
NX_BEGINMODEMASK      →    NX_WHEELMODEMASK
```

- `NXReadBitmap()`, `NXSizeBitmap()`, and `NXDrawBitmap()` have been moved from `tiff.h` to `graphics.h`. Because `tiff.h` imports `graphics.h`, this shouldn't cause any source changes. `NXImageBitmap()`, now obsolete, has been removed from `tiff.h`.

- In 2.1, in windows that become key only if needed a view that did not accept first mouse would be passed the mouse clicks even if the window was not key. In 3.0 this has been fixed. Custom controls in these windows should return YES to `acceptsFirstMouse:`. Applications compiled under 2.1 are not affected by this change.

- The global variables `NXSystemFont` and `NXBoldSystemFont` have been removed from the API. Use the new font methods below in their place.

- The location field in flags changed events is no longer valid in 3.0. Also note that the location field in key down and key up events has never been valid.

- The `defaults.h` header file has moved from the appkit headers directory to the defaults directory. Thus imports of `<appkit/defaults.h>` should be changed to `<defaults/defaults.h>`.

### New Features

> **Note:** The Release 3.0 New Features section is extensive and includes documentation on Color dragging, Application Tile, RTFD support, New Filter Services Support, New Classes (NXDataLinkManager, NXColorList, NXColorPicker, NXSpellChecker, etc.), New Methods, New Functions, Semantic Changes, and Other Changes. This section has not yet been converted to markdown format.
