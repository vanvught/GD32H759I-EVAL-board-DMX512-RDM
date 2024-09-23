/**
 * @file gd32h759_mcu.h
 *
 */
/* Copyright (C) 2024 by Arjan van Vught mailto:info@gd32-dmx.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef MCU_GD32H759_MCU_H_
#define MCU_GD32H759_MCU_H_

#if !defined(GD32H759)
# error This file should not be included
#endif

#include <stdint.h>

#define MCU_CLOCK_FREQ		(uint32_t)(600000000)
#define AHB_CLOCK_FREQ     	(uint32_t)(300000000)
#define APB1_CLOCK_FREQ		(uint32_t)(150000000)
#define APB2_CLOCK_FREQ		(uint32_t)(300000000)
#define APB3_CLOCK_FREQ     (uint32_t)(150000000)
#define APB4_CLOCK_FREQ     (uint32_t)(150000000)
#define TIMER_PSC_1MHZ		(uint16_t)(299)
#define TIMER_PSC_10KHZ		(uint16_t)(29999)

#include "gd32h7xx_mcu.h"

#endif /* MCU_GD32H759_MCU_H_ */
