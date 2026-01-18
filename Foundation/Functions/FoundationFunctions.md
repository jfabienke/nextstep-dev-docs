# Foundation Kit Functions

**Enterprise Objects Framework Release 1.0**
**Copyright ©1994 by NeXT Computer, Inc. All Rights Reserved.**

---

## Contents

- [Object Allocation and Memory Management](#object-allocation-and-memory-management)
- [Assertion Macros](#assertion-macros)
- [Runtime Introspection](#runtime-introspection)
- [Memory Zones](#memory-zones)
- [Exception Handling](#exception-handling)
- [Reference Counting](#reference-counting)
- [Range Utilities](#range-utilities)
- [Logging and Debugging](#logging-and-debugging)
- [Path Utilities](#path-utilities)

---

## Object Allocation and Memory Management

### NSAllocateObject()

**Declared In:** `foundation/NSObject.h`

```c
id NSAllocateObject(Class class, unsigned extraBytes, NSZone *zone)
```

Allocates and returns a pointer to an instance of `class`, created in the specified zone (or in the default zone, if `zone` is `NULL`). The `extraBytes` argument (usually zero) states the number of extra bytes required for indexed instance variables. Returns `nil` on failure.

**Parameters:**
- `class` - The class to instantiate
- `extraBytes` - Number of extra bytes for indexed instance variables (usually 0)
- `zone` - The memory zone to allocate in, or `NULL` for default zone

**Returns:** A pointer to an instance of `class`, or `nil` upon failure.

**See also:** `NSDeallocateObject()`, `NSCopyObject()`

---

### NSDeallocateObject()

**Declared In:** `foundation/NSObject.h`

```c
void NSDeallocateObject(id <NSObject> anObject)
```

Deallocates `anObject`, which must have been allocated using `NSAllocateObject()`.

**Parameters:**
- `anObject` - The object to deallocate

**Returns:** `void`

**See also:** `NSAllocateObject()`, `NSCopyObject()`

---

### NSCopyObject()

**Declared In:** `foundation/NSObject.h`

```c
NSObject *NSCopyObject(NSObject *anObject, unsigned extraBytes, NSZone *zone)
```

Creates and returns a new object that's an exact copy of `anObject`, created in the specified zone (or in the default zone, if `zone` is `NULL`). The `extraBytes` argument (usually zero) states the number of extra bytes required for indexed instance variables. Returns `nil` on failure.

**Parameters:**
- `anObject` - The object to copy
- `extraBytes` - Number of extra bytes for indexed instance variables (usually 0)
- `zone` - The memory zone to allocate in, or `NULL` for default zone

**Returns:** A pointer to a new `NSObject` that is an exact copy of `anObject`.

**See also:** `NSAllocateObject()`, `NSDeallocateObject()`

---

## Assertion Macros

**Declared In:** `foundation/NSExceptions.h`

### Overview

Assertions evaluate a condition and, if the condition evaluates to false, call the assertion handler for the current thread, passing it a format string and a variable number of arguments. Each thread has its own assertion handler, which is an object of class `NSAssertionHandler`. When invoked, an assertion handler prints an error message that includes method and class (or function name). It then raises an exception of type `NSInternalInconsistencyException`.

An assortment of macros evaluate the condition and serve as a front end to the assertion handler. These macros fall into two types: those for use within Objective-C methods (`NSAssert`), and those for use within C functions (`NSCAssert`). `NSAssert()` and `NSCAssert()` take no arguments other than the condition and the format string. The other macros take the number of format-string arguments as indicated by *n*.

**Important:** Assertions are compiled into code only if the preprocessor macro `DEBUG` is defined.

### Objective-C Method Assertions

```c
NSAssert(condition, NSString *description)
NSAssert1(condition, NSString *description, arg1)
NSAssert2(condition, NSString *description, arg1, arg2)
NSAssert3(condition, NSString *description, arg1, arg2, arg3)
NSAssert4(condition, NSString *description, arg1, arg2, arg3, arg4)
NSAssert5(condition, NSString *description, arg1, arg2, arg3, arg4, arg5)
```

**Parameters:**
- `condition` - An expression that evaluates to true or false
- `description` - A `printf()`-style format string that describes the failure condition
- `arg1...arg5` - Arguments to be inserted into the description

### C Function Assertions

```c
NSCAssert(condition, NSString *description)
NSCAssert1(condition, NSString *description, arg1)
NSCAssert2(condition, NSString *description, arg1, arg2)
NSCAssert3(condition, NSString *description, arg1, arg2, arg3)
NSCAssert4(condition, NSString *description, arg1, arg2, arg3, arg4)
NSCAssert5(condition, NSString *description, arg1, arg2, arg3, arg4, arg5)
```

### Parameter Validation Assertions

```c
NSParameterAssert(condition)
NSCParameterAssert(condition)
```

`NSParameterAssert()` and `NSCParameterAssert()` are assertion macros that validate parameters, one within Objective-C methods and the other within C functions. Simply provide the parameter as the condition argument. The macro evaluates the parameter and, if it is false, it logs an error message which includes the parameter and raises an exception.

**Returns:** All macros return `void`.

**See also:** `NSRaise()`, `NSRaisev()`, `NSLog()`, `NSLogv()`

---

## Runtime Introspection

### NSClassFromString()

**Declared In:** `foundation/NSObjCRuntime.h`

```c
Class NSClassFromString(NSString *aClassName)
```

Returns the class object named by `aClassName`, or `nil` if none by this name is currently loaded.

**Parameters:**
- `aClassName` - The name of the class to retrieve

**Returns:** The class object, or `nil` if not found.

**See also:** `NSStringFromClass()`

---

### NSStringFromClass()

**Declared In:** `foundation/NSObjCRuntime.h`

```c
NSString *NSStringFromClass(Class aClass)
```

Returns an `NSString` containing the name of `aClass`.

**Parameters:**
- `aClass` - The class to get the name of

**Returns:** An `NSString` containing the class name.

**See also:** `NSClassFromString()`

---

### NSSelectorFromString()

**Declared In:** `foundation/NSObjCRuntime.h`

```c
SEL NSSelectorFromString(NSString *aSelectorName)
```

Returns the selector named by `aSelectorName`, or zero if none by this name exists.

**Parameters:**
- `aSelectorName` - The name of the selector to retrieve

**Returns:** The selector, or zero if not found.

**See also:** `NSStringFromSelector()`

---

### NSStringFromSelector()

**Declared In:** `foundation/NSObjCRuntime.h`

```c
NSString *NSStringFromSelector(SEL aSelector)
```

Returns an `NSString` containing the name of `aSelector`.

**Parameters:**
- `aSelector` - The selector to get the name of

**Returns:** An `NSString` containing the selector name.

**See also:** `NSSelectorFromString()`

---

## Memory Zones

### NSCreateZone()

**Declared In:** `foundation/NSZone.h`

```c
NSZone *NSCreateZone(unsigned startSize, unsigned granularity, BOOL canFree)
```

Creates and returns a pointer to a new zone of `startSize` bytes, which will grow and shrink by `granularity` bytes. If `canFree` is zero, the allocator will never free memory, and `malloc()` will be fast.

**Parameters:**
- `startSize` - Initial size of the zone in bytes
- `granularity` - Amount to grow/shrink the zone by in bytes
- `canFree` - If zero, memory will never be freed (faster allocation)

**Returns:** A pointer to a new `NSZone`.

**See also:** `NSDefaultMallocZone()`, `NSRecycleZone()`, `NSSetZoneName()`

---

### NSDefaultMallocZone()

**Declared In:** `foundation/NSZone.h`

```c
NSZone *NSDefaultMallocZone(void)
```

Returns the default zone, which is created automatically at startup. This is the zone used by the standard C function `malloc()`.

**Returns:** A pointer to the default zone.

**See also:** `NSCreateZone()`

---

### NSRecycleZone()

**Declared In:** `foundation/NSZone.h`

```c
void NSRecycleZone(NSZone *zone)
```

Frees `zone` after adding any of its pointers still in use to the default zone. (This strategy prevents retained objects from being inadvertently destroyed.)

**Parameters:**
- `zone` - The zone to recycle

**Returns:** `void`

**See also:** `NSCreateZone()`, `NSZoneMalloc()`

---

### NSSetZoneName()

**Declared In:** `foundation/NSZone.h`

```c
void NSSetZoneName(NSZone *zone, NSString *name)
```

Sets the specified zone's name to `name`, which can aid in debugging.

**Parameters:**
- `zone` - The zone to name
- `name` - The name to assign to the zone

**Returns:** `void`

**See also:** `NSZoneName()`

---

### NSZoneName()

**Declared In:** `foundation/NSZone.h`

```c
NSString *NSZoneName(NSZone *zone)
```

Returns the name of the specified zone as an `NSString`.

**Parameters:**
- `zone` - The zone to query

**Returns:** An `NSString` containing the zone's name.

**See also:** `NSSetZoneName()`

---

### NSZoneCalloc()

**Declared In:** `foundation/NSZone.h`

```c
void *NSZoneCalloc(NSZone *zone, unsigned numElems, unsigned byteSize)
```

Allocates enough memory from `zone` for `numElems` elements, each with a size `numBytes` bytes, and returns a pointer to the allocated memory. The memory is initialized with zeros.

**Parameters:**
- `zone` - The zone to allocate from
- `numElems` - Number of elements to allocate
- `byteSize` - Size of each element in bytes

**Returns:** A pointer to the newly-allocated block of memory, or `nil` if the operation was unable to allocate the requested memory.

**See also:** `NSDefaultMallocZone()`, `NSRecycleZone()`, `NSZoneFree()`

---

### NSZoneMalloc()

**Declared In:** `foundation/NSZone.h`

```c
void *NSZoneMalloc(NSZone *zone, unsigned size)
```

Allocates `size` bytes in `zone`, and returns a pointer to the allocated memory.

**Parameters:**
- `zone` - The zone to allocate from
- `size` - Number of bytes to allocate

**Returns:** A pointer to the newly-allocated block of memory, or `nil` if the operation was unable to allocate the requested memory.

**See also:** `NSDefaultMallocZone()`, `NSRecycleZone()`, `NSZoneFree()`

---

### NSZoneRealloc()

**Declared In:** `foundation/NSZone.h`

```c
void *NSZoneRealloc(NSZone *zone, void *ptr, unsigned size)
```

Changes the size of the block of memory pointed to by `ptr` to `size` bytes. It may allocate new memory to replace the old, in which case it moves the contents of the old memory block to the new block, up to a maximum of `size` bytes. `ptr` may be `NULL`.

**Parameters:**
- `zone` - The zone to allocate from
- `ptr` - Pointer to existing memory block, or `NULL`
- `size` - New size in bytes

**Returns:** A pointer to the newly-allocated block of memory, or `nil` if the operation was unable to allocate the requested memory.

**See also:** `NSDefaultMallocZone()`, `NSRecycleZone()`, `NSZoneFree()`

---

### NSZoneFree()

**Declared In:** `foundation/NSZone.h`

```c
void NSZoneFree(NSZone *zone, void *pointer)
```

Returns the memory indicated by `pointer` to `zone`. The standard C function `free()` does the same, but spends time finding which zone the memory belongs to.

**Parameters:**
- `zone` - The zone to return memory to
- `pointer` - Pointer to the memory block to free

**Returns:** `void`

**See also:** `NSRecycleZone()`

---

### NSZoneFromPtr()

**Declared In:** `foundation/NSZone.h`

```c
NSZone *NSZoneFromPtr(void *pointer)
```

Returns the zone for the block of memory indicated by `pointer`, or `NULL` if the block was not allocated from a zone. The pointer must be one that was returned by a prior call to an allocation function.

**Parameters:**
- `pointer` - Pointer to a memory block

**Returns:** The zone for the indicated block of memory, or `NULL` if the block was not allocated from a zone.

**See also:** `NSZoneCalloc()`, `NSZoneMalloc()`, `NSZoneRealloc()`

---

## Exception Handling

### NSGetUncaughtExceptionHandler()

**Declared In:** `foundation/NSException.h`

```c
NSUncaughtExceptionHandler *NSGetUncaughtExceptionHandler(void)
```

Returns a pointer to the function serving as the top-level error handler. This handler will process exceptions raised outside of any exception-handling domain.

**Returns:** A pointer to the top-level error handler.

**See also:** `NSSetUncaughtExceptionHandler()`

---

### NSSetUncaughtExceptionHandler()

**Declared In:** `foundation/NSException.h`

```c
void NSSetUncaughtExceptionHandler(NSUncaughtExceptionHandler *handler)
```

Sets the top-level error-handling function to `handler`. If `handler` is `NULL` or this function is never invoked, the default top-level handler is used.

**Parameters:**
- `handler` - The function to use as the top-level error handler, or `NULL` for default

**Returns:** `void`

**See also:** `NSGetUncaughtExceptionHandler()`

---

## Reference Counting

### NSIncrementExtraRefCount()

**Declared In:** `foundation/NSObject.h`

```c
void NSIncrementExtraRefCount(id anObject)
```

Modifies the "extra reference" count of an object. Newly created objects have only one actual reference, so that a single `release` message results in the object being deallocated. Extra references are those beyond the single original reference, and are usually created by sending the object a `retain` message. Your code should generally not use these functions unless it's overriding the `retain` or `release` methods.

**Parameters:**
- `anObject` - The object whose reference count to increment

**Returns:** `void`

**See also:** `NSDecrementExtraRefCountWasZero()`

---

### NSDecrementExtraRefCountWasZero()

**Declared In:** `foundation/NSObject.h`

```c
BOOL NSDecrementExtraRefCountWasZero(id anObject)
```

Decrements the "extra reference" count of an object. Your code should generally not use this function unless it's overriding the `retain` or `release` methods.

**Parameters:**
- `anObject` - The object whose reference count to decrement

**Returns:** `NO` if `anObject` had an extra reference count. If `anObject` didn't have an extra reference count, it returns `YES`, indicating that the object should be deallocated (with `dealloc`).

**See also:** `NSIncrementExtraRefCount()`

---

### NSShouldRetainWithZone()

**Declared In:** `foundation/NSObject.h`

```c
BOOL NSShouldRetainWithZone(NSObject *anObject, NSZone *requestedZone)
```

Returns `YES` if `requestedZone` is `NULL`, the default zone, or the zone in which `anObject` was allocated. This function is typically called from inside an `NSObject`'s `copyWithZone:` method, when deciding whether to retain `anObject` as opposed to making a copy of it.

**Parameters:**
- `anObject` - The object to check
- `requestedZone` - The zone to compare against

**Returns:** `YES` if `anObject` should be retained with `requestedZone`.

---

## Range Utilities

### NSIntersectionRange()

**Declared In:** `foundation/NSRange.h`

```c
NSRange NSIntersectionRange(NSRange range1, NSRange range2)
```

Returns a range describing the intersection of `range1` and `range2`—that is, a range containing the indices that exist in both ranges. If the returned range's length field is zero, then the two ranges don't intersect, and the value of the location field is undefined.

**Parameters:**
- `range1` - First range
- `range2` - Second range

**Returns:** The resulting intersected range.

**See also:** `NSUnionRange()`

---

### NSUnionRange()

**Declared In:** `foundation/NSRange.h`

```c
NSRange NSUnionRange(NSRange range1, NSRange range2)
```

Returns a range covering all indices in and between `range1` and `range2`. If one range is completely contained in the other, the returned range is equal to the larger range.

**Parameters:**
- `range1` - First range
- `range2` - Second range

**Returns:** The resulting combined range.

**See also:** `NSIntersectionRange()`

---

### NSLocationInRange()

**Declared In:** `foundation/NSRange.h`

```c
BOOL NSLocationInRange(unsigned index, NSRange aRange)
```

Returns `YES` if the given `index` lies within `aRange`—that is, if it's greater than or equal to `aRange.location` and less than `aRange.location` plus `aRange.length`.

**Parameters:**
- `index` - The index to check
- `aRange` - The range to check against

**Returns:** `YES` if `index` is within `aRange`, `NO` otherwise.

**See also:** `NSMaxRange()`

---

### NSMaxRange()

**Declared In:** `foundation/NSRange.h`

```c
unsigned NSMaxRange(NSRange aRange)
```

Returns the location plus the length of `aRange`. This is the index for the item just past the end of the range, not the last item in the range. Note that `NSMaxRange()` can easily overflow if `aRange.length` is a large value.

**Parameters:**
- `aRange` - The range to query

**Returns:** The maximum range value (location + length).

**See also:** `NSLocationInRange()`

---

### NSStringFromRange()

**Declared In:** `foundation/NSRange.h`

```c
NSString *NSStringFromRange(NSRange range)
```

Returns a string representation of the specified range.

**Parameters:**
- `range` - The range to convert

**Returns:** A string of the form: `"{location = a; length = b}"`, where `a` and `b` are non-negative integers.

---

## Logging and Debugging

### NSLog()

**Declared In:** `foundation/NSUtilities.h`

```c
extern void NSLog(NSString *format, ...)
```

Logs an error message to `stderr`. The message consists of a timestamp and the process ID prefixed to the string you pass in. You compose this string with a format string and a variable number of arguments.

**Parameters:**
- `format` - A `printf()`-style format string
- `...` - One or more arguments to be inserted into the string

**Returns:** `void`

**See also:** `NSLogv()`, `NSRaise()`, `NSRaisev()`

---

### NSLogv()

**Declared In:** `foundation/NSUtilities.h`

```c
extern void NSLogv(NSString *format, va_list args)
```

Logs an error message to `stderr`. The message consists of a timestamp and the process ID prefixed to the string you pass in. You compose this string with a format string and a variable argument list. `NSLog()` simply passes along a variable number of arguments to `NSLogv()`.

**Parameters:**
- `format` - A `printf()`-style format string
- `args` - A `va_list` of arguments to be inserted into the string

**Returns:** `void`

**See also:** `NSLog()`, `NSRaise()`, `NSRaisev()`

---

### NSRaise()

**Declared In:** `foundation/NSExceptions.h`

```c
extern void NSRaise(unsigned int exceptionCode, NSString *format, ...)
```

Logs an error message and then raises an exception (`NX_RAISE()`). Use `NSRaise()` in place of `NX_RAISE()`.

**Parameters:**
- `exceptionCode` - A value of type `NSException` that indicates the basis of the error
- `format` - A `printf()`-style format string
- `...` - One or more arguments to be inserted into the string

**Returns:** `void`

**See also:** `NSRaisev()`, `NSAssert()`, `NSLog()`, `NSLogv()`

---

### NSRaisev()

**Declared In:** `foundation/NSExceptions.h`

```c
extern void NSRaisev(unsigned int exceptionCode, NSString *format, va_list args)
```

Logs an error message and then raises an exception (`NX_RAISE()`). `NSRaise()` simply passes along a variable number of arguments to `NSRaisev()`.

**Parameters:**
- `exceptionCode` - A value of type `NSException` that indicates the basis of the error
- `format` - A `printf()`-style format string
- `args` - A `va_list` of arguments to be inserted into the string

**Returns:** `void`

**See also:** `NSRaise()`, `NSAssert()`, `NSLog()`, `NSLogv()`

---

## Path Utilities

### NSUserName()

**Declared In:** `foundation/NSPathUtilities.h`

```c
NSString *NSUserName(void)
```

Returns the name of the current user.

**Returns:** An `NSString` containing the current user's name.

**See also:** `NSHomeDirectory()`, `NSHomeDirectoryForUser()`

---

### NSHomeDirectory()

**Declared In:** `foundation/NSPathUtilities.h`

```c
NSString *NSHomeDirectory(void)
```

Returns a path to the current user's home directory.

**Returns:** An `NSString` containing the path to the current user's home directory.

**See also:** `NSUserName()`, `NSHomeDirectoryForUser()`

---

### NSHomeDirectoryForUser()

**Declared In:** `foundation/NSPathUtilities.h`

```c
NSString *NSHomeDirectoryForUser(NSString *userName)
```

Returns a path to the home directory for the user specified by `userName`.

**Parameters:**
- `userName` - The name of the user to query

**Returns:** An `NSString` containing the path to the specified user's home directory.

**See also:** `NSUserName()`, `NSHomeDirectory()`
