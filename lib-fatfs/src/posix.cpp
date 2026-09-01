/**
 * @file file.cpp
 *
 */
/* Copyright (C) 2017-2026 by Arjan van Vught mailto:info@gd32-dmx.org
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

#include <cstddef>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <dirent.h>

#include "ff14b/source/ff.h"
#include "firmware/debug/debug_debug.h" // IWYU pragma: keep

#ifdef DEBUG_POSIX
#define POSIX_DEBUG_ENTRY() DEBUG_ENTRY()
#define POSIX_DEBUG_EXIT() DEBUG_EXIT()
#define POSIX_DEBUG_PRINTF(...) DEBUG_PRINTF(__VA_ARGS__)
#define POSIX_DEBUG_PUTS(...) DEBUG_PUTS(__VA_ARGS__)
#else
#define POSIX_DEBUG_ENTRY() \
    do {                    \
    } while (false)
#define POSIX_DEBUG_EXIT() \
    do {                   \
    } while (false)
#define POSIX_DEBUG_PRINTF(...) \
    do {                        \
    } while (false)
#define POSIX_DEBUG_PUTS(...) \
    do {                      \
    } while (false)
#endif // DEBUG_POSIX

#if !defined(CONFIG_POSIX_OPEN_FILES_MAX) || (CONFIG_POSIX_OPEN_FILES_MAX < 1)
#define CONFIG_POSIX_OPEN_FILES_MAX 2
#endif // !defined(CONFIG_POSIX_OPEN_FILES_MAX) || (CONFIG_POSIX_OPEN_FILES_MAX < 1)

namespace {
constexpr int kOpenFilesMax = CONFIG_POSIX_OPEN_FILES_MAX;
FILE s_file[kOpenFilesMax];
FIL s_ff_file[kOpenFilesMax];
FRESULT s_fresult;

int GetFileDescriptor() {
    for (int file_descriptor = 0; file_descriptor < kOpenFilesMax; file_descriptor++) {
        if (s_file[file_descriptor].udata == nullptr) {
            s_file[file_descriptor].udata = &s_ff_file[file_descriptor];
            return file_descriptor;
        }
    }

    errno = ENFILE;
    return -1;
}

int FatfsToErrno(BYTE error) {
    POSIX_DEBUG_PRINTF("s_fresult=%d", static_cast<int>(s_fresult));

    switch (static_cast<FRESULT>(error)) {
        case FR_OK:                  /* FatFS (0) Succeeded */
            return (0);              /* POSIX OK */
        case FR_DISK_ERR:            /* FatFS (1) A hard error occurred in the low level disk I/O layer */
            return (EIO);            /* POSIX Input/output error (POSIX.1) */
        case FR_INT_ERR:             /* FatFS (2) Assertion failed */
            return (EPERM);          /* POSIX Operation not permitted (POSIX.1) */
        case FR_NOT_READY:           /* FatFS (3) The physical drive cannot work */
            return (EBUSY);          /* POSIX Device or resource busy (POSIX.1) */
        case FR_NO_FILE:             /* FatFS (4) Could not find the file */
            return (ENOENT);         /* POSIX No such file or directory (POSIX.1) */
        case FR_NO_PATH:             /* FatFS (5) Could not find the path */
            return (ENOENT);         /* POSIX No such file or directory (POSIX.1) */
        case FR_INVALID_NAME:        /* FatFS (6) The path name format is invalid */
            return (EINVAL);         /* POSIX Invalid argument (POSIX.1) */
        case FR_DENIED:              /* FatFS (7) Access denied due to prohibited access or directory full */
            return (EACCES);         /* POSIX Permission denied (POSIX.1) */
        case FR_EXIST:               /* FatFS (8) Access denied due to prohibited access */
            return (EACCES);         /* POSIX Permission denied (POSIX.1) */
        case FR_INVALID_OBJECT:      /* FatFS (9) The file/directory object is invalid */
            return (EINVAL);         /* POSIX Invalid argument (POSIX.1) */
        case FR_WRITE_PROTECTED:     /* FatFS (10) The physical drive is write protected */
            return (EROFS);          /* POSIX Read-only filesystem (POSIX.1) */
        case FR_INVALID_DRIVE:       /* FatFS (11) The logical drive number is invalid */
            return (ENXIO);          /* POSIX No such device or address (POSIX.1) */
        case FR_NOT_ENABLED:         /* FatFS (12) The volume has no work area */
            return (ENOSPC);         /* POSIX No space left on device (POSIX.1) */
        case FR_NO_FILESYSTEM:       /* FatFS (13) There is no valid FAT volume */
            return (ENXIO);          /* POSIX No such device or address (POSIX.1) */
        case FR_MKFS_ABORTED:        /* FatFS (14) The f_mkfs() aborted due to any parameter error */
            return (EINVAL);         /* POSIX Invalid argument (POSIX.1) */
        case FR_TIMEOUT:             /* FatFS (15) Could not get a grant to access the volume within defined period */
            return (EBUSY);          /* POSIX Device or resource busy (POSIX.1) */
        case FR_LOCKED:              /* FatFS (16) The operation is rejected according to the file sharing policy */
            return (EBUSY);          /* POSIX Device or resource busy (POSIX.1) */
        case FR_NOT_ENOUGH_CORE:     /* FatFS (17) LFN working buffer could not be allocated */
            return (ENOMEM);         /* POSIX Not enough space (POSIX.1) */
        case FR_TOO_MANY_OPEN_FILES: /* FatFS (18) Number of open files > _FS_SHARE */
            return (EMFILE);         /* POSIX Too many open files (POSIX.1) */
        case FR_INVALID_PARAMETER:   /* FatFS (19) Given parameter is invalid */
            return (EINVAL);         /* POSIX Invalid argument (POSIX.1) */
    }

    return (EBADMSG); /* POSIX Bad message (POSIX.1) */
}
} // namespace

