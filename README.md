# x0
An x64 bootkit and usermode companion program that performs hardware-spoofing and virtualbox hardening.

<div align="center">
    <img src="https://user-images.githubusercontent.com/105472509/195727127-49de8c41-5af5-4b9a-af33-028735e21c98.PNG" width="700px"><br>
</div>

## Description
`dpatch` is a PoC kernel driver which patches the system call dispatcher for x64 Linux. It does this by first making a mutable/writeable copy of the 
system call table, overwriting the function pointers in that table with the function pointers that point to the hook functions, and then patching the first several bytes of the dispatcher to make it jump to a custom system call handler. The custom handler will then index and invoke system calls (or hooks, if the function pointer was overwritten) from the copied (writeable) table.

### Features
- Modification of SMBIOS tables
- Modification of UEFI variables containing data that could be used to detect a VirtualBox instance

### Demonstration
![uefibootkit](https://user-images.githubusercontent.com/105472509/205987751-baf93993-e184-4084-abcd-a9e923c4e013.PNG)

### Built with
- C

## Getting started
### Compiling
To compile `x0`, simply execute the following script:
- `./build.sh`

### Usage
- `insmod dpatchdriver.ko`

## Credits
```
https://github.com/xmmword
```
### Contributions 🎉
###### All contributions are accepted, simply open an Issue / Pull request.
