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

#ifndef __EFILOAD_H
#define __EFILOAD_H

#include <Uefi.h>
#include <Protocol/PciIo.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Protocol/DevicePath.h>
#include <Protocol/LoadedImage.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/FileHandleLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "PeStructs.h"

#define EFI_X0_PATH L"\\EFI\\x0.efi"
#define EFI_BOOT_BOOTX64_PATH L"\\EFI\\BOOT\\BOOTX64.EFI"


/*
    *    src/loader/EfiLoad.h
    *    Date: 11/27/22
    *    Author: @xmmword
*/


PE_IMAGE *CopyImageHeader(IN UINT8 *Buffer);
BOOLEAN ValidateImageIntegrity(IN EFI_HANDLE ImageHandle, IN CONST CHAR16 *ApplicationPath);

EFI_STATUS EFIAPI ChainLoadImage(IN EFI_HANDLE ImageHandle, IN CONST CHAR16 *ApplicationPath);
EFI_HANDLE EFIAPI MemoryMapImage(IN EFI_HANDLE ImageHandle, IN CONST CHAR16 *ApplicationPath);

#endif