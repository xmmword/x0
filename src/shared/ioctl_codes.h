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

#ifndef __IOCTL_CODES_H
#define __IOCTL_CODES_H

#include "structs.h"

#define IOCTL_MAGIC_VAL 0x65 /* IOCTL magic value. */

/* x0 driver IOCTLs. */
#define IOCTL_GET_SMBIOS_TABLE_VIRTADDR         _IOWR(IOCTL_MAGIC_VAL, 1, ioctl_struct_t *)
#define IOCTL_DUMP_RUNTIME_SERVICE_TABLE        _IOWR(IOCTL_MAGIC_VAL, 2, ioctl_struct_t *)

/* x0 driver interface information. */
#define X0_DRIVER "x0driver" /* x0 driver name. */
#define X0_DRIVER_NUMBER 66 /* x0 driver number. */
#define X0_DRIVER_FILENAME "x0driver.ko" /* x0 driver filename. */
#define X0_DEVICE_INTERFACE "/dev/x0interface" /* x0 device driver interface. */


/*
    *    src/shared/ioctl_codes.h
    *    Date: 12/14/22
    *    Author: @xmmword
*/


#endif