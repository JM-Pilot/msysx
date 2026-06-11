OUTPUT = msysx
ARCH = x86_64
BIN_DIR = bin
SRC_DIR = src
TOOLS_DIR = tools

QEMU_FLAGS = -d int,cpu_reset -D $(BIN_DIR)/QEMU_LOGS.txt \
		-no-reboot -no-shutdown -m 256M \
		-serial stdio
all: $(SRC_DIR)/boot/autoconf.h $(BIN_DIR)/$(OUTPUT).iso 
$(BIN_DIR)/$(OUTPUT)_kernel.elf:
	mkdir -p $(dir $@)
	$(MAKE) -C $(SRC_DIR)/kernel \
		BIN_DIR="$(PWD)/$(BIN_DIR)" OUTPUT="$(OUTPUT)" \
		ARCH="$(ARCH)"

$(TOOLS_DIR)/limine-binary:
	curl -L https://github.com/Limine-Bootloader/Limine/releases/latest/download/limine-binary.tar.gz | gunzip | tar -xf - \
	--one-top-level=$(dir $@)
	$(MAKE) -C $(TOOLS_DIR)/limine-binary

$(TOOLS_DIR)/edk2-ovmf-bins:
	curl -L https://github.com/osdev0/edk2-ovmf-stable-bins/releases/latest/download/edk2-ovmf-bins.tar.gz | gunzip | tar -xf - \
	--one-top-level=$(dir $@)

$(BIN_DIR)/$(OUTPUT).iso: $(BIN_DIR)/$(OUTPUT)_kernel.elf \
$(TOOLS_DIR)/edk2-ovmf-bins $(TOOLS_DIR)/limine-binary
	mkdir -p $(BIN_DIR)/iso_build/boot/limine
	cp $(BIN_DIR)/$(OUTPUT)_kernel.elf $(BIN_DIR)/iso_build/
	cp $(SRC_DIR)/boot/limine.conf $(BIN_DIR)/iso_build/boot/limine/
	cp $(TOOLS_DIR)/limine-binary/BOOTX64.EFI \
		$(BIN_DIR)/iso_build/boot/limine
	cp $(TOOLS_DIR)/limine-binary/*.bin $(BIN_DIR)/iso_build/boot/limine/
	cp $(TOOLS_DIR)/limine-binary/limine-bios.sys $(BIN_DIR)/iso_build/boot/limine/
	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
        	-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        	-apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
        	-efi-boot-part --efi-boot-image --protective-msdos-label \
       		$(BIN_DIR)/iso_build -o $@

	$(TOOLS_DIR)/limine-binary/limine bios-install $@

run-efi: $(SRC_DIR)/boot/autoconf.h $(BIN_DIR)/$(OUTPUT).iso \
	$(TOOLS_DIR)/edk2-ovmf-bins
	qemu-system-x86_64 -cdrom $(BIN_DIR)/$(OUTPUT).iso\
		-bios $(TOOLS_DIR)/edk2-ovmf-bins/ovmf-code-x86_64.fd \
		$(QEMU_FLAGS)
run: $(SRC_DIR)/boot/autoconf.h $(BIN_DIR)/$(OUTPUT).iso 
	qemu-system-x86_64 -cdrom $(BIN_DIR)/$(OUTPUT).iso \
		$(QEMU_FLAGS)
clean:
	rm -rf *.iso
	rm -rf $(BIN_DIR)
	rm -rf $(SRC_DIR)/boot/autoconf.h
	rm -rf .config*
clean-tools:
	rm -rf tools/*


menuconfig:
	menuconfig
 
$(SRC_DIR)/boot/autoconf.h: .config
	genconfig --header-path $@
 
.config:
	genconfig --header-path $(SRC_DIR)/boot/autoconf.h