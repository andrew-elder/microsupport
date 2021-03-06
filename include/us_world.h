#ifndef US_WORLD_H
#define US_WORLD_H

/*
Copyright (c) 2013, J.D. Koftinoff Software, Ltd.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of J.D. Koftinoff Software, Ltd.. nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL J.D. KOFTINOFF SOFTWARE, LTD.. BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 \addtogroup US_world Platform Specific Configuration
 */
/*@{*/

#ifdef US_FIRST_HEADER
#include US_FIRST_HEADER
#endif

#if defined(TARGET_PLATFORM_POSIX)
#define US_CONFIG_POSIX
#elif defined(TARGET_PLATFORM_LINUX)
#define US_CONFIG_POSIX
#define US_CONFIG_LINUX
#elif defined(TARGET_PLATFORM_MACOSX)
#undef US_CONFIG_POSIX
#define US_CONFIG_POSIX
#undef US_CONFIG_MACOSX
#define US_CONFIG_MACOSX
#endif

#if defined(__AVR__)
#define US_CONFIG_MICROCONTROLLER
#undef US_CONFIG_POSIX
#elif defined(__APPLE__)
#undef US_CONFIG_MACOSX
#define US_CONFIG_MACOSX
#undef US_CONFIG_POSIX
#define US_CONFIG_POSIX
#elif defined(__linux__)
#undef US_CONFIG_LINUX
#define US_CONFIG_LINUX
#undef US_CONFIG_POSIX
#define US_CONFIG_POSIX
#elif defined(__CYGWIN__)
#undef US_CONFIG_POSIX
#define US_CONFIG_POSIX
#elif defined(WIN32) || defined(_WIN32) || defined(_MSC_VER)
#undef US_CONFIG_WIN32
#define US_CONFIG_WIN32
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE 1
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#undef WINVER
#define WINVER _WIN32_WINNT
#endif
#ifndef _SCL_SECURE_NO_DEPRECATE
#define _SCL_SECURE_NO_DEPRECATE 1
#endif
#endif

#include "us_options.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4267)
#endif

#if defined(US_CONFIG_WIN32)
#ifndef inline
#define inline __inline
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>

#include <errno.h>
#ifndef EADDRINUSE
#define EADDRINUSE 112 /* Address already in use */
#endif

#include <windows.h>
#include <io.h>
#include <fcntl.h>

#include <process.h>

#include "us_ms.h"

#endif

#if defined(US_CONFIG_POSIX)
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif
#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE 1
#endif
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 1
#endif
#ifndef _DARWIN_C_SOURCE
#define _DARWIN_C_SOURCE 1
#endif
#include <locale.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <errno.h>
#include <strings.h>
#include <net/if.h>
#endif

#include <limits.h>
#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

#if defined(_WIN32)
#include <io.h>
#endif

#ifndef __cplusplus
#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#if !defined(bool)&&(defined(BOOLEAN) || defined(_WIN32))
#define bool BOOLEAN
#endif
#if !defined(true) && defined(TRUE)
#define true TRUE
#endif
#if !defined(false) && defined(FALSE)
#define false FALSE
#endif
#if !defined(bool)
#define bool int
#endif
#if !defined(true)
#define true 1
#endif
#if !defined(false)
#define false 0
#endif
#endif
#endif

#ifndef us_min
#define us_min(a, b) (a) < (b) ? (a) : (b)
#endif

#ifndef us_max
#define us_max(a, b) (a) < (b) ? (b) : (a)
#endif

#ifdef __cplusplus
extern "C" {
#endif

static inline bool us_strncpy(char *dest, const char *src, size_t dest_buf_size) {
    bool r = false;
    if (dest && src && dest_buf_size > 2) {
        size_t src_len = strlen(src);
        if (src_len < dest_buf_size - 1) {
            strncpy(dest, src, dest_buf_size - 1);
            r = true;
        }
    }
    return r;
}

static inline bool us_strncat(char *dest, const char *src, size_t dest_buf_size) {
    bool r = false;
    if (dest && src && dest_buf_size > 2) {
        size_t src_len = strlen(src);
        size_t dest_len = strlen(dest);
        if ((src_len + dest_len) < dest_buf_size - 1) {
            strncat(dest, src, dest_buf_size - 1);
            r = true;
        }
    }
    return r;
}

#ifdef __cplusplus
}
#endif

#include "us_platform.h"
#include "us_logger.h"
#include "us_bits.h"

#ifdef __cplusplus
#include "us_world.hpp"
#endif

/*@}*/

#endif
