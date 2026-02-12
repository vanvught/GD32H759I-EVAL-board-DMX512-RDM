/**
 * @file gd32_i2c.cpp
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

#if defined(CONFIG_I2C_OPTIMIZE_O2) || defined(CONFIG_I2C_OPTIMIZE_O3)
#pragma GCC push_options
#if defined(CONFIG_I2C_OPTIMIZE_O2)
#pragma GCC optimize("O2")
#else
#pragma GCC optimize("O3")
#endif
#endif

#include <cstdint>
#include <cassert>

#include "gd32_i2c.h"
#include "gd32.h"
#include "firmware/debug/debug_debug.h"

enum class State
{
    I2C_START,
    I2C_TRANSMIT_DATA,
    I2C_RELOAD,
    I2C_STOP
};

static constexpr uint8_t MAX_RELOAD_SIZE = 255;
static constexpr int32_t I2C_TIME_OUT = 50000;
static uint8_t s_nAddress;

static void GpioConfig()
{
    rcu_periph_clock_enable(I2C_SCL_RCU_GPIOx);
    rcu_periph_clock_enable(I2C_SDA_RCU_GPIOx);
    rcu_periph_clock_enable(I2C_RCU_I2Cx);

    gpio_af_set(I2C_SCL_GPIOx, I2C_GPIO_AFx, I2C_SCL_GPIO_PINx);
    gpio_af_set(I2C_SDA_GPIOx, I2C_GPIO_AFx, I2C_SDA_GPIO_PINx);

    gpio_mode_set(I2C_SCL_GPIOx, GPIO_MODE_AF, GPIO_PUPD_PULLUP, I2C_SCL_GPIO_PINx);
    gpio_output_options_set(I2C_SCL_GPIOx, GPIO_OTYPE_OD, GPIO_OSPEED_60MHZ, I2C_SCL_GPIO_PINx);

    gpio_mode_set(I2C_SDA_GPIOx, GPIO_MODE_AF, GPIO_PUPD_PULLUP, I2C_SDA_GPIO_PINx);
    gpio_output_options_set(I2C_SDA_GPIOx, GPIO_OTYPE_OD, GPIO_OSPEED_60MHZ, I2C_SDA_GPIO_PINx);
}

static void i2c_config()
{
    rcu_i2c_clock_config(I2C_RCU_IDX, RCU_I2CSRC_IRC64MDIV);
    i2c_timing_config(I2CX, 0x0, 0x6, 0);
    i2c_master_clock_config(I2CX, 0x26, 0x73);
    i2c_enable(I2CX);
}

/**
 * Public API's
 */

void Gd32I2cBegin()
{
    GpioConfig();
    i2c_config();
}

void Gd32I2cSetBaudrate(const uint32_t nBaudrate)
{
    // TODO
}

void Gd32I2cSetAddress(const uint8_t nAddress)
{
    s_nAddress = nAddress << 1;
}

