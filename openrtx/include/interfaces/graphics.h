/***************************************************************************
 *   Copyright (C) 2020 by Federico Amedeo Izzo IU2NUO,                    *
 *                         Niccolò Izzo IU2KIN                             *
 *                         Silvano Seva IU2KWO                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   As a special exception, if other files instantiate templates or use   *
 *   macros or inline functions from this file, or you compile this file   *
 *   and link it with other works to produce a work based on this file,    *
 *   this file does not by itself cause the resulting work to be covered   *
 *   by the GNU General Public License. However the source code for this   *
 *   file must still be made available in accordance with the GNU General  *
 *   Public License. This exception does not invalidate any other reasons  *
 *   why a work based on this file might be covered by the GNU General     *
 *   Public License.                                                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <stdbool.h>
#include <interfaces/display.h>
#include <state.h>
#include <gfxfont.h>
#include <TomThumb.h>
#include <FreeSans6pt7b.h>
#include <FreeSans8pt7b.h>
#include <FreeSans9pt7b.h>
#include <FreeSans10pt7b.h>
#include <FreeSans12pt7b.h>
#include <FreeSans16pt7b.h>
#include <FreeSans18pt7b.h>
#include <FreeSans24pt7b.h>

/**
 * Standard high-level graphic interface for all display types.
 * This interface is based on the lower level interface display.h
 * Many methods of graphics.h are basically calls to methods in display.h.
 *
 * On top of basic framebuffer control, graphics.h implements
 * graphics primitives for drawing fonts and shapes
 *
 *********************** HOW TO MANAGE FRAMEBUFFER *****************************
 *
 * This driver allocates the framebuffer as a block of linearly addressed memory
 * equivalent to an array of SCREEN_HEIGHT*SCREEN_WIDTH elements.
 * With respect to it, screen is indexed in this way:
 *
 *   (0,0)
 *     +-------> x
 *     |
 *     |    o (X,Y)
 *     |
 *     v
 *     y
 *
 * then to set the value of the pixel having coordinates (X,Y), framebuffer has
 * to be indexed in this way: buf[X + Y*SCREEN_WIDTH].
 *
 */

/**
 * Structure that represents the X,Y coordinates of a single point
 */
typedef struct point_t
{
    uint16_t x;
    uint16_t y;
} point_t;

/**
 * Structure that represents a single color in the RGB 8 bit per channel format
 */
typedef struct color_t
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t alpha;
} color_t;

typedef enum
{
    FONT_SIZE_5PT = 0,
    FONT_SIZE_6PT,
    FONT_SIZE_8PT,
    FONT_SIZE_9PT,
    FONT_SIZE_10PT,
    FONT_SIZE_12PT,
    FONT_SIZE_16PT,
    FONT_SIZE_18PT,
    FONT_SIZE_24PT
} fontSize_t;

typedef enum
{
    TEXT_ALIGN_LEFT = 0,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_RIGHT
} textAlign_t;

/**
 * Selection of the fonts, ordered by the fontSize_t enum.
 */
static GFXfont const fonts[] = { TomThumb,           // 5pt
                                 FreeSans6pt7b,      // 6pt
                                 FreeSans8pt7b,      // 8pt
                                 FreeSans9pt7b,      // 9pt
                                 FreeSans10pt7b,     // 10pt
                                 FreeSans12pt7b,     // 12pt
                                 FreeSans16pt7b,     // 16pt
                                 FreeSans18pt7b,     // 16pt
                                 FreeSans24pt7b };   // 24pt

/**
 * This function calls the correspondent method of the low level interface display.h
 * It initializes the display.
 */
void gfx_init();

/**
 * This function calls the correspondent method of the low level interface display.h
 * It terminates the display driver and deallocates the framebuffer.
 */
void gfx_terminate();

/**
 * This function calls the correspondent method of the low level interface display.h
 * Copy a given section, between two given rows, of framebuffer content to the
 * display.
 * @param startRow: first row of the framebuffer section to be copied
 * @param endRow: last row of the framebuffer section to be copied
 */
void gfx_renderRows(uint8_t startRow, uint8_t endRow);

/**
 * This function calls the correspondent method of the low level interface display.h
 * Copy framebuffer content to the display internal buffer. To be called
 * whenever there is need to update the display.
 */
void gfx_render();

/**
 * This function calls the correspondent method of the low level interface display.h
 * Check if framebuffer is being copied to the screen or not, in which case it
 * can be modified without problems.
 * @return false if rendering is not in progress.
 */
bool gfx_renderingInProgress();

/**
 * Clears a portion of the screen content
 * This results in a black screen on color displays
 * And a white screen on B/W displays
 * @param startRow: first row of the framebuffer section to be cleared
 * @param endRow: last row of the framebuffer section to be cleared
 */
void gfx_clearRows(uint8_t startRow, uint8_t endRow);

/**
 * Clears the content of the screen
 * This results in a black screen on color displays
 * And a white screen on B/W displays
 */
void gfx_clearScreen();

/**
 * Fills screen with the specified color.
 * @param color: border and fill color, in color_t format.
 */
