/*
 * Copyright (C) 2021 David VanKampen
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_sh1107
 * @{
 *
 * @file
 * @brief       Device driver implementation for the SH1107 OLED
 *
 * @author      David VanKampen <david.vankampen@bissell.com>
 *
 * @}
 */

#include "sh1107.h"
#include "sh1107_constants.h"
#include "sh1107_params.h"
#include "periph/i2c.h"
#include "xtimer.h"
#include <stdlib.h> // for malloc
#include <string.h> // for memset


#define grayoled_swap(a, b)                                                    \
    (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation

static uint8_t *buffer = NULL;

static uint8_t _bpp = 1; // bytes per pixel color for this display


int16_t _width;         ///< Display width as modified by current rotation
int16_t _height;        ///< Display height as modified by current rotation
int16_t cursor_x;       ///< x location to start print()ing text
int16_t cursor_y;       ///< y location to start print()ing text
uint16_t textcolor;     ///< 16-bit background color for print()
uint16_t textbgcolor;   ///< 16-bit text color for print()
uint8_t textsize_x;     ///< Desired magnification in X-axis of text to print()
uint8_t textsize_y;     ///< Desired magnification in Y-axis of text to print()
uint8_t rotation;       ///< Display rotation (0 thru 3)

int16_t window_x1,      ///< Dirty tracking window minimum x
        window_y1,      ///< Dirty tracking window minimum y
        window_x2,      ///< Dirty tracking window maximum x
        window_y2;      ///< Dirty tracking window maximum y

bool oled_command(const sh1107_t *dev, uint8_t c)
{
    uint8_t buf[2] = { 0x00, c }; // Co = 0, D/C = 0

    if (i2c_write_bytes(dev->params->i2c, dev->params->addr, buf, 2, 0)) {
        return false;
    }
    return true;
}

bool oled_commandList(sh1107_t *dev, const uint8_t *c, uint8_t n)
{
    if (i2c_write_bytes(dev->params->i2c, dev->params->addr, (uint8_t *)c, n,
                        0)) {
        return false;
    }
    return true;
}

int sh1107_init(sh1107_t *dev, const sh1107_params_t *params)
{
    if (dev == NULL || params == NULL) {
        return SH1107_NOARGS;
    }
    dev->params = params;
    if (dev->params->reset != GPIO_UNDEF) {
        gpio_init(dev->params->reset, GPIO_OUT);
        gpio_set(dev->params->reset);
        xtimer_usleep(200);
        gpio_clear(dev->params->reset);
        xtimer_usleep(200);
        gpio_set(dev->params->reset);
        xtimer_usleep(200);
    }
    i2c_init(I2C_DEV(params->i2c));

    if ((!buffer) &&
        !(buffer =
              (uint8_t *)malloc(_bpp * dev->params->width *
                                ((dev->params->height + 7) / 8)))) {
        return false;
    }

    sh1107_clear_display(dev);

    // set max dirty window
    window_x1 = 0;
    window_y1 = 0;
    window_x2 = dev->params->width - 1;
    window_y2 = dev->params->height - 1;

    sh1107_set_contrast(dev, 0x2F);
    sh1107_set_rotation(dev, 0);

    static const uint8_t init[] = {
        0x00,
        SH110X_DISPLAYOFF,                  // 0xAE
        SH110X_SETDISPLAYCLOCKDIV, 0x51,    // 0xd5, 0x51,
        SH110X_MEMORYMODE,                  // 0x20
        SH110X_SETCONTRAST, 0x4F,           // 0x81, 0x4F
        SH110X_DCDC, 0x8A,                  // 0xAD, 0x8A
        SH110X_SEGREMAP,                    // 0xA0
        SH110X_COMSCANINC,                  // 0xC0
        SH110X_SETDISPSTARTLINE, 0x0,       // 0xDC 0x00
        SH110X_SETDISPLAYOFFSET, 0x60,      // 0xd3, 0x60,
        SH110X_SETPRECHARGE, 0x22,          // 0xd9, 0x22,
        SH110X_SETVCOMDETECT, 0x35,         // 0xdb, 0x35,
        SH110X_SETMULTIPLEX, 0x3F,          // 0xa8, 0x3f,
        SH110X_DISPLAYALLON_RESUME,         // 0xa4
        SH110X_NORMALDISPLAY,               // 0xa6
    };

    if (!oled_commandList(dev, init, sizeof(init))) {
        printf("Failed to initialize display\r\n");
        return -1;
    }
    xtimer_usleep(1000000);
    if (!oled_command(dev, SH110X_DISPLAYON)) {
        printf("Failed to turn display on\r\n");
        return -2;
    }

    sh1107_display(dev);
    return 0;
}

void sh1107_set_contrast(sh1107_t *dev, uint8_t contrast_level)
{
    uint8_t cmd[] = { 0x00, GRAYOLED_SETCONTRAST, contrast_level };

    if (!oled_commandList(dev, cmd, 2)) {
        printf("Failed to set contrast\r\n");
    }

}

void sh1107_invert_display(const sh1107_t *dev, bool invert)
{
    if (!oled_command(
            dev, invert ? SH110X_INVERTDISPLAY : SH110X_NORMALDISPLAY)) {

        printf("Failed to set display invert: %u\r\n", invert);
    }
}

void sh1107_set_rotation(sh1107_t *dev, uint8_t rot)
{
    rotation = (rot & 3);
    switch (rotation) {
    case 0:
    case 2:
        _width = dev->params->width;
        _height = dev->params->height;
        break;
    case 1:
    case 3:
        _width = dev->params->height;
        _height = dev->params->width;
        break;
    }

}

void sh1107_clear_display(sh1107_t *dev)
{
    memset(buffer, 0,
           _bpp * dev->params->width * ((dev->params->height + 7) / 8));
    // set max dirty window
    window_x1 = 0;
    window_y1 = 0;
    window_x2 = dev->params->width - 1;
    window_y2 = dev->params->height - 1;
}

#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_word
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif

void sh1107_draw_bitmap(sh1107_t *dev, int16_t x, int16_t y,
                        const uint8_t bitmap[],
                        int16_t w, int16_t h,
                        uint16_t color)
{
    int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    uint8_t byte = 0;

    for (int16_t j = 0; j < h; j++, y++) {
        for (int16_t i = 0; i < w; i++) {
            if (i & 7) {
                byte <<= 1;
            }
            else {
                byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
            }
            if (byte & 0x80) {
                sh1107_draw_pixel(dev, x + i, y, color);
            }
        }
    }
}

void sh1107_draw_pixel(sh1107_t *dev, int16_t x, int16_t y, uint16_t color)
{
    if (buffer) {
        if ((x >= 0) && (x < _width) && (y >= 0) && (y < _height)) {
            // Pixel is in-bounds. Rotate coordinates if needed.
            switch (rotation) {
            case 1:
                grayoled_swap(x, y);
                x = dev->params->width - x - 1;
                break;
            case 2:
                x = dev->params->width - x - 1;
                y = dev->params->height - y - 1;
                break;
            case 3:
                grayoled_swap(x, y);
                y = dev->params->height - y - 1;
                break;
            }

            // adjust dirty window
            window_x1 = window_x1 < x ? window_x1 : x;
            window_y1 = window_y1 < y ? window_y1 : y;
            window_x2 = window_x2 > x ? window_x2 : x;
            window_y2 = window_y2 > y ? window_y2 : y;

            if (_bpp == 1) {
                switch (color) {
                case MONOOLED_WHITE:
                    buffer[x + (y / 8) * dev->params->width] |= (1 << (y & 7));
                    break;
                case MONOOLED_BLACK:
                    buffer[x + (y / 8) * dev->params->width] &= ~(1 << (y & 7));
                    break;
                case MONOOLED_INVERSE:
                    buffer[x + (y / 8) * dev->params->width] ^= (1 << (y & 7));
                    break;
                }
            }
            if (_bpp == 4) {
                uint8_t *pixelptr =
                    &buffer[x / 2 + (y * dev->params->width / 2)];
                if (x % 2 == 0) { // even, left nibble
                    uint8_t t = pixelptr[0] & 0x0F;
                    t |= (color & 0xF) << 4;
                    pixelptr[0] = t;
                }
                else { // odd, right lower nibble
                    uint8_t t = pixelptr[0] & 0xF0;
                    t |= color & 0xF;
                    pixelptr[0] = t;
                }
            }
        }
    }
}

void sh1107_display(sh1107_t *dev)
{
    // ESP8266 needs a periodic yield() call to avoid watchdog reset.
    // With the limited size of SH110X displays, and the fast bitrate
    // being used (1 MHz or more), I think one yield() immediately before
    // a screen write and one immediately after should cover it.  But if
    // not, if this becomes a problem, yields() might be added in the
    // 32-byte transfer condition below.
    thread_yield();

    uint8_t *ptr = buffer;
    uint8_t dc_byte = 0x40;
    uint8_t pages = ((dev->params->height + 7) / 8);

    uint8_t bytes_per_page = dev->params->width;
    uint16_t maxbuff = 31;


    uint8_t first_page = window_y1 / 8;
    uint8_t page_start = bytes_per_page <
                         (uint8_t)window_x1 ? bytes_per_page : (uint8_t)
                         window_x1;
    uint8_t page_end = 0 > window_x2 ? 0 : (uint8_t)window_x2;

    i2c_acquire(dev->params->i2c);
    for (uint8_t p = first_page; p < pages; p++) {
        uint8_t bytes_remaining = bytes_per_page;
        ptr = buffer + (uint16_t)p * (uint16_t)bytes_per_page;
        // fast forward to dirty rectangle beginning
        ptr += page_start;
        bytes_remaining -= page_start;
        // cut off end of dirty rectangle
        bytes_remaining -= (dev->params->width - 1) - page_end;

        uint8_t cmd[] =
        { 0x00, SH110X_SETPAGEADDR + p, 0x10 + (page_start >> 4),
          page_start & 0xF };
        int status = i2c_write_bytes(dev->params->i2c, dev->params->addr, cmd, 4,
                                     0);


        while (bytes_remaining && status == 0) {
            uint8_t buf[255] = { 0 };
            uint8_t to_write = bytes_remaining <
                               (uint8_t)maxbuff ? bytes_remaining : (uint8_t)
                               maxbuff;
            buf[0] = dc_byte;
            memcpy(&buf[1], ptr, to_write);
            status = i2c_write_bytes(dev->params->i2c, dev->params->addr, buf,
                                     to_write + 1,
                                     0);
            ptr += to_write;
            bytes_remaining -= to_write;
            thread_yield();
        }
    }
    i2c_release(dev->params->i2c);
    // reset dirty window
    window_x1 = 1024;
    window_y1 = 1024;
    window_x2 = -1;
    window_y2 = -1;
}
