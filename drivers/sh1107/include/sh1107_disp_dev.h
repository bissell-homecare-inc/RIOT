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

#ifndef SH1107_DISP_DEV_H
#define SH1107_DISP_DEV_H

#include "disp_dev.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Reference to the display device driver struct
 */
extern const disp_dev_driver_t sh1107_disp_dev_driver;

#ifdef __cplusplus
}
#endif

#endif /* SH1107_DISP_DEV_H */
/** @} */
