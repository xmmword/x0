/*
 * Copyright (C) 2022 xmmword
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __UTILS_H
#define __UTILS_H

#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "status.h"
#include "shared/structs.h"

#define SYS_FIRMWARE_EFI_SYSTAB_PATH  "/sys/firmware/efi/systab"  /* Path to the efi/systab. */
#define SYS_FIRMWARE_EFI_RUNTIME_PATH "/sys/firmware/efi/runtime" /* Path to the efi/runtime. */


/*
    *    src/utils.h
    *    Date: 12/14/22
    *    Author: @xmmword
*/


off_t return_file_size(const char* restrict filepath);
uint8_t* restrict malloc_file(const char* restrict filepath);

uintptr_t return_rtable_address(void);
uintptr_t return_smbios_address(void);

client_status_t print_rtable_address(void);
client_status_t map_kernel_driver(void);

void print_runtime_service_table(const efi_runtime_services_t* restrict table);

#endif