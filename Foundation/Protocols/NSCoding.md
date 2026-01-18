# NSCoding

**Enterprise Objects Framework Release 1.0**
**Copyright ©1994 by NeXT Computer, Inc. All Rights Reserved.**

**Adopted By:** NSObject
**Declared In:** `foundation/NSObject.h`

---

## Protocol Description

The `NSCoding` protocol declares the two methods that a class must implement so that objects of that class can be encoded and decoded. This capability provides the basis for archiving (where objects and other structures are stored on disk) and distribution (where objects are copied to different address spaces).

When an object receives an `encodeWithCoder:` message, it should write its instance variables (and, through a message to super, the instance variables that it inherits) to the supplied `NSCoder`. Similarly, when an object receives an `initWithCoder:` message, it should initialize its instance variables (and inherited instance variables, again through a message to super) from the data in the supplied `NSCoder`. See the `NSCoder` and `NSArchiver` class specifications for more complete information.

---

## Instance Methods

### encodeWithCoder:

```objc
- (void)encodeWithCoder:(NSCoder *)aCoder
```

Encodes the receiver using `aCoder`.

**Parameters:**
- `aCoder` - The coder to encode into

**Returns:** `void`

**See also:** `initWithCoder:`

---

### initWithCoder:

```objc
- (id)initWithCoder:(NSCoder *)aDecoder
```

Initializes and returns a new instance from the data in `aDecoder`.

**Parameters:**
- `aDecoder` - The decoder to initialize from

**Returns:** The initialized object.

**See also:** `encodeWithCoder:`
