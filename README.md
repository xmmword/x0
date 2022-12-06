# x0
An x64 bootkit and usermode companion program that performs hardware spoofing and virtualbox hardening.

<div align="center">
    <img src="https://user-images.githubusercontent.com/105472509/195727127-49de8c41-5af5-4b9a-af33-028735e21c98.PNG" width="700px"><br>
</div>

## Description
`x0` is a hardware spoofing and virtualbox hardening solution that consists of 3 main components:
- UEFI Bootkit (UEFI Runtime Driver)
- Kernel Driver
- Userspace Command-line Companion Program

Only 2 of these components are crucial for `x0` to work properly, the kernel driver is optional as it just provides extra features for the userspace program.
The main purpose of userspace program is to:
- Overwrite the native bootloader of the OS (usually GRUB) with the code of the bootkit loader
- Bootkit gets copied to the ESP (EFI System Partition)
- Invoke a reboot
- Bootkit loader gets executed

On execution, the bootkit loader does 3 main things:
- Validate the UEFI driver (Check for invalid signatures, etc)
- Map the UEFI driver into memory and execute it
- Chainload BOOTX64

When the bootkit gets loaded, it performs of series of tasks depending on how it's configured:
- Hooking `GetVariable` via swapping pointers in the system table, and then updating the CRC32 checksum
- Creating an event for `SetVirtualAddressMap` which calls `gRT->ConvertPointer(...)` to convert the pointer of the original `GetVariable`
- Scans through memory to find the SMBIOS tables    (Depending on configuration)
- If the table gets found, then it will be modified (Depending on configuration)

At the end of all of this, `BOOTX64` will be chainloaded so that we can boot into the OS despite GRUB being overwritten earlier. However, the usermode program has other usecases, aswell as a few debugging features.

### Bootkit Features
- Modification of SMBIOS tables
- Modification of UEFI variables containing data that could be used to detect a VirtualBox instance

### Usermode Program Features
- Locating the address of the UEFI runtime service table
- Dumping and parsing the contents of the UEFI runtime service table
- Performing the installation of the UEFI bootkit and bootkit loader
- Providing a bootkit-less hardware spoofing solution that utilizes a kernel driver for modifying SMBIOS tables.

### Chart
![image](https://user-images.githubusercontent.com/105472509/205988550-80a6d34f-fdde-4a12-aa56-af762e9e353e.png)

### Built with
- C

## Getting started
### Compiling
To compile `x0`, simply execute the following script:
- `./build.sh`

### Usage
- `./x0client [-h, --help]`

## Credits
```
https://github.com/xmmword
```
### Contributions 🎉
###### All contributions are accepted, simply open an Issue / Pull request.