extern "C" {
int fileno(FILE* stream) { // NOLINT
    if (stream == nullptr) {
        errno = EBADF;
        return -1;
    }

    for (int file_no = 0; file_no < kOpenFilesMax; file_no++) {
        if (&s_file[file_no] == stream) {
            return file_no;
        }
    }

    return EOF;
}

// http://elm-chan.org/fsw/ff/doc/open.html
FILE* fopen(const char* path, const char* mode) { // NOLINT
    assert(path != nullptr);
    assert(mode != nullptr);

    POSIX_DEBUG_PRINTF("%s %s", path, mode);

    errno = 0;
    BYTE file_mode;
    BYTE file_option;

    switch (mode[0]) {
        case 'r':
            file_mode = (BYTE)FA_READ;
            file_option = 0;
            break;
        case 'w':
            file_mode = (BYTE)FA_WRITE;
            file_option = (BYTE)FA_CREATE_ALWAYS;
            break;
        case 'a':
            file_mode = (BYTE)FA_WRITE;
            file_option = (BYTE)FA_OPEN_APPEND;
            break;
        default:
            return nullptr;
            break;
    }

    while (*++mode != '\0') {
        switch (*mode) {
            case '+':
                file_mode = (BYTE)(FA_READ | FA_WRITE);
                break;
            case 'x':
                file_option = (BYTE)FA_CREATE_NEW;
                break;
            default:
                return nullptr;
                break;
        }
    }

    const auto kFd = GetFileDescriptor();
    POSIX_DEBUG_PRINTF("kFd=%d", kFd);

    if (kFd < 0) {
        errno = EBADF;
        return nullptr;
    }

    s_fresult = f_open(&s_ff_file[kFd], (TCHAR*)path, (BYTE)(file_mode | file_option));
    errno = FatfsToErrno(s_fresult);

    POSIX_DEBUG_PRINTF("errno=%d", errno);

    if (s_fresult == FR_OK) {
        return &s_file[kFd];
    }

    s_file[kFd].udata = nullptr;
    return nullptr;
}

int fclose(FILE* stream) { // NOLINT
    errno = 0;

    if (stream == nullptr) {
        return 0;
    }

    const auto kFd = fileno(stream);
    POSIX_DEBUG_PRINTF("kFd=%d", kFd);

    if (kFd < 0) {
        errno = EBADF;
        return EOF;
    }

    s_fresult = f_close((FIL*)stream->udata);
    errno = FatfsToErrno(s_fresult);

    POSIX_DEBUG_PRINTF("errno=%d", errno);

    stream->udata = nullptr;

    if (s_fresult == FR_OK) {
        return 0;
    }

    return EOF;
}

int fgetc(FILE* stream) { // NOLINT
    UINT bytes_read;
    errno = 0;

    if (stream == nullptr) {
        return EOF;
    }

    char buffer;
    s_fresult = f_read(reinterpret_cast<FIL*>(stream->udata), &buffer, static_cast<UINT>(1), &bytes_read);

    if (s_fresult == FR_OK) {
        if (bytes_read > 0) {
            return buffer;
        }

        if (bytes_read < 1) {
            errno = FatfsToErrno(s_fresult);
            stream->flags |= __SEOF;
            return EOF;
        }
    }

    errno = FatfsToErrno(s_fresult);
    return EOF;
}

size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream) { // NOLINT
    UINT bytes_read;

    s_fresult = f_read((FIL*)stream->udata, ptr, (size * nmemb), &bytes_read);
    errno = FatfsToErrno(s_fresult);

    if (s_fresult == FR_OK) {
        return bytes_read;
    }

    return 0;
}

