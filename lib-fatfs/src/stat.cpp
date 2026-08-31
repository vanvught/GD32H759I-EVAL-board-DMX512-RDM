/**
 * @file stat.cpp
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

#include <sys/stat.h>
#include <cerrno>
#include <cstring>

#include "ff.h"

namespace {
time_t FatTimeToUnix(uint16_t date, uint16_t time) {
    struct tm tm_unix;

    memset(&tm_unix, 0, sizeof(struct tm));

    tm_unix.tm_sec = (time << 1) & 0x3e; // 2 second resolution
    tm_unix.tm_min = ((time >> 5) & 0x3f);
    tm_unix.tm_hour = ((time >> 11) & 0x1f);
    tm_unix.tm_mday = (date & 0x1f);
    tm_unix.tm_mon = ((date >> 5) & 0x0f) - 1;
    tm_unix.tm_year = ((date >> 9) & 0x7f) + 80;

    const auto kUnix = mktime(&tm_unix);

    return kUnix;
}
} // namespace

// error: 'int stat(const char*, stat*)' hides constructor for 'struct stat' [-Werror=shadow]
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"

int stat(const char* path, struct stat* buf) { // NOLINT
    FILINFO fno;
    FRESULT res;

    memset(buf, 0, sizeof(struct stat));

    // FatFs f_stat() typically fails on "/" or "." because it expects a file or subdirectory name.
    if (strcmp(path, "/") == 0 || strcmp(path, ".") == 0) {
        buf->st_mode = S_IFDIR;
        buf->st_size = 0;
        return 0;
    }

    res = f_stat(path, &fno);

    if (res != FR_OK) {
        if (res == FR_NO_FILE || res == FR_NO_PATH) {
            errno = ENOENT; // No such file or directory
        } else if (res == FR_DENIED) {
            errno = EACCES; // Permission denied
        } else {
            errno = EIO; // Generic I/O error
        }
        return -1; // POSIX returns -1 on error
    }

    buf->st_size = fno.fsize; // File size mapping

    auto epoch = FatTimeToUnix(fno.fdate, fno.ftime);
    buf->st_mtime = epoch;

    // Map file types and attributes (FatFs AM_DIR maps to S_IFDIR)
    if (fno.fattrib & AM_DIR) {
        buf->st_mode |= S_IFDIR;
    } else {
        buf->st_mode |= S_IFREG; // Regular file
    }

    // Map basic read/write/readonly permissions
    if (fno.fattrib & AM_RDO) {
        buf->st_mode |= (S_IRUSR | S_IRGRP | S_IROTH); // Read only
    } else {
        buf->st_mode |= (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); // Read/Write
    }

    return 0;
}

#pragma GCC diagnostic pop