# Using the Assembler

This chapter describes how to run the `as` assembler, which produces an object file from one or more files of assembly language source code.

> **Note:** Although `a.out` is the default file name that `as` gives to the object file it creates (as is conventional with most UNIX-style compiler systems), the format of the object file is not standard UNIX 4.3BSD `a.out` format. Object files produced by the assembler are in **Mach-O (Mach object)** file format. For more information about the Mach-O file format, see the *NEXTSTEP Development Tools and Techniques* manual.

---

## Command Syntax

To run the assembler, type the following command in a shell window:

```
as [ option ] ... [ file ] ...
```

You can specify one or more command-line options (described below).

You can specify one or more files containing assembly language source code. If no files are specified, `as` uses standard input (`stdin`) for the assembly source input.

> **Note:** By convention, files containing assembly language source code should have a `.s` extension.

---

## Assembler Options

The following command-line options are recognized by the assembler:

| Option | Description |
|--------|-------------|
| `-o name` | Use `name` as the output file name instead of `a.out`. |
| `--` | Use standard input (`stdin`) for assembly source input. |
| `-f` | Fast; do not run `app` (the assembler preprocessor). Intended for compilers emitting "clean" assembly (strict whitespace rules). `app` is needed for handwritten assembly and for files preprocessed by `cpp` (the C preprocessor), which commonly occurs when assembling via `cc(1)`. The assembler preprocessor strips excess spaces, converts each single-quoted character into a decimal constant, and converts occurrences of `# number filename level` into `.line number; .file filename`. It can also be disabled by starting the assembly file with `#NO_APP\n`. Once disabled this way, it can be toggled with `#APP\n` and `#NO_APP\n` at the beginning of lines; the compiler uses this to wrap assembly emitted from `asm()` statements. |
| `-g` | Produce debugging information for `gdb(1)` so the assembly source can be debugged symbolically. For include files (included via the C preprocessor `#include` or the assembler directive `.include`) that produce instructions in the `(__TEXT,__text)` section, the include file must be included while a `.text` directive is in effect (i.e., there must be a `.text` directive before the include), and it must end with a `.text` directive in effect at end of the include file. Otherwise the debugger may have trouble with that assembly file. |
| `-v` | Print the version of the assembler (both the NeXT version and the GNU version it is based on). |
| `-n` | Do not assume that the assembly file starts with a `.text` directive. |
| `-Idir` | Add `dir` to the list of directories searched for files included with the `.include` directive. Default search order: current directory, then `/usr/include`. |
| `-L` | Save defined labels beginning with `L` (compiler-generated temporary labels). Temporary labels are normally discarded to save space in the symbol table. |
| `-W` | Suppress warnings. |

---

## Architecture Options

The program `/bin/as` is a driver that executes assemblers for specific target architectures. If no target architecture is specified, it defaults to the architecture of the host it is running on.

### -arch arch_type

Specifies the target architecture `arch_type` for the assembler to be executed. Target assemblers for each architecture are in:

```
/lib/arch_type/as
```

### -arch_multiple

Used by the `cc(1)` driver when it is run with multiple `-arch arch_type` flags. It instructs programs like `as(1)` that if they print any messages, they must precede the messages with one line stating the program name (here `as`) and the architecture (from the `-arch arch_type` flag), to distinguish which architecture the messages refer to.

This flag is accepted **only** by the actual assemblers in `/lib/arch_type/as`, and **not** by the assembler driver `/bin/as`.

---

## M68000-Specific Options

| Option | Description |
|--------|-------------|
| `-l` | For offsets from an address register that refers to an undefined symbol (e.g., `a6@(var)` where `var` is not defined in the assembly file), make the offset and relocation entry width 32 bits rather than 16 bits. |
| `-k` | Produce a warning when a statement of the form `.word symbol1-symbol2+offset` does not fit in a 16-bit word (applicable to 68000 where `.word` is 16 bits and addresses are 16 bits; not applicable on NeXT computers). |
| `-mc68000` / `-mc68010` | Do not generate branches that use 32-bit PC-relative displacements (not implemented on 68000/68010; not applicable on NeXT computers). |
| `-mc68020` | Generate branches that use 32-bit PC-relative displacements (default behavior). |

---

*Copyright ©1995 NeXT Computer, Inc. All Rights Reserved.*
