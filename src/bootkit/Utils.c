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
    *    src/bootkit/Utils.c
    *    Date: 12/14/22
    *    Author: @xmmword
*/


/**
 * @brief Prints the x0 banner.
 */

VOID PrintBanner(VOID) {
  Print(L""
    "         ____ \n"
    "   _  __/ __ \\\n"
    "  | |//_/ / / / Hardware Spoofing / Virtualbox-Hardening x64 Bootkit\n"
    " _>  </ /_/ /   Written by: https://github.com/xmmword\n"
    "/_/|_|\\____/  \n\n"
  );
}