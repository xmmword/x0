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
    *    Date: 11/27/22
    *    Author: @xmmword
*/


extern EFI_SYSTEM_TABLE *gST; /* Global EFI System Table. */

/*int CheckEntry(VOID* entry) 
{
    if (!entry)
        return 0;

    char* pointer = (char*)entry;
    int checksum = 0;
    int length = entry->EntryPointLength;
    for (int i = 0; i < length; i++) 
    {
        checksum = checksum + (int)pointer[i];
    }

    return (checksum == 0);
}*/

VOID *ScanSmBiosTablePhysicalMemory(VOID) {
  const char *Signature = "_SM_";
  
  for (long offset = 0xf0000; offset < 0xfffff; offset += 0x10) 
    {
        if (*(int *)(offset) == *(int *)Signature) 
        {
            return (void *)offset;
        }
    }

  return NULL;
}