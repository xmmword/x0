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
#include "kstatus.h"


/*
    *    src/driver/ioctl.c
    *    Date: 12/14/22
    *    Author: @xmmword
*/


/* Structure containing initialization/de-initialization functions and the IOCTL handler. */
const struct file_operations operations = {
  .open = driver_ioctl_init,
  .release = driver_ioctl_exit,
  .unlocked_ioctl = driver_ioctl_handler
};

EXPORT_SYMBOL(operations);

/**
 * @brief The vmtrace IOCTL code handler.
 * @param file A structure representing an open file.
 * @param command The command.
 * @param argument The userspace argument.
 * @returns KERN_SUCCESS if the program executed successfully, KERN_FAILURE if otherwise.
 */

long int driver_ioctl_handler(struct file *file, const unsigned int command, const unsigned long int argument) {
  ioctl_struct_t ioctl_struct = {0};

  switch (command) {
    case IOCTL_DUMP_RUNTIME_SERVICE_TABLE:
      if (copy_from_user(&ioctl_struct, (ioctl_struct_t *)argument, sizeof(ioctl_struct)))
        return IOCTL_FAILURE;

      void *table = ioremap(ioctl_struct.address, EFI_RT_TABLE_SIZE);
      if (!table)
        return IOCTL_FAILURE;

      memcpy(&ioctl_struct.buffer, table, EFI_RT_TABLE_SIZE);
      iounmap(table);

      return ((copy_to_user((ioctl_struct_t *)argument, &ioctl_struct, 
          sizeof(ioctl_struct))) ? IOCTL_FAILURE : IOCTL_SUCCESS);
    case IOCTL_GET_SMBIOS_TABLE_VIRTADDR:
      if (copy_from_user(&ioctl_struct, (ioctl_struct_t *)argument, sizeof(ioctl_struct)))
        return IOCTL_FAILURE;

      smbios_table_entry_point_t *smbios_entry_point = phys_to_virt(ioctl_struct.address);
      if (!smbios_entry_point)
        return IOCTL_FAILURE;

      void *smbios_table_address = phys_to_virt(smbios_entry_point->TableAddress);
      if (!smbios_table_address)
        return IOCTL_FAILURE;

      memcpy(&ioctl_struct.address, &(uintptr_t){(uintptr_t)smbios_table_address}, sizeof(uintptr_t));
      
      return ((copy_to_user((ioctl_struct_t *)argument, &ioctl_struct, 
          sizeof(ioctl_struct))) ? IOCTL_FAILURE : IOCTL_SUCCESS);
  }

  return IOCTL_SUCCESS;
}

/**
 * @brief The entry point for the vmtrace driver IOCTL call.
 * @param dev The inode.
 * @param file A structure representing an open file.
 * @returns KERN_SUCCESS if the program executed successfully, KERN_FAILURE if otherwise.
 */

int driver_ioctl_init(struct inode *dev, struct file *file) {
  printk(KERN_INFO "x0 driver driver_ioctl_init() called!\n");

  return KERN_SUCCESS;
}

/**
 * @brief The de-initialization function for the vmtrace driver IOCTL call.
 * @param dev The inode.
 * @param file A structure representing an open file.
 * @returns KERN_SUCCESS if the program executed successfully, KERN_FAILURE if otherwise.
 */

int driver_ioctl_exit(struct inode *dev, struct file *file) {
  printk(KERN_INFO "x0 driver driver_ioctl_exit() called!\n");

  return KERN_SUCCESS;
}