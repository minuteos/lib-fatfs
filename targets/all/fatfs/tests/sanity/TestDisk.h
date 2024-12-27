/*
 * Copyright (c) 2024 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/tests/sanity/TestDisk.cpp
 */

#pragma once

#include <base/base.h>

#include <fatfs/fatfs.h>

class TestDisk : fatfs::DiskDriver
{
public:
    static void Setup()
    {
        Set(&s_instance);
    }

    virtual async(Init);
    virtual DSTATUS Status() const { return status; };
    virtual async(Read, void* buf, LBA_t sectorStart, size_t sectorCount);
    virtual async(Write, const void* buf, LBA_t sectorStart, size_t sectorCount);
    virtual async(IoCtl, uint8_t cmd, void* buff);

private:
    enum
    {
        SectorSize = 512,
        SectorCount = 80000,    // 32MB+ disk needed to support FAT32
        BlockSize = 4096,
        BlockCount = SectorCount / (BlockSize / SectorSize),
    };

    static TestDisk s_instance;
    int fd;
    DSTATUS status = STA_NOINIT;
    static TestDisk drv;
};
