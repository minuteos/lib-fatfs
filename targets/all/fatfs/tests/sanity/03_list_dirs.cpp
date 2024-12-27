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

TEST_CASE("01 List top level")
async_test
{
    async(Run)
    async_def(
        Directory d;
        FileInfo fi;
        int dcount, fcount;
    )
    {
        auto res = await(f.d.Open, "");
        AssertEqual(res, FR_OK);

        while (await(f.d.Read, f.fi))
        {
            DBGCL("DIR", "%s %X", f.fi.Name(), f.fi.Attributes());
            (f.fi.IsFile() ? f.fcount : f.dcount)++;
        }

        AssertEqual(f.fcount, 1);
        AssertEqual(f.dcount, 2);
    }
    async_end
}
async_test_end

TEST_CASE("02 List non-existent")
async_test
{
    async(Run)
    async_def(
        Directory d;
    )
    {
        auto res = await(f.d.Open, "nx");
        AssertEqual(res, FR_NO_PATH);
    }
    async_end
}
async_test_end

TEST_CASE("03 List Many Files")
async_test
{
    async(Run)
    async_def(
        Directory d;
        FileInfo fi;
        int dcount, fcount;
    )
    {
        auto res = await(f.d.Open, "many");
        AssertEqual(res, FR_OK);

        while (await(f.d.Read, f.fi))
        {
            DBGCL("DIR", "%s %X", f.fi.Name(), f.fi.Attributes());
            (f.fi.IsFile() ? f.fcount : f.dcount)++;
        }

        AssertEqual(f.fcount, 100);
        AssertEqual(f.dcount, 0);
    }
    async_end
}
async_test_end

}
