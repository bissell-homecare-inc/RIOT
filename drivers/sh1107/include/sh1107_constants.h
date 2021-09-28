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
 * @brief       Internal addresses, registers and constants
 *
 * @author      David VanKampen <david.vankampen@bissell.com>
 */

#ifndef SH1107_CONSTANTS_H
#define SH1107_CONSTANTS_H

#ifdef __cplusplus
extern "C" {
#endif

/* define here the addresses, register and constants of the driver */
#define SH1107_ADDR 0x3C
/// fit into the SH110X_ naming scheme
#define SH110X_BLACK 0                  ///< Draw 'off' pixels
#define SH110X_WHITE 1                  ///< Draw 'on' pixels
#define SH110X_INVERSE 2                ///< Invert pixels

#define SH110X_MEMORYMODE 0x20          ///< See datasheet
#define SH110X_COLUMNADDR 0x21          ///< See datasheet
#define SH110X_PAGEADDR 0x22            ///< See datasheet
#define SH110X_SETCONTRAST 0x81         ///< See datasheet
#define SH110X_CHARGEPUMP 0x8D          ///< See datasheet
#define SH110X_SEGREMAP 0xA0            ///< See datasheet
#define SH110X_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SH110X_DISPLAYALLON 0xA5        ///< Not currently used
#define SH110X_NORMALDISPLAY 0xA6       ///< See datasheet
#define SH110X_INVERTDISPLAY 0xA7       ///< See datasheet
#define SH110X_SETMULTIPLEX 0xA8        ///< See datasheet
#define SH110X_DCDC 0xAD                ///< See datasheet
#define SH110X_DISPLAYOFF 0xAE          ///< See datasheet
#define SH110X_DISPLAYON 0xAF           ///< See datasheet
#define SH110X_SETPAGEADDR                                                     \
    0xb0                                ///< Specify page address to load display RAM data to page address
                                        ///< register
#define SH110X_COMSCANINC 0xC0          ///< Not currently used
#define SH110X_COMSCANDEC 0xC8          ///< See datasheet
#define SH110X_SETDISPLAYOFFSET 0xD3    ///< See datasheet
#define SH110X_SETDISPLAYCLOCKDIV 0xD5  ///< See datasheet
#define SH110X_SETPRECHARGE 0xD9        ///< See datasheet
#define SH110X_SETCOMPINS 0xDA          ///< See datasheet
#define SH110X_SETVCOMDETECT 0xDB       ///< See datasheet
#define SH110X_SETDISPSTARTLINE                                                \
    0xDC                                ///< Specify Column address to determine the initial display line or
                                        ///< COM0.

#define SH110X_SETLOWCOLUMN 0x00        ///< Not currently used
#define SH110X_SETHIGHCOLUMN 0x10       ///< Not currently used
#define SH110X_SETSTARTLINE 0x40        ///< See datasheet

#define GRAYOLED_SETCONTRAST 0x81       ///< Generic contrast for almost all OLEDs
#define GRAYOLED_NORMALDISPLAY 0xA6     ///< Generic non-invert for almost all OLEDs
#define GRAYOLED_INVERTDISPLAY 0xA7     ///< Generic invert for almost all OLEDs

#define MONOOLED_BLACK 0                ///< Default black 'color' for monochrome OLEDS
#define MONOOLED_WHITE 1                ///< Default white 'color' for monochrome OLEDS
#define MONOOLED_INVERSE 2              ///< Default inversion command for monochrome OLEDS

#ifdef __cplusplus
}
#endif

#endif /* SH1107_CONSTANTS_H */
/** @} */
