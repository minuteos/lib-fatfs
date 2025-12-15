/*
 * Copyright (c) 2025 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/fatfs-lib.h
 *
 * Exposes internal FatFs details for specific use cases
 */

#pragma once

#include <kernel/kernel.h>

namespace fatfs
{

async_once(ff_AcquireMutex, int vol = 0);
void ff_ReleaseMutex(int vol = 0);

}
