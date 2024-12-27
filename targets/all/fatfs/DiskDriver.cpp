/*
 * Copyright (c) 2024 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/DiskDriver.cpp
 */

#include "DiskDriver.h"

namespace fatfs
{

DiskDriver* DiskDriver::drivers[FF_VOLUMES];

}
