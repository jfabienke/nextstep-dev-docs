# 3.3 Release Notes: Reorganization of Header File Directories

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

This file contains information about the Release 3.0 reorganization of header file directories. There is no additional information specific to Release 3.1, 3.2 or 3.3.

---

## Notes Specific to Release 3.0

In Software Release 3, NeXT has reorganized the directories containing the header files. The goals of the reorganization were to clean up the ad-hoc approach to the placement of header files, to support additional architectures, and to group header files by category. Another important goal was to maintain a high degree of source compatibility with Release 2.

### Incompatible Changes

Some header files that should be used only inside NeXT have been removed from the release.

### New Features

A new directory, `/NextDeveloper/Headers`, has been created, and header files have been moved there from `/usr/include`. (`/usr/include` is now a symbolic link to `/NextDeveloper/Headers`.)

`/NextDeveloper/Headers` contains only directories. Header files that used to exist directly under `/usr/include` have been moved, generally to `/NextDeveloper/Headers/bsd` or `/NextDeveloper/Headers/ansi`. Source files should recompile without change, as long as they don't use the absolute path `/usr/include/....`

### Implementation Changes

To maintain compatibility with existing source, the default search path for included files has been changed, and a set of compatibility header files has been added. The directories in the new search path are listed below, in the order they're searched:

```
/NextDeveloper/Headers
/NextDeveloper/Headers/ansi
/NextDeveloper/Headers/bsd
/LocalDeveloper/Headers
/NextDeveloper/2.0CompatibleHeaders
/usr/include
/usr/local/include
```

The following examples show how the `#include` directive is resolved:

```c
#include <mach/cthreads.h>
```
results in the inclusion of `/NextDeveloper/Headers/mach/cthreads.h`.

```c
#include <stdio.h>
```
results in the inclusion of `/NextDeveloper/Headers/ansi/stdio.h`.

```c
#include <sys/proc.h>
```
results in the inclusion of `/NextDeveloper/Headers/bsd/sys/proc.h`.

```c
#include <nlist.h>
```
results in the inclusion of `/NextDeveloper/2.0CompatibleHeaders/nlist.h`. This compatibility header includes the new file (`<mach-o/nlist.h>`) and warns that a compatibility header file was included in the source.

If a source file uses the absolute pathname `/usr/include/...` to include a header file, it should be changed. For example:

```c
#include "/usr/include/stdio.h"     // --> #include <stdio.h>
#include "/usr/include/errno.h"     // --> #include <errno.h>
#include "/usr/include/sys/file.h"  // --> #include <sys/file.h>
```

In cases where the source file can't be modified, a workaround is to add symbolic links to the `/NextDeveloper/Headers` directory. This will restore full pathname references at the expense of clutter. For example, at a shell prompt, enter:

```bash
ln -s ansi/stdio.h /NextDeveloper/Headers/stdio.h
```

> **Note:** The inclusion of `/NextDeveloper/2.0CompatibleHeaders` in the search path is temporary. In the future, you'll have to explicitly use the flag `-I/NextDeveloper/2.0CompatibleHeaders` if you don't change your source code to refer to the new names of these header files.
