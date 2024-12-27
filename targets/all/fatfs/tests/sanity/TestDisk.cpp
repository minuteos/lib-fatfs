/*
 * Copyright (c) 2024 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/tests/sanity/TestDisk.cpp
 */

#include "TestDisk.h"

#include <fcntl.h>
#include <unistd.h>

#define MYDBG(...)  DBGCL("DISK", __VA_ARGS__)

TestDisk TestDisk::s_instance;

async(TestDisk::Init)
async_def_sync()
{
    if (status & STA_NOINIT)
    {
        const char* filename = "/tmp/testdisk.img";
        fd = open(filename, O_CREAT | O_RDWR, 0600);
        MYDBG("Initializing %s...", filename);
        auto zero = new char[BlockSize];
        memset(zero, 0, BlockSize);
        for (int i = 0; i < BlockCount; i++)
        {
            write(fd, zero, BlockSize);
        }
        delete[] zero;
        MYDBG("Initialized disk with %d %d-byte sectors (%d %d-byte blocks)", SectorCount, SectorSize, BlockCount, BlockSize);
        status &= ~STA_NOINIT;
    }

    async_return(fatfs::RES_OK);
}
async_end

async(TestDisk::Read, void* buf, LBA_t sectorStart, size_t sectorCount)
async_def()
{
    if (sectorStart >= SectorCount || sectorStart + sectorCount >= SectorCount)
    {
        async_return(fatfs::RES_PARERR);
    }

    MYDBG("RD %d+%d", sectorStart, sectorCount);
    lseek(fd, sectorStart * SectorSize, SEEK_SET);
    read(fd, buf, sectorCount * SectorSize);
    async_yield();  // just to exercise async over sync
    async_return(fatfs::RES_OK);
}
async_end

async(TestDisk::Write, const void* buf, LBA_t sectorStart, size_t sectorCount)
async_def()
{
    if (sectorStart >= SectorCount || sectorStart + sectorCount >= SectorCount)
    {
        async_return(fatfs::RES_PARERR);
    }

    MYDBG("WR %d+%d", sectorStart, sectorCount);
    lseek(fd, sectorStart * SectorSize, SEEK_SET);
    write(fd, buf, sectorCount * SectorSize);
    async_yield();  // just to exercise async over sync
    async_return(fatfs::RES_OK);
}
async_end

async(TestDisk::IoCtl, uint8_t cmd, void* buff)
async_def()
{
    switch (cmd)
    {
    case CTRL_SYNC:
        MYDBG("SYNC");
        async_yield();  // just to exercise async over sync
        break;
    case GET_SECTOR_COUNT:
        *(uint32_t*)buff = SectorCount;
        break;
    case GET_SECTOR_SIZE:
        *(uint32_t*)buff = SectorSize;
        break;
    case GET_BLOCK_SIZE:
        *(uint32_t*)buff = BlockSize;
        break;
    default:
        async_return(fatfs::RES_PARERR);
    }

    async_return(fatfs::RES_OK);
}
async_end