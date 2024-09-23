/**
 * @file gd32h759i_eval.h
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

#ifndef BOARD_GD32H759I_EVAL_H_
#define BOARD_GD32H759I_EVAL_H_

#if !defined(BOARD_GD32H759I_EVAL)
# error This file should not be included
#endif

#if defined (MCU_GD32H759_MCU_H_)
# error This file should be included later
#endif

#include <stdint.h>

#define USE_ENET0

#define USE_USB_HS
#define USE_USBHS1

/**
 * U(S)ART
 */

#define USART2_FULL_REMAP

/**
 * LEDs
 */

#define LED1_GPIO_PINx		GPIO_PIN_10
#define LED1_GPIOx			GPIOF
#define LED1_RCU_GPIOx		RCU_GPIOF

#define LED2_GPIO_PINx		GPIO_PIN_6
#define LED2_GPIOx			GPIOA
#define LED2_RCU_GPIOx		RCU_GPIOA

#define LED3_GPIO_PINx		GPIO_PIN_6
#define LED3_GPIOx			GPIOA
#define LED3_RCU_GPIOx		RCU_GPIOA

#define LED_BLINK_PIN       LED1_GPIO_PINx
#define LED_BLINK_GPIO_PORT LED1_GPIOx
#define LED_BLINK_GPIO_CLK	LED1_RCU_GPIOx

/**
 * KEYs
 */

#define KEY1_PINx						GPIO_PIN_0
#define KEY1_GPIOx						GPIOA
#define KEY1_RCU_GPIOx					RCU_GPIOA

#define KEY2_PINx						GPIO_PIN_13
#define KEY2_GPIOx						GPIOC
#define KEY2_RCU_GPIOx					RCU_GPIOC

#define KEY3_PINx						GPIO_PIN_8
#define KEY3_GPIOx						GPIOF
#define KEY3_RCU_GPIOx					RCU_GPIOF

#define KEY_BOOTLOADER_TFTP_GPIO_PINx	KEY2_PINx
#define KEY_BOOTLOADER_TFTP_GPIOx		KEY2_GPIOx
#define KEY_BOOTLOADER_TFTP_RCU_GPIOx	KEY2_RCU_GPIOx

/**
 * I2C
 */

#define I2C_PERIPH			I2C1_PERIPH
#define I2C_RCU_I2Cx		I2C1_RCU_I2C1
#define I2C_RCU_IDX			IDX_I2C1
#define I2C_SCL_GPIOx		I2C1_SCL_GPIOx
#define I2C_SCL_RCU_GPIOx	I2C1_SCL_RCU_GPIOx
#define I2C_SDA_GPIOx		I2C1_SDA_GPIOx
#define I2C_SDA_RCU_GPIOx	I2C1_SDA_RCU_GPIOx
#define I2C_SCL_GPIO_PINx	I2C1_SCL_GPIO_PINx
#define I2C_SDA_GPIO_PINx	I2C1_SDA_GPIO_PINx
#define I2C_GPIO_AFx		I2C1_GPIO_AFx
#define I2CX                I2C1

/**
 * SPI
 */

#define SPI_PERIPH			SPI4_PERIPH
#define SPI_IDX				IDX_SPI4
#define SPI_GPIO_AFx		SPI4_GPIO_AFx
#define SPI_RCU				SPI4_RCU_SPI4
#define SPI_RCU_IDX			IDX_SPI4
#define SPI_SCK_RCU_GPIO	SPI4_SCK_RCU_GPIOx
#define SPI_SCK_GPIO		SPI4_SCK_GPIOx
#define SPI_SCK_GPIO_PIN	SPI4_SCK_GPIO_PINx
#define SPI_MOSI_RCU_GPIO	SPI4_MOSI_RCU_GPIOx
#define SPI_MOSI_GPIO		SPI4_MOSI_GPIOx
#define SPI_MOSI_GPIO_PIN	SPI4_MOSI_GPIO_PINx
#define SPI_MISO_RCU_GPIO	SPI4_MISO_RCU_GPIOx
#define SPI_MISO_GPIO		SPI4_MISO_GPIOx
#define SPI_MISO_GPIO_PIN	SPI4_MISO_GPIO_PINx
#define SPI_NSS_RCU_GPIO	SPI4_NSS_RCU_GPIOx
#define SPI_NSS_GPIO		SPI4_NSS_GPIOx
#define SPI_NSS_GPIO_PIN	SPI4_NSS_GPIO_PINx
#define SPI_DMAx			SPI4_DMAx
#define SPI_DMA_CHx			SPI4_TX_DMA_CHx

/**
 * I2S
 */

