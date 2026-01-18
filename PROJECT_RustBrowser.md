# Project: Rust-Powered MVP Browser for NeXTSTEP 3.3

**Status:** Conceptual Design
**Target Platform:** NeXTSTEP 3.3 (m68k, i386, SPARC, PA-RISC)
**Primary Language:** Rust + Objective-C
**Estimated Timeline:** 5-8 months
**Last Updated:** 2026-01-18

---

## Executive Summary

This document outlines a project to build a minimal viable web browser for NeXTSTEP 3.3 by leveraging modern Rust ecosystem crates through a custom Rust compiler port. The goal is to demonstrate that retrocomputing platforms can benefit from modern language tooling while respecting historical constraints.

### Key Innovation
Instead of writing 8,000-12,000 lines of Objective-C from scratch, we leverage battle-tested Rust crates (html5ever, url, image decoders) to reduce custom code by 40-50%, resulting in a more robust and maintainable browser implementation.

---

## Table of Contents

1. [Background & Motivation](#background--motivation)
2. [Technical Architecture](#technical-architecture)
3. [Implementation Phases](#implementation-phases)
4. [Component Specifications](#component-specifications)
5. [Rust Crate Analysis](#rust-crate-analysis)
6. [FFI Bridge Design](#ffi-bridge-design)
7. [Build System](#build-system)
8. [Testing Strategy](#testing-strategy)
9. [Known Challenges](#known-challenges)
10. [Alternative Approaches](#alternative-approaches)
11. [References](#references)

---

## Background & Motivation

### Historical Context

NeXTSTEP 3.3 (1995) was a sophisticated operating system with:
- Display PostScript for resolution-independent graphics
- Objective-C and AppKit for object-oriented UI development
- BSD 4.3 networking stack with TCP/IP
- Multi-architecture support (fat binaries)

Despite having Tim Berners-Lee's WorldWideWeb browser (1991), NeXTSTEP never became a major web platform. Commercial browsers like OmniWeb (1995) existed but were proprietary.

### Why Rust?

**The Gap:** Building a web browser requires:
1. HTTP client (~500-800 LOC)
2. HTML parser (~1500-2000 LOC)
3. Layout engine (~3000-5000 LOC)
4. Image decoders (GIF, JPEG, PNG)
5. URL parsing (~200-300 LOC)
6. CSS parser (optional)

**Total:** ~8,000-12,000 lines of Objective-C

**The Opportunity:** Rust ecosystem provides:
- `html5ever` - Mozilla's production HTML5 parser
- `url` - WHATWG URL standard implementation
- `image` - Pure Rust image decoders
- `cssparser` - Mozilla's CSS parser

**Result:** ~2,500-3,500 lines of custom code + integration work

### Project Goals

1. **Technical Demonstration:** Prove Rust can target vintage platforms
2. **Code Reuse:** Leverage modern parsing libraries on 1995 hardware
3. **Educational Value:** Bridge retrocomputing and modern systems programming
4. **Functional Browser:** Deliver working MVP supporting HTML 2.0, basic CSS, inline images

---

## Technical Architecture

### System Architecture Diagram

```
┌───────────────────────────────────────────────────────────┐
│                    Browser Application                     │
│                      (Objective-C)                         │
├───────────────────────────────────────────────────────────┤
│  BrowserWindow  │  WebView  │  AddressBar  │  Navigation  │
└────────────────┬──────────────────────────────────────────┘
                 │
                 │ FFI Calls
                 ▼
┌───────────────────────────────────────────────────────────┐
│              C Bridge Layer (C/Objective-C)                │
├───────────────────────────────────────────────────────────┤
│  • Type conversions (Rust ↔ C ↔ Objective-C)             │
│  • Memory management (malloc/free coordination)           │
│  • Callback marshalling                                   │
└────────────────┬──────────────────────────────────────────┘
                 │
                 │ extern "C" ABI
                 ▼
┌───────────────────────────────────────────────────────────┐
│           Rust Core Engine (Static Library)               │
├───────────────────────────────────────────────────────────┤
│  HTML Parser    │  URL Parser     │  HTTP Client          │
│  (html5ever)    │  (url crate)    │  (ureq/custom)        │
├─────────────────┼─────────────────┼──────────────────────┤
│  Image Decode   │  CSS Parser     │  Layout Calculator    │
│  (image crate)  │  (cssparser)    │  (custom)             │
└────────────────┬──────────────────────────────────────────┘
                 │
                 │ System Calls
                 ▼
┌───────────────────────────────────────────────────────────┐
│                  NeXTSTEP 3.3 OS                          │
├───────────────────────────────────────────────────────────┤
│  Mach Kernel  │  BSD Sockets  │  Display PostScript      │
└───────────────────────────────────────────────────────────┘
```

### Technology Stack

| Layer | Technology | Purpose |
|-------|-----------|---------|
| **UI Layer** | Objective-C + AppKit | Window management, event handling, rendering |
| **Bridge Layer** | C + Objective-C | FFI between Rust and AppKit |
| **Core Engine** | Rust (no_std + minimal std shim) | Parsing, decoding, protocol handling |
| **System Layer** | NeXTSTEP 3.3 (Mach + BSD) | OS services, networking, graphics |

---

## Implementation Phases

### Phase 1: Rust Toolchain Port (8-12 weeks)

**Goal:** Get Rust compiler working on NeXTSTEP with m68k/i386 targets

#### Tasks:
1. **LLVM Backend Integration**
   - Verify LLVM m68k backend functionality
   - Test code generation for NeXTSTEP ABI
   - Generate Mach-O object files
   - Support fat binary generation

2. **Rust std Library Port**
   - Port core allocator to NeXTSTEP malloc
   - Implement minimal std shim:
     - `std::io::Read/Write` traits
     - `std::net::TcpStream` (BSD socket wrapper)
     - `std::collections::HashMap` (via hashbrown)
     - `std::thread` (via Mach C-Threads)
   - Test basic Rust programs

3. **Build System Integration**
   - Cargo target specification for NeXTSTEP
   - Link against NeXTSTEP libraries
   - Static library generation for FFI
   - Fat binary support

**Deliverables:**
- `rustc` targeting `m68k-unknown-nextstep` and `i386-unknown-nextstep`
- Minimal std library (~1,000 LOC)
- Sample "Hello World" Rust → Objective-C FFI demo
- Documentation for cross-compilation

**Success Criteria:**
- Rust compiles and links against NeXTSTEP libraries
- FFI calls work bidirectionally
- Memory allocation/deallocation works correctly
- Can generate fat binaries

---

### Phase 2: Core Library Integration (4-6 weeks)

**Goal:** Port and test essential Rust crates on NeXTSTEP

#### Tasks:

##### 2.1 URL Parsing (Week 1)
```toml
url = { version = "2.5", default-features = false, features = ["alloc"] }
```
- Test on NeXTSTEP
- Verify relative URL resolution
- Create C FFI wrapper

##### 2.2 HTTP Client (Weeks 1-2)
**Option A:** Use `ureq` if std shim sufficient
**Option B:** Custom implementation with `httparse`

```rust
// Custom HTTP client using BSD sockets
extern "C" {
    fn socket(domain: i32, ty: i32, protocol: i32) -> i32;
    fn connect(sockfd: i32, addr: *const sockaddr, len: u32) -> i32;
    // ...
}
```

##### 2.3 HTML Parser (Weeks 2-3)
**Critical Decision:** html5ever vs custom parser

**Option A: html5ever** (preferred if std shim works)
```toml
html5ever = "0.26"
```

**Option B: Custom HTML 2.0 parser** (fallback)
- Tokenizer (~300 LOC)
- Recursive descent parser (~500 LOC)
- DOM tree construction (~400 LOC)

##### 2.4 Image Decoders (Week 3-4)
```toml
png = { version = "0.17", default-features = false }
gif = { version = "0.12", default-features = false }
```

For JPEG: FFI wrapper around libjpeg

##### 2.5 CSS Parser (Week 4)
```toml
cssparser = { version = "0.31", default-features = false }
```

**Deliverables:**
- All crates compile and run on NeXTSTEP
- C FFI wrappers for each library
- Integration tests
- Documentation of API surface

**Success Criteria:**
- Parse real-world HTML/CSS without crashes
- Decode actual image files
- HTTP GET/POST requests work
- Memory usage is reasonable

---

### Phase 3: Browser Implementation (8-12 weeks)

**Goal:** Build working browser with Rust core + Objective-C UI

#### 3.1 Layout Engine (Weeks 1-5)

**Most complex component** - converts HTML DOM to positioned views

```rust
pub struct LayoutEngine;

impl LayoutEngine {
    pub fn layout(
        &self,
        dom: &Node,
        viewport_width: f32,
        viewport_height: f32
    ) -> Vec<LayoutBox> {
        // Calculate positions and sizes for all elements
        // Handle text flow, inline elements, block elements
        // Position images and form elements
    }
}

pub struct LayoutBox {
    pub x: f32,
    pub y: f32,
    pub width: f32,
    pub height: f32,
    pub element_type: ElementType,
    pub style: ComputedStyle,
}
```

**Algorithms needed:**
- Text measurement using AppKit font metrics
- Block/inline layout
- Box model (margins, padding, borders)
- Positioning (static, relative)
- Float handling (basic)
- Table layout (simplified)

**Estimated effort:** 3,000-5,000 LOC

#### 3.2 WebView Implementation (Weeks 2-6)

Objective-C custom view that renders HTML content:

```objc
@interface WebView : NSView {
    void* rustEngine;  // Opaque pointer to Rust engine
    NSMutableArray* subviews;
    NSURL* baseURL;
}

- (void)loadURL:(NSURL*)url;
- (void)loadHTML:(NSString*)html baseURL:(NSURL*)url;
- (void)handleClickAtPoint:(NSPoint)point;
- (void)reload;
- (void)goBack;
- (void)goForward;

@end
```

**Implementation:**
1. Call Rust to parse HTML
2. Call Rust to compute layout
3. Create NSViews for visible elements:
   - NSTextField for text
   - NSImageView for images
   - NSButton for links
   - Custom views for forms
4. Position views based on layout
5. Handle events (clicks, form input)

#### 3.3 Browser UI (Weeks 5-7)

Interface Builder design:

```
┌────────────────────────────────────────────────────┐
│ File  Edit  View  Go  Bookmarks                    │
├────────────────────────────────────────────────────┤
│  ◄  ►  ↻  ✕  │ http://www.example.com/            │
├────────────────────────────────────────────────────┤
│                                                     │
│                  WebView                            │
│              (scrollable content)                   │
│                                                     │
├────────────────────────────────────────────────────┤
│ Status: Loading... | 45% complete                  │
└────────────────────────────────────────────────────┘
```

Components:
- BrowserWindow (NSWindow subclass)
- Navigation buttons (NSButton)
- Address bar (NSTextField)
- WebView (custom NSView)
- Status bar (NSTextField)
- Bookmark manager (NSPanel)
- History sidebar (NSTableView)

#### 3.4 Integration & Polish (Weeks 7-12)

- Form submission
- Cookie storage
- Image caching
- History persistence
- Bookmark management
- Print to PostScript
- View source
- Preferences
- Error handling

**Deliverables:**
- Functional browser application
- User documentation
- Developer documentation
- Test suite
- Sample bookmarks

**Success Criteria:**
- Can browse static HTML 2.0 sites
- Images display correctly
- Forms work (GET/POST)
- Navigation (back/forward/reload) works
- Bookmarks persist across launches
- No crashes during normal use

---

## Component Specifications

### 1. HTTP Client

**Requirements:**
- HTTP/1.0 GET and POST
- Header parsing (Content-Type, Content-Length, Location, Set-Cookie)
- Redirect following (301, 302, 303)
- Basic authentication
- Cookie jar
- Timeout handling

**API:**
```rust
pub struct HttpClient {
    cookie_jar: HashMap<String, String>,
}

impl HttpClient {
    pub fn new() -> Self;

    pub fn get(&mut self, url: &str) -> Result<Response, HttpError>;

    pub fn post(
        &mut self,
        url: &str,
        body: &[u8],
        content_type: &str
    ) -> Result<Response, HttpError>;
}

pub struct Response {
    pub status: u16,
    pub headers: HashMap<String, String>,
    pub body: Vec<u8>,
}
```

**C FFI:**
```c
typedef struct HttpResponse {
    uint16_t status;
    const char* content_type;
    const uint8_t* body;
    size_t body_len;
} HttpResponse;

HttpResponse* http_get(const char* url);
void http_response_free(HttpResponse* resp);
```

---

### 2. HTML Parser

**Requirements:**
- HTML 2.0 support (minimum)
- Error recovery (lenient parsing)
- DOM tree construction
- Attribute parsing
- Entity decoding (`&lt;`, `&gt;`, `&amp;`, etc.)

**Supported Tags (MVP):**
```
Structure: <html>, <head>, <body>, <title>
Headings: <h1> through <h6>
Text: <p>, <br>, <pre>, <blockquote>
Formatting: <b>, <i>, <u>, <strong>, <em>, <code>
Links: <a href="">
Images: <img src="" alt="">
Lists: <ul>, <ol>, <li>
Tables: <table>, <tr>, <td>, <th>
Forms: <form>, <input>, <textarea>, <select>, <option>, <button>
```

**API:**
```rust
pub enum Node {
    Element {
        tag: String,
        attrs: HashMap<String, String>,
        children: Vec<Node>,
    },
    Text(String),
    Comment(String),
}

pub fn parse_html(html: &str) -> Result<Node, ParseError>;
```

**C FFI:**
```c
typedef struct HtmlNode HtmlNode;

HtmlNode* html_parse(const char* html, size_t len);
const char* html_node_tag(const HtmlNode* node);
const char* html_node_attr(const HtmlNode* node, const char* attr);
HtmlNode* html_node_child(const HtmlNode* node, size_t index);
size_t html_node_child_count(const HtmlNode* node);
void html_node_free(HtmlNode* node);
```

---

### 3. Layout Engine

**Requirements:**
- Box model calculation
- Text wrapping and line breaking
- Block vs inline layout
- Absolute positioning (basic)
- CSS inheritance
- Z-index ordering

**Algorithm:**

```rust
pub struct LayoutEngine {
    font_cache: FontCache,
}

pub struct LayoutBox {
    pub x: f32,
    pub y: f32,
    pub width: f32,
    pub height: f32,
    pub content: BoxContent,
    pub style: ComputedStyle,
    pub children: Vec<LayoutBox>,
}

pub enum BoxContent {
    Text(String),
    Image { url: String, width: f32, height: f32 },
    Input { input_type: InputType },
    Block,
    Inline,
}

impl LayoutEngine {
    // Main layout algorithm
    pub fn layout(
        &mut self,
        root: &Node,
        viewport_width: f32
    ) -> LayoutBox {
        // 1. Build box tree from DOM
        // 2. Calculate computed styles
        // 3. Measure text dimensions
        // 4. Position blocks vertically
        // 5. Position inlines horizontally
        // 6. Handle floats and clears
        // 7. Return root layout box
    }

    fn measure_text(&self, text: &str, style: &Style) -> (f32, f32);
    fn wrap_text(&self, text: &str, max_width: f32, style: &Style) -> Vec<String>;
}
```

**C FFI:**
```c
typedef struct LayoutBox {
    float x, y, width, height;
    LayoutBoxType type;
    const char* content;  // For text/image URL
    uint32_t color;
    // ... style properties
} LayoutBox;

LayoutBox* layout_compute(HtmlNode* dom, float viewport_width);
size_t layout_box_count(const LayoutBox* root);
LayoutBox* layout_get_box(const LayoutBox* root, size_t index);
void layout_free(LayoutBox* layout);
```

---

### 4. Image Decoders

**Requirements:**
- GIF (87a/89a) decoding
- JPEG (baseline) decoding
- PNG decoding
- Transparency support
- Color space conversion

**API:**
```rust
pub struct Image {
    pub width: u32,
    pub height: u32,
    pub data: Vec<u8>,  // RGBA format
}

pub fn decode_gif(data: &[u8]) -> Result<Image, ImageError>;
pub fn decode_jpeg(data: &[u8]) -> Result<Image, ImageError>;
pub fn decode_png(data: &[u8]) -> Result<Image, ImageError>;
```

**C FFI:**
```c
typedef struct RustImage {
    uint32_t width;
    uint32_t height;
    const uint8_t* rgba_data;  // 4 bytes per pixel
} RustImage;

RustImage* image_decode_gif(const uint8_t* data, size_t len);
RustImage* image_decode_jpeg(const uint8_t* data, size_t len);
RustImage* image_decode_png(const uint8_t* data, size_t len);
void image_free(RustImage* img);
```

**Integration with NXImage:**
```objc
- (NSImage*)createNSImageFromRust:(RustImage*)rustImg {
    NSBitmapImageRep* rep = [[NSBitmapImageRep alloc]
        initWithBitmapDataPlanes:NULL
        pixelsWide:rustImg->width
        pixelsHigh:rustImg->height
        bitsPerSample:8
        samplesPerPixel:4
        hasAlpha:YES
        isPlanar:NO
        colorSpaceName:NSCalibratedRGBColorSpace
        bytesPerRow:rustImg->width * 4
        bitsPerPixel:32];

    memcpy([rep bitmapData], rustImg->rgba_data,
           rustImg->width * rustImg->height * 4);

    NSImage* image = [[NSImage alloc] init];
    [image addRepresentation:rep];
    return image;
}
```

---

## Rust Crate Analysis

### Recommended Crate Stack

#### Option A: Minimal std Shim (Recommended)

```toml
[dependencies]
# Core parsing - requires minimal std shim
html5ever = "0.26"
markup5ever = "0.11"
url = { version = "2.5", default-features = false, features = ["alloc"] }
cssparser = { version = "0.31", default-features = false }

# HTTP - requires std::net
ureq = { version = "2.9", default-features = false, features = ["native-tls"] }

# Image decoding - mostly no_std friendly
png = { version = "0.17", default-features = false }
gif = { version = "0.12", default-features = false }

# Supporting crates
hashbrown = "0.14"  # no_std HashMap for std shim
```

**std shim required:** ~1,000 LOC
**Custom code required:** ~2,500-3,500 LOC

#### Option B: Pure no_std

```toml
[dependencies]
# Parsing - no_std compatible
url = { version = "2.5", default-features = false, features = ["alloc"] }
cssparser = { version = "0.31", default-features = false }
httparse = { version = "1.8", default-features = false }

# Image decoding
png = { version = "0.17", default-features = false }
gif = { version = "0.12", default-features = false }

# Utilities
hashbrown = "0.14"
```

**Custom code required:**
- HTTP client: ~400 LOC
- HTML parser: ~800-1,200 LOC
- JPEG decoder FFI: ~200 LOC
- Layout engine: ~3,000-5,000 LOC

**Total:** ~4,400-6,800 LOC

### Crate Compatibility Matrix

| Crate | no_std | Minimal std | Full std | Notes |
|-------|--------|-------------|----------|-------|
| `url` | ✅ (with alloc) | ✅ | ✅ | Perfect for no_std |
| `cssparser` | ✅ (with alloc) | ✅ | ✅ | Servo project, well-designed |
| `httparse` | ✅ | ✅ | ✅ | Parser only, no I/O |
| `png` | ✅ (with alloc) | ✅ | ✅ | Pure Rust decoder |
| `gif` | ✅ (with alloc) | ✅ | ✅ | Pure Rust decoder |
| `hashbrown` | ✅ | ✅ | ✅ | no_std HashMap |
| `html5ever` | ❌ | ✅ (shim needed) | ✅ | Requires HashMap, I/O traits |
| `ureq` | ❌ | ✅ (shim needed) | ✅ | Requires std::net |
| `jpeg-decoder` | ⚠️ | ⚠️ | ✅ | Check compatibility |

---

## FFI Bridge Design

### Memory Management Strategy

**Rule 1: Rust owns complex data structures**
- DOM trees, layout boxes, images stay in Rust heap
- Objective-C receives opaque pointers
- Explicit free functions required

**Rule 2: Simple data crosses boundary by copy**
- Strings converted to C strings (malloc'd)
- Primitives (integers, floats) passed by value
- Caller responsible for freeing malloc'd data

**Rule 3: Callbacks use C function pointers**
- Objective-C passes function pointers to Rust
- Rust calls back with C-compatible types
- No Objective-C objects cross boundary directly

### Bridge API Design

```c
// rust_browser.h - C header for FFI

#ifndef RUST_BROWSER_H
#define RUST_BROWSER_H

#include <stdint.h>
#include <stddef.h>

// Opaque types
typedef struct RustHttpClient RustHttpClient;
typedef struct RustHtmlNode RustHtmlNode;
typedef struct RustLayoutBox RustLayoutBox;
typedef struct RustImage RustImage;

// HTTP Client
RustHttpClient* http_client_new(void);
void http_client_free(RustHttpClient* client);

typedef struct {
    uint16_t status;
    const char* content_type;
    const uint8_t* body;
    size_t body_len;
} HttpResponse;

HttpResponse* http_get(RustHttpClient* client, const char* url);
HttpResponse* http_post(
    RustHttpClient* client,
    const char* url,
    const uint8_t* body,
    size_t body_len,
    const char* content_type
);
void http_response_free(HttpResponse* resp);

// HTML Parser
RustHtmlNode* html_parse(const char* html, size_t len);
void html_node_free(RustHtmlNode* node);

typedef enum {
    NODE_TYPE_ELEMENT,
    NODE_TYPE_TEXT,
    NODE_TYPE_COMMENT
} NodeType;

NodeType html_node_type(const RustHtmlNode* node);
const char* html_node_tag(const RustHtmlNode* node);
const char* html_node_text(const RustHtmlNode* node);
const char* html_node_attr(const RustHtmlNode* node, const char* key);
size_t html_node_child_count(const RustHtmlNode* node);
RustHtmlNode* html_node_child(const RustHtmlNode* node, size_t index);

// Layout Engine
typedef struct {
    float x, y, width, height;
    uint8_t type;  // 0=block, 1=inline, 2=text, 3=image
    const char* content;
    uint32_t text_color;
    uint32_t bg_color;
    const char* font_family;
    float font_size;
    uint8_t font_weight;  // 0=normal, 1=bold
    uint8_t font_style;   // 0=normal, 1=italic
} LayoutBoxData;

RustLayoutBox* layout_compute(
    const RustHtmlNode* dom,
    float viewport_width,
    float viewport_height
);
void layout_free(RustLayoutBox* layout);

size_t layout_box_count(const RustLayoutBox* layout);
int layout_get_box(const RustLayoutBox* layout, size_t index, LayoutBoxData* out);

// Image Decoder
RustImage* image_decode(const uint8_t* data, size_t len, const char* mime_type);
void image_free(RustImage* img);

uint32_t image_width(const RustImage* img);
uint32_t image_height(const RustImage* img);
const uint8_t* image_rgba_data(const RustImage* img);

// URL Parser
typedef struct {
    const char* scheme;
    const char* host;
    uint16_t port;
    const char* path;
    const char* query;
} ParsedUrl;

ParsedUrl* url_parse(const char* url);
char* url_join(const char* base, const char* relative);
void url_free(ParsedUrl* url);
void url_string_free(char* str);

#endif // RUST_BROWSER_H
```

### Rust Implementation

```rust
// lib.rs

#![no_std]
extern crate alloc;

use alloc::string::String;
use alloc::vec::Vec;
use alloc::boxed::Box;
use core::ffi::c_char;

// Re-export for C
#[no_mangle]
pub extern "C" fn http_client_new() -> *mut HttpClient {
    Box::into_raw(Box::new(HttpClient::new()))
}

#[no_mangle]
pub unsafe extern "C" fn http_client_free(client: *mut HttpClient) {
    if !client.is_null() {
        drop(Box::from_raw(client));
    }
}

#[repr(C)]
pub struct HttpResponse {
    status: u16,
    content_type: *const c_char,
    body: *const u8,
    body_len: usize,
}

#[no_mangle]
pub unsafe extern "C" fn http_get(
    client: *mut HttpClient,
    url: *const c_char
) -> *mut HttpResponse {
    let client = &mut *client;
    let url = CStr::from_ptr(url).to_str().unwrap();

    match client.get(url) {
        Ok(response) => {
            let content_type = CString::new(
                response.content_type().unwrap_or("text/html")
            ).unwrap();

            let resp = HttpResponse {
                status: response.status,
                content_type: content_type.into_raw(),
                body: response.body.as_ptr(),
                body_len: response.body.len(),
            };

            Box::into_raw(Box::new(resp))
        }
        Err(_) => core::ptr::null_mut()
    }
}

// ... more FFI functions
```

### Objective-C Wrapper

```objc
// RustBridge.h
#import <Foundation/Foundation.h>
#import "rust_browser.h"

@interface RustBridge : NSObject

+ (NSDictionary*)httpGet:(NSString*)urlString;
+ (NSDictionary*)parseHTML:(NSString*)html;
+ (NSArray*)computeLayout:(NSDictionary*)dom width:(float)width height:(float)height;
+ (NSImage*)decodeImage:(NSData*)data mimeType:(NSString*)type;

@end

// RustBridge.m
@implementation RustBridge

+ (NSDictionary*)httpGet:(NSString*)urlString {
    RustHttpClient* client = http_client_new();
    const char* url = [urlString cString];

    HttpResponse* resp = http_get(client, url);
    if (!resp) {
        http_client_free(client);
        return nil;
    }

    NSData* body = [NSData dataWithBytes:resp->body length:resp->body_len];
    NSString* contentType = [NSString stringWithCString:resp->content_type];

    NSDictionary* result = @{
        @"status": @(resp->status),
        @"contentType": contentType,
        @"body": body
    };

    http_response_free(resp);
    http_client_free(client);

    return result;
}

// ... more wrapper methods

@end
```

---

## Build System

### Cross-Compilation Setup

```bash
# Install Rust with custom target
rustup install nightly
rustup component add rust-src --toolchain nightly

# Add NeXTSTEP target
cat > ~/.cargo/config.toml << EOF
[build]
target = "m68k-unknown-nextstep"

[target.m68k-unknown-nextstep]
linker = "m68k-next-nextstep3-gcc"
ar = "m68k-next-nextstep3-ar"

[target.i386-unknown-nextstep]
linker = "i386-next-nextstep3-gcc"
ar = "i386-next-nextstep3-ar"
EOF
```

### Project Structure

```
RustBrowser/
├── Cargo.toml              # Rust dependencies
├── rust/
│   ├── src/
│   │   ├── lib.rs          # FFI exports
│   │   ├── http.rs         # HTTP client
│   │   ├── html.rs         # HTML parser
│   │   ├── layout.rs       # Layout engine
│   │   ├── image.rs        # Image decoders
│   │   └── std_shim.rs     # Minimal std implementation
│   └── Cargo.toml
├── bridge/
│   ├── rust_browser.h      # C header
│   ├── RustBridge.h        # Objective-C wrapper header
│   └── RustBridge.m        # Objective-C wrapper impl
├── Browser/
│   ├── Browser.m           # Application delegate
│   ├── BrowserWindow.m     # Main window
│   ├── WebView.m           # HTML rendering view
│   ├── AddressBar.m        # URL input
│   └── NavigationBar.m     # Back/forward buttons
├── Resources/
│   ├── Browser.nib         # Interface Builder file
│   └── Info.plist          # App metadata
├── PB.project              # Project Builder project
└── Makefile.postamble      # Custom build rules
```

### Makefile Integration

```makefile
# Makefile.postamble

RUST_LIB = rust/target/m68k-unknown-nextstep/release/librustbrowser.a

# Build Rust library before linking
before-all::
	cd rust && cargo build --release --target m68k-unknown-nextstep

# Link Rust static library
OTHER_LDFLAGS = $(RUST_LIB) -lpthread

# Clean Rust artifacts
after-clean::
	cd rust && cargo clean
```

### Fat Binary Generation

```bash
#!/bin/bash
# build_fat.sh - Build fat binary for all architectures

ARCHS="m68k i386 sparc hppa"

for arch in $ARCHS; do
    echo "Building for $arch..."
    cargo build --release --target ${arch}-unknown-nextstep
done

# Combine into fat binary
lipo -create \
    rust/target/m68k-unknown-nextstep/release/librustbrowser.a \
    rust/target/i386-unknown-nextstep/release/librustbrowser.a \
    rust/target/sparc-unknown-nextstep/release/librustbrowser.a \
    rust/target/hppa-unknown-nextstep/release/librustbrowser.a \
    -output librustbrowser_fat.a
```

---

## Testing Strategy

### Unit Tests (Rust)

```rust
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_url_parsing() {
        let url = parse_url("http://example.com/path?query=1").unwrap();
        assert_eq!(url.scheme, "http");
        assert_eq!(url.host, "example.com");
        assert_eq!(url.path, "/path");
    }

    #[test]
    fn test_html_parsing() {
        let html = "<html><body><p>Hello</p></body></html>";
        let dom = parse_html(html).unwrap();
        // Assertions...
    }

    #[test]
    fn test_image_decoding() {
        let gif_data = include_bytes!("../test_data/test.gif");
        let img = decode_gif(gif_data).unwrap();
        assert_eq!(img.width, 100);
        assert_eq!(img.height, 100);
    }
}
```

### Integration Tests (Objective-C)

```objc
// BrowserTests.m
#import <XCTest/XCTest.h>
#import "RustBridge.h"

@interface BrowserTests : XCTestCase
@end

@implementation BrowserTests

- (void)testHTTPGet {
    NSDictionary* resp = [RustBridge httpGet:@"http://example.com"];
    XCTAssertNotNil(resp);
    XCTAssertEqual([[resp objectForKey:@"status"] intValue], 200);
}

- (void)testHTMLParsing {
    NSString* html = @"<html><body><h1>Test</h1></body></html>";
    NSDictionary* dom = [RustBridge parseHTML:html];
    XCTAssertNotNil(dom);
}

- (void)testLayout {
    NSString* html = @"<html><body><p>Paragraph</p></body></html>";
    NSDictionary* dom = [RustBridge parseHTML:html];
    NSArray* boxes = [RustBridge computeLayout:dom width:400 height:600];
    XCTAssertGreaterThan([boxes count], 0);
}

@end
```

### Manual Test Cases

**Test Suite 1: Basic Browsing**
1. Launch browser
2. Enter URL: `http://info.cern.ch/` (first website)
3. Verify page loads
4. Click a link
5. Use back button
6. Use forward button

**Test Suite 2: Rendering**
1. Load page with headers (h1-h6)
2. Load page with lists (ul, ol)
3. Load page with images
4. Load page with tables
5. Verify text formatting (bold, italic)

**Test Suite 3: Forms**
1. Load page with form
2. Enter text in input field
3. Submit form (GET)
4. Submit form (POST)
5. Verify data received correctly

**Test Suite 4: Edge Cases**
1. Load malformed HTML
2. Load very large page (>1MB)
3. Load page with many images
4. Network timeout
5. Invalid URL
6. 404 error
7. Redirect handling

---

## Known Challenges

### 1. Layout Engine Complexity

**Challenge:** HTML flow layout is fundamentally different from NeXTSTEP's frame-based positioning.

**Approaches:**

**Option A: View-per-Element**
- Create NSView for every HTML element
- Position manually based on layout calculation
- Pro: Simple, native feel
- Con: Memory intensive, slow for large pages

**Option B: Canvas-style Rendering**
- Single NSView, draw everything in `drawRect:`
- Use Display PostScript commands
- Pro: Fast, memory efficient
- Con: Lose native controls, accessibility

**Option C: Hybrid**
- Canvas for static content (text, images)
- Native views for interactive (forms, links)
- Pro: Balanced performance and functionality
- Con: Complex to implement

**Recommendation:** Start with Option A, migrate to Option C if needed.

---

### 2. Threading and Async I/O

**Challenge:** NeXTSTEP has Mach C-Threads, not pthreads. Network I/O must not block UI.

**Solutions:**

**Option A: DPSAddFD() Integration**
```c
// Integrate BSD socket with AppKit run loop
int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
DPSAddFD(socket_fd, download_callback, context, NX_MODALRESPTHRESHOLD);
```

**Option B: Background Thread**
```c
// Use Mach C-Thread for download
void* download_thread(void* url) {
    // Download in background
    // Post notification when done
}
cthread_t thread = cthread_fork(download_thread, url);
cthread_detach(thread);
```

**Option C: Port std::thread to C-Threads**
```rust
// Implement std::thread backend using C-Threads
pub fn spawn<F, T>(f: F) -> JoinHandle<T>
where
    F: FnOnce() -> T,
{
    // Map to cthread_fork()
}
```

**Recommendation:** Option B (simplest), with Option C for better Rust integration.

---

### 3. Memory Management Across FFI

**Challenge:** Rust owns data, Objective-C needs pointers, manual coordination required.

**Rules:**

1. **Rust allocates, Rust frees** (default)
   ```objc
   RustHtmlNode* node = html_parse(html, len);
   // Use node...
   html_node_free(node);  // Must call
   ```

2. **Copy for Objective-C ownership**
   ```objc
   const char* text = html_node_text(node);
   NSString* str = [NSString stringWithCString:text];
   // str is Objective-C owned, no manual free needed
   ```

3. **Never pass Objective-C objects to Rust**
   ```objc
   // NO!
   rust_function(myNSArray);  // Disaster

   // YES!
   const char* cstr = [myString cString];
   rust_function(cstr);
   ```

---

### 4. Fat Binary Considerations

**Challenge:** Must support 4 architectures (m68k, i386, SPARC, PA-RISC).

**Implications:**

- LLVM must support all targets
- Different endianness (68k/SPARC big, x86 little)
- Different calling conventions
- Different alignment requirements
- Cross-compilation toolchain for each

**Testing matrix:** 4 architectures × 3 major features = 12 test configurations

---

### 5. no_std Limitations

**Challenge:** Many Rust crates assume full std library.

**Mitigation strategies:**

1. **Prefer no_std crates** when available
2. **Implement minimal std shim** (~1k LOC) for critical APIs
3. **Fork and patch** crates if necessary
4. **Write custom** for unavoidable cases

**Critical decision:** How much std to port?
- **Minimal:** Just enough for html5ever (~1k LOC)
- **Moderate:** Add threading, files (~3k LOC)
- **Full:** Complete std library port (months of work)

**Recommendation:** Minimal shim is sweet spot.

---

## Alternative Approaches

### Alternative 1: Pure Objective-C (Historical Approach)

**What OmniWeb (1995) actually did:**

- ~12,000 LOC of Objective-C
- Used libwww for HTTP
- Custom HTML parser
- Custom layout engine
- Took Omni Group ~1 year

**Pros:**
- Period-appropriate
- No toolchain porting
- Native integration
- Proven to work

**Cons:**
- More code to write
- Parsing is tedious
- Error-prone (no memory safety)

**Timeline:** 6-12 months

---

### Alternative 2: Use Existing C Libraries

**Available in 1995:**

- **libwww** (W3C) - HTTP client
- **libgif** - GIF decoder
- **libjpeg** - JPEG decoder
- **expat** or **sgmls** - XML/SGML parser (can parse HTML)

**Approach:**
```objc
#import <WWWLib/WWWLib.h>
#import <gif_lib.h>
#import <jpeglib.h>

- (void)loadURL:(NSURL*)url {
    // Use libwww for HTTP
    HTRequest* request = HTRequest_new();
    HTRequest_setOutputFormat(request, WWW_HTML);
    // ...
}
```

**Pros:**
- Battle-tested libraries
- No Rust porting needed
- C FFI is straightforward
- Period-appropriate

**Cons:**
- Still need custom HTML parser (expat is XML)
- Still need layout engine
- C memory management (unsafe)

**Timeline:** 4-8 months

---

### Alternative 3: Port Modern C++ Browser Engine

**Options:**
- WebKit (too large)
- Gecko (too large)
- Servo (Rust, but huge)
- Simple browser engines (e.g., Dillo's core)

**Challenges:**
- Modern C++ (NeXTSTEP has gcc 2.x)
- Massive dependencies
- Assume modern OS features
- Would take years to port

**Verdict:** Not feasible.

---

### Alternative 4: Minimal HTML Renderer Only

**Scope reduction:**
- No HTTP client (use existing fetch tool)
- No JavaScript
- No CSS
- HTML → PostScript converter
- Display with Yap (PS previewer)

**Example:**
```bash
$ fetch http://example.com > page.html
$ html2ps page.html > page.ps
$ yap page.ps
```

**Pros:**
- Much simpler (~2k LOC)
- Leverage existing tools
- Could work in days/weeks

**Cons:**
- Not interactive
- Can't click links
- Not really a browser

**Timeline:** 2-4 weeks

---

## Comparison Matrix

| Approach | Code to Write | Timeline | Complexity | Modern Tech | Historical |
|----------|---------------|----------|------------|-------------|------------|
| **Rust + Crates** | 2.5-3.5k LOC | 5-8 months | High (toolchain) | ✅ Yes | ❌ No |
| **Pure Objective-C** | 8-12k LOC | 6-12 months | Medium | ❌ No | ✅ Yes |
| **C Libraries** | 5-8k LOC | 4-8 months | Medium | ❌ No | ✅ Yes |
| **HTML to PS** | 2k LOC | 2-4 weeks | Low | ❌ No | ✅ Yes |

---

## References

### Historical Browsers

- **WorldWideWeb** (1991, NeXTSTEP) - Tim Berners-Lee's original browser
- **Mosaic** (1993, UNIX) - First popular graphical browser
- **Netscape Navigator 1.0** (1994) - First commercial browser
- **OmniWeb** (1995, NeXTSTEP) - First commercial NeXTSTEP browser

### Technical Specifications

- **HTML 2.0** - RFC 1866 (1995)
- **HTTP/1.0** - RFC 1945 (1996)
- **GIF89a** - Graphics Interchange Format (1989)
- **JPEG** - ISO/IEC 10918 (1992)
- **CSS Level 1** - W3C Recommendation (1996)

### Rust Documentation

- **The Rust Programming Language** - https://doc.rust-lang.org/book/
- **Rustonomicon** (unsafe/FFI) - https://doc.rust-lang.org/nomicon/
- **Embedded Rust Book** (no_std) - https://rust-embedded.github.io/book/

### NeXTSTEP Documentation

- **Concepts: Objective C** - NeXT Developer Documentation
- **Application Kit** - AppKit class hierarchy
- **Display PostScript** - Window Server documentation
- **Mach Kernel Interface** - IPC and threading

### Relevant Crates

- `html5ever` - https://github.com/servo/html5ever
- `url` - https://github.com/servo/rust-url
- `image` - https://github.com/image-rs/image
- `ureq` - https://github.com/algesten/ureq
- `cssparser` - https://github.com/servo/rust-cssparser

---

## Conclusion

This project demonstrates that modern language ecosystems can bring value to historical platforms. While the Rust toolchain port is substantial work upfront, the ability to leverage battle-tested parsing libraries (html5ever, url, image) reduces custom code by 40-50% compared to pure Objective-C.

**Key Takeaways:**

1. **Technically feasible** - No fundamental blockers
2. **Significant upfront cost** - Rust toolchain port is 2-3 months
3. **Long-term benefits** - Less code, more robust, educational value
4. **Not the fastest path** - Pure Objective-C or C libraries would ship sooner
5. **Unique achievement** - No one has done this before

**Recommended Path Forward:**

1. **Phase 1:** Prove Rust compiler works on NeXTSTEP (3 months)
2. **Phase 2:** Port minimal std + test crates (2 months)
3. **Decision point:** If successful, continue with browser
4. **Phase 3:** Build MVP browser (3 months)

**Total:** 8 months to working browser, with 5 months to know if it's viable.

**Alternative:** Build with Objective-C + C libraries in 6 months guaranteed.

The choice depends on whether the goal is:
- **Ship a browser for NeXTSTEP** → Use Objective-C
- **Prove Rust works on vintage platforms** → Use Rust
- **Educational retrocomputing project** → Use Rust

---

**Document Version:** 1.0
**Author:** Technical Architecture Team
**Date:** 2026-01-18
**Status:** Design Complete, Awaiting Implementation Decision