uint8_t Gd32I2cWrite(const char* pBuffer, uint32_t nLength)
{
    I2C_STAT(I2CX) |= I2C_STAT_TBE;
    i2c_reload_enable(I2CX);
    i2c_automatic_end_disable(I2CX);
    i2c_master_addressing(I2CX, s_nAddress, I2C_MASTER_TRANSMIT);

    if (i2c_flag_get(I2CX, I2C_FLAG_I2CBSY))
    {
        DEBUG_PUTS("I2CBSY");
    }

    auto state = State::I2C_RELOAD;
    auto isFirstReload = true;
    auto isLastReload = false;
    uint32_t nBytesReload = MAX_RELOAD_SIZE;

    while (true)
    {
        switch (state)
        {
            case State::I2C_RELOAD:
            {
                if (nLength > MAX_RELOAD_SIZE)
                {
                    nLength = nLength - MAX_RELOAD_SIZE;
                    nBytesReload = MAX_RELOAD_SIZE;
                }
                else
                {
                    nBytesReload = nLength;
                    isLastReload = true;
                }

                if (isFirstReload)
                {
                    isFirstReload = false;

                    i2c_transfer_byte_number_config(I2CX, nBytesReload);

                    if (isLastReload)
                    {
                        isLastReload = false;

                        if (nLength <= MAX_RELOAD_SIZE)
                        {
                            i2c_reload_disable(I2CX);
                            i2c_automatic_end_enable(I2CX);
                        }
                    }

                    i2c_start_on_bus(I2CX);

                    auto nTimeout = I2C_TIME_OUT;
                    while ((!i2c_flag_get(I2CX, I2C_FLAG_TBE)) && (nTimeout > 0))
                    {
                        nTimeout--;
                    }

                    if (nTimeout <= 0)
                    {
                        state = State::I2C_STOP;
                        continue;
                    }
                }
                else
                {
                    auto nTimeout = I2C_TIME_OUT;
                    while ((!i2c_flag_get(I2CX, I2C_FLAG_TCR)) && (nTimeout > 0))
                    {
                        nTimeout--;
                    }

                    if (nTimeout <= 0)
                    {
                        state = State::I2C_STOP;
                        continue;
                    }

                    i2c_transfer_byte_number_config(I2CX, nBytesReload);

                    if (nLength <= MAX_RELOAD_SIZE)
                    {
                        i2c_reload_disable(I2CX);
                        i2c_automatic_end_enable(I2CX);
                    }
                }

                state = State::I2C_TRANSMIT_DATA;
            }
            break;
            case State::I2C_TRANSMIT_DATA:
            {
                while (nBytesReload)
                {
                    auto nTimeout = I2C_TIME_OUT;
                    while ((!i2c_flag_get(I2CX, I2C_FLAG_TI)) && (nTimeout > 0))
                    {
                        nTimeout--;
                    }

                    if (nTimeout <= 0)
                    {
                        state = State::I2C_STOP;
                        break;
                    }
                    else
                    {
                        i2c_data_transmit(I2CX, *pBuffer++);
                        nBytesReload--;
                    }
                }

                if (state == State::I2C_STOP)
                {
                    continue;
                }

                if (I2C_CTL1(I2CX) & I2C_CTL1_RELOAD)
                {
                    state = State::I2C_RELOAD;
                }
                else
                {
                    state = State::I2C_STOP;
                }
            }
            break;
            case State::I2C_STOP:
            {
                auto nTimeout = I2C_TIME_OUT;
                while ((!i2c_flag_get(I2CX, I2C_FLAG_STPDET)) && (nTimeout > 0))
                {
                    nTimeout--;
                }

                if (nTimeout <= 0)
                {
                    return GD32_I2C_NOK_TOUT;
                }

                i2c_flag_clear(I2CX, I2C_FLAG_STPDET);

                if (i2c_flag_get(I2CX, I2C_FLAG_NACK))
                {
                    i2c_flag_clear(I2CX, I2C_FLAG_NACK);
                    return GD32_I2C_NACK;
                }

                return GD32_I2C_OK;
            }
            break;
            default:
                assert(false && "Invalid state");
                break;
        }
    }

    assert(false && "GD32_I2C_NOK");
    return GD32_I2C_NOK;
}

