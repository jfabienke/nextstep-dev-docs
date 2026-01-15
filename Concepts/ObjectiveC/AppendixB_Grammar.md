# Appendix B: Reference Manual for the Objective C Language

This appendix presents a formal grammar for the Objective C extensions to the C language—as the Objective C language is implemented for the NEXTSTEP development environment. It adds to the grammar for ANSI standard C found in Appendix A of *The C Programming Language* (second edition, 1988) by Brian W. Kernighan and Dennis M. Ritchie, published by Prentice Hall, and should be read in conjunction with that book.

The Objective C extensions introduce some new symbols (such as *class-interface*), but also make use of symbols (such as *function-definition*) that are explained in the standard C grammar. The symbols mentioned but not explained here are listed below:

- compound statement
- identifier
- constant
- parameter-type-list
- declaration
- string
- declaration-list
- struct-declaration-list
- enum-specifier
- struct-or-union
- expression
- typedef-name
- function-definition
- type-name

Of these, **identifier** and **string** are undefined terminal symbols. Objective C adds no undefined terminal symbols of its own.

## Notational Conventions

Two notational conventions used here differ from those used in *The C Programming Language*:

- Literal symbols are shown in **bold type**.
- Brackets enclose optional elements and are in *italic type*. Literal brackets, like other literal symbols, are nonitalic and **bold**.

Otherwise, this appendix follows the conventions of the C reference manual. Each part of the grammar consists of a symbol followed by a colon and an indented list of mutually-exclusive possibilities for expanding the symbol. For example:

**receiver**:
&nbsp;&nbsp;&nbsp;&nbsp;*expression*
&nbsp;&nbsp;&nbsp;&nbsp;*class-name*
&nbsp;&nbsp;&nbsp;&nbsp;**super**

However, there is an exception: Even though they're not mutually exclusive, the constituents of classes, categories, and protocols are listed on separate lines to clearly show the ordering of elements. For example:

**protocol-declaration**:
&nbsp;&nbsp;&nbsp;&nbsp;**@protocol** *protocol-name*
&nbsp;&nbsp;&nbsp;&nbsp;[*protocol-reference-list*]
&nbsp;&nbsp;&nbsp;&nbsp;[*interface-declaration-list*]
&nbsp;&nbsp;&nbsp;&nbsp;**@end**

This exception to the general rule is easily recognized since each list terminates with **@end**.

There are just four entry points where the Objective C language modifies the rules defined for standard C:

1. External declarations
2. Type specifiers
3. Type qualifiers
4. Primary expressions

---

## External Declarations

**external-declaration**:
&nbsp;&nbsp;&nbsp;&nbsp;*function-definition*
&nbsp;&nbsp;&nbsp;&nbsp;*declaration*
&nbsp;&nbsp;&nbsp;&nbsp;*class-interface*
&nbsp;&nbsp;&nbsp;&nbsp;*class-implementation*
&nbsp;&nbsp;&nbsp;&nbsp;*category-interface*
&nbsp;&nbsp;&nbsp;&nbsp;*category-implementation*
&nbsp;&nbsp;&nbsp;&nbsp;*protocol-declaration*
&nbsp;&nbsp;&nbsp;&nbsp;*class-declaration-list*

**class-interface**:
&nbsp;&nbsp;&nbsp;&nbsp;**@interface** *class-name* [ **:** *superclass-name* ]
&nbsp;&nbsp;&nbsp;&nbsp;[*protocol-reference-list*]
&nbsp;&nbsp;&nbsp;&nbsp;[*instance-variables*]
&nbsp;&nbsp;&nbsp;&nbsp;[*interface-declaration-list*]
&nbsp;&nbsp;&nbsp;&nbsp;**@end**

**class-implementation**:
&nbsp;&nbsp;&nbsp;&nbsp;**@implementation** *class-name* [ **:** *superclass-name* ]
&nbsp;&nbsp;&nbsp;&nbsp;[*instance-variables*]
&nbsp;&nbsp;&nbsp;&nbsp;[*implementation-definition-list*]
&nbsp;&nbsp;&nbsp;&nbsp;**@end**

**category-interface**:
&nbsp;&nbsp;&nbsp;&nbsp;**@interface** *class-name* **(** *category-name* **)**
&nbsp;&nbsp;&nbsp;&nbsp;[*protocol-reference-list*]
&nbsp;&nbsp;&nbsp;&nbsp;[*interface-declaration-list*]
&nbsp;&nbsp;&nbsp;&nbsp;**@end**

**category-implementation**:
&nbsp;&nbsp;&nbsp;&nbsp;**@implementation** *class-name* **(** *category-name* **)**
&nbsp;&nbsp;&nbsp;&nbsp;[*implementation-definition-list*]
&nbsp;&nbsp;&nbsp;&nbsp;**@end**

