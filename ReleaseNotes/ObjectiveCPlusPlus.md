# 3.3 Release Notes: Objective C++ Compiler

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

This file contains developer release notes for the 3.3, 3.2, 3.1, and 3.0 releases of the Objective C++ Compiler.

---

## Notes Specific to Release 3.2

The Release 3.2 Objective-C++ compiler is based on version 2.2.2 of the GNU compiler.

### New Features

- The new language features supported by the 2.2.2 version are described in the GNU documentation that accompanies the compiler sources.
- The list of C++ extensions is now: `.C`, `.cc`, `.cxx`, `.cpp` and `.M`.
- The `-ObjC++` flag overrides the auto-language-selection so that, for example, a `.c` file can be compiled as Objective-C++ code. However, the `/bin/cc++` shell script which calls the `/bin/cc` driver with the `-ObjC++` flag continues to be supported.
- The new system header path for C++-specific headers is `/NextDeveloper/Headers/g++`. Currently this directory only contains headers specific to libg++ and C++ compatible system header files.
- The `cc(1)` driver has been modified to invoke the script `ld++` to demangle symbol names in ld output for a more user-friendly error messages.

### Notes

- The current version of the compiler does not support templates.
- Most system headers still require that Objective-C++ source files wrap inclusions to them with the `extern "C"` or `extern "Objective-C"` syntax. For example:

```cpp
extern "C" {
#import <stddef.h>
#import <streams/streams.h>
};
```

---

## Notes Specific to Release 3.0

The Release 3.0 Objective-C++ compiler is based on version 1.39.1 of the GNU Compiler which directly supports version 2.0 of the C++ language, as specified by AT&T (the Release 2.0 compiler was based on version 1.36.4 of G++). The GNU compiler has been extended to recognize Objective-C constructs within C++ source files.

### New Features

#### Integrated Compiler Driver

The Release 3.0 compiler driver `/bin/cc` handles C, Objective-C, C++ and Objective-C++ source files. The C++ compiler driver program `/bin/cc++` is no longer needed (but is still present for compatibility). The compiler driver determines the appropriate language from the suffix of the source files or command line options. Files ending in `.c` are taken to be C source files, `.m` indicates an Objective-C source file, and `.cc`, `.cxx`, and `.C` are recognized as C++ source files. The `-ObjC` flag specifies that source files should be considered to be Objective C source files regardless of their extension. Similarly, the `-ObjC+` flag specifies that source files should be considered to be Objective-C++ source files. The `/bin/cc++` driver is simply a shell script which calls the `/bin/cc` driver with the `-ObjC+` flag.

#### New Constructor and Destructor Sections

The Release 3.0 compiler no longer uses the collect program to gather references to C++ constructors and destructors (which requires linking twice). The compiler now automatically places these references in the new `__constructor` and `__destructor` sections of the `__TEXT` segment, which are automatically coalesced by the linker. Link time should be substantially decreased.

C++ constructors are called from startup routines in `crt0.o`, and destructors are registered to be called in the reverse order when the program terminates using `atexit()`. The archive library `/usr/lib/libc++.a` present in Release 2.0 is no longer needed.
