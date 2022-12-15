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

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include "ioctl.h"
#include "kstatus.h"

MODULE_LICENSE("GPL"); /* Kernel driver license. */


/*
    *    src/driver/main.c
    *    Date: 12/14/22
    *    Author: @xmmword
*/


extern const struct file_operations operations; /* Structure containing initialization/de-initialization functions and the IOCTL handler. */

/**
 * @brief Entry point for x0 companion kernel driver.
 * @returns KERN_SUCCESS on success, KERN_FAILURE on failure.
 */

static __init int driver_entry(void) {
  printk(KERN_INFO "Initializing the x0client driver\n");

  register_chrdev(X0_DRIVER_NUMBER, X0_DRIVER, &operations);

  return KERN_SUCCESS;
}

/**
 * @brief Exit point for dpatch.
 */

static __exit void driver_exit(void) {
  printk(KERN_INFO "Exiting the x0client driver\n");
}

module_init(driver_entry); /* Kernel driver entry point. */
module_exit(driver_exit);  /* Kernel driver exit point. */