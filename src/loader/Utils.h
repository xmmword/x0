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

#include <Uefi.h>
#include <Guid/FileInfo.h>
#include <Protocol/PciIo.h>
#include <Library/UefiLib.h>
#include <Protocol/DevicePath.h>
#include <Protocol/LoadedImage.h>
#include <Library/DevicePathLib.h>
#include <Library/FileHandleLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>


/*
    *    src/loader/Utils.h
    *    Date: 12/14/22
    *    Author: @xmmword
*/


UINT64 ReturnImageSize(IN EFI_FILE_HANDLE FileHandle);
EFI_FILE_HANDLE CreateFileHandle(IN EFI_HANDLE ImageHandle);

#endif