/**
 * @file get_fattime.cpp
 *
 */
/* Copyright (C) 2026 by Arjan van Vught mailto:info@gd32-dmx.org
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

#include <ctime>

extern "C" {
#include "ff14b/source/ff.h"

DWORD get_fattime() { // NOLINT
    auto ltime = time(nullptr);
    auto* local_time = gmtime(&ltime);

    const auto kYear = ((DWORD)(local_time->tm_year - 80)) & 0x7F; // 7 bits (0-127)
    const auto kMonth = ((DWORD)(local_time->tm_mon + 1)) & 0x0F;  // 4 bits (1-12)
    const auto kDay = ((DWORD)local_time->tm_mday) & 0x1F;         // 5 bits (1-31)
    const auto kHour = ((DWORD)local_time->tm_hour) & 0x1F;        // 5 bits (0-23)
    const auto kMin = ((DWORD)local_time->tm_min) & 0x3F;          // 6 bits (0-59)
    const auto kSec = ((DWORD)(local_time->tm_sec >> 1)) & 0x1F;   // 5 bits (0-29)

    const auto kPacked = (kYear << 25) | (kMonth << 21) | (kDay << 16) | (kHour << 11) | (kMin << 5) | kSec;
    return kPacked;
}
}