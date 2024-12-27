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

private:
    static NO_INLINE FRESULT FormatImpl(const TCHAR* path, VolumeFormat format, int bufferSectors)
    {
        auto buf = new char[FF_MAX_SS * bufferSectors];
        MKFS_PARM parm = { .fmt = format };
        auto res = f_mkfs(path, &parm, buf, FF_MAX_SS * bufferSectors);
        delete[] buf;
        return res;
    }
};

}
