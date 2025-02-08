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

TEST_CASE("01 Read File")
async_test
{
    async(Run)
    async_def(
        fatfs::File f;
        uint8_t buffer[200];
    )
    {
        await(f.f.Open, "many/test file 1.txt");
        auto res = await(f.f.Read, f.buffer);
        AssertEqual(res, 100);
        AssertEqual(Span(f.buffer, 22), Span("CONTENT OF TEST FILE 1"));
        await(f.f.Close);
    }
    async_end
}
async_test_end

TEST_CASE("02 Read many files")
async_test
{
    async(Run)
    async_def(
        int i;
        Directory d;
        FileInfo fi;
        fatfs::File f;
        char buf[200];
    )
    {
        await(f.d.Open, "many");

        while (await(f.d.Read, f.fi))
        {
            sprintf(f.buf, "many/%s", f.fi.Name());
            await(f.f.Open, f.buf);
            auto read = await(f.f.Read, f.buf);
            AssertEqual(read, 100);
            AssertEqual(Span(f.buf, 21), Span("CONTENT OF TEST FILE "));
        }
    }
    async_end
}
async_test_end

async(LargeFileTest, size_t blockSize)
async_def(
    size_t i;
    fatfs::File f;
    FileInfo fi;
    uint8_t* buf;
)
{
    await(f.f.Open, "large/large.bin");
    await(f.fi.Get, "large/large.bin");
    AssertEqual(f.fi.Size(), 16u * 1024 * 1024);
    f.buf = new uint8_t[blockSize];

    while (size_t read = await(f.f.Read, Buffer(f.buf, blockSize)))
    {
        AssertEqual(read, blockSize);
    }

    delete f.buf;

    await(f.f.Close);
}
async_end

TEST_CASE("03a Read large file 128b")
async_test
{
    async(Run) { return async_forward(LargeFileTest, 128); }
}
async_test_end


TEST_CASE("03b Read large file 256b")
async_test
{
    async(Run) { return async_forward(LargeFileTest, 256); }
}
async_test_end

TEST_CASE("03c Read large file 512b")
async_test
{
    async(Run) { return async_forward(LargeFileTest, 512); }
}
async_test_end

TEST_CASE("03d Read large file 1k")
async_test
{
    async(Run) { return async_forward(LargeFileTest, 1024); }
}
async_test_end

TEST_CASE("03e Read large file 4k")
async_test
{
    async(Run) { return async_forward(LargeFileTest, 4096); }
}
async_test_end

}
