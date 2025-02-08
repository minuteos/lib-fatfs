/*
 * Copyright (c) 2025 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/File.cpp
 */

#include "File.h"

namespace fatfs
{

async_res_t File::ReadImpl(FIL* f, void* buf, UINT len)
{
    UINT read;
    if (auto err = f_read(f, buf, len, &read))
    {
        async_once_throw(Error, err);
    }
    async_once_return(read);
}

async_res_t File::WriteImpl(FIL* f, const void* buf, UINT len)
{
    UINT written;
    if (auto err = f_write(f, buf, len, &written))
    {
        async_once_throw(Error, err);
    }
    async_once_return(written);
}

}
