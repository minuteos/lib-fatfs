#
# Copyright (c) 2024 triaxis s.r.o.
# Licensed under the MIT license. See LICENSE.txt file in the repository root
# for full license information.
#
# Include.mk
#

FATFS_MAKEFILE := $(call curmake)
FATFS_DIR = $(call parentdir, $(call parentdir, $(call parentdir, $(dir $(FATFS_MAKEFILE)))))fatfs/source/

INCLUDE_DIRS += $(FATFS_DIR)

COMPONENTS += kernel base
