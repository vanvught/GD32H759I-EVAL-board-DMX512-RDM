/**
 * @file gd32_spi.cpp
 *
 */
/* Copyright (C) 2024-2025 by Arjan van Vught mailto:info@gd32-dmx.org
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

#include <cstdint>
#include <cassert>

#include "gd32_spi.h"
#include "gd32.h"

static uint8_t s_n_chip_select = GD32_SPI_CS0;

static inline void cs_high()
{
    if (s_n_chip_select == GD32_SPI_CS0)
    {
        GPIO_BOP(SPI_NSS_GPIOx) = static_cast<uint32_t>(SPI_NSS_GPIO_PINx);
    }
}

static inline void cs_low()
{
    if (s_n_chip_select == GD32_SPI_CS0)
    {
        GPIO_BC(SPI_NSS_GPIOx) = static_cast<uint32_t>(SPI_NSS_GPIO_PINx);
    }
}

static uint8_t send_byte(uint8_t byte)
{
    while (RESET == spi_i2s_flag_get(SPI_PERIPH, SPI_FLAG_TP));

    SPI_TDATA(SPI_PERIPH) = static_cast<uint32_t>(byte);

    while (RESET == spi_i2s_flag_get(SPI_PERIPH, SPI_FLAG_RP));

    return static_cast<uint8_t>(SPI_RDATA(SPI_PERIPH));
}

void Gd32SpiBegin()
{
    rcu_periph_clock_enable(SPI_SCK_RCU_GPIOx);
    rcu_periph_clock_enable(SPI_MOSI_RCU_GPIOx);
    rcu_periph_clock_enable(SPI_MISO_RCU_GPIOx);
    rcu_periph_clock_enable(SPI_RCU);
    rcu_spi_clock_config(SPI_IDX, RCU_SPISRC_PLL0Q);

    gpio_af_set(SPI_SCK_GPIOx, SPI_GPIO_AFx, SPI_SCK_GPIO_PINx);
    gpio_mode_set(SPI_SCK_GPIOx, GPIO_MODE_AF, GPIO_PUPD_NONE, SPI_SCK_GPIO_PINx);
    gpio_output_options_set(SPI_SCK_GPIOx, GPIO_OTYPE_PP, GPIO_OSPEED_60MHZ, SPI_SCK_GPIO_PINx);

    gpio_af_set(SPI_MOSI_GPIOx, SPI_GPIO_AFx, SPI_MOSI_GPIO_PINx);
    gpio_mode_set(SPI_MOSI_GPIOx, GPIO_MODE_AF, GPIO_PUPD_NONE, SPI_MOSI_GPIO_PINx);
    gpio_output_options_set(SPI_MOSI_GPIOx, GPIO_OTYPE_PP, GPIO_OSPEED_60MHZ, SPI_MOSI_GPIO_PINx);

    gpio_af_set(SPI_MISO_GPIOx, SPI_GPIO_AFx, SPI_MISO_GPIO_PINx);
    gpio_mode_set(SPI_MISO_GPIOx, GPIO_MODE_AF, GPIO_PUPD_NONE, SPI_MISO_GPIO_PINx);
    gpio_output_options_set(SPI_MISO_GPIOx, GPIO_OTYPE_PP, GPIO_OSPEED_60MHZ, SPI_MISO_GPIO_PINx);

    if (s_n_chip_select == GD32_SPI_CS0)
    {
        gpio_mode_set(SPI_NSS_GPIOx, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SPI_NSS_GPIO_PINx);
        gpio_output_options_set(SPI_NSS_GPIOx, GPIO_OTYPE_PP, GPIO_OSPEED_60MHZ, SPI_NSS_GPIO_PINx);
    }

    spi_parameter_struct spi_init_struct;
    spi_i2s_deinit(SPI_PERIPH);
    spi_struct_para_init(&spi_init_struct);

    spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode = SPI_MASTER;
    spi_init_struct.data_size = SPI_DATASIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss = SPI_NSS_SOFT;
    spi_init_struct.prescale = SPI_PSC_64;
    spi_init_struct.endian = SPI_ENDIAN_MSB;
    spi_init(SPI_PERIPH, &spi_init_struct);

    spi_byte_access_enable(SPI_PERIPH);

    if (s_n_chip_select == GD32_SPI_CS0)
    {
        spi_nss_output_enable(SPI_PERIPH);
    }

    spi_enable(SPI_PERIPH);
}

void Gd32SpiEnd() {}

void Gd32SpiSetSpeedHz(uint32_t nSpeedHz) {}

void Gd32SpiSetDataMode(uint8_t nMode)
{
    uint32_t nCTL0 = SPI_CTL0(SPI_PERIPH);
    nCTL0 &= ~0x3;
    nCTL0 |= (nMode & 0x3);

    spi_disable(SPI_PERIPH);
    SPI_CTL0(SPI_PERIPH) = nCTL0;
    spi_enable(SPI_PERIPH);
}

void Gd32SpiChipSelect(uint8_t nChipSelect)
{
    s_n_chip_select = nChipSelect;
}

void Gd32SpiTransfernb(const char* pTxBuffer, char* pRxBuffer, uint32_t nDataLength)
{
    assert(pTxBuffer != nullptr);
    assert(pRxBuffer != nullptr);

    cs_low();

    while (nDataLength-- > 0)
    {
        *pRxBuffer = send_byte(static_cast<uint8_t>(*pTxBuffer));
        pRxBuffer++;
        pTxBuffer++;
    }

    cs_high();
}

void Gd32SpiTransfern(char* pTxBuffer, uint32_t nDataLength)
{
    Gd32SpiTransfernb(pTxBuffer, pTxBuffer, nDataLength);
}

void Gd32SpiWrite(const uint16_t nData)
{
    cs_low();

    spi_master_transfer_start(SPI_PERIPH, SPI_TRANS_START);

    send_byte(static_cast<uint8_t>(nData >> 8));
    send_byte(static_cast<uint8_t>(nData & 0xFF));

    cs_high();
}

void Gd32SpiWritenb(const char* pTxBuffer, uint32_t nDataLength)
{
    assert(pTxBuffer != nullptr);

    cs_low();

    spi_master_transfer_start(SPI_PERIPH, SPI_TRANS_START);

    while (nDataLength-- > 0)
    {
        send_byte(static_cast<uint8_t>(*pTxBuffer));
        pTxBuffer++;
    }

    cs_high();
}
