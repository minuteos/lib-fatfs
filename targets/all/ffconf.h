/*
 * Copyright (c) 2024 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/ffconf.h
 *
 * FatFs config file
 *
 * the default enable most FatFs functionality, feel free to override individual
 * settings as needed either in defs.h or an application-specific ffconf.h
 */

#pragma once

#include <base/base.h>

// see the original ffconf.h for detailed settings descriptions,
// the defaults are configured for typical MinuteOS use

#define FFCONF_DEF	5380	/* Revision ID, must be the same as in original ffconf.h */

// can be useful to enable e.g. in bootloaders
#ifndef FF_FS_READONLY
#define FF_FS_READONLY	0
#endif

// generally no reason to enable, linker takes care of unused functions
#ifndef FF_FS_MINIMIZE
#define FF_FS_MINIMIZE	0
#endif

// enable pattern matching, linker takes care of it if disable
// redefine to 2 if you wish to match on SFN as well
#ifndef FF_USE_FIND
#define FF_USE_FIND		1
#endif

// linker takes care of it if unused
#ifndef FF_USE_MKFS
#define FF_USE_MKFS		1
#endif

// not enabled by default - see docu for details
#ifndef FF_USE_FASTSEEK
#define FF_USE_FASTSEEK	0
#endif

// linker takes care of it if unused
#ifndef FF_USE_EXPAND
#define FF_USE_EXPAND	1
#endif

// linker takes care of it if unused
#ifndef FF_USE_CHMOD
#define FF_USE_CHMOD	1
#endif

// linker takes care of it if unused
#ifndef FF_USE_LABEL
#define FF_USE_LABEL	1
#endif

// linker takes care of it if unused
#ifndef FF_USE_FORWARD
#define FF_USE_FORWARD	0
#endif

// see docu for these, we generally have other ways of achieving the same
#ifndef FF_USE_STRFUNC
#define FF_USE_STRFUNC	0
#define FF_PRINT_LLI	0
#define FF_PRINT_FLOAT	0
#define FF_STRF_ENCODE	3
#endif

// default codepage - supports many western languages
#ifndef FF_CODE_PAGE
#define FF_CODE_PAGE	852
#endif

// use LFN with stack-based buffers
#ifndef FF_USE_LFN
#define FF_USE_LFN		2
#endif

// use a reasonable LFN length, 255 is far too much
#ifndef FF_MAX_LFN
#define FF_MAX_LFN		32
#endif

// unicode uses a lot of additional space, rarely needed
#ifndef FF_LFN_UNICODE
#define FF_LFN_UNICODE	0
#endif

// buffer the full LFN by default
#ifndef FF_LFN_BUF
#define FF_LFN_BUF		FF_MAX_LFN
#endif

// buffer full SFN by default
#ifndef FF_SFN_BUF
#define FF_SFN_BUF		12
#endif

// relative path support is usually not needed
#ifndef FF_FS_RPATH
#define FF_FS_RPATH		0
#endif

// just one volume by default
#ifndef FF_VOLUMES
#define FF_VOLUMES		1
#endif

// no string volume IDs
#ifndef FF_STR_VOLUME_ID
#define FF_STR_VOLUME_ID	0
#endif

// no partition support
#ifndef FF_MULTI_PARTITION
#define FF_MULTI_PARTITION	0
#endif

// default sector sizes
#ifndef FF_MIN_SS
#define FF_MIN_SS		512
#endif

#ifndef FF_MAN_SS
#define FF_MAX_SS		512
#endif

// don't enable 64-bit LBA by default - required for >2TB disks
#ifndef FF_LBA64
#define FF_LBA64		0
#endif

#ifndef FF_MIN_GPT
#define FF_MIN_GPT		0x10000000
#endif

// typically targeting SD cards which don't have good TRIM support, so no
#ifndef FF_USE_TRIM
#define FF_USE_TRIM		0
#endif

// use sector buffer per file, the cost offsets the benefits
#ifndef FF_FS_TINY
#define FF_FS_TINY		0
#endif

// enable ExFAT by default
#ifndef FF_FS_EXFAT
#define FF_FS_EXFAT		1
#endif

// enable RTC
#ifndef FF_FS_NORTC
#define FF_FS_NORTC		0
#endif

// see docu, enable if precise free space on FAT32
#ifndef FF_FS_NOFSINFO
#define FF_FS_NOFSINFO	0
#endif

// we don't want FF to handle locking
#ifndef FF_FS_LOCK
#define FF_FS_LOCK		0
#endif

// we need to handle reentrancy as disk access can switch contexts
#ifndef FF_FS_REENTRANT
#define FF_FS_REENTRANT	1
#endif

#ifndef FF_STACK_SIZE
#define FF_STACK_SIZE   1536
#endif
