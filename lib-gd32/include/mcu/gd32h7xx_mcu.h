/**
 * @file gd32h7xx_mcu.h
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

#ifndef MCU_GD32H7XX_MCU_H_
#define MCU_GD32H7XX_MCU_H_

#if !defined(MCU_GD32H759_MCU_H_)
# error This file should not be included
#endif

/**
 * U(S)ART
 */

#define USART0_RCU_USART0		RCU_USART0
#if defined (USART0_REMAP)
# define USART0_GPIO_AFx		GPIO_AF_7
# define USART0_RCU_GPIOx		RCU_GPIOB
# define USART0_GPIOx			GPIOB
# define USART0_TX_GPIO_PINx	GPIO_PIN_6
# define USART0_RX_GPIO_PINx	GPIO_PIN_7
#else
# define USART0_GPIO_AFx		GPIO_AF_7
# define USART0_RCU_GPIOx		RCU_GPIOA
# define USART0_GPIOx			GPIOA
# define USART0_TX_GPIO_PINx	GPIO_PIN_9
# define USART0_RX_GPIO_PINx	GPIO_PIN_10
#endif

#define USART1_RCU_USART1		RCU_USART1
#if defined (USART1_REMAP)
# define USART1_GPIO_AFx		GPIO_AF_7
# define USART1_RCU_GPIOx		RCU_GPIOD
# define USART1_GPIOx			GPIOD
# define USART1_TX_GPIO_PINx	GPIO_PIN_5
# define USART1_RX_GPIO_PINx	GPIO_PIN_6
#else
# define USART1_GPIO_AFx		GPIO_AF_7
# define USART1_RCU_GPIOx		RCU_GPIOA
# define USART1_GPIOx			GPIOA
# define USART1_TX_GPIO_PINx	GPIO_PIN_2
# define USART1_RX_GPIO_PINx	GPIO_PIN_3
#endif

#define USART2_RCU_USART2		RCU_USART2
#if defined (USART2_FULL_REMAP)
# define USART2_GPIO_AFx		GPIO_AF_7
# define USART2_RCU_GPIOx		RCU_GPIOD
# define USART2_GPIOx			GPIOD
# define USART2_TX_GPIO_PINx	GPIO_PIN_8
# define USART2_RX_GPIO_PINx	GPIO_PIN_9
#elif defined (USART2_PARTIAL_REMAP)
# define USART2_GPIO_AFx		GPIO_AF_7
# define USART2_RCU_GPIOx		RCU_GPIOC
# define USART2_GPIOx			GPIOC
# define USART2_TX_GPIO_PINx	GPIO_PIN_10
# define USART2_RX_GPIO_PINx	GPIO_PIN_11
#else
# define USART2_GPIO_AFx		GPIO_AF_7
# define USART2_RCU_GPIOx		RCU_GPIOB
# define USART2_GPIOx			GPIOB
# define USART2_TX_GPIO_PINx	GPIO_PIN_10
# define USART2_RX_GPIO_PINx	GPIO_PIN_11
#endif

#define UART3_RCU_UART3			RCU_UART3
#if defined (UART3_REMAP)
# define UART3_GPIO_AFx			GPIO_AF_8
# define UART3_RCU_GPIOx		RCU_GPIOA
# define UART3_GPIOx			GPIOA
# define UART3_TX_GPIO_PINx		GPIO_PIN_0
# define UART3_RX_GPIO_PINx		GPIO_PIN_1
#else
# define UART3_GPIO_AFx			GPIO_AF_8
# define UART3_RCU_GPIOx		RCU_GPIOC
# define UART3_GPIOx			GPIOC
# define UART3_TX_GPIO_PINx		GPIO_PIN_10
# define UART3_RX_GPIO_PINx		GPIO_PIN_11
#endif

#define UART4_RCU_UART4			RCU_UART4
#define UART4_GPIO_AFx			GPIO_AF_8
#define UART4_TX_RCU_GPIOx		RCU_GPIOC
#define UART4_TX_GPIOx			GPIOC
#define UART4_TX_GPIO_PINx		GPIO_PIN_12
#define UART4_RX_RCU_GPIOx		RCU_GPIOD
#define UART4_RX_GPIOx			GPIOD
#define UART4_RX_GPIO_PINx		GPIO_PIN_2

#define USART5_RCU_USART5		RCU_USART5
#if defined (USART5_REMAP)
# define USART5_GPIO_AFx		GPIO_AF_8
# define USART5_RCU_GPIOx		RCU_GPIOG
# define USART5_GPIOx			GPIOG
# define USART5_TX_GPIO_PINx	GPIO_PIN_14
# define USART5_RX_GPIO_PINx	GPIO_PIN_9
#else
# define USART5_GPIO_AFx		GPIO_AF_7
# define USART5_RCU_GPIOx		RCU_GPIOC
# define USART5_GPIOx			GPIOC
# define USART5_TX_GPIO_PINx	GPIO_PIN_6
# define USART5_RX_GPIO_PINx	GPIO_PIN_7
#endif

