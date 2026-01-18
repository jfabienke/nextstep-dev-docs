# 3.3 Release Notes: GNU Source Code

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

This file contains release notes for building the GNU projects provided on the NEXTSTEP Developer CD-ROM in Release 3.2. There are no changes for Release 3.3.

---

## Known Problems

**Building without bison installed:** If you don't have bison already installed then you'll need to make and install it before building the compiler. To do so, cd to `/NextDeveloper/Source/GNU/cc/bison` and type `make` (since this directory will likely be owned by root you'll need to be root to do this). Then type the following commands:

```bash
mkdir /usr/local/bin  # (this may already exist)
mkdir /usr/local/lib  # (this may already exist)

cp bison.NEW /usr/local/bin/bison
cp bison.hairy bison.simple /usr/local/lib
```

**Compiler project default target:** For the compiler project in `/NextDeveloper/Source/GNU/cc/cc`, typing `make` without a target will attempt to do a `make install`, which may overwrite your existing compiler. You should therefore use `make all` instead.
