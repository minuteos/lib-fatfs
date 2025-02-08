/*
 * Copyright (c) 2025 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/Directory.cpp
 */

#include "Directory.h"

namespace fatfs
{

async_res_t Directory::MkdirImpl(const TCHAR* path)
{
    if (auto err = f_mkdir(path))
    {
        if (err == FR_EXIST) { async_once_return(false); }
        async_once_throw(Error, err);
    }
    async_once_return(true);
}

async_res_t Directory::ReaddirImpl(DIR* dir, FILINFO* fi)
{
    if (auto err = f_readdir(dir, fi))
    {
        async_once_throw(Error, err);
    }
    async_once_return(fi->fname[0]);
}

async_res_t Directory::FindNextImpl(DIR* dir, FILINFO* fi)
{
    if (auto err = f_findnext(dir, fi))
    {
        async_once_throw(Error, err);
    }
    async_once_return(fi->fname[0]);
}

}
