/*
 * Copyright (c) 2024 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/File.h
 */

#pragma once

#include <kernel/kernel.h>
#include <base/Span.h>

#include "config.h"
#include "helpers.h"

namespace fatfs
{

class File : private FIL
{
public:
    enum struct Mode
    {
        // file access flags
        Read = FA_READ,
        Write = FA_WRITE,
        ReadWrite = FA_READ | FA_WRITE,

        // file creation flags
        Open = FA_OPEN_EXISTING,
        OpenOrCreate = FA_OPEN_ALWAYS,
        Create = FA_CREATE_NEW,
        CreateOrTruncate = FA_CREATE_ALWAYS,
        Append = FA_OPEN_APPEND,
    };

    DECLARE_FLAG_ENUM(Mode);

    //! Tries to opens an existing file
    FLATTEN static async_once(Stat, const TCHAR* path, FILINFO* info) { return async_forward(_ff_call_ar, NULL, StatImpl, path, info); }

    //! Tries to opens an existing file
    ALWAYS_INLINE async_once(Open, const TCHAR* path, bool write = true)
        { return async_forward(Open, path, Mode::Open | Mode::Read | Mode::Write * write); }
    //! Opens an existing file or creates a new one
    ALWAYS_INLINE async_once(OpenOrCreate, const TCHAR* path, bool write = true) { return async_forward(Open, path, Mode::OpenOrCreate | Mode::Read | Mode::Write * write); }
    //! Tries to create a new file, fails if already exists
    ALWAYS_INLINE async_once(Create, const TCHAR* path, bool write = true) { return async_forward(Open, path, Mode::Create | Mode::Read | Mode::Write * write); }
    //! Creates a new file or overwrites an existing one
    ALWAYS_INLINE async_once(CreateOrTruncate, const TCHAR* path, bool write = true) { return async_forward(Open, path, Mode::CreateOrTruncate | Mode::Read | Mode::Write * write); }
    //! Opens an existing file or creates a new one and moves the cursor to the end of it
    ALWAYS_INLINE async_once(Append, const TCHAR* path, bool write = true) { return async_forward(Open, path, Mode::Append | Mode::Read | Mode::Write * write); }

    //! Tries to open a file with the specified mode
    FLATTEN async_once(Open, const TCHAR* path, Mode mode) { return async_forward(_ff_call, f_open, (FIL*)this, path, BYTE(mode)); }
    //! Reads data from a file
    FLATTEN async_once(Read, Buffer buf) { return async_forward(_ff_call_ar, NULL, ReadImpl, (FIL*)this, buf.Pointer(), buf.Length()); }
    //! Writes data to a file
    FLATTEN async_once(Write, Span buf) { return async_forward(_ff_call_ar, NULL, WriteImpl, (FIL*)this, buf.Pointer(), buf.Length()); }
    //! Moves the cursor to the specified position in a file
    FLATTEN async_once(Seek, FSIZE_t position) { return async_forward(_ff_call, f_lseek, (FIL*)this, position); }
    //! Truncates the file
    FLATTEN async_once(Truncate) { return async_forward(_ff_call, f_truncate, (FIL*)this); }
    //! Pre-allocates space for the file
    FLATTEN async_once(Expand, FSIZE_t size, bool commit) { return async_forward(_ff_call, f_expand, (FIL*)this, size, commit); }

    //! Closes the file, writing all remaining data
    FLATTEN async_once(Close) { return async_forward(_ff_call, f_close, (FIL*)this); }
    //! Writes all remaining file data without closing the file
    FLATTEN async_once(Sync) { return async_forward(_ff_call, f_sync, (FIL*)this); }
    //! Updates FAT directory entry for already written (i.e. block-aligned) data
    FLATTEN async_once(SoftSync) { return async_forward(_ff_call, SoftSyncImpl, (FIL*)this); }

    //! Gets the current position in the file
    FLATTEN size_t Position() { return f_tell(this); }
    //! Gets the current file size
    FLATTEN size_t Size() { return f_size(this); }

    //! Raw access to the current sector buffer - use with caution
    FLATTEN Buffer SectorBuffer() { return Buffer(buf); }

    //! Access to the file error status
    FRESULT GetError() { return (FRESULT)f_error(this); }

private:
    static async_res_t ReadImpl(FIL* f, void* buf, UINT len);
    static async_res_t WriteImpl(FIL* f, const void* buf, UINT len);
    static FRESULT SoftSyncImpl(FIL* f);
    static async_res_t StatImpl(const TCHAR* path, FILINFO* info);

    friend class RapidJsonInputStream;
    friend class RapidJsonOutputStream;
};

DEFINE_FLAG_ENUM(File::Mode);

}
