/*
 * Copyright (C) 2021 David VanKampen
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    drivers_sh1107 SH1107 OLED
 * @ingroup     drivers_display
 * @brief       SH1107 OLED Display
 *
 * @{
 *
 * @file
 *
 * @author      David VanKampen <david.vankampen@bissell.com>
 */

#ifndef SH1107_H
#define SH1107_H

#include <stdbool.h>
#include "periph/i2c.h"
#include "periph/gpio.h"

#ifdef MODULE_DISP_DEV
#include "disp_dev.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Declare the API of the driver */

/**
 * @brief   Named return values
 */
enum {
    SH1107_OK       =  0,               /**< everything was fine */
    SH1107_NOI2C    = -1,               /**< I2C communication failed */
    SH1107_NODEV    = -2,               /**< no SH1107 device found on the bus */
    SH1107_NODATA   = -3,               /**< no data available */
    SH1107_NOARGS   = -4,
    SH1107_INVALID  = -5
};

/**
 * @brief   Device initialization parameters
 */
typedef struct {
    i2c_t i2c;
    uint8_t addr;
    gpio_t reset;
    uint8_t width;
    uint8_t height;
} sh1107_params_t;

/**
 * @brief   Device descriptor for the driver
 */
typedef struct {
#ifdef MODULE_DISP_DEV
    disp_dev_t *dev;
#endif
    const sh1107_params_t *params;
} sh1107_t;

/**
 * @brief   Initialize the given device
 *
 * @param[inout] dev        Device descriptor of the driver
 * @param[in]    params     Initialization parameters
 *
 * @return                  0 on success
 */
int sh1107_init(sh1107_t *dev, const sh1107_params_t *params);

void sh1107_clear_display(sh1107_t *dev);

void sh1107_invert_display(const sh1107_t *dev, bool invert);

void sh1107_set_rotation(sh1107_t *dev, uint8_t rot);

int sh1107_set_text_size(sh1107_t *dev, uint8_t size);

int sh1107_set_text_color(sh1107_t *dev, uint16_t color);

int sh1107_set_cursor(sh1107_t *dev, uint8_t x, uint8_t y);

int sh1107_print(sh1107_t *dev, char *msg);

void sh1107_display(sh1107_t *dev);

void sh1107_fill_screen(sh1107_t *dev, uint16_t color);

void sh1107_draw_pixel(sh1107_t *dev, int16_t x, int16_t y, uint16_t color);

void sh1107_byte_swap(sh1107_t *dev);

uint16_t sh1107_get_pixel(sh1107_t *dev, int16_t x, int16_t y);

void sh1107_draw_fast_v_line(sh1107_t *dev, int16_t x, int16_t y, int16_t h,
                             uint16_t color);
void sh1107_draw_fast_h_line(sh1107_t *dev, int16_t x, int16_t y, int16_t w,
                             uint16_t color);

void sh1107_set_contrast(sh1107_t *dev, uint8_t contrast_level);

void sh1107_draw_bitmap(sh1107_t *dev, int16_t x, int16_t y,
                        const uint8_t bitmap[],
                        int16_t w, int16_t h,
                        uint16_t color);

#ifdef __cplusplus
}
#endif

#endif /* SH1107_H */
/** @} */
