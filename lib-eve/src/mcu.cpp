/**
 * @file mcu.cpp
 *
 */
/* Copyright (C) 2024-2026 by Arjan van Vught mailto:info@gd32-dmx.org
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

#include "hal_gpio.h"
#include "spi.h"
#include "firmware/debug/debug_printbits.h"
 #include "firmware/debug/debug_debug.h"

void MCU_Init() {
	DEBUG_ENTRY();

	spi::Begin();
	spi::ChipSelect(spi::kCs);
	spi::SetSpeedHz(1000000);
	spi::SetDataMode(spi::kMode0);

	FUNC_PREFIX(GpioFsel(FT8XX_LCD_CS_GPIO, GPIO_FSEL_OUTPUT));
	FUNC_PREFIX(GpioFsel(FT8XX_LCD_DC_GPIO, GPIO_FSEL_OUTPUT));

	FUNC_PREFIX(GpioFsel(FT8XX_LCD_INT_GPIO, GPIO_FSEL_INPUT));
	FUNC_PREFIX(GpioSetPud(FT8XX_LCD_INT_GPIO, GPIO_PULL_UP));

	FUNC_PREFIX(GpioSet(FT8XX_LCD_CS_GPIO));
	FUNC_PREFIX(GpioSet(FT8XX_LCD_DC_GPIO));

	DEBUG_EXIT();
}

#include "EVE.h"
#include "HAL.h"

void MCU_Setup() {
	DEBUG_ENTRY();

	HAL_MemWrite8(EVE_REG_INT_EN, 1);
	// TOUCH, TAG and CMDEMPTY
	const auto kMask = static_cast<uint8_t>((1U << 1) | (1U << 2) | (1U << 5));
	debug::PrintBits(kMask);
	HAL_MemWrite8(EVE_REG_INT_MASK, kMask);

	DEBUG_EXIT();
}
