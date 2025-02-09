/*
 * Copyright (c) 2025 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/Volume.cpp
 */

#include "Volume.h"

namespace fatfs
{

intptr_t Volume::FormatImpl(const TCHAR* path, VolumeFormat format, int bufferSectors)
{
    auto buf = new char[FF_MAX_SS * bufferSectors];
    MKFS_PARM parm = { .fmt = format };
    auto res = f_mkfs(path, &parm, buf, FF_MAX_SS * bufferSectors);
    delete[] buf;
    return res;
}

async_res_t Volume::GetFreeImpl(const TCHAR* path)
{
    DWORD nclst;
    FATFS* fs;
    if (auto err = f_getfree(path, &nclst, &fs))
    {
        async_once_throw(Error, err);
    }
    else
    {
        async_once_return(nclst);
    }
}

}
