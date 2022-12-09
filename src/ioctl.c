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

#include "ioctl.h"
#include "utils.h"
#include "shared/structs.h"
#include "shared/ioctl_codes.h"


/*
    *    src/ioctl.c
    *    Date: 07/17/22
    *    Author: @xmmword
*/


/**
 * @brief Dumps the data of UEFI Runtime Service Table.
 * @returns CLIENT_STATUS_SUCCESS if the UEFI Runtime Service Table could be dumped, CLIENT_STATUS_FAILURE if otherwise.
 */

client_status_t dump_runtime_service_table(void) {
  ioctl_struct_t memory = {0};

  if (!(memory.address = return_rtable_address())) {
    printf("Failed to get value of the UEFI Runtime Service Table\n");

    return CLIENT_STATUS_FAILURE;
  }

  int descriptor = open(X0_DEVICE_INTERFACE, O_RDWR);
  if (descriptor == -1)
    return CLIENT_STATUS_FAILURE;

  if (ioctl(descriptor, IOCTL_DUMP_RUNTIME_SERVICE_TABLE, &memory) == -1) {
    printf("Failed to communicate with the x0 driver!\n");

    close(descriptor);
    return CLIENT_STATUS_FAILURE;
  }

  print_runtime_service_table((efi_runtime_services_t *)memory.buffer);
  close(descriptor);

  return CLIENT_STATUS_SUCCESS;
}