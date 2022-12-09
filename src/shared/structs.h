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

#ifndef __STRUCTS_H
#define __STRUCTS_H

#define EFI_RT_TABLE_SIZE sizeof(efi_runtime_services_t) /* Size of the UEFI runtime service table. */

/* These 3 macros were taken from https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Base.h */
#define SIGNATURE_16(A, B) ((A) | (B << 8))
#define SIGNATURE_32(A, B, C, D)  (SIGNATURE_16(A, B) | (SIGNATURE_16(C, D) << 16))
#define SIGNATURE_64(A, B, C, D, E, F, G, H) \
  (SIGNATURE_32(A, B, C, D) | ((uint64_t)(SIGNATURE_32(E, F, G, H)) << 32))

/* This macro was taken from https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi/UefiSpec.h */
#define EFI_RUNTIME_SERVICES_SIGNATURE SIGNATURE_64('R','U','N','T','S','E','R','V')


/*
    *    src/shared/structs.h
    *    Date: 07/17/22
    *    Author: @xmmword
*/


typedef struct _efi_header {
  uint64_t Signature;
  uint32_t Revision;
  uint32_t HeaderSize;
  uint32_t CRC32;
  uint32_t Reserved;
} efi_header_t;

typedef struct _efi_runtime_services {
  efi_header_t Hdr;

  void *GetTime;
  void *SetTime;

  void *GetWakeupTime;
  void *SetWakeupTime;

  void *SetVirtualAddressMap;
  void *ConvertPointer;

  void *GetVariable;
  void *GetNextVariableName;
  void *SetVariable;

  void *GetNextHighMonotonicCount;
  void *ResetSystem;

  void *UpdateCapsule;
  void *QueryCapsuleCapabilities;
} efi_runtime_services_t;

typedef struct _ioctl_struct {
  uintptr_t address;
  uint8_t buffer[EFI_RT_TABLE_SIZE];
} ioctl_struct_t;

#endif