int fseek(FILE* stream, long offset, int whence) { // NOLINT
    if (whence == SEEK_SET) {
        s_fresult = f_lseek((FIL*)stream->udata, (FSIZE_t)offset);
    } else if (whence == SEEK_END) {
        s_fresult = f_lseek((FIL*)stream->udata, f_size((FIL*)stream->udata));
    }

    errno = FatfsToErrno(s_fresult);

    if (s_fresult == FR_OK) {
        return 0;
    }

    return -1;
}

long ftell(FILE* stream) { // NOLINT
    return (long)f_tell((FIL*)stream->udata);
}

char* fgets(char* string, int size, FILE* stream) { // NOLINT
    assert(string != nullptr);
    errno = 0;

    if (stream == nullptr) {
        *string = '\0';
        return nullptr;
    }

    if (f_gets(string, size, (FIL*)stream->udata) != string) {
        *string = '\0';
        errno = FatfsToErrno(f_error((FIL*)stream->udata));
        return nullptr;
    }

    return string;
}

void clearerr(FILE* stream) { // NOLINT
    stream->flags &= static_cast<uint8_t>(~__SEOF);
    stream->flags &= static_cast<uint8_t>(~__SERR);
}

int ferror(FILE* stream) { // NOLINT
    return (stream->flags & __SERR) == __SERR ? 1 : 0;
}

int feof(FILE* stream) { // NOLINT
    return (stream->flags & __SEOF) == __SEOF ? 1 : 0;
}

// The following API´s are implemented when CONFIG_FS_ENABLE_WRITE is defined

int fputs([[maybe_unused]] const char* s, [[maybe_unused]] FILE* stream) { // NOLINT
#ifndef CONFIG_FS_ENABLE_WRITE
    errno = ENOSYS;
    return -1;
#else
    assert(s != nullptr);
    assert(stream != nullptr);
    errno = 0;

    return f_puts(s, (FIL*)stream->udata);
#endif // CONFIG_FS_ENABLE_WRITE
}

