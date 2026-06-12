OUTPUT_FORMAT(elf64-x86-64)

ENTRY(init_main)

PHDRS
{
	limine_requests PT_LOAD;
	text PT_LOAD;
	rodata PT_LOAD;
	data PT_LOAD;
}

SECTIONS
{
	. = 0xffffffff80000000;
	_kernel_start = .;
	.limine_requests : {
		KEEP(*(.limine_requests_start))
		KEEP(*(.limine_requests))
		KEEP(*(.limine_requests_end))
	} :limine_requests

	. = ALIGN(CONSTANT(MAXPAGESIZE));

	.text : {
		_text_start = .;
		*(.text.entry)
		*(.text .text.*)
		_text_end = .;
	} :text

	. = ALIGN(CONSTANT(MAXPAGESIZE));

	.rodata : {
		_rodata_start = .;
		*(.rodata .rodata.*)
		_rodata_end = .;
	} :rodata

	.note.gnu.build-id : {
		*(.note.gnu.build-id)
	} :rodata

	. = ALIGN(CONSTANT(MAXPAGESIZE));

	.data : {
		_data_start = .;
		*(.data .data.*)
	} :data

	.bss : {
		*(.bss .bss.*)
		*(COMMON)
		_data_end = .;
	} :data

	/DISCARD/ : {
		*(.eh_frame*)
		*(.note .note.*)
	}
	_kernel_end = .;
}