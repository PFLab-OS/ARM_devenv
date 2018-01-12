#!/bin/bash

RR_IMG=main-rr.img

OVMF_DIR="$HOME/edk2-UDK2017"
QEMU_UEFI_OPTION="-drive if=pflash,readonly,file=$OVMF_DIR/OVMF_CODE.fd,format=raw -drive if=pflash,file=$OVMF_DIR/OVMF_VARS.fd,format=raw"

TELNET_ADDR=127.0.0.1
TELNET_PORT=12345

GDB_PORT=23456

if pgrep -f qemu-system-aarch64 > /dev/null; then
	pkill -f qemu-system-aarch64
fi

QEMU_OPTION=""
# QEMU_OPTION="$QEMU_OPTION -cpu qemu64 -machine q35"
QEMU_OPTION="$QEMU_OPTION -m 1G"
QEMU_OPTION="$QEMU_OPTION -nographic"
QEMU_OPTION="$QEMU_OPTION -curses"
# QEMU_OPTION="$QEMU_OPTION -serial stdio"
QEMU_OPTION="$QEMU_OPTION -monitor telnet:$TELNET_ADDR:$TELNET_PORT,server,nowait"
# QEMU_OPTION="$QEMU_OPTION -gdb tcp::$GDB_PORT -S"

echo "QEMU_OPTION=$QEMU_OPTION"
qemu-system-aarch64 $QEMU_UEFI_OPTION $QEMU_OPTION -drive format=raw,file=$RR_IMG
sleep 0.2
telnet $TELNET_ADDR $TELNET_PORT

stty sane
