/*
 * Copyright (c) 2025 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/RapidJson.h
 *
 * A set of helpers for interfacing with the RapidJSON library
 */

#pragma once

#include "File.h"

namespace fatfs
{

class RapidJsonInputStream
{
public:
    RapidJsonInputStream(fatfs::File& f)
        : f(&f), buf(f.buf), i(0)
    {
        Fill(0);
    }

    typedef char Ch;

    //! Gets the upcoming input character
    ALWAYS_INLINE char Peek() const { return buf[i]; }

    //! Consumes the next input character
    ALWAYS_INLINE char Take()
    {
        char res = buf[i++];
        return i % FF_MAX_SS ? res : Fill(res);
    }

    //! Returns the current offset in the file
    ALWAYS_INLINE size_t Tell() { return f->fptr - e + i; }

private:
    fatfs::FIL* f;
    uint8_t* buf;
    unsigned i, e;

    //! Fill the input buffer with data
    NO_INLINE char Fill(char res)
    {
        f_read(f, buf, FF_MAX_SS, &e);
        i = 0;
        memset(buf + e, 0, FF_MAX_SS - e);
        return res;
    }
};

class RapidJsonOutputStream
{
public:
    RapidJsonOutputStream(fatfs::File& f)
        : f(&f), buf(f.buf)
    {
        i = s = f.fptr % FF_MAX_SS;
    }

    ~RapidJsonOutputStream()
    {
        Sync();
    }

    NO_INLINE void Put(char ch)
    {
        auto b = buf;
        auto w = ++i;
        b[w - 1] = ch;
        if (w == FF_MAX_SS)
        {
            // sector complete, write it out
            fatfs::f_write(f, buf + s, FF_MAX_SS - s, &i);
            s = i = 0;
        }
    }

    void Flush() {}

    NO_INLINE void Sync()
    {
        // partial write of last block
        fatfs::f_write(f, buf + s, i - s, &i);
        s = i;
        fatfs::f_sync(f);
    }

    typedef char Ch;

private:
    fatfs::FIL* f;
    uint8_t* buf;
    unsigned i, s;
};

}
