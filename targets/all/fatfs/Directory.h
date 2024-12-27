/*
 * Copyright (c) 2024 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/Directory.h
 */

/*
 * Copyright (c) 2024 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/File.h
 */

#pragma once

#include <kernel/kernel.h>

#include "config.h"
#include "helpers.h"
#include "FileInfo.h"

namespace fatfs
{

class Directory : private DIR
{
public:
    FLATTEN static async_once(Create, const TCHAR* path) { return async_forward(_ff_call, f_mkdir, path); }
    FLATTEN static async_once(Delete, const TCHAR* path) { return async_forward(_ff_call, f_unlink, path); }
    FLATTEN static async_once(Rename, const TCHAR* oldPath, const TCHAR* newPath) { return async_forward(_ff_call, f_rename, oldPath, newPath); }

    FLATTEN async_once(Read, FileInfo& fi) { return async_forward(_ff_call, ReadImpl, (DIR*)this, (FILINFO*)&fi); }
    FLATTEN async_once(FindFirst, FileInfo& fi, const TCHAR* path, const TCHAR* pattern) { return async_forward(_ff_call, FindFirstImpl, (DIR*)this, (FILINFO*&)fi, path, pattern); }
    FLATTEN async_once(FindNext, FileInfo& fi) { return async_forward(_ff_call, FindNextImpl, (DIR*)this, (FILINFO*)&fi); }
    FLATTEN async_once(Open, const TCHAR* path) { return async_forward(_ff_call, f_opendir, (DIR*)this, path); }
    FLATTEN async_once(Close, const TCHAR* path) { return async_forward(_ff_call, f_closedir, (DIR*)this); }

    NO_INLINE static bool ReadImpl(DIR* dir, FILINFO* fi)
    {
        auto res = f_readdir(dir, fi);
        return res == FR_OK && fi->fname[0];
    }

    NO_INLINE static bool FindFirstImpl(DIR* dir, FILINFO* fi, const TCHAR* path, const TCHAR* pattern)
    {
        auto res = f_findfirst(dir, fi, path, pattern);
        return res == FR_OK && fi->fname[0];
    }

    NO_INLINE static bool FindNextImpl(DIR* dir, FILINFO* fi)
    {
        auto res = f_findnext(dir, fi);
        return res == FR_OK && fi->fname[0];
    }
};

}
