/*
 * Copyright (c) 2024 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/DiskDriver.h
 */

#pragma once

#include <kernel/kernel.h>
#include <base/Span.h>

#include "config.h"

namespace fatfs
{

struct DiskDriver
{
    // type aliases so implementation doesn't have to live in the fatfs namespace
    using DSTATUS = fatfs::DSTATUS;
    using LBA_t = fatfs::LBA_t;

    DSTATUS Status() const { return status; }

    virtual async(Init) = 0;
    virtual async(Read, void* buf, LBA_t sectorStart, size_t sectorCount) = 0;
    virtual async(Write, const void* buf, LBA_t sectorStart, size_t sectorCount) = 0;
    virtual async(IoCtl, uint8_t cmd, void* buff) = 0;

    static DiskDriver* Get(unsigned volume = 0) { ASSERT(volume < FF_VOLUMES); return drivers[volume]; }
    static void Set(DiskDriver* driver) { Set(0, driver); }
    static void Set(unsigned volume, DiskDriver* driver)
    {
        ASSERT(volume < FF_VOLUMES);
        ASSERT(!drivers[volume] || drivers[volume] == driver);
        drivers[volume] = driver;
    }

protected:
    void Status(DSTATUS status) { this->status = status; }

private:
    static DiskDriver* drivers[FF_VOLUMES];
    DSTATUS status;
};

}
