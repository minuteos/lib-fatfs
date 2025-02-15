/*
 * Copyright (c) 2024 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/fatfs-lib.c
 *
 * Compiles the entire FatFs library in a single file to allow inter-module
 * optimizations
 *
 * The entire original library is places in the fatfs namespace to avoid
 * conflicts in the global namespace
 */

#pragma GCC optimize("O3", "Os")

#include <kernel/kernel.h>

#include "config.h"
#include "DiskDriver.h"

DEFINE_EXCEPTION(fatfs::Error);
DEFINE_EXCEPTION(fatfs::DiskError);

namespace fatfs
{

#pragma region ff_mutex implementations

static uint32_t l_mutexes = 0;

static async_once(ff_AcquireMutex, int vol)
{
    return async_forward(AcquireMask, l_mutexes, BIT(vol), Timeout::Seconds(1));
}

// Mutex functions so they can be inlined
int ff_mutex_create(int vol) { return true; }
void ff_mutex_delete (int vol) { (void)vol; }

int ff_mutex_take (int vol) { return kernel::Worker::Await(ff_AcquireMutex, vol).Value(); }
void ff_mutex_give (int vol) { RESBIT(l_mutexes, vol); }

#pragma endregion

#pragma region disk_ implementations

DSTATUS disk_initialize(BYTE pdrv)
{
    auto driver = DiskDriver::Get(pdrv);
    if (!driver) { return 0xFF; }
    auto res = kernel::Worker::Await(GetMethodDelegate(driver, Init));
    return res.Success() || res.ExceptionType() == DiskError ? DRESULT(res.Value()) : RES_ERROR;
}

DSTATUS disk_status(BYTE pdrv)
{
    auto driver = DiskDriver::Get(pdrv);
    if (!driver) { return 0xFF; }
    return driver->Status();
}

DRESULT disk_read(BYTE pdrv, BYTE* buff, LBA_t sector, UINT count)
{
    auto driver = DiskDriver::Get(pdrv);
    if (!driver) { return RES_NOTRDY; }
#if FATFS_TRY_SYNC_FIRST
    // could be called without a worker, we need to fail in this case
    if (!kernel::Worker::CanAwait()) { return RES_NOTRDY; }
#endif
    auto res = kernel::Worker::Await(GetMethodDelegate(driver, Read), buff, sector, count);
    return res.Success() || res.ExceptionType() == DiskError ? DRESULT(res.Value()) : RES_ERROR;
}

DRESULT disk_write(BYTE pdrv, const BYTE* buff, LBA_t sector, UINT count)
{
    auto driver = DiskDriver::Get(pdrv);
    if (!driver) { return RES_NOTRDY; }
#if FATFS_TRY_SYNC_FIRST
    // could be called without a worker, we need to fail in this case
    if (!kernel::Worker::CanAwait()) { return RES_NOTRDY; }
#endif
    auto res = kernel::Worker::Await(GetMethodDelegate(driver, Write), buff, sector, count);
    return res.Success() || res.ExceptionType() == DiskError ? DRESULT(res.Value()) : RES_ERROR;
}

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void* buff)
{
    auto driver = DiskDriver::Get(pdrv);
    if (!driver) { return RES_NOTRDY; }
#if FATFS_TRY_SYNC_FIRST
    // could be called without a worker, we need to fail in this case
    if (!kernel::Worker::CanAwait()) { return RES_NOTRDY; }
#endif
    auto res = kernel::Worker::Await(GetMethodDelegate(driver, IoCtl), cmd, buff);
    return res.Success() || res.ExceptionType() == DiskError ? DRESULT(res.Value()) : RES_ERROR;
}

[[gnu::weak]] DWORD get_fattime()
{
    // TODO
    return 0;
}

#pragma endregion

// include actual ff library

// suppress the array bounds warning that seems to be misdetected in f_mount
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"

#include <ff.c>

#pragma GCC diagnostic pop

}
