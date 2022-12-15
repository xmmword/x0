# x0
An x64 bootkit, usermode client, and kernel driver that performs hardware spoofing and virtualbox hardening.

<div align="center">
    <img src=https://user-images.githubusercontent.com/105472509/207756908-b8fd715f-268a-4e44-8513-df881cc90d39.png width="750px"><br>
</div>

## Description
`x0` is a hardware spoofing and virtualbox hardening solution that consists of 4 main components:
- Kernel Driver
- Bootkit Loader (UEFI Application)
- UEFI Bootkit (UEFI Runtime Driver)
- Userspace Command-line Companion Program

Only 2 of these components are crucial for `x0` to work properly (UEFI bootkit and bootkitloader), the kernel driver is optional as it just provides extra features for the userspace client. The main purpose for the userspace client is for debugging the states of UEFI data structures post OS-boot, and other things.

On execution, the bootkit loader does 3 main things:
- Validate the UEFI driver (Check for invalid signatures, etc)
- Map the UEFI driver into memory and execute it
- Chainload BOOTX64

When the bootkit gets loaded, it performs of series of tasks depending on how it's configured:
- Hooking `GetVariable` and `SetVariable` via swapping pointers in the system table, and then updating the CRC32 checksum
- Creating an event for `SetVirtualAddressMap` which calls `gRT->ConvertPointer(...)` to convert the pointer of the original `GetVariable` and `SetVariable`
- Scans the configuration tables in the system table    (Depending on configuration)
- If the table gets found, then the bootkit will patch it (Depending on configuration)

At the end of all of this, `BOOTX64` will be chainloaded so that we can boot into the OS.

### Bootkit Features
- Patching SMBIOS tables
- Spoofing SecureBoot status
- Modification of UEFI variables containing data that could be used to detect a VirtualBox instance

### Usermode Program Features
- Locating the address of the UEFI runtime service table
- Locating the address of the SMBIOS table in kernel virtual memory
- Dumping and parsing the contents of the UEFI runtime service table

### Chart
![image](https://user-images.githubusercontent.com/105472509/205988550-80a6d34f-fdde-4a12-aa56-af762e9e353e.png)

### Built with
- C

## Getting started
### Compiling
First, take a look at this tutorial from OSdev on how to setup EDK2: https://wiki.osdev.org/EDK2

Once you're done with that, make sure that you `git clone` this repository within the EDK2 directory, and add
the paths to the `.inf` files in the `src` directory of the `x0` folder into the `[Components]` section in `MdeModulePkg/MdeModulePkg.dsc`

Once EDK2 is properly setup, you can compile `x0` by executing the following script:
- `./build.sh`

### UEFI Bootkit Installation
In order to install the bootkit (and load it), follow these instructions:
- Copy the compiled bootkit and bootkit loader (`x0.efi` and `x0loader.efi`) into the EFI System Partition (AKA `/boot/efi/EFI`)
- Execute `systemctl --firmware-setup reboot`
- Select the boot manager option and then select the 'EFI Shell' option
- Once booted in the EFI shell, enter the `fs0` mapping by running `fs0:`
- Execute the following commands:
    * `cd EFI`
    * `x0loader.efi`
- The bootkit loader will load the bootkit and then boot into the OS

### Usage
- `./x0client [-h, --help]`

## Credits
```
https://github.com/xmmword
```
### Contributions 🎉
###### All contributions are accepted, simply open an Issue / Pull request.
