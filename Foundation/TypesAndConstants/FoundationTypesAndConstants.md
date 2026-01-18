# Foundation Kit Types and Constants

**Enterprise Objects Framework Release 1.0**
**Copyright ©1994 by NeXT Computer, Inc. All Rights Reserved.**

---

## Contents

- [Defined Types](#defined-types)
  - [NSComparisonResult](#nscomparisonresult)
  - [NSRange](#nsrange)
  - [NSStringEncoding](#nsstringencoding)
- [Symbolic Constants](#symbolic-constants)
  - [NSHashStringLength](#nshashstringlength)
  - [NSMaximumStringLength](#nsmaximumstringlength)
  - [NSNotFound](#nsnotfound)
- [Enumerations](#enumerations)
  - [NSStringSearchFlags](#nsstringsearchflags)

---

## Defined Types

### NSComparisonResult

**Declared In:** `foundation/NSObject.h`

```c
typedef enum _NSComparisonResult {
    NSOrderedAscending = -1,
    NSOrderedSame,
    NSOrderedDescending
} NSComparisonResult
```

An `NSComparisonResult` indicates how items in a request are ordered, from the first one given in a method invocation or function call to the last (that is, left-to-right in code).

**Values:**
- `NSOrderedAscending` - The left operand is smaller than the right operand
- `NSOrderedSame` - The operands are equal
- `NSOrderedDescending` - The left operand is greater than the right operand

---

### NSRange

**Declared In:** `foundation/NSRange.h`

```c
typedef struct _NSRange {
    unsigned int location;
    unsigned int length;
} NSRange
```

An `NSRange` describes a portion of a series—such as characters in a string or objects in an `NSArray`. Its `location` member gives the start index (0 is the first, as in C arrays), and its `length` member gives the number of items in the range (and can be zero).

**Members:**
- `location` - The starting index (0-based)
- `length` - The number of items in the range

**See also:** `NSIntersectionRange()`, `NSUnionRange()`, `NSLocationInRange()`, `NSMaxRange()`, `NSStringFromRange()`

---

### NSStringEncoding

**Declared In:** `foundation/NSString.h`

```c
typedef enum _NSStringEncoding {
    NSUnicodeStringEncoding = 0,
    NSASCIIStringEncoding,
    NSNEXTSTEPStringEncoding,
    NSEUCStringEncoding,
    NSUTFStringEncoding,
    NSISOLatin1StringEncoding,
    NSSymbolStringEncoding,
    NSNonLossyASCIIStringEncoding
} NSStringEncoding
```

These values represent the various character encodings supported by the `NSString` classes. These encodings are documented more fully elsewhere; here are some short descriptions:

| Encoding | Description |
|----------|-------------|
| `NSUnicodeStringEncoding` | The canonical Unicode encoding for string objects |
| `NSASCIIStringEncoding` | Strict 7-bit ASCII encoding within 8-bit chars |
| `NSNEXTSTEPStringEncoding` | 8-bit ASCII encoding with NEXTSTEP extensions (see Appendix C of the NEXTSTEP General Reference) |
| `NSEUCStringEncoding` | 8-bit EUC encoding for Japanese text |
| `NSUTFStringEncoding` | An 8-bit representation of Unicode characters, suitable for transmission or storage by ASCII-based systems |
| `NSISOLatin1StringEncoding` | 8-bit ISO Latin 1 encoding |
| `NSSymbolStringEncoding` | <<forthcoming>> |
| `NSNonLossyASCIIStringEncoding` | <<forthcoming>> |

**See also:** `NSString` class cluster, `defaultCStringEncoding` (NSString)

---

## Symbolic Constants

### NSHashStringLength

**Declared In:** `foundation/NSString.h`

```c
NSHashStringLength
```

---

### NSMaximumStringLength

**Declared In:** `foundation/NSString.h`

```c
NSMaximumStringLength
```

`NSMaximumStringLength` is the greatest possible length for an `NSString`.

---

### NSNotFound

**Declared In:** `foundation/NSObject.h`

```c
NSNotFound
```

A method or function return value of `NSNotFound` indicates that the item requested couldn't be found or doesn't exist. It's typically used by various methods and functions that search for items in serial data and return indices, such as characters in a string object or ids in an `NSArray`.

**Example:**

```objc
NSString *myString = @"Hello, World!";
NSRange range = [myString rangeOfString:@"Goodbye"];
if (range.location == NSNotFound) {
    // String not found
}
```

---

## Enumerations

### NSStringSearchFlags

**Declared In:** `foundation/NSString.h`

```c
enum {
    NSCaseInsensitiveSearch = 1,
    NSLiteralSearch = 2,
    NSBackwardsSearch = 4,
    NSAnchoredSearch = 8
};
```

These values represent the options available to many of the string classes' searching and comparison methods. See the `NSString` class cluster description for details on the effects of these options.

**Flags:**

| Flag | Description |
|------|-------------|
| `NSCaseInsensitiveSearch` | Ignores case distinctions when comparing strings |
| `NSLiteralSearch` | Performs a byte-for-byte comparison; cannot be combined with other options |
| `NSBackwardsSearch` | Searches from the end of the string toward the beginning |
| `NSAnchoredSearch` | Searches are anchored to the start (or end if combined with `NSBackwardsSearch`) of the string |

**Usage:**

These flags can be combined using the bitwise OR operator (`|`):

```objc
NSRange range = [myString rangeOfString:@"hello"
                                options:NSCaseInsensitiveSearch | NSBackwardsSearch];
```

**See also:** `rangeOfString:options:` (NSString), `compare:options:` (NSString)
