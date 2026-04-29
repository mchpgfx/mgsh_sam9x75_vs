/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "fonts/fonts.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

/* Color Palette Lookup Table

  Summary:
    Predefined color palette in RGBA8888 format.

  Description:
    Contains colors for the test pattern including: transparent, grays,
    primary colors (red, green, blue), secondary colors, and a grayscale
    gradient ramp for calibration purposes.

  Remarks:
    Colors are stored as 32-bit RGBA values (0xRRGGBBAA format).
*/
uint32_t color_map[] =
    {
        0x00000000,
        0x333333FF,
        0xFF0000FF,
        0xFF8000FF,
        0x00FF00FF,
        0x80FF00FF,
        0x0000FFFF,
        0x0180FFFF,
        0xFF01FD84,
        0xFFFF00FF,
        0x01FEFDFF,
        0x8200FDFF,
        0xFD01FCFF,
        0xFE017FFF,
        0x030003FF,
        0x060307FF,
        0x080808FF,
        0x090909FF,
        0x0E0C0FFF,
        0x100D10FF,
        0x111111FF,
        0x151515FF,
        0x161616FF,
        0x1A1A1AFF,
        0x1B1B1BFF,
        0x1F1F21FF,
        0x060307FF,
        0x0E0C0FFF,
        0x1A1A1AFF,
        0x343436FF,
        0x4D4A4EFF,
        0x686568FF,
        0x7F7D80FF,
        0x9A979BFF,
        0xB4B1B4FF,
        0xCDCBCEFF,
        0xE7E4E7FF,
        0xFFFFFFFF,
};
/* Maximum valid index into color_map array (used for white/last color) */
const size_t color_max = sizeof(color_map) / sizeof(color_map[0]) - 1;
// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void draw_pixel(uint32_t x, uint32_t y, uint32_t color)

  Summary:
    Draws a single pixel to the framebuffer.

  Description:
    Sets the pixel at coordinates (x, y) to the specified color. Performs
    bounds checking to prevent writing outside the framebuffer.

  Parameters:
    x     - Horizontal coordinate (0 = left edge)
    y     - Vertical coordinate (0 = top edge)
    color - 32-bit RGBA color value

  Returns:
    None.
*******************************************************************************/
void draw_pixel(uint32_t x, uint32_t y, uint32_t color)
{
    if (x < appData.width && y < appData.height)
    {
        appData.buffer[y * appData.width + x] = color;
    }
}

