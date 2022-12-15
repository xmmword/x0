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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "opts.h"
#include "ioctl.h"
#include "utils.h"
#include "mapper.h"
#include "status.h"


/*
    *    src/main.c
    *    Date: 12/14/22
    *    Author: @xmmword
*/


/* Global function declarations. */
static client_status_t print_help(void);
static client_status_t handle_args(char **argv, const int argc);

/* Command-line options. */
static const flag_t opt_table[OPTAMOUNT_MAX] = {
  {"-h", "--help", "\tDisplays all the available flags", print_help},
  {"-m", "--map-driver", "Maps the x0 driver onto kernel memory (more options will be use-able)", map_kernel_driver},
  {"-u", "--unmap-driver", "Unmaps the x0 driver from kernel memory", remove_kernel_driver},
  {"-a", "--address-rt", "Dump the address of the UEFI Runtime Service Table", print_rtable_address},
  {"-s", "--address-smbios", "Dump the address of the SMBIOS Table containing type0, type1, type2 tables etc", return_smbios_virtaddr},
  {"-d", "--dump-rt-table", "Dump UEFI Runtime Service Table data", dump_runtime_service_table}
};

/**
 * @brief Prints all the command-line options.
 * @returns CLIENT_STATUS_SUCCESS.
 */

static client_status_t print_help(void) {
  printf("General Options:\n");

  for (int i = 0; i < OPTAMOUNT_MAX; ++i)
    printf("\t%s, %s\t%s\n", opt_table[i].short_flag, opt_table[i].long_flag, opt_table[i].description);

  putchar('\n');
  return CLIENT_STATUS_SUCCESS;
}

/**
 * @brief Handles the command-line arguments.
 * @param argv The argument vector.
 * @param argc The argument count.
 * @returns CLIENT_STATUS_SUCCESS if the program executed successfully, CLIENT_STATUS_FAILURE if otherwise.
 */

static client_status_t handle_args(char **argv, const int argc) {
  for (int i = 0; i < OPTAMOUNT_MAX; ++i)
    
    if (!strncmp(opt_table[i].short_flag, argv[1], strlen(opt_table[i].short_flag))
      || !strncmp(opt_table[i].long_flag, argv[1], strlen(opt_table[i].long_flag)))
    return opt_table[i].function();

  return CLIENT_STATUS_FAILURE;
}

/**
 * @brief Entry point for the x0client program.
 * @param argc The argument count.
 * @param argv The argument vector.
 * @returns EXIT_SUCCESS if the program executed successfully, EXIT_FAILURE if otherwise.
 */

int main(int argc, char **argv) {
  fprintf(stderr, "Usage: %s [-h, --help]\n\n", *argv);

  if (argc < 2)
    return EXIT_FAILURE;

  return ((handle_args(argv, argc) == CLIENT_STATUS_SUCCESS)  ? EXIT_SUCCESS : EXIT_FAILURE);
}
