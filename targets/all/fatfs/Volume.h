/*
 * Copyright (c) 2024 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/Volume.h
 */

#include <kernel/kernel.h>

#include "config.h"
#include "helpers.h"

namespace fatfs
{

class Volume
{
public:
    enum VolumeFormat
    {
        FormatFat = FM_FAT,
        FormatFat32 = FM_FAT32,
        FormatExFat = FM_EXFAT,
        FormatAny = FM_ANY,
    };

    FLATTEN static async_once(Format, const TCHAR* path, VolumeFormat format, int bufferSectors = 8)
        { return async_forward(_ff_call, FormatImpl, path, format, bufferSectors); }
    FLATTEN static async_once(Delete, const TCHAR* path)
        { return async_forward(_ff_call, f_unlink, path); }
    FLATTEN static async_once(Rename, const TCHAR* oldPath, const TCHAR* newPath)
        { return async_forward(_ff_call, f_rename, oldPath, newPath); }
    FLATTEN static async_once(Mount, FATFS* fs, const TCHAR* path, bool immediate = false)
        { return async_forward(_ff_call, f_mount, fs, path, immediate); }
    FLATTEN static async_once(GetFree, const TCHAR* path)
        { return async_forward(_ff_call_ar, {}, GetFreeImpl, path); }

private:
    static intptr_t FormatImpl(const TCHAR* path, VolumeFormat format, int bufferSectors);
    static async_res_t GetFreeImpl(const TCHAR* path);
};

}
