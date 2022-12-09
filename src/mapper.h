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

#ifndef __MAPPER_H
#define __MAPPER_H

#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/sysmacros.h>

#include "shared/ioctl_codes.h"

#define NO_DRIVER_ARG "" /* An empty string. */


/*
    *    src/mapper.h
    *    Date: 09/28/22
    *    Author: @xmmword
*/


/* Enumerations that represent the mapper codes/statuses. */
typedef enum _loader_status {
  KERN_LOAD_SUCCESS =  1,
  KERN_UNLOAD_SUCCESS = 1,
  KERN_DEVICE_INTERFACE_SUCCESS = 1,

  KERN_LOAD_FAILURE = -1,
  KERN_UNLOAD_FAILURE = -2,
  KERN_LOAD_INSTANCE_ERR = -3,
  KERN_DEVICE_INTERFACE_FAILURE = -4
} loader_status_t;

/* Struct for /proc/modules entries. */
typedef struct _kdriver {
  int instances; /* The amount of driver instances. */
  uint64_t size, offset; /* The size of the driver. */ /* Kernel memory offset. (Address if you're on x64) */
  char state[BUFSIZ], kmodule[BUFSIZ], dependencies[BUFSIZ]; /* Current driver state. */ /* The driver name. */ /* Driver dependencies. */
} kdriver_t;

static inline __always_inline loader_status_t create_driver_interface(void);
static inline __always_inline bool parse_kernel_driver(kdriver_t *driver, const char* restrict string);

static void *return_elf_object(const char* restrict path);
static bool invoke_finit_module(const char* restrict module_path);

const char* restrict handle_loader_err(const loader_status_t err);

kdriver_t *return_driver_object(const char* restrict driver);

loader_status_t x0_loader_init(void);
loader_status_t remove_kernel_driver(const char* restrict driver);
loader_status_t insert_kernel_driver(const char* restrict module_path);

#endif