# 3.3 Release Notes: Edit

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

This file contains developer release notes for the 3.2, 3.1, and 3.0 releases of the Edit appliction. There are no specific notes for Release 3.3. Since the many of notes for Edit are user or administrator related, those notes have been moved to `/NextLibrary/Documentation/NextAdmin/ReleaseNotes/Edit.rtf`.

---

## Notes Specific to Release 3.2

### New Features (Developer Mode Only)

Due to the addition of RTF support in the compiler, source code can now be made rich. This means that you can save a rich text file, which might normally be saved to `Foo.rtf`, as `Foo.h` or `Foo.m`. A change has been made to allow the typical source code file extensions (`.h`, `.c`, `.m`, `.H`, `.C`, `.M`, `.cc`, `.cpp`, `.cxx`, and `.hxx`) to be used instead of the standard `.rtf` extension. Edit can now create and modify such files as rich text.

One good example of how to use rich source code effectively, besides the obvious use of italics, bold, underlining, fonts and color of comments and other syntactic constructs, is to use Help links to help document portions of code and link them to other portions of code. Under the Format menu, the Help submenu provides the ability to insert a link to another file or to a marker placed in another file. Markers can be inserted anywhere in the text and will be automatically selected (along with the remainder of the line following them) when a link to that marker is clicked on. Note that marker names can be inspected or changed by using "Show Markers" and Command-clicking them.

---

## Notes Specific to Release 3.0

### New features

#### C Source Identifier Expansion

Windows that contain C source code now have Emacs-style escape key expansion for variable and procedure names. When a conflict occurs, you can enumerate the choices by repeated pressing the escape key.

#### Preferences Panel

Under C Options there is the Include Path, which is used to find import files (`.h`) when you triple click on them in your source files and then use the Open Selection command (Cmd-O). Also, this same path is used in the Open Folder... command, so if you specify the file `sys/file.h`, Edit will check the Include Path to attempt to open this file.
