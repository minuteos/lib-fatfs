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

#include "TestDisk.h"

using namespace fatfs;

namespace
{

TEST_CASE("01 Try Open Missing File")
async_test
{
    async(Run)
    async_def(
        fatfs::File f;
    )
    {
        auto res = await_catch(f.f.Open, "test1.txt");
        AssertException(res, fatfs::Error, FR_NO_FILE);
    }
    async_end
}
async_test_end

TEST_CASE("02 Create File")
async_test
{
    async(Run)
    async_def(
        fatfs::File f;
    )
    {
        await(f.f.Create, "test1.txt");
        await(f.f.Close);
    }
    async_end
}
async_test_end

TEST_CASE("03 Write to File")
async_test
{
    async(Run)
    async_def(
        fatfs::File f;
    )
    {
        await(f.f.Open, "test1.txt");
        auto written = await(f.f.Write, "TEST DATA");
        AssertEqual(written, 9);
        await(f.f.Close);
    }
    async_end
}
async_test_end

TEST_CASE("04 Try Replace File")
async_test
{
    async(Run)
    async_def(
        fatfs::File f;
    )
    {
        auto res = await_catch(f.f.Create, "test1.txt");
        AssertException(res, fatfs::Error, FR_EXIST);
    }
    async_end
}
async_test_end

TEST_CASE("05 Replace File")
async_test
{
    async(Run)
    async_def(
        fatfs::File f;
    )
    {
        await(f.f.CreateOrTruncate, "test1.txt");
        await(f.f.Close);
    }
    async_end
}
async_test_end

TEST_CASE("06 Create many files")
async_test
{
    async(Run)
    async_def(
        int i;
        fatfs::File f;
        char fname[100];
    )
    {
        await(Directory::Create, "many");
        for (f.i = 0; f.i < 100; f.i++)
        {
            sprintf(f.fname, "many/test file %d.txt", f.i);
            await(f.f.CreateOrTruncate, f.fname);
            sprintf(f.fname, "CONTENT OF TEST FILE %d", f.i);
            size_t written = await(f.f.Write, Span(f.fname, sizeof(f.fname)));
            AssertEqual(written, sizeof(f.fname));
            await(f.f.Close);
        }
    }
    async_end
}
async_test_end

extern "C" int main();

async(LargeFileTest, size_t blockSize, size_t totalSize)
async_def(
    size_t i;
    fatfs::File f;
    unsigned written;
)
{
    await(Directory::Create, "large");
    await(f.f.CreateOrTruncate, "large/large.bin");

    for (f.i = 0; f.i < totalSize; f.i += blockSize)
    {
        size_t written = await(f.f.Write, Span((const char*)(intptr_t(&main) & ~0xFFFF), blockSize));
        AssertEqual(written, blockSize);
    }

    await(f.f.Close);
}
async_end

TEST_CASE("07a Create large file 128b")
async_test
{
    async(Run) { return async_forward(LargeFileTest, 128, 16 * 1024 * 1024); }
}
async_test_end


TEST_CASE("07b Create large file 256b")
async_test
{
    async(Run) { return async_forward(LargeFileTest, 256, 16 * 1024 * 1024); }
}
async_test_end

TEST_CASE("07c Create large file 512b")
async_test
{
    async(Run) { return async_forward(LargeFileTest, 512, 16 * 1024 * 1024); }
}
async_test_end

TEST_CASE("07d Create large file 1k")
async_test
{
    async(Run) { return async_forward(LargeFileTest, 1024, 16 * 1024 * 1024); }
}
async_test_end

TEST_CASE("07e Create large file 4k")
async_test
{
    async(Run) { return async_forward(LargeFileTest, 4096, 16 * 1024 * 1024); }
}
async_test_end

}
