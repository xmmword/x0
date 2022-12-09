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
#include "EfiLoad.h"


/*
    *    src/loader/EfiLoad.c
    *    Date: 11/27/22
    *    Author: @xmmword
*/


EFI_GUID GlobalLoadedImageProtocolGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID; /* Loaded Image Protocol GUID. */

/**
 * @brief Copies the PE headers of the EFI image.
 * @param Buffer The memory-mapped EFI image.
 * @returns A struct containing the copied headers, NULL if otherwise.
 */

PE_IMAGE *CopyImageHeader(IN UINT8 *Buffer) {
  PE_IMAGE *ParsedImageInformation = AllocatePool(sizeof(PE_IMAGE));
  if (!ParsedImageInformation)
    return NULL;

  CopyMem(&ParsedImageInformation->DosHeader, Buffer, sizeof(IMAGE_DOS_HEADER));
  CopyMem(&ParsedImageInformation->NtHeader, (Buffer + ParsedImageInformation->DosHeader.e_lfanew), sizeof(IMAGE_NT_HEADERS));

  return ParsedImageInformation;
}

/**
 * @brief Chainloads an EFI image.
 * @param ImageHandle The Image Handle.
 * @param ApplicationPath The path to the EFI image.
 * @returns EFI_SUCCESS if the image could be chainloaded, 'ReturnStatus' if otherwise.
 */

EFI_STATUS EFIAPI ChainLoadImage(IN EFI_HANDLE ImageHandle, IN CONST CHAR16 *ApplicationPath) {
  EFI_STATUS ReturnStatus = EFI_NOT_FOUND;
  EFI_HANDLE BootApplicationHandle = NULL;

  if (!(BootApplicationHandle = MemoryMapImage(ImageHandle, ApplicationPath)))
    return ReturnStatus;

  return (EFI_ERROR((ReturnStatus = gBS->StartImage(BootApplicationHandle, NULL, NULL))) ? ReturnStatus : EFI_SUCCESS);
}

/**
 * @brief Maps an EFI image into memory.
 * @param ImageHandle The Image Handle.
 * @param ApplicationPath The path to the EFI image.
 * @returns EFI_SUCCESS if the image could be mapped into memory, NULL if otherwise.
 */

EFI_HANDLE EFIAPI MemoryMapImage(IN EFI_HANDLE ImageHandle, IN CONST CHAR16 *ApplicationPath) {
  EFI_STATUS ReturnStatus;

  EFI_HANDLE ApplicationHandle = NULL;
  EFI_LOADED_IMAGE *ApplicationImageProtocol = NULL;
  EFI_DEVICE_PATH_PROTOCOL *ApplicationDevicePath = NULL;

  if (EFI_ERROR((ReturnStatus = gBS->HandleProtocol(ImageHandle, &GlobalLoadedImageProtocolGuid, (void *)&ApplicationImageProtocol))))
    return NULL;

  if (!(ApplicationDevicePath = FileDevicePath(ApplicationImageProtocol->DeviceHandle, ApplicationPath)))
    return NULL;

  ReturnStatus = gBS->LoadImage(TRUE, ImageHandle, ApplicationDevicePath, NULL, 0, &ApplicationHandle);
  FreePool(ApplicationDevicePath);

  return (EFI_ERROR(ReturnStatus) ? NULL : ApplicationHandle);
}

/**
 * @brief Validates the integrity of an EFI image.
 * @param ImageHandle The Image Handle.
 * @param ApplicationPath The path to the EFI image.
 * @returns TRUE if the image is valid, FALSE if otherwise.
 */

BOOLEAN ValidateImageIntegrity(IN EFI_HANDLE ImageHandle, IN CONST CHAR16 *ApplicationPath) {
  BOOLEAN ReturnStatus = FALSE;
  
  UINT64 ImageSize;
  UINT8 *FileBuffer = NULL;

  EFI_FILE_HANDLE FileHandle = NULL, VolumeHandle = CreateFileHandle(ImageHandle);
  if (!VolumeHandle)
    return ReturnStatus;

  VolumeHandle->Open(VolumeHandle, &FileHandle, (CHAR16 *)ApplicationPath, EFI_FILE_MODE_READ, (EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM));

  if (!(ImageSize = ReturnImageSize(FileHandle)))
    return ReturnStatus;

  if (!(FileBuffer = AllocatePool(ImageSize)))
    return ReturnStatus;

  FileHandle->Read(FileHandle, &ImageSize, FileBuffer);

  PE_IMAGE *HeaderInformation = CopyImageHeader(FileBuffer);
  if (!HeaderInformation)
    return ReturnStatus;

  if (HeaderInformation->DosHeader.e_magic == DOS_SIGNATURE && HeaderInformation->NtHeader.Signature == NT_SIGNATURE)
    ReturnStatus = TRUE;

  FileHandle->Close(FileHandle);
  VolumeHandle->Close(VolumeHandle);

  FreePool(FileBuffer);
  FreePool(HeaderInformation);

  return ReturnStatus;
}