uint8_t Gd32I2cRead(char* pBuffer, uint32_t nLength)
{
    I2C_STAT(I2CX) |= I2C_STAT_TBE;
    i2c_reload_enable(I2CX);
    i2c_automatic_end_disable(I2CX);
    i2c_master_addressing(I2CX, s_nAddress, I2C_MASTER_RECEIVE);

    if (i2c_flag_get(I2CX, I2C_FLAG_I2CBSY))
    {
        DEBUG_PUTS("I2CBSY");
    }

    auto state = State::I2C_RELOAD;
    auto isFirstReload = true;
    auto isLastReload = false;
    uint32_t nBytesReload = MAX_RELOAD_SIZE;

    while (true)
    {
        switch (state)
        {
            case State::I2C_RELOAD:
            {
                if (nLength > MAX_RELOAD_SIZE)
                {
                    nLength = nLength - MAX_RELOAD_SIZE;
                    nBytesReload = MAX_RELOAD_SIZE;
                }
                else
                {
                    nBytesReload = nLength;
                    isLastReload = true;
                }

                if (isFirstReload)
                {
                    isFirstReload = false;

                    i2c_transfer_byte_number_config(I2CX, nBytesReload);

                    if (isLastReload)
                    {
                        isLastReload = false;

                        if (nLength <= MAX_RELOAD_SIZE)
                        {
                            i2c_reload_disable(I2CX);
                            i2c_automatic_end_enable(I2CX);
                        }
                    }

                    i2c_start_on_bus(I2CX);

                    auto nTimeout = I2C_TIME_OUT;
                    while ((!i2c_flag_get(I2CX, I2C_FLAG_TBE)) && (nTimeout > 0))
                    {
                        nTimeout--;
                    }

                    if (nTimeout <= 0)
                    {
                        state = State::I2C_STOP;
                        continue;
                    }
                }
                else
                {
                    auto nTimeout = I2C_TIME_OUT;
                    while ((!i2c_flag_get(I2CX, I2C_FLAG_TCR)) && (nTimeout > 0))
                    {
                        nTimeout--;
                    }

                    if (nTimeout <= 0)
                    {
                        state = State::I2C_STOP;
                        continue;
                    }

                    i2c_transfer_byte_number_config(I2CX, nBytesReload);

                    if (nLength <= MAX_RELOAD_SIZE)
                    {
                        i2c_reload_disable(I2CX);
                        i2c_automatic_end_enable(I2CX);
                    }
                }

                state = State::I2C_TRANSMIT_DATA;
            }
            break;
            case State::I2C_TRANSMIT_DATA:
            {
                while (nBytesReload)
                {
                    auto nTimeout = I2C_TIME_OUT;
                    while ((!i2c_flag_get(I2CX, I2C_FLAG_RBNE)) && (nTimeout > 0))
                    {
                        if (i2c_flag_get(I2CX, I2C_FLAG_NACK))
                        {
                            state = State::I2C_STOP;
                            break;
                        }
                        nTimeout--;
                    }

                    if (nTimeout <= 0)
                    {
                        state = State::I2C_STOP;
                        break;
                    }

                    *pBuffer++ = i2c_data_receive(I2CX);
                    nBytesReload--;
                }

                if (state == State::I2C_STOP)
                {
                    continue;
                }

                if (I2C_CTL1(I2CX) & I2C_CTL1_RELOAD)
                {
                    state = State::I2C_RELOAD;
                }
                else
                {
                    state = State::I2C_STOP;
                }
            }
            break;
            case State::I2C_STOP:
            {
                auto nTimeout = I2C_TIME_OUT;
                while ((!i2c_flag_get(I2CX, I2C_FLAG_STPDET)) && (nTimeout > 0))
                {
                    nTimeout--;
                }

                if (nTimeout <= 0)
                {
                    return GD32_I2C_NOK_TOUT;
                }

                i2c_flag_clear(I2CX, I2C_FLAG_STPDET);

                if (i2c_flag_get(I2CX, I2C_FLAG_NACK))
                {
                    i2c_flag_clear(I2CX, I2C_FLAG_NACK);
                    return GD32_I2C_NACK;
                }

                return GD32_I2C_OK;
            }
            break;
            default:
                assert(false && "Invalid state");
                break;
        }
    }

    assert(false && "GD32_I2C_NOK");
    return GD32_I2C_NOK;
}

bool Gd32I2cIsConnected(const uint8_t nAddress, const uint32_t nBaudrate)
{
    Gd32I2cSetAddress(nAddress);
    Gd32I2cSetBaudrate(nBaudrate);

    uint8_t nResult;
    char buffer;

    if ((nAddress >= 0x30 && nAddress <= 0x37) || (nAddress >= 0x50 && nAddress <= 0x5F))
    {
        nResult = Gd32I2cRead(&buffer, 1);
    }
    else
    {
        /* This is known to corrupt the Atmel AT24RF08 EEPROM */
        nResult = Gd32I2cWrite(nullptr, 0);
    }

    return (nResult == 0) ? true : false;
}

void Gd32I2cWriteRegister(const uint8_t nRegister, const uint8_t nValue)
{
    char buffer[2];

    buffer[0] = static_cast<char>(nRegister);
    buffer[1] = static_cast<char>(nValue);

    Gd32I2cWrite(buffer, 2);
}

void Gd32I2cReadRegister(const uint8_t nRegister, uint8_t& nValue)
{
    char buffer[1];

    buffer[0] = static_cast<char>(nRegister);

    Gd32I2cWrite(buffer, 1);
    Gd32I2cRead(buffer, 1);

    nValue = buffer[0];
}
