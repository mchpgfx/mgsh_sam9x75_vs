
# Blank Quickstart - SAM9X75 Curiosity + AC69T88A (RGB565)

## Introduction

This is a **headless graphics application** that demonstrates how to directly access the display and touch interfaces on Microchip embedded platforms. Unlike applications that rely on a graphics library such as MGS Harmony or Legato, this project provides direct framebuffer access, making it ideal for:

- **Custom rendering engines** - Integrate your own graphics library (e.g., LVGL, emWin, or a proprietary solution)
- **Minimal footprint applications** - When a full graphics stack is unnecessary
- **Learning and prototyping** - Understand the low-level display and touch driver interfaces
- **Performance-critical applications** - Direct pixel manipulation without library overhead

The application renders a test pattern with touch-responsive regions to verify display calibration and touch input functionality.

## Hardware

| Component | Description |
|-----------|-------------|
| [SAM9X75 Curiosity Development Board](https://www.microchip.com/en-us/development-tool/EV31H43A) | MPU development platform featuring the SAM9X75 processor |
| [AC69T88A WVGA Display](https://www.microchip.com/en-us/development-tool/AC69T88A) | 5" 800x480 TFT LCD with capacitive touch |

## Color Mode

This project uses **16-bit RGB565 color mode**, which provides a good balance between color depth and memory efficiency.

## Getting Started

### Interfacing with the Display Framebuffer

The XLCDC driver provides direct access to the display framebuffer, allowing you to draw pixels without using a graphics library.

#### 1. Include Required Headers

```c
#include "definitions.h"
```

#### 2. Get Display Parameters

Use IOCTL calls to retrieve the display dimensions and framebuffer pointer:

```c
gfxIOCTLArg_DisplaySize argDispSize;
gfxIOCTLArg_Value argVal;

// Get display width and height
if (DRV_XLCDC_IOCTL(GFX_IOCTL_GET_DISPLAY_SIZE, &argDispSize) == GFX_IOCTL_OK)
{
    uint32_t width = argDispSize.width;
    uint32_t height = argDispSize.height;
}

// Get framebuffer pointer (cast to 16-bit for RGB565)
if (DRV_XLCDC_IOCTL(GFX_IOCTL_GET_FRAMEBUFFER, &argVal) == GFX_IOCTL_OK)
{
    uint16_t *buffer = (uint16_t *)argVal.value.v_pbuffer->pixels;
}
```

#### 3. Write Pixels to the Framebuffer

Pixels are stored in a linear array in row-major order. Each pixel is a 16-bit RGB565 value:

```c
void draw_pixel(uint32_t x, uint32_t y, uint16_t color)
{
    if (x < width && y < height)
    {
        buffer[y * width + x] = color;
    }
}
```

**Color Format (RGB565):** Colors are 16-bit values packed as `RRRRRGGGGGGBBBBB` (5 bits red, 6 bits green, 5 bits blue). Examples:
- `0xF800` - Red
- `0x07E0` - Green
- `0x001F` - Blue
- `0xFFFF` - White
- `0x0000` - Black
- `0xFFE0` - Yellow
- `0x07FF` - Cyan
- `0xF81F` - Magenta

#### 4. Swap the Framebuffer (Double Buffering)

This project uses double buffering. After drawing to the back buffer, call `GFX_IOCTL_LAYER_SWAP` to display the completed frame:

```c
// Draw your graphics to the buffer...
draw_test_pattern(width, height);

// Swap buffers to display the frame
DRV_XLCDC_IOCTL(GFX_IOCTL_LAYER_SWAP, NULL);
```

The swap operation moves the back buffer to the front, making it visible on screen. This prevents tearing artifacts that would occur if drawing directly to the visible buffer.

### Interfacing with the Touch Input System

The System Input Service (SYS_INP) provides touch event callbacks for handling user interaction.

#### 1. Define Touch Event Handlers

Create callback functions for touch events:

```c
void touchDownHandler(const SYS_INP_TouchStateEvent *const evt)
{
    // evt->x and evt->y contain touch coordinates
    uint32_t touch_x = evt->x;
    uint32_t touch_y = evt->y;
    
    // Handle touch press
    LED_GREEN_On();
}

void touchUpHandler(const SYS_INP_TouchStateEvent *const evt)
{
    // Handle touch release
    LED_GREEN_Off();
}

void touchMoveHandler(const SYS_INP_TouchMoveEvent *const evt)
{
    // evt->x and evt->y contain current position
    // Handle touch drag/move
    LED_BLUE_On();
}
```

#### 2. Register the Input Listener

During initialization, create a listener structure and register it with the input system:

```c
void APP_Initialize(void)
{
    SYS_INP_InputListener appInputListener;
    
    // Assign callback handlers
    appInputListener.handleTouchDown = &touchDownHandler;
    appInputListener.handleTouchUp = &touchUpHandler;
    appInputListener.handleTouchMove = &touchMoveHandler;
    
    // Register the listener
    SYS_INP_AddListener(&appInputListener);
}
```

#### Touch Event Structures

| Structure | Fields | Description |
|-----------|--------|-------------|
| `SYS_INP_TouchStateEvent` | `x`, `y` | Touch coordinates for down/up events |
| `SYS_INP_TouchMoveEvent` | `x`, `y` | Touch coordinates for move/drag events |

### Example: Creating Touch Zones

Define rectangular regions that respond to touch:

```c
typedef struct {
    uint32_t x1, y1;  // Top-left corner
    uint32_t x2, y2;  // Bottom-right corner
} touch_zone_t;

touch_zone_t zones[4];

// Initialize zones (example: four corners)
zones[0] = (touch_zone_t){0, 0, 100, 100};           // Top-left
zones[1] = (touch_zone_t){0, 380, 100, 480};         // Bottom-left
zones[2] = (touch_zone_t){700, 0, 800, 100};         // Top-right
zones[3] = (touch_zone_t){700, 380, 800, 480};       // Bottom-right

// Check touch in handler
void touchDownHandler(const SYS_INP_TouchStateEvent *const evt)
{
    for (int i = 0; i < 4; i++)
    {
        if (evt->x > zones[i].x1 && evt->x < zones[i].x2 &&
            evt->y > zones[i].y1 && evt->y < zones[i].y2)
        {
            // Touch is within zone[i]
        }
    }
}
```

## Project Structure

| Path | Purpose |
|------|---------|
| `curiosity_ac69t88a/src/app.c` | Main application source with framebuffer and touch examples |
| `curiosity_ac69t88a/src/app.h` | Application header with state machine definitions |
| `_build` | The [CMake build tree](https://cmake.org/cmake/help/latest/manual/cmake.1.html#introduction-to-cmake-buildsystems), can be deleted |
| `cmake` | Generated [CMake](https://cmake.org/) files. May be deleted if user.cmake has not been added |
| `.vscode` | See [VSCode](https://code.visualstudio.com/docs/getstarted/settings) |
| `.vscode/settings.json` | Workspace specific settings |
| `.vscode/mgs_qs_9x75_curiosity_ac69t88a.mplab.json` | The MPLAB project file, should not be deleted |
| `out` | Final build artifacts |

## Notes

**Why RGB565?** The 16-bit RGB565 format uses half the memory of 32-bit RGBA8888, reducing framebuffer size from ~1.5MB to ~768KB for a 800x480 display. This is beneficial for memory-constrained systems and improves memory bandwidth.

## References

- See [app.c](curiosity_ac69t88a/src/app.c) for the complete working example
- XLCDC Driver documentation in MPLAB Harmony Graphics
- System Input Service (SYS_INP) documentation in MPLAB Harmony
