/*
 * Copyright (c) 2024 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/helpers.h
 */

#pragma once

#include "config.h"

namespace fatfs
{

template<typename Fn, typename Arg0, typename... Args> ALWAYS_INLINE async_once(_ff_call, Fn fn, Arg0 arg0, Args... args)
{
#if FATFS_TRY_SYNC_FIRST
    // WARNING! experimental mode where we try running FatFs code synchronously
    // before spawning a worker, not really stable yet
    auto res = fn(arg0, args...);
    if (res != FR_DISK_ERR)
    {
        async_once_return(res);
    }
    if constexpr (std::is_same_v<Arg0, FIL*>)
    {
        arg0->err = 0;
    }
#endif
    return async_forward(kernel::Worker::RunWithOptions, { .noPreempt = true, .trySync = true }, fn, arg0, args...);
}

}
