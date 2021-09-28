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
 * @brief       SH1107 OLED Display disp_dev generic interface
 *
 * @{
 *
 * @file
 *
 * @author      David VanKampen <david.vankampen@bissell.com>
 */

#include <assert.h>
#include <stdint.h>
#include "byteorder.h"

#include "sh1107.h"
#include "sh1107_disp_dev.h"

#ifndef SH1107_DISP_COLOR_DEPTH
#define SH1107_DISP_COLOR_DEPTH    (1U)
#endif

static void _sh1107_map(const disp_dev_t *dev, uint16_t x1, uint16_t x2,
                        uint16_t y1, uint16_t y2, const uint16_t *color)
{
    sh1107_t *sh1107 = (sh1107_t *)dev;
    assert(sh1107);

    uint8_t* ncolor = (uint8_t*)color;
    for (uint8_t y = y1; y <= y2; y++) {
        for (uint8_t x = x1; x <= x2; x++) {
            sh1107_draw_pixel(sh1107, x, y, *ncolor);
            ncolor++;
        }
    }

    sh1107_display(sh1107);
}

static uint16_t _sh1107_height(const disp_dev_t *disp_dev)
{
    const sh1107_t *dev = (sh1107_t *)disp_dev;

    assert(dev);

    return dev->params->height;
}

static uint16_t _sh1107_width(const disp_dev_t *disp_dev)
{
    const sh1107_t *dev = (sh1107_t *)disp_dev;

    assert(dev);

    return dev->params->width;
}

static uint8_t _sh1107_color_depth(const disp_dev_t *disp_dev)
{
    (void)disp_dev;
    return SH1107_DISP_COLOR_DEPTH;
}

static void _sh1107_set_invert(const disp_dev_t *disp_dev, bool invert)
{
    const sh1107_t *dev = (sh1107_t *)disp_dev;

    assert(dev);

    sh1107_invert_display(dev, invert);
}

const disp_dev_driver_t sh1107_disp_dev_driver = {
    .map = _sh1107_map,
    .height = _sh1107_height,
    .width = _sh1107_width,
    .color_depth = _sh1107_color_depth,
    .set_invert = _sh1107_set_invert,
};

/** @} */