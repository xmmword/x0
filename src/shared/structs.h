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
    *    Date: 12/14/22
    *    Author: @xmmword
*/


/* SMBIOS Entry Point Table. */
typedef struct _smbios_table_entry_point {
  uint8_t AnchorString[4]; /* The signature. */
  uint8_t EntryPointStructureChecksum; /* Checksum. */
  uint8_t EntryPointLength; /* Length of the entry point. */
  uint8_t MajorVersion; /* Major version. */
  uint8_t MinorVersion; /* Minor version. */
  uint16_t MaxStructureSize; /* Maximum structure size. */
  uint8_t EntryPointRevision; /* Entry point revision. */
  uint8_t FormattedArea[5]; /* Formatted area. */
  uint8_t IntermediateAnchorString[5]; /* Intermediate anchor string. */
  uint8_t IntermediateChecksum; /* Intermediate checksum. */
  uint16_t TableLength; /* Table length. */
  uint32_t TableAddress; /* Address of tables containing pointers to the type 0, type 1, type 2 tables, etc. */
  uint16_t NumberOfSmbiosStructures; /* Number of SMBIOS structures. */
  uint8_t SmbiosBcdRevision; /* SMBIOS BCD Revision. */
} smbios_table_entry_point_t;

/* The EFI Table Header. */
typedef struct _efi_header {
  uint64_t Signature; /* The magic value (signature) of the header. */
  uint32_t Revision; /* Revision. */
  uint32_t HeaderSize; /* The size of the header. */
  uint32_t CRC32; /* Checksum. */
  uint32_t Reserved; /* Reserved. */
} efi_header_t;

/* The EFI Runtime Services Table. */
typedef struct _efi_runtime_services {
  efi_header_t Hdr; /* The header. */

  void *GetTime; /* The GetTime service. */
  void *SetTime; /* The SetTime service. */

  void *GetWakeupTime; /* The GetWakeupTime service. */
  void *SetWakeupTime; /* The SetWakeupTime service. */

  void *SetVirtualAddressMap; /* The SetVirtualAddressMap service. */
  void *ConvertPointer; /* The ConvertPointer service. */

  void *GetVariable; /* The GetVariable service. */
  void *GetNextVariableName; /* The GetNextVariableName service. */
  void *SetVariable; /* The SetVariable service. */

  void *GetNextHighMonotonicCount; /* The GetNextHighMonotonicCount service. */
  void *ResetSystem; /* The ResetSystem service. */

  void *UpdateCapsule; /* The UpdateCapsule service. */
  void *QueryCapsuleCapabilities; /* The QueryCapsuleCapabilities service. */
} efi_runtime_services_t;

/* The IOCTL struct that will contain information that will be handled by the x0 kernel driver. */
typedef struct _ioctl_struct {
  uintptr_t address; /* The physical address of the object that we're going to access. */
  uint8_t buffer[EFI_RT_TABLE_SIZE]; /* The buffer. */
} ioctl_struct_t;

#endif