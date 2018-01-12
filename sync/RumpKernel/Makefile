SRCS = main.c

PRODUCT = main

CFLAGS = -O3 -DNDEBUG

RR_CC = arm-rumprun-netbsdelf-eabihf-gcc
RR_OUT = $(PRODUCT)-rr.out

BAKERY = rumprun-bake hw_generic
RR_BIN = $(PRODUCT)-rr.bin

RR_IMG = $(PRODUCT)-rr.img
MOUNT_DIR = /mnt
MOUNT_DIR_RUMP = $(MOUNT_DIR)/rumpkernel
MOUNT_DIR_EFI = $(MOUNT_DIR)/efi

HOST_OUT = $(PRODUCT)-host.out

.PHONY: all qemu help clean

all: $(RR_IMG)

$(RR_OUT): $(SRCS)
	$(RR_CC) $(CFLAGS) $(SRCS) -o $(RR_OUT)

$(HOST_OUT): $(SRCS)
	$(CC) $(CFLAGS) -static $(SRCS) -o $(HOST_OUT)

$(RR_BIN): $(RR_OUT)
	$(BAKERY) $(RR_BIN) $(RR_OUT)

.PHONY: kpartx mount umount

$(RR_IMG): $(RR_BIN) grub.cfg
	$(MAKE) umount

	# Create new disk image
	dd if=/dev/zero of=$(RR_IMG) bs=1M count=200
	sgdisk -g -o -n 1::100M -t 1:EF00 -n 2:: -t 2:8300 $(RR_IMG)

	# Mount
	mkdir -p $(MOUNT_DIR_EFI) $(MOUNT_DIR_RUMP)
	$(MAKE) kpartx
	sudo mkfs.vfat -F32 /dev/dm-2
	sudo mount -t vfat /dev/dm-2 $(MOUNT_DIR_EFI)
	sudo mkfs.ext2 /dev/dm-3
	sudo mount -t ext2 /dev/dm-3 $(MOUNT_DIR_RUMP)

	## EFI/boot/grubaa64.efi
	sudo mkdir -p $(MOUNT_DIR_EFI)/EFI/boot
	sudo cp grubaa64.efi $(MOUNT_DIR_EFI)/EFI/boot/grubaa64.efi

	## grub.cfg
	sudo mkdir -p $(MOUNT_DIR_RUMP)/boot/grub
	sudo cp grub.cfg $(MOUNT_DIR_RUMP)/boot/grub/grub.cfg

	# Rump Kernel ELF
	sudo cp $(RR_BIN) $(MOUNT_DIR_RUMP)/boot/$(RR_BIN)

	$(MAKE) umount

mount: $(RR_IMG)
	$(MAKE) umount
	$(MAKE) kpartx
	sudo mount -t vfat /dev/dm-2 $(MOUNT_DIR_EFI)
	sudo mount -t ext2 /dev/dm-3 $(MOUNT_DIR_RUMP)

kpartx:
	$(if $(shell if [ -n "$(find /dev/mapper -name "loop*")" ]; then echo "Other loop device exist"; fi), \
		@echo "Error: Other loop device exist"; exit 1, \
	)
	sudo kpartx -av $(RR_IMG)

umount:
	$(if $(shell mountpoint -q $(MOUNT_DIR_RUMP); if [ $$? = 0 ]; then echo "mounted"; fi), sudo umount $(MOUNT_DIR_RUMP))
	$(if $(shell mountpoint -q $(MOUNT_DIR_EFI); if [ $$? = 0 ]; then echo "mounted"; fi), sudo umount $(MOUNT_DIR_EFI))
	sudo kpartx -d $(RR_IMG) || return 0
	sudo losetup -d /dev/loop[0-9] > /dev/null 2>&1 || return 0

qemu: $(RR_IMG)
	bash qemu.sh

help:
	@grep -E "^[a-Z-]+:" Makefile

clean:
	rm -rf $(RR_OUT) $(HOST_OUT) $(RR_BIN) $(RR_IMG)
