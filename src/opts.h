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

#ifndef __OPTS_H
#define __OPTS_H

#include "status.h"

#define OPTAMOUNT_MAX 4 /* The amount of command-line options. */


/*
    *    src/opts.h
    *    Date: 07/17/22
    *    Author: @xmmword
*/


/* Struct containing information about a command-line option. */
typedef struct _flag {
  const char* restrict short_flag; /* Short flag. */
  const char* restrict long_flag; /* Long flag. */
  const char* restrict description; /* The flag description. */
  client_status_t (*function)(); /* The function pointer. */
} flag_t;

#endif