**protocol-declaration**:
&nbsp;&nbsp;&nbsp;&nbsp;**@protocol** *protocol-name*
&nbsp;&nbsp;&nbsp;&nbsp;[*protocol-reference-list*]
&nbsp;&nbsp;&nbsp;&nbsp;[*interface-declaration-list*]
&nbsp;&nbsp;&nbsp;&nbsp;**@end**

**class-declaration-list**:
&nbsp;&nbsp;&nbsp;&nbsp;**@class** *class-list* **;**

**class-list**:
&nbsp;&nbsp;&nbsp;&nbsp;*class-name*
&nbsp;&nbsp;&nbsp;&nbsp;*class-list* **,** *class-name*

**protocol-reference-list**:
&nbsp;&nbsp;&nbsp;&nbsp;**<** *protocol-list* **>**

**protocol-list**:
&nbsp;&nbsp;&nbsp;&nbsp;*protocol-name*
&nbsp;&nbsp;&nbsp;&nbsp;*protocol-list* **,** *protocol-name*

**class-name**:
&nbsp;&nbsp;&nbsp;&nbsp;*identifier*

**superclass-name**:
&nbsp;&nbsp;&nbsp;&nbsp;*identifier*

**category-name**:
&nbsp;&nbsp;&nbsp;&nbsp;*identifier*

**protocol-name**:
&nbsp;&nbsp;&nbsp;&nbsp;*identifier*

**instance-variables**:
&nbsp;&nbsp;&nbsp;&nbsp;**{** [*visibility-specification*] *struct-declaration-list* [*instance-variables*] **}**

**visibility-specification**:
&nbsp;&nbsp;&nbsp;&nbsp;**@private**
&nbsp;&nbsp;&nbsp;&nbsp;**@protected**
&nbsp;&nbsp;&nbsp;&nbsp;**@public**

**interface-declaration-list**:
&nbsp;&nbsp;&nbsp;&nbsp;*declaration*
&nbsp;&nbsp;&nbsp;&nbsp;*method-declaration*
&nbsp;&nbsp;&nbsp;&nbsp;*interface-declaration-list* *declaration*
&nbsp;&nbsp;&nbsp;&nbsp;*interface-declaration-list* *method-declaration*

**method-declaration**:
&nbsp;&nbsp;&nbsp;&nbsp;*class-method-declaration*
&nbsp;&nbsp;&nbsp;&nbsp;*instance-method-declaration*

**class-method-declaration**:
&nbsp;&nbsp;&nbsp;&nbsp;**+** [*method-type*] *method-selector* **;**

**instance-method-declaration**:
&nbsp;&nbsp;&nbsp;&nbsp;**-** [*method-type*] *method-selector* **;**

**implementation-definition-list**:
&nbsp;&nbsp;&nbsp;&nbsp;*function-definition*
&nbsp;&nbsp;&nbsp;&nbsp;*declaration*
&nbsp;&nbsp;&nbsp;&nbsp;*method-definition*
&nbsp;&nbsp;&nbsp;&nbsp;*implementation-definition-list* *function-definition*
&nbsp;&nbsp;&nbsp;&nbsp;*implementation-definition-list* *declaration*
&nbsp;&nbsp;&nbsp;&nbsp;*implementation-definition-list* *method-definition*

**method-definition**:
&nbsp;&nbsp;&nbsp;&nbsp;*class-method-definition*
&nbsp;&nbsp;&nbsp;&nbsp;*instance-method-definition*

**class-method-definition**:
&nbsp;&nbsp;&nbsp;&nbsp;**+** [*method-type*] *method-selector* [*declaration-list*] *compound-statement*

**instance-method-definition**:
&nbsp;&nbsp;&nbsp;&nbsp;**-** [*method-type*] *method-selector* [*declaration-list*] *compound-statement*

**method-selector**:
&nbsp;&nbsp;&nbsp;&nbsp;*unary-selector*
&nbsp;&nbsp;&nbsp;&nbsp;*keyword-selector* [ **, ...** ]
&nbsp;&nbsp;&nbsp;&nbsp;*keyword-selector* [ **,** *parameter-type-list* ]

**unary-selector**:
&nbsp;&nbsp;&nbsp;&nbsp;*selector*

**keyword-selector**:
&nbsp;&nbsp;&nbsp;&nbsp;*keyword-declarator*
&nbsp;&nbsp;&nbsp;&nbsp;*keyword-selector* *keyword-declarator*

**keyword-declarator**:
&nbsp;&nbsp;&nbsp;&nbsp;**:** [*method-type*] *identifier*
&nbsp;&nbsp;&nbsp;&nbsp;*selector* **:** [*method-type*] *identifier*

**selector**:
&nbsp;&nbsp;&nbsp;&nbsp;*identifier*

**method-type**:
&nbsp;&nbsp;&nbsp;&nbsp;**(** *type-name* **)**

---

## Type Specifiers

