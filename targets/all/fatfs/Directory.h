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
    FLATTEN static async_once(Create, const TCHAR* path) { return async_forward(_ff_call_ar, NULL, MkdirImpl, path); }
    FLATTEN static async_once(Delete, const TCHAR* path) { return async_forward(_ff_call, f_unlink, path); }
    FLATTEN static async_once(Rename, const TCHAR* oldPath, const TCHAR* newPath) { return async_forward(_ff_call, f_rename, oldPath, newPath); }

    FLATTEN async_once(Read, FileInfo& fi) { return async_forward(_ff_call_ar, NULL, ReaddirImpl, (DIR*)this, (FILINFO*)&fi); }
    FLATTEN async_once(FindNext, FileInfo& fi, const TCHAR* pattern) { pat = pattern; return async_forward(_ff_call_ar, NULL, FindNextImpl, (DIR*)this, (FILINFO*)&fi); }
    FLATTEN async_once(Open, const TCHAR* path) { return async_forward(_ff_call, f_opendir, (DIR*)this, path); }
    FLATTEN async_once(Close, const TCHAR* path) { return async_forward(_ff_call, f_closedir, (DIR*)this); }

private:
    static async_res_t MkdirImpl(const TCHAR* path);
    static async_res_t ReaddirImpl(DIR* dir, FILINFO* fi);
    static async_res_t FindNextImpl(DIR* dir, FILINFO* fi);
};

}
