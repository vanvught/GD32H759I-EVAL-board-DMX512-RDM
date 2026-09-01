/**
 * @file diskio.cpp
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

/*
The linker script should contain:

   .ramdisk (NOLOAD) :
   {
     . = ALIGN(4);
     ramdisk_start = .;
     . = . + __ramdisk_size;
     ramdisk_end = .;
     . = ALIGN(4);
   } > RAM
 */

#include <cassert>
#include <cstdint>
#include <cstring>
#include <cstdio>

#include "ff14b/source/diskio.h"

static_assert(FF_MIN_SS == FF_MAX_SS);
static_assert(FF_MAX_SS == 512);

#ifndef DEV_RAM
#define DEV_RAM 0
#endif

extern "C" {
extern uint8_t ramdisk_start; // NOLINT // Start of ramdisk address
extern uint8_t ramdisk_end;   // NOLINT // End of ramdisk address
}

namespace {
constexpr uint32_t kSectorSize = FF_MAX_SS;
static_assert((kSectorSize & (kSectorSize - 1U)) == 0);
constexpr uint32_t kRootDirectoryEntries = 32;

size_t RamdiskSize() {
    return static_cast<size_t>(&ramdisk_end - &ramdisk_start);
}

LBA_t SectorCount() {
    return (RamdiskSize() / kSectorSize);
}

bool IsValidRange(LBA_t sector, UINT count) {
    const auto kSectorCount = SectorCount();
    return (count != 0) && (sector < kSectorCount) && (static_cast<LBA_t>(count) <= kSectorCount - sector);
}

uint8_t* RamdiskData() {
    return &ramdisk_start;
}

DSTATUS state = STA_NOINIT;

FATFS ram_fs;
MKFS_PARM opt = {
    .fmt = FM_FAT | FM_SFD,          // FAT12/16 on a Super Floppy Disk layout
    .n_fat = 1,                      // Reduce to 1 FAT table copy (wastes less space than 2)
    .align = 0,                      // No special block alignment needed for RAM
    .n_root = kRootDirectoryEntries, // Max files in root directory
    .au_size = 0,                    // Let FatFs choose auto cluster allocation size
};

alignas(4) BYTE work_buffer[FF_MAX_SS];
} // namespace

extern "C" {
DSTATUS disk_initialize(BYTE pdrv) {
    if (pdrv != DEV_RAM) {
        return STA_NOINIT;
    }

    state &= static_cast<DSTATUS>(~STA_NOINIT);

    return state;
}

DSTATUS disk_status(BYTE pdrv) {
    if (pdrv != DEV_RAM) {
        return STA_NOINIT;
    }

    return state;
}

DRESULT disk_read(BYTE pdrv, BYTE* buff, LBA_t sector, UINT count) {
    if (pdrv != DEV_RAM || (state & STA_NOINIT) == STA_NOINIT) {
        return RES_NOTRDY;
    }

    if (!IsValidRange(sector, count)) {
        return RES_PARERR;
    }

    const auto kOffset = static_cast<size_t>(sector) * kSectorSize;

    memcpy(buff, RamdiskData() + kOffset, static_cast<size_t>(count) * kSectorSize);

    return RES_OK;
}

DRESULT disk_write(BYTE pdrv, const BYTE* buff, LBA_t sector, UINT count) {
    if (pdrv != DEV_RAM || (state & STA_NOINIT) == STA_NOINIT) {
        return RES_NOTRDY;
    }

    if (!IsValidRange(sector, count)) {
        return RES_PARERR;
    }

    const auto kOffset = static_cast<size_t>(sector) * kSectorSize;

    memcpy(RamdiskData() + kOffset, buff, count * kSectorSize);

    return RES_OK;
}

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void* buff) { // NOLINT(bugprone-easily-swappable-parameters)
    if (pdrv != DEV_RAM || (state & STA_NOINIT) == STA_NOINIT) {
        return RES_NOTRDY;
    }

    switch (cmd) {
        case CTRL_SYNC:
            // Cache is always synchronized for pure RAM arrays
            return RES_OK;

        case GET_SECTOR_COUNT:
            *static_cast<LBA_t*>(buff) = SectorCount();
            return RES_OK;

        case GET_SECTOR_SIZE:
            *static_cast<WORD*>(buff) = kSectorSize;
            return RES_OK;

        case GET_BLOCK_SIZE:
            // Erase block size in sectors (1 means non-flash/byte-erasable RAM)
            *static_cast<DWORD*>(buff) = 1;
            return RES_OK;

        default:
            return RES_PARERR;
    }
}
}

namespace fatfs::ram {
bool Init() {
    const auto kSize = RamdiskSize();

    if (kSize < kSectorSize || (kSize % kSectorSize) != 0) {
        printf("Invalid RAM disk size: %u\n", kSize);
        return false;
    }

    printf("Init RAM file system -> %u bytes\n", kSize);

    auto res = f_mkfs("0:", &opt, work_buffer, sizeof(work_buffer));
    if (res != FR_OK) {
        printf("f_mkfs failed: %d\n", res);
        return false;
    }

    res = f_mount(&ram_fs, "0:", 1);
    if (res != FR_OK) {
        printf("f_mount failed: %d\n", res);
        return false;
    }

    puts("RAM Disk Ready!");

    return true;
}
} // namespace fatfs::ram
