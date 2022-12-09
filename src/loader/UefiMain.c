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
#include <Library/UefiBootServicesTableLib.h>

#include "EfiLoad.h"


/*
    *    src/loader/UefiMain.c
    *    Date: 11/27/22
    *    Author: @xmmword
*/


/**
 * @brief Entry point for the x0 loader.
 * @param ImageHandle The Image Handle.
 * @param SystemTable The EFI System Table.
 * @returns EFI_SUCCESS if the EFI application was able to properly execute, 'ChainLoadStatus' if otherwise.
 */

EFI_STATUS EFIAPI UefiEntry(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  if (!ValidateImageIntegrity(ImageHandle, EFI_X0_PATH)) {
    Print(L"[x0]: Please provide a valid UEFI image.\n");

    return EFI_NOT_FOUND;
  }

  EFI_STATUS ChainLoadStatus = ChainLoadImage(ImageHandle, EFI_X0_PATH);
  if (ChainLoadStatus != EFI_SUCCESS)
    return ChainLoadStatus;

  Print(L"[x0]: Chainloading BOOTX64 from [%s]\n", EFI_BOOT_BOOTX64_PATH);
  return (((ChainLoadStatus = ChainLoadImage(ImageHandle, EFI_BOOT_BOOTX64_PATH)) != EFI_SUCCESS) ? ChainLoadStatus : EFI_SUCCESS);
}