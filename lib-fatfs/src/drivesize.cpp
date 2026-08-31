/**
 * @file drivesize.cpp
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

#include <cstdint>

#include "ff14b/source/ff.h"

void DriveSize(const char* path, uint32_t& total_bytes, uint32_t& free_bytes) {
    FATFS* fat_fs;
    DWORD fre_clust;
    FRESULT res;

    // Get volume information and free clusters
    res = f_getfree(reinterpret_cast<const TCHAR*>(path), &fre_clust, &fat_fs);

    if (res == FR_OK) {
        // 1. Calculate total and free sectors
        DWORD total_sectors = (fat_fs->n_fatent - 2) * fat_fs->csize;
        DWORD free_sectors = fre_clust * fat_fs->csize;

        // 2. Determine sector size (handle variable sector size configs)
#if FF_MAX_SS == FF_MIN_SS
        WORD sector_size = FF_MIN_SS; // Usually 512 bytes
#else
        WORD sector_size = fs->ssize;
#endif // FF_MAX_SS == FF_MIN_SS
        total_bytes = static_cast<uint32_t>(total_sectors) * sector_size;
        free_bytes = static_cast<uint32_t>(free_sectors) * sector_size;
    }
}
