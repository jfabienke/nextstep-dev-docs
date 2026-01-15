# Assembly Language Syntax

This chapter first describes the basic lexical elements of assembly language programming, and then describes how those elements combine to form complete assembly language expressions.

The following chapter explains how sequences of expressions are put together to form the statements that make up an assembly language program.

---

## Elements of Assembly Language

This section describes the basic building blocks of an assembly language program: characters, symbols, labels, and constants.

### Characters

The following characters are used in assembly language programs:

- Alphanumeric characters: `A` through `Z`, `a` through `z`, and `0` through `9`
- Other printable ASCII characters (such as `#`, `$`, `:`, `.`, `+`, `-`, `*`, `/`, `!`, and `|`)
- Non-printing ASCII characters (such as space, tab, return, and newline)

Some of these characters have special meanings, which are described in "Expression Syntax" and in the following chapter.

### Identifiers

An identifier (also known as a symbol) can be used for several purposes:

- As the label for an assembler statement (see "Labels" below)
- As a location tag for data
- As the symbolic name of a constant

Each identifier consists of a sequence of alphanumeric characters (which may include other printable ASCII characters such as `.`, `_`, and `$`). The first character must not be numeric. Identifiers may be of any length, and all characters are significant. Letter case is significant; for example, `var` is different from `Var`.

It is also possible to define a new identifier by enclosing multiple identifiers within a pair of double quotes. For example:

```asm
"Object +new:":
.long "Object +new:"
```

### Labels

A label is written as an identifier immediately followed by a colon (`:`). The label represents the current value of the current location counter; it can be used in assembler instructions as an operand.

> **Note:** You may not use a single identifier to represent two different locations.

#### Numeric Labels

Local numeric labels allow compilers and programmers to use names temporarily. A numeric label consists of a digit (between 0 and 9) followed by a colon. These ten local symbol names can be reused any number of times throughout the program. As with alphanumeric labels, a numeric label assigns the current value of the location counter to the symbol.

Although multiple numeric labels with the same digit may be used within the same program, only the next definition and the most recent previous definition of a label can be referenced:

- To refer to the most recent previous definition of a local numeric label, write `digitb` (using the same digit as when you defined the label).
- To refer to the next definition of a numeric label, write `digitf`.

#### The Scope of a Label

The scope of a label is the distance over which it is visible to (and referenceable by) other parts of the program.

Normally, a label that tags a location or data is visible only within the current assembly unit.

The `.globl` directive (described in Chapter 4) may be used to make a label external. In this case, the symbol is visible to other assembly units at link time.

### Constants

Four types of constants are available: numeric constants, character constants, string constants, and floating point constants. All constants are interpreted as absolute quantities when they appear in an expression.

#### Numeric Constants

A numeric constant is a token that starts with a digit. Numeric constants can be decimal, hexadecimal, or octal. The following restrictions apply:

- **Decimal constants** contain only digits `0` through `9`, and normally aren't longer than 32 bits—having a value between `-2,147,483,648` and `2,147,483,647` (values that don't fit in 32 bits are **bignums**, which are legal but should fit within the designated format). Decimal constants cannot contain leading zeros or commas.
- **Hexadecimal constants** start with `0x` (or `0X`), followed by between one and eight decimal or hexadecimal digits (`0` through `9`, `a` through `f`, and `A` through `F`). Values that don't fit in 32 bits are bignums.
- **Octal constants** start with `0`, followed by from one to eleven octal digits (`0` through `7`). Values that don't fit in 32 bits are bignums.

#### Character Constants

A single-character constant consists of a single quote (`'`) followed by any ASCII character. The constant's value is the code for the given character.

#### String Constants

A string constant is a sequence of 0 or more ASCII characters surrounded by quotation marks (`"characters"`).

#### Floating Point Constants

The general lexical form of a floating point number is:

```
0flt_char[{+-}]dec...[.][dec...][exp_char[{+-}][dec...]]
```

where:

| Token | Meaning |
|-------|---------|
| `flt_char` | Required type specification character (see table below) |
| `[{+-}]` | Optional occurrence of either `+` or `-` (but not both) |
| `dec...` | Required sequence of 1 or more decimal digits |
| `[.]` | Optional `.` |
| `[dec...]` | Optional sequence of 1 or more decimal digits |
| `[exp_char]` | Optional exponent delimiter character (see table below) |

The type specification character `flt_char` specifies the type and representation of the constructed number. The legal type and exponent characters depend on processor architecture:

| Architecture | `flt_char` | `exp_char` |
|--------------|------------|------------|
| M68000 | `{rRsSfFdDxXeEpP}` | `{eE}` |
| M88000 | `{rRdDfF}` | `{eE}` |
| M98000 | `{dDfF}` | `{eE}` |
| i386 | `{fFdDxX}` | `{eE}` |
| i860 | `{rRsSfFdDxXpP}` | `{eE}` |
| hppa | `{dDfF}` | `{eE}` |

On the M68000 architecture, `0b` can be used to specify an immediate hexadecimal bit pattern. For example:

```asm
fmoves #0b7f80001,fp0
```

moves the signaling NaN into the register `fp0`, and:

