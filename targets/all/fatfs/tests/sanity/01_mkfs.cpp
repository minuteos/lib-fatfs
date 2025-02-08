/*
 * Copyright (c) 2024 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/tests/sanity/01_mkfs.cpp
 */

#include <testrunner/TestCase.h>

#include <kernel/kernel.h>

#include <fatfs/fatfs.h>

using namespace fatfs;

namespace
{

TEST_CASE("01a Format FAT")
async_test
{
    async(Run)
    async_def()
    {
        await(Volume::Format, "", Volume::FormatFat);
    }
    async_end
}
async_test_end

TEST_CASE("01b Format exFAT")
async_test
{
    async(Run)
    async_def()
    {
        await(Volume::Format, "", Volume::FormatExFat);
    }
    async_end
}
async_test_end

TEST_CASE("01c Format FAT32")
async_test
{
    async(Run)
    async_def()
    {
        await(Volume::Format, "", Volume::FormatFat32);
    }
    async_end
}
async_test_end

}
