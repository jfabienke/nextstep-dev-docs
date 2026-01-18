# NSEnumerator

**Enterprise Objects Framework Release 1.0**
**Copyright ©1994 by NeXT Computer, Inc. All Rights Reserved.**

**Inherits From:** NSObject
**Declared In:** `foundation/NSUtilities.h`

---

## Class Description

`NSEnumerator` is a simple abstract class whose instances enumerate collections of other objects. Collection objects -- such as arrays and dictionaries -- provide special `NSEnumerator` objects with which to enumerate their contents. You send `nextObject` repeatedly to the `NSEnumerator` object to have it return the next object in the original collection until `nil` is encountered.

Collection classes such as `NSArray` and `NSDictionary` include methods that return an enumerator appropriate to the type of collection. `NSArray` has two methods that return an `NSEnumerator` object, `objectEnumerator` and `reverseObjectEnumerator` (the former traverses the array starting at base zero, while the other starts with the last object in the array and works down). `NSDictionary` has two enumerator-providing methods: `keyEnumerator` and `objectEnumerator`.

**Note:** Enumeration through mutable collections is not safe if the original collection is modified.

---

## Instance Variables

None declared in this class.

---

## Method Types

| Purpose | Methods |
|---------|---------|
| Getting the next object | `nextObject` |

---

## Instance Methods

### nextObject

```objc
- nextObject
```

Returns the next object in the enumeration object (which can be an array, dictionary, or similar object). When `nextObject` returns `nil`, the enumeration object has been traversed. The following code illustrates how this might work:

```objc
NSEnumerator *enumerator = [anArray objectEnumerator];
id object;
while (object = [enumerator nextObject]) {
    // do something with object...
}
```