```asm
fmoves #0x7f80001,fp0
```

moves the decimal number `2,139,095,041` (`0x7f80001` in hexadecimal) into the register `fp0`.

When floating-point constants are used as arguments to the `.single` and `.double` directives, the type specification character isn't actually used in determining the type of the number. For convenience, `r` or `R` can be used consistently to specify all types of floating-point numbers.

Collectively, all floating point numbers, together with quad and octal scalars, are called **Bignums**. When `as` requires a Bignum, a 32-bit scalar quantity may also be used.

Floating point constants are internally represented as **flonums**, in a machine-independent, precision-independent floating point format (for accurate cross-assembly).

### Assembly Location Counter

A single period (`.`), usually referred to as "dot," is used to represent the current location counter. There is no way to explicitly reference any other location counters besides the current location counter.

Even if it occurs in the operand field of a statement, dot refers to the address of the first byte of that statement; the value of dot isn't updated until the next machine instruction or assembler directive.

---

## Expression Syntax

Expressions are combinations of operand terms (which can be numeric constants or symbolic identifiers) and operators. This section lists the available operators, and describes the rules for combining these operators with operands to produce legal expressions.

### Operators

Identifiers and numeric constants can be combined through the use of operators to form expressions. Each operator operates on 32-bit values. If a term occupies 8 or 16 bits, it is sign-extended to a 32-bit value.

The assembler provides both unary and binary operators:

- A unary operator precedes its operand (e.g., `!var`)
- A binary operator follows its first operand and precedes its second operand (e.g., `var+5`)

#### Unary Operators

| Operator | Meaning |
|----------|---------|
| `-` | Unary minus: result is the two's complement of the operand |
| `~` | One's complement: result is the one's complement of the operand |
| `!` | Logical negation: result is `0` if the operand is non-zero, and `1` if the operand is `0` |

#### Binary Operators

| Operator | Meaning |
|----------|---------|
| `+` | Addition |
| `-` | Subtraction |
| `*` | Multiplication |
| `/` | Division (integer division, truncates toward zero) |
| `%` | Modulus (integral operands only) |
| `>>` | Right shift (integral operands only; arithmetic shift) |
| `<<` | Left shift (integral operands only) |
| `&` | Bitwise AND (integral operands only) |
| `^` | Bitwise XOR (integral operands only) |
| `\|` | Bitwise OR (integral operands only). Not usable on M68000 because `|` starts a comment on that family. |
| `<` | Less than |
| `>` | Greater than |
| `<=` | Less than or equal |
| `>=` | Greater than or equal |
| `==` | Equal |
| `!=` | Not equal (same as `<>`) |

### Terms

A term is part of an expression. It may be:

- An identifier.
- A numeric constant (its 32-bit value is used). The assembly location counter (`.`) is a valid numeric constant.
- An expression or term enclosed in parentheses. Anything in parentheses is evaluated before the rest of the expression. This can alter normal evaluation order (e.g., `x * (y + z)`), or apply a unary operator to an entire expression (e.g., `-(x * y + z)`).
- A term preceded by a unary operator (e.g., `~var`). Multiple unary operators may be used (e.g., `!~var`).

### Expressions

Expressions are combinations of terms joined by binary operators. An expression is always evaluated to a 32-bit value, but in some situations a different value will be used:

- If an operand requires a one-byte value (e.g., a `.byte` directive), the low-order 8 bits of the expression are used.
- If an operand requires a 16-bit value (e.g., a `.short` directive or a `movem` instruction), the low-order 16 bits of the expression are used.

All expressions are evaluated using the same operator precedence rules used by the C programming language.

When an expression is evaluated, its value is **absolute**, **relocatable**, or **external**, as described below.

#### Absolute Expressions

An expression is absolute if its value is fixed. Examples include:

- An expression whose terms are constants
- An identifier whose value is a constant via a direct assignment statement
- A relocatable expression minus a relocatable term, if both belong to the same program section

#### Relocatable Expressions

An expression (or term) is relocatable if its value is fixed relative to a base address, but will have an offset when it is linked or loaded into memory. For example, labels defined in relocatable sections are relocatable.

Expressions that contain relocatable terms must only add or subtract constants to their value. If identifiers `var` and `dat` are defined in a relocatable section, then:

| Example | Status | Explanation |
|---------|--------|-------------|
| `var` | relocatable | Simple relocatable term; offset from base address of current control section. |
| `var+5` | relocatable | Adding a constant preserves relocatability. |
| `var*2` | not relocatable | Multiplying a relocatable term invalidates relocatability. |
| `2-var` | not relocatable | Cannot be linked by adding `var`'s offset. |
| `var+dat+5` | relocatable (if both defined) | Used for position-independent code; supported in NEXTSTEP Release 3.3 and later. |

#### External Expressions

An expression is external (or global) if it contains an external identifier not defined in the current program. In general, the same restrictions as relocatable expressions apply.

One exception: `var-dat` is incorrect when both `var` and `dat` are external identifiers (you cannot subtract two external relocatable expressions). Also, you cannot multiply or divide any relocatable expression.

---

*Copyright ©1995 NeXT Computer, Inc. All Rights Reserved.*