#define UART6_RCU_UART6			RCU_UART6
#if defined (UART6_REMAP)
# define UART6_GPIO_AFx			GPIO_AF_7
# define UART6_RCU_GPIOx		RCU_GPIOF
# define UART6_GPIOx			GPIOF
# define UART6_TX_GPIO_PINx		GPIO_PIN_7
# define UART6_RX_GPIO_PINx		GPIO_PIN_6
#else
# define UART6_GPIO_AFx			GPIO_AF_7
# define UART6_RCU_GPIOx		RCU_GPIOE
# define UART6_GPIOx			GPIOE
# define UART6_TX_GPIO_PINx		GPIO_PIN_8
# define UART6_RX_GPIO_PINx		GPIO_PIN_7
#endif

#define UART7_RCU_UART7			RCU_UART7
#define UART7_GPIO_AFx			GPIO_AF_8
#define UART7_RCU_GPIOx			RCU_GPIOE
#define UART7_GPIOx				GPIOE
#define UART7_TX_GPIO_PINx		GPIO_PIN_1
#define UART7_RX_GPIO_PINx		GPIO_PIN_0

/**
 * I2C
 */

#define I2C1_PERIPH				I2C1
#define I2C1_GPIO_AFx			GPIO_AF_4
#define I2C1_RCU_I2C1			RCU_I2C1
#define I2C1_SCL_RCU_GPIOx		RCU_GPIOH
#define I2C1_SCL_GPIOx			GPIOH
#define I2C1_SCL_GPIO_PINx		GPIO_PIN_4
#define I2C1_SDA_RCU_GPIOx		RCU_GPIOB
#define I2C1_SDA_GPIOx			GPIOB
#define I2C1_SDA_GPIO_PINx		GPIO_PIN_11

/**
 * SPI
 */

#define SPI0_PERIPH				SPI0
#define SPI0_GPIO_AFx			GPIO_AF_5
#define SPI0_RCU_SPI0			RCU_SPI4
#define SPI0_SCK_RCU_GPIOx		RCU_GPIOH
#define SPI0_SCK_GPIOx			GPIOH
#define SPI0_SCK_GPIO_PINx		GPIO_PIN_6
#define SPI0_MISO_RCU_GPIOx		RCU_GPIOH
#define SPI0_MISO_GPIOx			GPIOH
#define SPI0_MISO_GPIO_PINx		GPIO_PIN_7
#define SPI0_MOSI_RCU_GPIOx		RCU_GPIOF
#define SPI0_MOSI_GPIOx			GPIOF
#define SPI0_MOSI_GPIO_PINx		GPIO_PIN_9
#define SPI0_NSS_RCU_GPIOx		RCU_GPIOG
#define SPI0_NSS_GPIOx			GPIOG
#define SPI0_NSS_GPIO_PINx		GPIO_PIN_6

#define SPI4_PERIPH				SPI4
#define SPI4_GPIO_AFx			GPIO_AF_5
#define SPI4_RCU_SPI4			RCU_SPI4
#define SPI4_SCK_RCU_GPIOx		RCU_GPIOH
#define SPI4_SCK_GPIOx			GPIOH
#define SPI4_SCK_GPIO_PINx		GPIO_PIN_6
#define SPI4_MISO_RCU_GPIOx		RCU_GPIOH
#define SPI4_MISO_GPIOx			GPIOH
#define SPI4_MISO_GPIO_PINx		GPIO_PIN_7
#define SPI4_MOSI_RCU_GPIOx		RCU_GPIOF
#define SPI4_MOSI_GPIOx			GPIOF
#define SPI4_MOSI_GPIO_PINx		GPIO_PIN_9
#define SPI4_NSS_RCU_GPIOx		RCU_GPIOG
#define SPI4_NSS_GPIOx			GPIOG
#define SPI4_NSS_GPIO_PINx		GPIO_PIN_6

/**
 * TIMER GPIO
 */

#define TIMER2CH0_RCU_GPIOx		RCU_GPIOA
#define TIMER2CH0_AFx			GPIO_AF_2
#define TIMER2CH0_GPIOx			GPIOA
#define TIMER2CH0_GPIO_PINx		GPIO_PIN_6

#if defined(TIMER2_FULL_REMAP)
# define TIMER2_GPIO_AFx		GPIO_AF_2
# define TIMER2_CH0_RCU_GPIOx	RCU_GPIOC
# define TIMER2_CH0_GPIOx		GPIOC
# define TIMER2_CH0_GPIO_PINx	GPIO_PIN_6

