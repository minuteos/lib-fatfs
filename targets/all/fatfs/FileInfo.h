/*
 * Copyright (c) 2024 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/FileInfo.h
 */

#pragma once

#include <kernel/kernel.h>

#include "config.h"
#include "helpers.h"

#include "Attributes.h"

namespace fatfs
{

class FileInfo : private FILINFO
{
public:
    FLATTEN async_once(Get, const TCHAR* path) { return async_forward(_ff_call, f_stat, path, (FILINFO*)this); }

    FSIZE_t Size() const { return fsize; }
    DWORD Time() const { return fdate << 16 | ftime; }
    fatfs::Attributes Attributes() const { return fatfs::Attributes(fattrib); }

    bool IsValid() const { return fname[0]; }
    bool IsFile() const { return !(fattrib & AM_DIR); }
    bool IsDirectory() const { return fattrib & AM_DIR; }
    bool IsHidden() const { return fattrib & AM_HID; }
    bool IsSystem() const { return fattrib & AM_SYS; }

    operator bool() const { return IsValid(); }

    const char* Name() const { return fname; }
    Span NameSpan() const { return Span(fname).Split(0); }

#if FF_USE_LFN
    const char* ShortName() const { return altname; }
#else
    const char* ShortName() const { return fname; }
#endif
};

}
