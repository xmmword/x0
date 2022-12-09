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


/*
    *    src/loader/Utils.c
    *    Date: 11/27/22
    *    Author: @xmmword
*/


extern EFI_GUID GlobalLoadedImageProtocolGuid; /* Loaded Image Protocol GUID. */
EFI_GUID GlobalFileSystemProtocolGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID; /* file system interface GUID */

/**
 * @brief Returns the size of an EFI image.
 * @param FileHandle Handle to the ESP file.
 * @returns The size of the EFI image if it could be calculated, 0 if otherwise.
 */

UINT64 ReturnImageSize(IN EFI_FILE_HANDLE FileHandle) {
  UINT64 ReturnSize;

  EFI_FILE_INFO *FileInfo = FileHandleGetInfo(FileHandle);
  if (!FileInfo)
    return 0;

  ReturnSize = FileInfo->FileSize;

  FreePool(FileInfo);
  return ReturnSize;
}

/**
 * @brief Creates and returns a file handle.
 * @param ImageHandle The image handle.
 * @returns The file handle, NULL if otherwise.
 */

EFI_FILE_HANDLE CreateFileHandle(IN EFI_HANDLE ImageHandle) {
  EFI_FILE_HANDLE FileHandle = NULL;
  EFI_LOADED_IMAGE *EfiImage = NULL;
  EFI_FILE_IO_INTERFACE *FileIO = NULL;

  if (EFI_ERROR(gBS->HandleProtocol(ImageHandle, &GlobalLoadedImageProtocolGuid, (VOID **)&EfiImage)))
    return NULL;
  
  if (EFI_ERROR(gBS->HandleProtocol(EfiImage->DeviceHandle, &GlobalFileSystemProtocolGuid, (VOID *)&FileIO)))
    return NULL;

  FileIO->OpenVolume(FileIO, &FileHandle);
  return FileHandle;
}