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
    *    src/mapper.c
    *    Date: 07/17/22
    *    Author: @xmmword
*/


/**
 * @brief Handles a given loader error code.
 * @param err The loader error.
 * @returns A string containing information related to the error status.
 */

const char* restrict handle_loader_err(const loader_status_t err) {
  switch (err) {
    case KERN_LOAD_FAILURE:
      return "Failed to load driver, is the x0 driver in the same directory as the x0 client binary?\n";
    case KERN_LOAD_INSTANCE_ERR: /* The chances of getting this error are slim, would likely be attributed to a mistake when compiling the driver. */
      return "Another instance of the x0 driver has been detected, could not load!\n";
    case KERN_UNLOAD_FAILURE:
      return "Failed to unload the x0 driver from kernel memory!";
    case KERN_DEVICE_INTERFACE_FAILURE:
      return "Failed to create a driver interface for the x0 driver!";
    default:
      return NULL;
  }
}

/**
 * @brief Creates a driver interface.
 * @returns A status code indicating whether the driver interface could be created or not.
 */

static inline __always_inline loader_status_t create_driver_interface(void) {
  return (
    (mknodat(AT_FDCWD, X0_DEVICE_INTERFACE, (S_IFCHR | 0666),
      makedev(X0_DRIVER_NUMBER, 0)) == -1) ? KERN_DEVICE_INTERFACE_FAILURE : KERN_DEVICE_INTERFACE_SUCCESS
  );
}

/**
 * @brief Removes a kernel module.
 * @param driver The name of the kernel module that will be removed.
 * @returns True if the module was able to be removed, false if otherwise.
 */

loader_status_t remove_kernel_driver(const char* restrict driver) {
  return (
    (!syscall(__NR_delete_module, driver, O_NONBLOCK)) ? KERN_UNLOAD_SUCCESS : KERN_UNLOAD_FAILURE
  );
}

/**
 * @brief Reads the data in the ELF file.
 * @param descriptor The file descriptor.
 * @returns A pointer to the read data.
 */

static void *return_elf_object(const char* restrict path) {
  void *object = NULL;

  int descriptor = open(path, O_RDONLY);
  if (descriptor == -1)
    return NULL;

  if (!(object = mmap(NULL, return_file_size(path), PROT_READ, MAP_PRIVATE, open(path, O_RDONLY), 0))) {
    close(descriptor);
    
    return NULL;
  }

  close(descriptor);
  return object;
}

/**
 * @brief Parses the information in the given entry.
 * @param driver The struct where the parsed information will be stored in.
 * @param string The string.
 * @returns A struct containing the parsed module information, NULL if otherwise.
 */

static inline __always_inline bool parse_kernel_driver(kdriver_t *driver, const char* restrict string) {
  return (
    (!sscanf(string, "%s %lx %d %s %s %lx", driver->kmodule, &driver->size, &driver->instances, 
      driver->dependencies, driver->state, &driver->offset)
  ) ? false : true);
}

/**
 * @brief Returns the parsed kernel driver data.
 * @param driver The driver name.
 * @returns A structure containing the kernel driver data if the kernel driver was found, NULL if otherwise.
 */

kdriver_t *return_driver_object(const char* restrict driver) {
  static kdriver_t kdriver = {0};

  char buffer[BUFSIZ] = {0};
  FILE *descriptor = fopen("/proc/modules", "r");
  
  if (!descriptor)
    return NULL;
  
  while (fgets(buffer, sizeof(buffer), descriptor)) {
    if (!(parse_kernel_driver(&kdriver, buffer)))
      continue;

    if (!strncmp(kdriver.kmodule, driver, strlen(kdriver.kmodule))) {
      fclose(descriptor);
      
      return &kdriver;
    }
  }

  fclose(descriptor);
  return NULL;
}

/**
 * @brief Inserts a kernel module via invoking the finite_module system call.
 * @param driver The driver path.
 * @returns True if the module was able to be inserted, false if otherwise.
 */

static bool invoke_finit_module(const char* restrict driver) {
  int descriptor = open(driver, O_RDONLY);
  if (descriptor == -1)
    return false;

  if (!driver && syscall(__NR_finit_module, descriptor, NO_DRIVER_ARG, 0) != 0)
    return false;

  if (syscall(__NR_finit_module, descriptor, driver, 0) != 0)
    return false;

  close(descriptor);
  return true;
}

/**
 * @brief Inserts a kernel module via invoking the init_module system call.
 * @param driver The driver path.
 * @returns True if the module was able to be inserted, false if otherwise.
 */

loader_status_t insert_kernel_driver(const char* restrict driver) {
  void *module = return_elf_object(driver);
  if (!module)
    return KERN_LOAD_FAILURE;

  if (syscall(__NR_init_module, module, return_file_size(driver), NO_DRIVER_ARG) != 0) {
    if (!access(driver, F_OK))
      return KERN_LOAD_INSTANCE_ERR; /* Another instance of the x0 driver is running. */

    munmap(module, return_file_size(driver));
    return invoke_finit_module(driver); /* Safeguard just in-case ELF module loading fails. */
  }

  munmap(module, return_file_size(driver));
  return KERN_LOAD_SUCCESS;
}

/**
 * @brief Attempts to load the x0 driver if it's not loaded already, check the x0 driver interface, and ensure the backend is running correctly.
 * @returns KERN_LOAD_SUCCESS if the x0 driver and important services were successfully initalized, KERN_LOAD_FAILURE if otherwise.
 */

loader_status_t x0_loader_init(void) {
  kdriver_t *driver = return_driver_object(X0_DRIVER);

  if (!driver) {
    const loader_status_t status = insert_kernel_driver(X0_DRIVER_FILENAME);

    if (status < 0) {
      printf("%s\n", handle_loader_err(status));

      return KERN_LOAD_FAILURE;
    }

    printf(
      "x0 driver loaded at 0x%lx [Status: %s]\n", 
      (driver = return_driver_object(X0_DRIVER))->offset, 
      ((driver = return_driver_object(X0_DRIVER))->state)
    );
  }

  if (access(X0_DEVICE_INTERFACE, F_OK) == -1) {
    const loader_status_t status = create_driver_interface();

    if (status < 0) {
      printf("%s\n", handle_loader_err(status));

      return KERN_LOAD_FAILURE;
    }
  }

  return KERN_LOAD_SUCCESS;
}