**type-specifier**:
&nbsp;&nbsp;&nbsp;&nbsp;**void**
&nbsp;&nbsp;&nbsp;&nbsp;**char**
&nbsp;&nbsp;&nbsp;&nbsp;**short**
&nbsp;&nbsp;&nbsp;&nbsp;**int**
&nbsp;&nbsp;&nbsp;&nbsp;**long**
&nbsp;&nbsp;&nbsp;&nbsp;**float**
&nbsp;&nbsp;&nbsp;&nbsp;**double**
&nbsp;&nbsp;&nbsp;&nbsp;**signed**
&nbsp;&nbsp;&nbsp;&nbsp;**unsigned**
&nbsp;&nbsp;&nbsp;&nbsp;**id** [*protocol-reference-list*]
&nbsp;&nbsp;&nbsp;&nbsp;*class-name* [*protocol-reference-list*]
&nbsp;&nbsp;&nbsp;&nbsp;*struct-or-union-specifier*
&nbsp;&nbsp;&nbsp;&nbsp;*enum-specifier*
&nbsp;&nbsp;&nbsp;&nbsp;*typedef-name*

**struct-or-union-specifier**:
&nbsp;&nbsp;&nbsp;&nbsp;*struct-or-union* [*identifier*] **{** *struct-declaration-list* **}**
&nbsp;&nbsp;&nbsp;&nbsp;*struct-or-union* [*identifier*] **{ @defs (** *class-name* **) }**
&nbsp;&nbsp;&nbsp;&nbsp;*struct-or-union* *identifier*

---

## Type Qualifiers

**type-qualifier**:
&nbsp;&nbsp;&nbsp;&nbsp;**const**
&nbsp;&nbsp;&nbsp;&nbsp;**volatile**
&nbsp;&nbsp;&nbsp;&nbsp;*protocol-qualifier*

**protocol-qualifier**:
&nbsp;&nbsp;&nbsp;&nbsp;**in**
&nbsp;&nbsp;&nbsp;&nbsp;**out**
&nbsp;&nbsp;&nbsp;&nbsp;**inout**
&nbsp;&nbsp;&nbsp;&nbsp;**bycopy**
&nbsp;&nbsp;&nbsp;&nbsp;**oneway**

---

## Primary Expressions

**primary-expression**:
&nbsp;&nbsp;&nbsp;&nbsp;*identifier*
&nbsp;&nbsp;&nbsp;&nbsp;*constant*
&nbsp;&nbsp;&nbsp;&nbsp;*string*
&nbsp;&nbsp;&nbsp;&nbsp;**(** *expression* **)**
&nbsp;&nbsp;&nbsp;&nbsp;**self**
&nbsp;&nbsp;&nbsp;&nbsp;*message-expression*
&nbsp;&nbsp;&nbsp;&nbsp;*selector-expression*
&nbsp;&nbsp;&nbsp;&nbsp;*protocol-expression*
&nbsp;&nbsp;&nbsp;&nbsp;*encode-expression*

**message-expression**:
&nbsp;&nbsp;&nbsp;&nbsp;**[** *receiver* *message-selector* **]**

**receiver**:
&nbsp;&nbsp;&nbsp;&nbsp;*expression*
&nbsp;&nbsp;&nbsp;&nbsp;*class-name*
&nbsp;&nbsp;&nbsp;&nbsp;**super**

**message-selector**:
&nbsp;&nbsp;&nbsp;&nbsp;*selector*
&nbsp;&nbsp;&nbsp;&nbsp;*keyword-argument-list*

**keyword-argument-list**:
&nbsp;&nbsp;&nbsp;&nbsp;*keyword-argument*
&nbsp;&nbsp;&nbsp;&nbsp;*keyword-argument-list* *keyword-argument*

**keyword-argument**:
&nbsp;&nbsp;&nbsp;&nbsp;*selector* **:** *expression*
&nbsp;&nbsp;&nbsp;&nbsp;**:** *expression*

**selector-expression**:
&nbsp;&nbsp;&nbsp;&nbsp;**@selector (** *selector-name* **)**

**selector-name**:
&nbsp;&nbsp;&nbsp;&nbsp;*selector*
&nbsp;&nbsp;&nbsp;&nbsp;*keyword-name-list*

**keyword-name-list**:
&nbsp;&nbsp;&nbsp;&nbsp;*keyword-name*
&nbsp;&nbsp;&nbsp;&nbsp;*keyword-name-list* *keyword-name*

**keyword-name**:
&nbsp;&nbsp;&nbsp;&nbsp;*selector* **:**
&nbsp;&nbsp;&nbsp;&nbsp;**:**

**protocol-expression**:
&nbsp;&nbsp;&nbsp;&nbsp;**@protocol (** *protocol-name* **)**

**encode-expression**:
&nbsp;&nbsp;&nbsp;&nbsp;**@encode (** *type-name* **)**

---
*Release 3.3 Copyright ©1995 NeXT Computer, Inc. All Rights Reserved.*