/*******************************************************************************
  Function:
    void draw_line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color)

  Summary:
    Draws a line between two points using Bresenham's algorithm.

  Description:
    Renders a straight line from point (x1, y1) to point (x2, y2) using
    Bresenham's line algorithm for efficient integer-only computation.
    Works for lines at any angle.

  Parameters:
    x1    - Starting point horizontal coordinate
    y1    - Starting point vertical coordinate
    x2    - Ending point horizontal coordinate
    y2    - Ending point vertical coordinate
    color - 32-bit RGBA color value

  Returns:
    None.
*******************************************************************************/
void draw_line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color)
{
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx - dy, e2;

    while (1)
    {
        draw_pixel(x1, y1, color);
        if (x1 == x2 && y1 == y2)
            break;
        e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

/*******************************************************************************
  Function:
    void draw_circle(uint32_t x, uint32_t y, uint32_t r, uint32_t color, bool fill)

  Summary:
    Draws a circle using Bresenham's midpoint circle algorithm.

  Description:
    Renders a circle centered at (x, y) with radius r. Uses the midpoint
    circle algorithm for efficient integer arithmetic. Can draw either
    an outline or a filled circle based on the fill parameter.

  Parameters:
    x     - Center point horizontal coordinate
    y     - Center point vertical coordinate
    r     - Circle radius in pixels
    color - 32-bit RGBA color value
    fill  - true for filled circle, false for outline only

  Returns:
    None.
*******************************************************************************/
void draw_circle(uint32_t x, uint32_t y, uint32_t r, uint32_t color, bool fill)
{
    int32_t d = 3 - (r << 1);
    uint32_t cx = 0, cy = r;

    while (cx <= cy)
    {
        if (fill)
        {
            draw_line(x - cx, y - cy, x + cx, y - cy, color);
            draw_line(x - cx, y + cy, x + cx, y + cy, color);
            draw_line(x - cy, y - cx, x + cy, y - cx, color);
            draw_line(x - cy, y + cx, x + cy, y + cx, color);
        }
        else
        {
            draw_pixel(x + cx, y + cy, color);
            draw_pixel(x + cx, y - cy, color);
            draw_pixel(x - cx, y + cy, color);
            draw_pixel(x - cx, y - cy, color);
            draw_pixel(x + cy, y + cx, color);
            draw_pixel(x + cy, y - cx, color);
            draw_pixel(x - cy, y + cx, color);
            draw_pixel(x - cy, y - cx, color);
        }
        if (d < 0)
        {
            d += (cx << 2) + 6;
        }
        else
        {
            d += ((cx - cy) << 2) + 10;
            cy--;
        }
        cx++;
    }
}

/*******************************************************************************
  Function:
    void draw_rect_fill(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color)

  Summary:
    Draws a filled rectangle.

  Description:
    Renders a solid filled rectangle defined by its top-left corner (x1, y1)
    and bottom-right corner (x2, y2).

  Parameters:
    x1    - Left edge horizontal coordinate
    y1    - Top edge vertical coordinate
    x2    - Right edge horizontal coordinate
    y2    - Bottom edge vertical coordinate
    color - 32-bit RGBA fill color value

  Returns:
    None.
*******************************************************************************/
void draw_rect_fill(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color)
{
    for (uint32_t y = y1; y <= y2; y++)
    {
        for (uint32_t x = x1; x <= x2; x++)
        {
            draw_pixel(x, y, color);
        }
    }
}

/*******************************************************************************
  Function:
    uint32_t draw_char(uint32_t x, uint32_t y, char ascii_char, font_size_alias_t font_size, uint32_t color)

  Summary:
    Draws a single character from the bitmap font.

  Description:
    Renders an ASCII character at the specified position using the bitmap
    font system. The font data is retrieved based on the character and size,
    then rendered pixel-by-pixel from the bitmap data.

  Parameters:
    x          - Left edge horizontal coordinate for character placement
    y          - Top edge vertical coordinate for character placement
    ascii_char - ASCII character code to render
    font_size  - Font size selector (FONT_SM, FONT_BASE, etc.)
    color      - 32-bit RGBA color value for the character

  Returns:
    Width of the rendered character in pixels (for text cursor advancement).
*******************************************************************************/
uint32_t draw_char(uint32_t x, uint32_t y, char ascii_char, font_size_alias_t font_size, uint32_t color)
{
    font_t font;
    get_font(ascii_char, font_size, &font);
    uint32_t bytes_per_row = font.data_len / font.height;

    for (int i = 0; i < font.height; i++)
    {
        for (int j = 0; j < bytes_per_row; j++)
        {
            uint32_t line = font.data[i * bytes_per_row + j];
            for (int k = 0; k < 8; k++)
            {
                if (line & (0x80 >> k))
                {
                    draw_pixel(x + j * 8 + k, y + i, color);
                }
            }
        }
    }
    return font.width;
}

/*******************************************************************************
  Function:
    uint32_t draw_string(uint32_t x, uint32_t y, const char *str, font_size_alias_t font_size, uint32_t color)

  Summary:
    Draws a null-terminated string.

  Description:
    Renders a string of characters horizontally starting at position (x, y).
    Each character is drawn sequentially with a 1-pixel gap between them.

  Parameters:
    x         - Starting horizontal coordinate
    y         - Vertical coordinate (baseline)
    str       - Pointer to null-terminated ASCII string
    font_size - Font size selector (FONT_SM, FONT_BASE, etc.)
    color     - 32-bit RGBA color value for the text

  Returns:
    Total width of the rendered string in pixels.
*******************************************************************************/
uint32_t draw_string(uint32_t x, uint32_t y, const char *str, font_size_alias_t font_size, uint32_t color)
{
    uint32_t nx = x;
    while (*str)
    {
        nx += draw_char(nx, y, *str++, font_size, color) + 1;
    }
    return nx - x;
}

/*******************************************************************************
  Function:
    uint32_t get_gradient(uint32_t start_color, uint32_t end_color, uint32_t current_step, uint32_t total_steps)

  Summary:
    Calculates an interpolated color for gradient rendering.

  Description:
    Performs linear interpolation between two colors based on the current
    position within a gradient. Each color channel (R, G, B, A) is
    interpolated independently.

  Parameters:
    start_color  - Starting color (32-bit RGBA)
    end_color    - Ending color (32-bit RGBA)
    current_step - Current position in the gradient (0 to total_steps)
    total_steps  - Total number of steps in the gradient

  Returns:
    Interpolated 32-bit RGBA color value.
*******************************************************************************/
uint32_t get_gradient(uint32_t start_color, uint32_t end_color, uint32_t current_step, uint32_t total_steps)
{
    uint32_t result = 0;
    for (int i = 0; i < 4; i++)
    {
        uint8_t start = (start_color >> (8 * i)) & 0xFF;
        uint8_t end = (end_color >> (8 * i)) & 0xFF;
        uint8_t grad = start + (end - start) * current_step / total_steps;
        result |= (uint32_t)grad << (8 * i);
    }
    return result;
}

/*******************************************************************************
  Function:
    void draw_test_pattern(uint32_t width, uint32_t height)

  Summary:
    Renders a display calibration test pattern.

  Description:
    Draws a comprehensive test pattern for display calibration including:
    - Grid lines for geometry verification
    - Center circle for aspect ratio check
    - Corner circles for overscan detection
    - Color bars for color accuracy testing
    - Grayscale ramps for gamma calibration
    - Gradient bars for smooth color transition testing
    The pattern adapts to different display resolutions.

  Parameters:
    width  - Display width in pixels
    height - Display height in pixels

  Returns:
    None.
*******************************************************************************/
void draw_test_pattern(uint32_t width, uint32_t height)
{
    int x_div = (width < 800) ? 10 : 16;
    int y_div = 9;
    int str_width = draw_string(0, 0, "Blank Quickstart", FONT_BASE, 0);

    // Draw grid lines
    for (int i = 0; i <= x_div; i++)
    {
        int x = (width - 1) * i / x_div;
        draw_line(x, 0, x, height - 1, color_map[1]);
    }
    for (int i = 0; i <= y_div; i++)
    {
        int y = (height - 1) * i / y_div;
        draw_line(0, y, width - 1, y, color_map[1]);
    }

    // Draw center circle
    draw_circle(width / 2 - 1, height / 2 - 1, height / 2, color_map[1], false);

    // Draw corner circles
    int r = (width - 1) / x_div;
    for (int x = 1; x < x_div; x += x_div - 2)
    {
        for (int y = 1; y < y_div; y += y_div - 2)
        {
            int cx = (width - 1) * x / x_div;
            int cy = (height - 1) * y / y_div;
            draw_circle(cx, cy, r, color_map[1], false);
            draw_circle(cx, cy, r / 3, color_map[color_max], true);
        }
    }

    // Draw central rectangle and text
    int x1 = (width - 1) / x_div;
    int y1 = (height - 1) / y_div;
    int x2 = (width - 1) * (x_div - 1) / x_div;
    int y2 = (height - 1) * (y_div - 1) / y_div;
    draw_rect_fill(x1 * 2 + 2, y1 * 2 + 1, x2 - x1 - 2, y2 - y1 - 1, 0);
    x1 = ((x1 * 2 + 2) + x2 - x1 - 2) / 2;
    draw_string(x1 - str_width / 2, y2 - y1 + y1 / 2.75f, "Blank Quickstart", FONT_BASE, color_map[color_max]);

    // Draw color bars
    char tmp[16];
    for (int i = 2; i < x_div - 2; i++)
    {
        int cx = (width - 1) * i / x_div;
        int y_offset = (height - 1) / y_div;

        draw_rect_fill(cx + 2, y_offset * 2 + 2, cx + r - 1, y_offset * 3 - 2, color_map[i]);
        draw_rect_fill(cx + 2, y_offset * 3 + 2, cx + r - 1, y_offset * 4 - 2, color_map[i + 12]);
        draw_rect_fill(cx + 2, y_offset * 4 + 2, cx + r - 1, y_offset * 5 - 2, color_map[i + 24]);

        snprintf(tmp, sizeof(tmp), "%d%%", i - 1);
        draw_string(cx + 4, y_offset * 3 + 4, tmp, FONT_SM, color_map[color_max]);

        if (i == 2)
            snprintf(tmp, sizeof(tmp), "2%%");
        else if (i == 3)
            snprintf(tmp, sizeof(tmp), "5%%");
        else
            snprintf(tmp, sizeof(tmp), "%d%%", (i - 3) * 10);
        draw_string(cx + 4, y_offset * 4 + 4, tmp, FONT_SM, (i < 7) ? 0xFFFFFFFF : 0);
    }

    // Draw gradient bars
    x1 = (width - 1) * 2 / x_div + 2;
    x2 = (width - 1) * (x_div - 2) / x_div - 2;
    int mid = (x1 + x2) / 2;

    for (int j = 0; j < 3; j++)
    {
        y1 = (height - 1) * (5 + j * 0.667) / y_div + 2;
        y2 = (height - 1) * (5.667 + j * 0.667) / y_div - 2;

        for (int i = x1; i <= mid; i++)
        {
            draw_line(i, y1, i, y2, get_gradient(color_map[0], color_map[2 + 2 * j], i - x1, (x2 - x1) / 2));
        }
        for (int i = mid + 1; i <= x2; i++)
        {
            draw_line(i, y1, i, y2, get_gradient(color_map[2 + 2 * j], color_map[color_max], i - mid, (x2 - x1) / 2));
        }
    }
}

/*******************************************************************************
  Function:
    void init_touch_map(uint32_t width, uint32_t height)

  Summary:
    Initializes touch-sensitive regions for the test pattern.

  Description:
    Configures four rectangular touch zones corresponding to the corner
    circles in the test pattern. These zones are used to detect touch
    events for interactive testing of the touchscreen calibration.

  Parameters:
    width  - Display width in pixels
    height - Display height in pixels

  Returns:
    None.

  Remarks:
    Touch zones are stored in appData.touch_map[0..3] representing:
    [0] = Top-left corner, [1] = Bottom-left corner,
    [2] = Top-right corner, [3] = Bottom-right corner.
*******************************************************************************/
void init_touch_map(uint32_t width, uint32_t height)
{
    int x_div = (width < 800) ? 10 : 16;
    int y_div = 9;

    appData.touch_map[0].x1 = 0;
    appData.touch_map[0].y1 = 0;
    appData.touch_map[0].x2 = (width - 1) * 2 / x_div;
    appData.touch_map[0].y2 = (height - 1) * 2 / y_div;

    appData.touch_map[1].x1 = 0;
    appData.touch_map[1].y1 = (height - 1) * 7 / y_div;
    appData.touch_map[1].x2 = (width - 1) * 2 / x_div;
    appData.touch_map[1].y2 = height - 1;

    appData.touch_map[2].x1 = (width - 1) * (x_div - 2) / x_div;
    appData.touch_map[2].y1 = 0;
    appData.touch_map[2].x2 = width - 1;
    appData.touch_map[2].y2 = (height - 1) * 2 / y_div;

    appData.touch_map[3].x1 = (width - 1) * (x_div - 2) / x_div;
    appData.touch_map[3].y1 = (height - 1) * 7 / y_div;
    appData.touch_map[3].x2 = width - 1;
    appData.touch_map[3].y2 = height - 1;
}

/*******************************************************************************
  Function:
    void touchDownHandler(const SYS_INP_TouchStateEvent *const evt)

  Summary:
    Callback handler for touch press events.

  Description:
    Called by the input system when a touch-down event occurs. Checks if
    the touch coordinates fall within any of the four corner touch zones.
    If a zone is touched, a filled green circle is drawn as visual feedback.
    Also turns on the green LED to indicate active touch.

  Parameters:
    evt - Pointer to touch event structure containing x, y coordinates

  Returns:
    None.
*******************************************************************************/
void touchDownHandler(const SYS_INP_TouchStateEvent *const evt)
{
    for (int i = 0; i < 4; i++)
    {
        if (evt->x > appData.touch_map[i].x1 &&
            evt->x < appData.touch_map[i].x2 &&
            evt->y > appData.touch_map[i].y1 &&
            evt->y < appData.touch_map[i].y2)
        {
            draw_circle((appData.touch_map[i].x1 + appData.touch_map[i].x2) / 2,
                        (appData.touch_map[i].y1 + appData.touch_map[i].y2) / 2,
                        (appData.touch_map[0].x1 + appData.touch_map[0].x2) / 6,
                        color_map[4], true);
        }
    }
    LED_GREEN_On();
}

/*******************************************************************************
  Function:
    void touchUpHandler(const SYS_INP_TouchStateEvent *const evt)

  Summary:
    Callback handler for touch release events.

  Description:
    Called by the input system when a touch-up event occurs. Turns off
    both the green and blue LEDs to indicate the touch has ended.

  Parameters:
    evt - Pointer to touch event structure (coordinates unused)

  Returns:
    None.
*******************************************************************************/
void touchUpHandler(const SYS_INP_TouchStateEvent *const evt)
{
    LED_GREEN_Off();
    LED_BLUE_Off();
}

/*******************************************************************************
  Function:
    void touchMoveHandler(const SYS_INP_TouchMoveEvent *const evt)

  Summary:
    Callback handler for touch drag/move events.

  Description:
    Called by the input system when a touch-move (drag) event occurs.
    Turns on the blue LED to provide visual feedback that touch movement
    is being detected.

  Parameters:
    evt - Pointer to touch move event structure (coordinates unused)

  Returns:
    None.
*******************************************************************************/
void touchMoveHandler(const SYS_INP_TouchMoveEvent *const evt)
{
    LED_BLUE_On();
}

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
    Initializes the application to its default state.

  Description:
    Called once at system startup to initialize the application state machine
    and clear the application data structure. Sets the initial state to
    APP_STATE_INIT for the state machine to begin processing.

  Precondition:
    All other system initialization must be complete before calling.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    This function is called from SYS_Initialize.
 */

void APP_Initialize(void)
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    memset(&appData, 0, sizeof(appData));
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Summary:
    Application state machine task function.

  Description:
    Implements the application's main state machine. Called repeatedly from
    the system's main loop to process application states:

    - APP_STATE_INIT: Registers touch input handlers and retrieves display
      parameters (width, height, framebuffer pointer) from the XLCDC driver.
    - APP_STATE_SERVICE_TASKS: Draws the test pattern and initializes touch
      zones, then transitions to idle.
    - APP_STATE_IDLE: Normal operation state; touch events handled via callbacks.
    - APP_STATE_ERROR: Error state if display initialization fails.

  Precondition:
    APP_Initialize has been called.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    This function is called from SYS_Tasks.
 */

void APP_Tasks(void)
{
    /* Check the application's current state. */
    switch (appData.state)
    {
    /* Application's initial state. */
    case APP_STATE_INIT:
    {
        SYS_INP_InputListener appInputListener;
        gfxIOCTLArg_DisplaySize argDispSize;
        gfxIOCTLArg_Value argVal;
        bool appInitialized = true;

        if (appInitialized)
        {
            // Register the input event handlers
            appInputListener.handleTouchDown = &touchDownHandler;
            appInputListener.handleTouchUp = &touchUpHandler;
            appInputListener.handleTouchMove = &touchMoveHandler;
            SYS_INP_AddListener(&appInputListener);

            // Fetch display parameters
            if ((DRV_XLCDC_IOCTL(GFX_IOCTL_GET_DISPLAY_SIZE, &argDispSize) == GFX_IOCTL_OK) &&
                (DRV_XLCDC_IOCTL(GFX_IOCTL_GET_FRAMEBUFFER, &argVal) == GFX_IOCTL_OK))
            {
                appData.width = argDispSize.width;
                appData.height = argDispSize.height;
                appData.buffer = argVal.value.v_pbuffer->pixels;
                appData.state = APP_STATE_SERVICE_TASKS;
            }
            else
            {
                appData.state = APP_STATE_ERROR;
            }
        }
        break;
    }

    case APP_STATE_SERVICE_TASKS:
    {
        draw_test_pattern(appData.width, appData.height);
        init_touch_map(appData.width, appData.height);

        /* Call Swap to move the frame buffer to front */
        DRV_XLCDC_IOCTL(GFX_IOCTL_LAYER_SWAP, NULL);

        appData.state = APP_STATE_IDLE;
        break;
    }

    case APP_STATE_IDLE:
    {
        break;
    }

    case APP_STATE_ERROR:
    {
        break;
    }

    default:
    {
        break;
    }
    }
}

/*******************************************************************************
 End of File
 */
