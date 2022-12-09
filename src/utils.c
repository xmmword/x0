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

#include "utils.h"
#include "mapper.h"


/*
    *    src/utils.c
    *    Date: 09/28/22
    *    Author: @xmmword
*/


/**
 * @brief Returns the size of a given file path.
 * @param filepath The path to the file.
 * @returns The size of the file, 0 if otherwise.
 */

off_t return_file_size(const char* restrict filepath) {
  struct stat st_stat = {0};

  return ((stat(filepath, &st_stat) == -1) ? 0 : st_stat.st_size);
}

/**
 * @brief Allocates memory according to the size of a given file.
 * @param filepath The path to the file.
 * @returns A pointer to the heap-allocated memory, NULL if otherwise.
 */

uint8_t* restrict malloc_file(const char* restrict filepath) {
  uint8_t* restrict memory = NULL;

  off_t file_size = return_file_size(filepath);
  if (!file_size)
    return NULL;

  if (!(memory = malloc(file_size)))
    return NULL;

  return memory;
}

/**
 * @brief Returns the address of the UEFI Runtime Service Table.
 * @returns The address of the UEFI Runtime Service Table, 0 if otherwise.
 */

uintptr_t return_rtable_address(void) {  
  uintptr_t address = 0;

  int descriptor = open(SYS_FIRMWARE_EFI_RUNTIME_PATH, O_RDONLY);
  if (descriptor == -1)
    return 0;

  uint8_t *buffer = malloc_file(SYS_FIRMWARE_EFI_RUNTIME_PATH);
  if (!buffer) {
    close(descriptor);

    return 0;
  }

  if (read(descriptor, buffer, return_file_size(SYS_FIRMWARE_EFI_RUNTIME_PATH)) == -1) {
    close(descriptor);
    free(buffer);

    return 0;
  }

  address = strtoul(buffer, NULL, 16);

  free(buffer);
  close(descriptor);
  
  return address;
}

/**
 * @brief Internally calls the kernel driver mapper to map the x0 driver into kernel memory.
 * @returns CLIENT_STATUS_SUCCESS if the driver was loaded, CLIENT_STATUS_FAILURE if otherwise.
 */

client_status_t map_kernel_driver(void) {
  return (
    (x0_loader_init() == KERN_LOAD_SUCCESS) ? CLIENT_STATUS_SUCCESS : CLIENT_STATUS_FAILURE
  );
}

/**
 * @brief Prints the address of the UEFI Runtime Service Table.
 * @returns CLIENT_STATUS_SUCCESS if the address was found, CLIENT_STATUS_FAILURE if otherwise.
 */

client_status_t print_rtable_address(void) {
  const uintptr_t address = return_rtable_address();

  if (!address) {
    printf("Failed to get value of the UEFI Runtime Service Table\n");

    return CLIENT_STATUS_FAILURE;
  }

  printf("UEFI Runtime Service Table address: 0x%lx\n", address);
  return CLIENT_STATUS_SUCCESS;
}

/**
 * @brief Prints the contents of the UEFI Runtime Service Table.
 */

void print_runtime_service_table(const efi_runtime_services_t* restrict table) {
  printf(
    "UEFI Runtime Service Table:\n"
    "\tHeader:\n"
    "\t\tSignature: 0x%lx\n"
    "\t\tRevision: 0x%x\n"
    "\t\tHeaderSize: %d\n"
    "\t\tCRC32: 0x%x\n"
    "\t\tReserved: 0x%x\n"
    "\tGetTime: %p\n"
    "\tSetTime: %p\n"
    "\tGetWakeupTime: %p\n"
    "\tSetWakeupTIme: %p\n"
    "\tSetVirtualAddressMap: %p\n"
    "\tConvertPointer: %p\n"
    "\tGetVariable: %p\n"
    "\tGetNextVariableName: %p\n"
    "\tSetVariable: %p\n"
    "\tGetNextHighMonotonicCount: %p\n"
    "\tResetSystem: %p\n"
    "\tUpdateCapsule: %p\n"
    "\tQueryCapsuleCapabilities: %p\n"
    "\n",
    table->Hdr.Signature,
    table->Hdr.Revision,
    table->Hdr.HeaderSize,
    table->Hdr.CRC32,
    table->Hdr.Reserved,
    table->GetTime,
    table->SetTime,
    table->GetWakeupTime,
    table->SetWakeupTime,
    table->SetVirtualAddressMap,
    table->ConvertPointer,
    table->GetVariable,
    table->GetNextVariableName,
    table->SetVariable,
    table->GetNextHighMonotonicCount,
    table->ResetSystem,
    table->UpdateCapsule,
    table->QueryCapsuleCapabilities
  );
}