/*
 * Copyright (C) 2021 David VanKampen
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_sh1107
 *
 * @{
 * @file
 * @brief       Default configuration
 *
 * @author      David VanKampen <david.vankampen@bissell.com>
 */

#ifndef SH1107_PARAMS_H
#define SH1107_PARAMS_H

#include "board.h"
#include "sh1107.h"
#include "sh1107_constants.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Set default configuration parameters
 * @{
 */
#ifndef SH1107_PARAM_PARAM1
#define SH1107_PARAM_PARAM1
#endif

#ifndef SH1107_PARAMS
#define SH1107_PARAMS { \
        .i2c = I2C_DEV(0), \
        .addr = SH1107_ADDR, \
        .width = 64, \
        .height = 128, \
        .reset = GPIO_UNDEF \
}
#endif
/**@}*/

/**
 * @brief   Configuration struct
 */
static const sh1107_params_t sh1107_params[] = {
    SH1107_PARAMS
};

/**
 * @brief   Default screen identifiers
 */
#ifndef SH1107_PARAM_SCREEN_IDS
#define SH1107_PARAM_SCREEN_IDS    0
#endif

/**
 * @brief   Configure screen identifiers
 */
static const uint8_t sh1107_screen_ids[] =
{
    SH1107_PARAM_SCREEN_IDS,
};

#ifdef __cplusplus
}
#endif

#endif /* SH1107_PARAMS_H */
/** @} */