#define I2S_PERIPH				SPI0_PERIPH
#define I2S_GPIO_AFx			SPI0_GPIO_AFx
#define I2S_CK_RCU_GPIOx		SPI0_SCK_RCU_GPIOx
#define I2S_CK_GPIOx			SPI0_SCK_GPIOx
#define I2S_CK_GPIO_PINx		SPI0_SCK_GPIO_PINx
#define I2S_SD_RCU_GPIOx		SPI0_MOSI_RCU_GPIOx
#define I2S_SD_GPIOx			SPI0_MOSI_GPIOx
#define I2S_SD_GPIO_PINx		SPI0_MOSI_GPIO_PINx
#define I2S_WS_RCU_GPIOx		SPI0_NSS_RCU_GPIOx
#define I2S_WS_GPIOx			SPI0_NSS_GPIOx
#define I2S_WS_GPIO_PINx		SPI0_NSS_GPIO_PINx
#define I2S_DMAx				SPI0_DMAx
#define I2S_DMA_CHx				SPI0_TX_DMA_CHx
#define I2S_DMA_REQUEST_I2S_TX	SPI0_TX_DMA_REQUEST_SPI0

/**
 * PWM
 */

#define PWM_TIMERx				TIMER2
#define PWM_RCU_TIMERx			RCU_TIMER2

#define TIMER2_FULL_REMAP
#define PWM_TIMER_REMAP			TIMER2_REMAP
#define PWM_GPIO_AFx			TIMER2_GPIO_AFx

#define PWM_CH0_RCU_GPIOx 		TIMER2_CH0_RCU_GPIOx
#define PWM_CH0_GPIOx			TIMER2_CH0_GPIOx
#define PWM_CH0_GPIO_PINx		TIMER2_CH0_GPIO_PINx

#define PWM_CH1_RCU_GPIOx 		TIMER2_CH1_RCU_GPIOx
#define PWM_CH1_GPIOx			TIMER2_CH1_GPIOx
#define PWM_CH1_GPIO_PINx		TIMER2_CH1_GPIO_PINx

#define PWM_CH2_RCU_GPIOx 		TIMER2_CH2_RCU_GPIOx
#define PWM_CH2_GPIOx			TIMER2_CH2_GPIOx
#define PWM_CH2_GPIO_PINx		TIMER2_CH2_GPIO_PINx

#define PWM_CH3_RCU_GPIOx 		TIMER2_CH3_RCU_GPIOx
#define PWM_CH3_GPIOx			TIMER2_CH3_GPIOx
#define PWM_CH3_GPIO_PINx		TIMER2_CH3_GPIO_PINx

/**
 * Panel LEDs
 */

#ifdef __cplusplus
namespace hal {
namespace panelled {
static constexpr uint32_t ACTIVITY = 0;
static constexpr uint32_t ARTNET = 0;
static constexpr uint32_t DDP = 0;
static constexpr uint32_t SACN = 0;
static constexpr uint32_t LTC_IN = 0;
static constexpr uint32_t LTC_OUT = 0;
static constexpr uint32_t MIDI_IN = 0;
static constexpr uint32_t MIDI_OUT = 0;
static constexpr uint32_t OSC_IN = 0;
static constexpr uint32_t OSC_OUT = 0;
static constexpr uint32_t TCNET = 0;
// DMX
static constexpr uint32_t PORT_A_RX = 0;
static constexpr uint32_t PORT_A_TX = 0;
}  // namespace panelled
}  // namespace hal
#endif

/**
 * SPI flash
 */

#define SPI_FLASH_CS_RCU_GPIOx		SPI_NSS_RCU_GPIOx
#define SPI_FLASH_CS_GPIOx			SPI_NSS_GPIOx
#define SPI_FLASH_CS_GPIO_PINx		SPI_NSS_GPIO_PINx

/**
 * USB
 */

#ifdef USE_USBHS0
# define USB_HOST_VBUS_GPIOx		GPIOA
# define USB_HOST_VBUS_RCU_GPIOx	RCU_GPIOA
# define USB_HOST_VBUS_GPIO_PINx	GPIO_PIN_6
#endif

#ifdef USE_USBHS1
# define USB_HOST_VBUS_GPIOx		GPIOB
# define USB_HOST_VBUS_RCU_GPIOx	RCU_GPIOB
# define USB_HOST_VBUS_GPIO_PINx	GPIO_PIN_2
#endif

#define HOST_SOF_OUTPUT_RCC			RCC_APB2PERIPH_GPIOA
#define HOST_SOF_PORT				GPIOA
#define HOST_SOF_SIGNAL				GPIO_PIN_8

/**
 * MCU and BOARD name
 */

#define GD32_MCU_NAME			"GD32H759I"
#define GD32_BOARD_NAME			"GD32H759I_EVAL"

#include "mcu/gd32h759_mcu.h"
#include "gd32_gpio.h"

/**
 * LCD
 */

#define DISPLAYTIMEOUT_GPIO		GD32_PORT_TO_GPIO(GD32_GPIO_PORTC, 13)	// KEY2

/**
 * FT8xx LCD
 */

#define FT8XX_LCD_DC_GPIO		GD32_PORT_TO_GPIO(GD32_GPIO_PORTF, 8)
#define FT8XX_LCD_CS_GPIO		GD32_PORT_TO_GPIO(GD32_GPIO_PORTF, 6)
#define FT8XX_LCD_INT_GPIO		GD32_PORT_TO_GPIO(GD32_GPIO_PORTG, 3)

#endif /* BOARD_GD32H759I_EVAL_H_ */
