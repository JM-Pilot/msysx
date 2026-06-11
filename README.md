# MSYSX

## A system built for compatibility with other unix like systems

### Information

- **Kernel:** msysx kernel
- **Boot Protocol:** [Limine](https://github.com/limine-bootloader/limine)
- **Architecture:** x86_64
- **Languages:** C, Assembly

### Building

Tools required for building

```sh
(ARCH) # select from the list of architectures supported
(ARCH)-elf-gcc
(ARCH)-binutils
xorriso
git
make
qemu-system-x86_64 
```

Running and Compiling:

```sh
make run       # Run it using bios with qemu
make run-efi   # Run it using uefi with qemu
make clean     # Clean compiled code
make clean-tools # Clean the tools directory
make menuconfig # Configure the system
```

### WARNINGS

```sh
/dev/ttyS0 IS NOT YET IMPLEMENTED DO NOT DISABLE THE "COM1" FEATURE AT THE
MENUCONFIG
```