void gfx_fillScreen(color_t color);

/**
 * Change the color of a single pixel.
 * @param pos: x,y coordinates of the pixel.
 * @param color: desired pixel color, in color_t format.
 */
void gfx_setPixel(point_t pos, color_t color);

/**
 * Draw a line from start to end coordinates, ends included.
 * @param start: line start point, in pixel coordinates.
 * @param end: line end point, in pixel coordinates.
 * @param color: line color, in color_t format.
 */
void gfx_drawLine(point_t start, point_t end, color_t color);


/**
 * Draw a horizontal line with specified vertical position and width.
 * @param y: vertical position, in pixel coordinates.
 * @param height: line height, in pixel coordinates.
 * @param color: line color, in color_t format.
 */
void gfx_drawHLine(uint16_t y, uint16_t height, color_t color);

/**
 * Draw a vertical line with specified horizontal position and width.
 * @param x: horizontal position, in pixel coordinates.
 * @param width: line width, in pixel coordinates.
 * @param color: line color, in color_t format.
 */
void gfx_drawVLine(uint16_t x, uint16_t width, color_t color);

/**
 * Draw a rectangle of specified width, height and color.
 * @param start: screen position of the rectangle, in pixels
 * @param width: rectangle width, in pixels, borders included.
 * @param height: rectangle height, in pixels, borders included.
 * @param color: border and fill color, in color_t format.
 * @param fill: if true the rectangle will be solid, otherwise it will be empty with a 1-pixel border
 */
void gfx_drawRect(point_t start, uint16_t width, uint16_t height, color_t color, bool fill);

/**
 * Draw the outline of a circle of specified radius and color.
 * @param start: screen position of the center of the circle, in pixels
 * @param r: circle radius, in pixels, border included.
 * @param color: border color, in color_t format.
 */
void gfx_drawCircle(point_t start, uint16_t r, color_t color);

/**
 * Prints text on the screen at the specified coordinates.
 * @param start: text line start point, in pixel coordinates.
 * @param size: text font size, defined as enum.
 * @param alignment: text alignment type, defined as enum.
 * @param color: text color, in color_t format.
 * @param fmt: printf style format string
 * @return text width and height as point_t coordinates
 */
point_t gfx_print(point_t start, fontSize_t size, textAlign_t alignment, color_t color, const char* fmt, ... );

/**
 * Prints text on the screen, calculating the print position.
 * The print position is calculated to fit the desired number of lines in the vertical space
 * @param cur: current line number over total (1-based)
 * @param tot: number of lines to fit in screen
 * @param startY: starting Y coordinate to leave space for top bar
 * @param endY: ending Y coordinate to leave space for bottom bar
 * @param startX: starting X coordinate to leave space on the screen sides
 * @param size: text font size, defined as enum.
 * @param alignment: text alignment type, defined as enum.
 * @param color: text color, in color_t format.
 * @param fmt: printf style format string
 * @return text width and height as point_t coordinates
 */
point_t gfx_printLine(uint8_t cur, uint8_t tot, uint16_t startY, uint16_t endY, uint16_t startX, 
                      fontSize_t size, textAlign_t alignment, color_t color, const char* fmt, ... );

/**
 * Prints an error message surrounded by a red box on the screen.
 * @param text: text to print.
 * @param size: text font size, defined as enum.
 */
void gfx_printError(const char *text, fontSize_t size);

/**
 * Function to draw battery of arbitrary size.
 * Starting coordinates are relative to the top left point.
 * @param start: battery icon start point, in pixel coordinates.
 * @param width: battery icon width
 * @param height: battery icon height
 * @param percentage: battery charge percentage
 */
void gfx_drawBattery(point_t start, uint16_t width, uint16_t height, float percentage);

/**
 * Function to draw Smeter of arbitrary size.
 * Starting coordinates are relative to the top left point.
 * @param start: Smeter start point, in pixel coordinates.
 * @param width: Smeter width
 * @param height: Smeter height
 * @param rssi: rssi level in dBm
 * @param squelch: squelch level in percentage
 * @param color: color of the squelch and smeter bar
 */
void gfx_drawSmeter(point_t start, uint16_t width, uint16_t height, float rssi, float squelch, color_t color);

/**
 * Function to draw GPS SNR bar graph of arbitrary size.
 * Starting coordinates are relative to the top left point.
 * @param start: Bar graph start point, in pixel coordinates.
 * @param width: Bar graph width
 * @param height: Bar graph height
 * @param sats: pointer to the array of satellites data
 * @param active_sats: bitset representing which sats are part of the fix
 */
void gfx_drawGPSgraph(point_t start, uint16_t width, uint16_t height, sat_t *sats, uint32_t active_sats);

/**
 * Function to draw a compass of arbitrary size.
 * Starting coordinates are relative to the top left point.
 * @param start: Compass start point, in pixel coordinates.
 * @param width: Compass radius
 * @param deg: degrees marked by the compass needle
 * @param active: whether the needle is to be drawn or not
 */
void gfx_drawGPScompass(point_t start, uint16_t radius, float deg, bool active);

#endif /* GRAPHICS_H */
