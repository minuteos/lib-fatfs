/*
 * Copyright (c) 2025 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/File.cpp
 */

#include "File.h"

// copied from ff.c
#define FA_MODIFIED	0x40	/* File has been modified */
#define FA_DIRTY	0x80	/* FIL.buf[] needs to be written-back */

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

FRESULT File::SoftSyncImpl(FIL* f)
{
    if (!(f->flag & FA_MODIFIED))
    {
        // no change at all
        return FR_OK;
    }
    if (!(f->flag & FA_DIRTY))
    {
        // buffer flushed, just sync
        return f_sync(f);
    }
    if (f_tell(f) != f_size(f))
    {
        // write pointer *not* at the end of file, cannot do soft sync
        return f_sync(f);
    }

    // we need to act as if there was nothing in the buffer, so we have to reduce the size by the buffered amount
    auto hide = f_tell(f) & (FF_MAX_SS - 1);
    f_tell(f) -= hide;
    f_size(f) -= hide;
    f->flag &= ~FA_DIRTY;
    auto res = f_sync(f);
    f_tell(f) += hide;
    f_size(f) += hide;
    f->flag |= FA_DIRTY;
    return res;
}

}
