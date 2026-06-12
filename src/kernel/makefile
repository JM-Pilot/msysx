OUTPUT = 
BIN_DIR =
ARCH = 
CC = x86_64-elf-gcc
ASM = nasm 

FONT_SRC = misc/fonts/ter-u16n.psfu
FONT_OBJ = $(BIN_DIR)/kernel/misc/default_font.o

C_SRCS = $(shell find . -iname "*.c")
C_OBJS = $(patsubst %.c, $(BIN_DIR)/kernel/%.c.o, $(C_SRCS))

ASM_SRCS = $(shell find . -iname "*.asm")
ASM_OBJS = $(patsubst %.asm, $(BIN_DIR)/kernel/%.asm.o, $(ASM_SRCS))

OBJS = $(C_OBJS) $(ASM_OBJS) $(FONT_OBJ)

C_FLAGS = \
	-Wall \
	-Wextra \
	-std=gnu11 \
	-ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fno-PIC \
	-ffunction-sections \
	-fdata-sections \
	-m64 \
	-march=x86-64 \
	-mabi=sysv \
	-mno-80387 \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-mcmodel=kernel \
	-I include \
	-I arch/$(ARCH)/include \
	-I include \
	-I include/lib \
	-I drivers/include \
	-I drivers/apci/uacpi/include \
	-I ../boot/

LD_FLAGS = \
	-nostdlib \
	-ffreestanding \
	-T arch/x86_64/boot/linker.ld

all: $(BIN_DIR)/$(OUTPUT)_kernel.elf

$(FONT_OBJ): $(FONT_SRC)
	mkdir -p $(dir $@)
	ln -sf $(notdir $<) misc/fonts/default.psfu
	objcopy -O elf64-x86-64 -B i386 -I binary \
		misc/fonts/default.psfu $@ 
	rm -f misc/fonts/default.psfu
	readelf -sW $@

$(BIN_DIR)/kernel/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< -o $@

$(BIN_DIR)/kernel/%.asm.o: %.asm
	mkdir -p $(dir $@)
	$(ASM) -felf64 $< -o $@

$(BIN_DIR)/$(OUTPUT)_kernel.elf: $(OBJS)
	$(CC) $(LD_FLAGS) $^ -o $@
