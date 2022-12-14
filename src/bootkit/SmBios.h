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

#ifndef __SMBIOS_H
#define __SMBIOS_H

#include <Uefi.h>
#include <Guid/EventGroup.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <IndustryStandard/SmBios.h>

#define BUFFER_SIZEMAX       1024
#define CONFIG_TABLE_SIZEMAX 1000


/*
    *    src/bootkit/SmBios.h
    *    Date: 12/14/22
    *    Author: @xmmword
*/


UINT64 ReturnRdtsc(VOID);
BOOLEAN IsValidCharacter(const UINT8 Character);

UINTN GetSmBiosStringLength(const UINT8 *Memory);
UINT8 *GenerateXorString(const UINT8 *Data, const UINTN Length);

VOID PatchSmBiosTables(VOID *TableAddress);
SMBIOS_TABLE_ENTRY_POINT *ReturnSmBiosEntryPointAddress(VOID);

#endif