size_t fwrite([[maybe_unused]] const void* ptr, [[maybe_unused]] size_t size, [[maybe_unused]] size_t nmemb, [[maybe_unused]] FILE* stream) {
#ifndef CONFIG_FS_ENABLE_WRITE
    errno = ENOSYS;
    return 0;
#else
    assert(stream != nullptr);
    UINT bytes_write;

    s_fresult = f_write((FIL*)stream->udata, ptr, (size * nmemb), &bytes_write);
    errno = FatfsToErrno(s_fresult);

    if (s_fresult == FR_OK) {
        return bytes_write;
    }

    return 0;
#endif // CONFIG_FS_ENABLE_WRITE
}

int fputc([[maybe_unused]] int c, [[maybe_unused]] FILE* stream) { // NOLINT
#ifndef CONFIG_FS_ENABLE_WRITE
    errno = ENOSYS;
    return 0;
#else
    assert(stream != nullptr);

    UINT bytes_write;

    s_fresult = f_write((FIL*)stream->udata, &c, 1, &bytes_write);
    errno = FatfsToErrno(s_fresult);

    if (s_fresult == FR_OK) {
        return 1;
    }

    return 0;
#endif // CONFIG_FS_ENABLE_WRITE
}

int unlink([[maybe_unused]] const char* pathname) { // NOLINT
#ifndef CONFIG_FS_ENABLE_WRITE
    errno = ENOSYS;
    return -1;
#else
    s_fresult = f_unlink(pathname);
    errno = FatfsToErrno(s_fresult);

    if (s_fresult == FR_OK) {
        return 0;
    }

    return -1;
#endif // CONFIG_FS_ENABLE_WRITE
}

#ifndef CONFIG_FS_ENABLE_WRITE
#else
static DIR s_dir;
static dirent_t s_dirent;
#endif // CONFIG_FS_ENABLE_WRITE

DIR* opendir([[maybe_unused]] const char* dirname) {
#ifndef CONFIG_FS_ENABLE_WRITE
    errno = ENOSYS;
    POSIX_DEBUG_EXIT();
    return nullptr;
#else
    const auto kLen = strlen(dirname);

    if ((kLen > 0) && (dirname[kLen - 1] == '.')) {
        char* pathdir = (char*)dirname;
        pathdir[kLen - 1] = '\0';
        s_fresult = f_opendir(&s_dir, pathdir);
    } else {
        s_fresult = f_opendir(&s_dir, dirname);
    }

    errno = FatfsToErrno(s_fresult);

    if (s_fresult != FR_OK) {
        return nullptr;
    }

    return &s_dir;
#endif // CONFIG_FS_ENABLE_WRITE
}

/**
 * On success, readdir() returns a pointer to a dirent structure.
 * If the end of the directory stream is reached, nullptr is returned and errno is not changed.
 * If an error occurs, nullptr is returned and errno is set appropriately.
 */

struct dirent* readdir([[maybe_unused]] DIR* dirp) {
#ifndef CONFIG_FS_ENABLE_WRITE
    errno = ENOSYS;
    return nullptr;
#else
    FILINFO fno;

    fno.fname[0] = '\0';

    s_fresult = f_readdir(&s_dir, &fno);

    if (s_fresult != FR_OK) {
        errno = FatfsToErrno(s_fresult);
        return nullptr;
    }

    if (fno.fname[0] == '\0') {
        return nullptr;
    }

    errno = 0;

    strncpy(s_dirent.d_name, fno.fname, sizeof(s_dirent.d_name) - 1);

    if (fno.fattrib == AM_DIR) {
        s_dirent.d_type = DT_DIR;
    } else {
        s_dirent.d_type = DT_REG;
    }

    return &s_dirent;
#endif // CONFIG_FS_ENABLE_WRITE
}

int closedir([[maybe_unused]] DIR* dirp) {
#ifndef CONFIG_FS_ENABLE_WRITE
    errno = ENOSYS;
    return -1;
#else
    s_fresult = f_closedir(&s_dir);
    errno = FatfsToErrno(s_fresult);

    if (s_fresult == FR_OK) {
        return 0;
    }

    return -1;
#endif // CONFIG_FS_ENABLE_WRITE
}
}
