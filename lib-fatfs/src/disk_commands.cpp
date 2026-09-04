/**
 * @file disk_commands.cpp
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

#include <dirent.h>
#include <sys/stat.h>

#include <uart0.h>

#include "disk_commands.h"

void DriveSize(const char* path, uint32_t& total_bytes, uint32_t& free_bytes);

namespace disk::commands {
namespace {
bool Filter(const char* name) {
    return *name == '.';
}

constexpr char kDirName[] = ".";
} // namespace

void Dir() {
    auto* dirp = opendir(kDirName);

    if (dirp == nullptr) {
        perror("opendir");
        return;
    }

    uint32_t total_bytes{};
    uint32_t free_bytes{};
    DriveSize(kDirName, total_bytes, free_bytes);

    uart0::Printf("Capacity: %u\nFree: %u\n\n", static_cast<unsigned>(total_bytes), static_cast<unsigned>(free_bytes));

    struct dirent* read_dir{};

    do {
        read_dir = readdir(dirp);
        if (read_dir != nullptr) {
            if (read_dir->d_type == DT_DIR) {
                continue;
            }

            if (Filter(read_dir->d_name)) {
                continue;
            }

            struct stat buf;
            const auto kStat = stat(read_dir->d_name, &buf);

            if (kStat == 0) {
                time_t epoch_time = buf.st_mtime;
                auto* local_time = localtime(&epoch_time);
                printf("%d-%.2d-%.2d %.2d:%.2d %6u %s\n", 
                1900 + local_time->tm_year, 
                1 + local_time->tm_mon, 
                local_time->tm_mday, 
                local_time->tm_hour, 
                local_time->tm_min, 
                static_cast<unsigned>(buf.st_size),
                read_dir->d_name);
            }
        }
    } while (read_dir != nullptr);

    closedir(dirp);
    uart0::PutChar('\n');
}

void Del(const char* file_name) {
    uart0::Printf("Del: %s\n", file_name);
}
} // namespace disk::commands
