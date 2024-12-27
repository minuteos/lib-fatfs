/*
 * Copyright (c) 2024 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/tests/sanity/00_setup.cpp
 */

#include <testrunner/TestCase.h>

#include <kernel/kernel.h>

#include <fatfs/fatfs.h>

#include "TestDisk.h"

using namespace fatfs;

static FATFS _ffs;

namespace
{

TEST_CASE("Setup")
async_test
{
    async(Run)
    async_def()
    {
        TestDisk::Setup();
        f_mount(&_ffs, "", 0);
        await(DiskDriver::Get()->Init);
    }
    async_end
}
async_test_end

}
