/*
 * Copyright (c) 2024 triaxis s.r.o.
 * Licensed under the MIT license. See LICENSE.txt file in the repository root
 * for full license information.
 *
 * fatfs/Attributes.h
 */

#pragma once

#include <base/base.h>

#include "config.h"

namespace fatfs
{

enum struct Attributes
{
    ReadOnly = AM_RDO,
    Hidden = AM_HID,
    System = AM_SYS,
    Directory = AM_DIR,
    Archive = AM_ARC,
};

DEFINE_FLAG_ENUM(Attributes);

}