# define TIMER2_CH1_RCU_GPIOx	RCU_GPIOC
# define TIMER2_CH1_GPIOx		GPIOC
# define TIMER2_CH1_GPIO_PINx	GPIO_PIN_7

# define TIMER2_CH2_RCU_GPIOx	RCU_GPIOC
# define TIMER2_CH2_GPIOx		GPIOC
# define TIMER2_CH2_GPIO_PINx	GPIO_PIN_8

# define TIMER2_CH3_RCU_GPIOx	RCU_GPIOC
# define TIMER2_CH3_GPIOx		GPIOC
# define TIMER2_CH3_GPIO_PINx	GPIO_PIN_9
#elif defined(TIMER2_PARTIAL_REMAP)
# define TIMER2_GPIO_AFx		GPIO_AF_2
# define TIMER2_CH0_RCU_GPIOx	RCU_GPIOB
# define TIMER2_CH0_GPIOx		GPIOB
# define TIMER2_CH0_GPIO_PINx	GPIO_PIN_4

# define TIMER2_CH1_RCU_GPIOx	RCU_GPIOB
# define TIMER2_CH1_GPIOx		GPIOB
# define TIMER2_CH1_GPIO_PINx	GPIO_PIN_5

# define TIMER2_CH2_RCU_GPIOx	RCU_GPIOB
# define TIMER2_CH2_GPIOx		GPIOB
# define TIMER2_CH2_GPIO_PINx	GPIO_PIN_0

# define TIMER2_CH3_RCU_GPIOx	RCU_GPIOB
# define TIMER2_CH3_GPIOx		GPIOB
# define TIMER2_CH3_GPIO_PINx	GPIO_PIN_1
#else
# define TIMER2_GPIO_AFx		GPIO_AF_2
# define TIMER2_CH0_RCU_GPIOx	RCU_GPIOA
# define TIMER2_CH0_GPIOx		GPIOA
# define TIMER2_CH0_GPIO_PINx	GPIO_PIN_6

# define TIMER2_CH1_RCU_GPIOx	RCU_GPIOA
# define TIMER2_CH1_GPIOx		GPIOA
# define TIMER2_CH1_GPIO_PINx	GPIO_PIN_7

# define TIMER2_CH2_RCU_GPIOx	RCU_GPIOB
# define TIMER2_CH2_GPIOx		GPIOB
# define TIMER2_CH2_GPIO_PINx	GPIO_PIN_0

# define TIMER2_CH3_RCU_GPIOx	RCU_GPIOB
# define TIMER2_CH3_GPIOx		GPIOB
# define TIMER2_CH3_GPIO_PINx	GPIO_PIN_1
#endif

/**
 * DMA
 */

#define SPI0_DMAx               	DMA1
#define SPI0_TX_DMA_CHx         	DMA_CH2
#define SPI0_TX_DMA_REQUEST_SPI0	DMA_REQUEST_SPI0_TX

#define SPI1_DMAx               DMA0
#define SPI1_TX_DMA_CHx         DMA_CH4

#define SPI2_DMAx               DMA0
#define SPI2_TX_DMA_CHx         DMA_CH5

#define SPI3_DMAx               DMA1
#define SPI3_TX_DMA_CHx         DMA_CH1

#define SPI4_DMAx               DMA1
#define SPI4_TX_DMA_CHx         DMA_CH4

#define SPI5_DMAx               DMA1
#define SPI5_TX_DMA_CHx         DMA_CH5

#define USART0_DMAx				DMA1
#define USART0_TX_DMA_CHx		DMA_CH7
#define USART0_RX_DMA_CHx		DMA_CH5

#define USART1_DMAx				DMA0
#define USART1_TX_DMA_CHx		DMA_CH6
#define USART1_RX_DMA_CHx		DMA_CH5

#define USART2_DMAx				DMA0
#define USART2_TX_DMA_CHx		DMA_CH3
#define USART2_RX_DMA_CHx		DMA_CH1

#define UART3_DMAx				DMA0
#define UART3_TX_DMA_CHx		DMA_CH4
#define UART3_RX_DMA_CHx		DMA_CH2

#define UART4_DMAx				DMA0
#define UART4_TX_DMA_CHx		DMA_CH7
#define UART4_RX_DMA_CHx		DMA_CH0

#define USART5_DMAx				DMA1
#define USART5_TX_DMA_CHx		DMA_CH6
#define USART5_RX_DMA_CHx		DMA_CH1

#define UART6_DMAx				DMA0
#define UART6_TX_DMA_CHx		DMA_CH1
#define UART6_RX_DMA_CHx		DMA_CH3

#define UART7_DMAx				DMA0
#define UART7_TX_DMA_CHx		DMA_CH0
#define UART7_RX_DMA_CHx		DMA_CH6

#endif /* MCU_GD32H7XX_MCU_H_ */
