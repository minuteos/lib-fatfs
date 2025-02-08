/*
 * Copyright (c) 2024 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/config.h
 *
 * Loads the FatFs config and library headers in the correct order
 */

#pragma once

// resolve config using include path (ff.h uses relative include so it always finds its own version)
#include <ffconf.h>

#include <kernel/kernel.h>

// the actual library lives in a namespace, so we need to include it inside it

namespace fatfs
{

#include "ff.h"
#include "diskio.h"

DECLARE_EXCEPTION(Error);
DECLARE_EXCEPTION(DiskError);

}
