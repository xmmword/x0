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

#include "SmBios.h"


/*
    *    src/bootkit/SmBios.c
    *    Date: 12/14/22
    *    Author: @xmmword
*/


/**
 * @brief Checks if the character is valid or not.
 * @param Character The character.
 * @returns TRUE if the character is valid, FALSE if otherwise.
 */

BOOLEAN IsValidCharacter(const UINT8 Character) {
  return (
    ((Character >= 'a' && Character <= 'z') || (Character >= 'A' && Character <= 'Z') || (Character >= '0' && Character <= '9')) ? TRUE : FALSE
  );
}

/**
 * @brief Returns the RDTSC output which will be used as the seed for the XOR key generation.
 * @returns The seed value.
 */

UINT64 ReturnRdtsc(VOID) {
  UINT64 Var = 0, Out = 0;

  __asm__ volatile("rdtsc" : "=a" (Var), "=d" (Out));

  return ((Out << 32) | Var);
}

/**
 * @brief Gets the length of a SMBIOS string.
 * @param Memory The bytes of the SMBIOS table.
 * @returns The string length.
 */

UINTN GetSmBiosStringLength(const UINT8 *Memory) {
  UINTN Offset = 0;

  while (IsValidCharacter(Memory[Offset]))
    Offset++;

  return Offset;
}

/**
 * @brief Encrypts the SMBIOS string with each byte XOR'red with a unique XOR key.
 * @param Data The SMBIOS string.
 * @param Length The length of the SMBIOS string.
 * @returns The encrypted string.
 */

UINT8 *GenerateXorString(const UINT8 *Data, const UINTN Length) {
  static UINT8 Buffer[BUFFER_SIZEMAX] = {0};

  if (Length >= BUFFER_SIZEMAX)
    return NULL;

  CopyMem(Buffer, Data, BUFFER_SIZEMAX);

  for (int i = 0; i < Length; ++i)
    Buffer[i] = Buffer[i] ^ (ReturnRdtsc() % 26);

  return Buffer;
}

/**
 * @brief Patches the SMBIOS tables.
 * @param TableAddress The physical address of the table.
 */

VOID PatchSmBiosTables(VOID *TableAddress) {
  for (UINTN i = 10; i < 500; ++i) {
    if (!IsValidCharacter(*(UINT8 *)(TableAddress + i)))
      continue;

    const UINTN CachedOffset = i, SmBiosStringLength = GetSmBiosStringLength((UINT8 *)(TableAddress + CachedOffset));

    CopyMem((TableAddress + CachedOffset), GenerateXorString((UINT8 *)(TableAddress + CachedOffset), SmBiosStringLength), SmBiosStringLength);
    i += SmBiosStringLength;
  }
}

/**
 * @brief Attempts to find the pointer to the SMBIOS Entry Point via scanning the configuration tables.
 * @returns The pointer to the SMBIOS Entry Point, NULL if otherwise.
 */

SMBIOS_TABLE_ENTRY_POINT *ReturnSmBiosEntryPointAddress(VOID) {
  for (UINTN i = 0; i < CONFIG_TABLE_SIZEMAX; ++i)

    if (!CompareMem(&gST->ConfigurationTable[i].VendorGuid, &gEfiSmbiosTableGuid, sizeof(gEfiSmbiosTableGuid)))
      return (SMBIOS_TABLE_ENTRY_POINT *)gST->ConfigurationTable[i].VendorTable;

  return NULL;
}