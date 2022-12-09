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

#ifndef __IOCTL_H
#define __IOCTL_H

#include <asm/io.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/uaccess.h>
#include "../shared/ioctl_codes.h"


/*
    *    src/driver/ioctl.h
    *    Date: 09/28/22
    *    Author: @xmmword
*/


int driver_ioctl_init(struct inode *dev, struct file *file);
int driver_ioctl_exit(struct inode *dev, struct file *file);
long int driver_ioctl_handler(struct file *file, const unsigned int command, const unsigned long int argument);

#endif