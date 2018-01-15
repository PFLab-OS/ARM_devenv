CROSS_PREFIX = $(HOME)/GCC-Linaro/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-elf/bin/aarch64-elf-
CFLAGS = -Wall -Wextra -Wconversion -O0 -ggdb -Wno-builtin-declaration-mismatch

QEMU = qemu-system-aarch64 -M virt -cpu cortex-a53 -nographic

.PHONY: qemu qemu-gdb gdb clean

qemu: elf_loader.bin
	$(QEMU) -kernel $<

qemu-gdb: elf_loader.bin
	$(QEMU) -kernel $< -monitor stdio -serial none
	# -gdb tcp::23456 -S

gdb:
	$(CROSS_PREFIX)gdb -x ./gdb.scr

%.bin: %.out
	$(CROSS_PREFIX)objcopy -O binary $< $@

elf_loader.out: main.tmp elf_loader.o uart.o startup.o kernel.ld
	$(CROSS_PREFIX)ld -Tkernel.ld $+ -o $@
	rm $<

main.tmp: main.o test.out
	$(CROSS_PREFIX)objcopy --add-section=elf_exec=test.out \
		--set-section-flags=elf_exec=contents,alloc,load,code $< $@

%.o: %.c
	$(CROSS_PREFIX)gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CROSS_PREFIX)as -c $< -o $@

test.out: test.c
	$(CROSS_PREFIX)gcc $(CFLAGS) -nostartfiles -fPIC -static $< -o $@

clean:
	rm -f *.bin *.out *.o
