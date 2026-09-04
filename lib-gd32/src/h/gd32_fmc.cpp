/**
 * @file gd32_fmc.cpp
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

#include <cstdint>
#include <span>
#include <cassert>

#include "gd32_fmc.h"
#include "gd32_debug.h"
#include "gd32.h" // IWYU pragma: keep

namespace {
constexpr uint32_t k1KiB = 1024;
constexpr uint32_t kStartAddress = FLASH_BASE;
const auto kEndAddress = (kStartAddress + (FMC_SIZE * 1024) - 1);
constexpr auto kBanK1FlashPage = 4 * k1KiB;

enum class State { kIdle, kEraseBusy, kEraseProgram, kWriteBusy, kWriteProgram, kError };

State s_state = State::kIdle;
uint32_t s_page;
uint32_t s_length;
uint32_t s_address;
const uint32_t* s_data;
} // namespace

namespace gd32::fmc {
// Blocking API's
bool Read(uint32_t offset, std::span<uint8_t> buffer) {
    GD32_FMC_DEBUG_ENTRY();

    const auto kAddress = offset + FLASH_BASE;

    if (buffer.empty() || ((buffer.size() % sizeof(uint32_t)) != 0) || (kAddress < kStartAddress) || (kAddress >= kEndAddress) || (buffer.size() > (kEndAddress - kAddress))) {
        return false;
    }

    assert((reinterpret_cast<uintptr_t>(buffer.data()) % alignof(uint32_t)) == 0);

    GD32_FMC_DEBUG_PRINTF("offset=%x, length=%u, data=%p", static_cast<unsigned>(offset), buffer.size(), buffer.data());

    const auto* src = reinterpret_cast<const uint32_t*>(kAddress);
    auto* dst = reinterpret_cast<uint32_t*>(buffer.data());

    auto length = buffer.size();

    while (length != 0) {
        *dst++ = *src++;
        length -= sizeof(uint32_t);
    }

    GD32_FMC_DEBUG_EXIT();
    return true;
}

bool Erase(uint32_t offset, uint32_t length) {
    GD32_FMC_DEBUG_ENTRY();

    Result result;
    while (!Erase(offset, length, result)) {
    }

    GD32_FMC_DEBUG_EXIT();
    return result == Result::kOk;
}

bool Write(uint32_t offset, std::span<const uint8_t> buffer) {
    GD32_FMC_DEBUG_ENTRY();

    Result result;
    while (!Write(offset, buffer, result)) {
    }

    GD32_FMC_DEBUG_EXIT();
    return result == Result::kOk;
}

// State-machine API's
bool Erase(uint32_t offset, uint32_t length, Result& result) {
    GD32_FMC_DEBUG_ENTRY();
    GD32_FMC_DEBUG_PRINTF("State=%d", static_cast<int>(s_state));

    result = Result::kOk;

    switch (s_state) {
        case State::kIdle:
            GD32_FMC_DEBUG_ENTRY();
            GD32_FMC_DEBUG_PUTS("State::IDLE");

            s_page = offset + FLASH_BASE;
            s_length = length;
            fmc_unlock();
            s_state = State::kEraseBusy;

            return false;

        case State::kEraseBusy:
            if (SET == fmc_flag_get(FMC_FLAG_BUSY)) {
                return false;
            }

            if (s_length == 0) {
                fmc_lock();
                s_state = State::kIdle;

                GD32_FMC_DEBUG_EXIT();
                return true; // Ending state-machine
            }

            s_state = State::kEraseProgram;
            return false;

        case State::kEraseProgram:
            if (s_length > 0) {
                GD32_FMC_DEBUG_PRINTF("s_page=%p", reinterpret_cast<void*>(s_page));

                fmc_sector_erase(s_page);

                s_length -= kBanK1FlashPage;
                s_page += kBanK1FlashPage;
            }

            s_state = State::kEraseBusy;
            return false;

        default:
            assert(0);
            __builtin_unreachable();
            break;
    }

    assert(0);
    __builtin_unreachable();
    return true;
}

bool Write(uint32_t offset, std::span<const uint8_t> buffer, Result& result) {
    result = Result::kOk;

    switch (s_state) {
        case State::kIdle: {
            GD32_FMC_DEBUG_ENTRY();
            GD32_FMC_DEBUG_PUTS("State::IDLE");

            const auto kAddress = offset + FLASH_BASE;

            if (buffer.empty() || ((buffer.size() % sizeof(uint32_t)) != 0) || (kAddress < kStartAddress) || (kAddress >= kEndAddress) || (buffer.size() > (kEndAddress - kAddress))) {
                result = Result::kError;
                GD32_FMC_DEBUG_EXIT();
                return true; // Ending state-machine
            }

            assert((reinterpret_cast<uintptr_t>(buffer.data()) % alignof(uint32_t)) == 0);

            s_address = kAddress;
            s_data = reinterpret_cast<const uint32_t*>(buffer.data());
            s_length = static_cast<uint32_t>(buffer.size());

            fmc_unlock();

            s_state = State::kWriteBusy;

            return false;
        }

        case State::kWriteBusy:
            if (SET == fmc_flag_get(FMC_FLAG_BUSY)) {
                return false;
            }

            FMC_CTL &= ~FMC_CTL_PG;

            if (s_length == 0) {
                fmc_lock();
                s_state = State::kIdle;

                GD32_FMC_DEBUG_EXIT();
                return true; // Ending state-machine
            }

            s_address += sizeof(uint32_t);
            s_state = State::kWriteProgram;
            return false;

        case State::kWriteProgram:
            FMC_CTL |= FMC_CTL_PG;

            __ISB();
            __DSB();

            REG32(s_address) = *s_data;

            __ISB();
            __DSB();

            ++s_data;
            s_length -= sizeof(uint32_t);

            s_state = State::kWriteBusy;
            return false;

        case State::kEraseBusy:
            /*@fallthrough@*/
            /* no break */

        case State::kEraseProgram:
            s_state = State::kIdle;
            return false;

        default:
            assert(false);
            __builtin_unreachable();
    }
}
} // namespace gd32::fmc