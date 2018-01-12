#!/bin/bash

RR_BIN=main-rr.bin

TELNET_ADDR=127.0.0.1
TELNET_PORT=12345

if pgrep -f qemu-system-aarch64 > /dev/null; then
    pkill -KILL qemu
fi

qemu-system-aarch64 \
    -M virt \
    -cpu cortex-a57 \
    -m 1G \
    -nographic \
    -kernel $RR_BIN \
    -monitor telnet:$TELNET_ADDR:$TELNET_PORT,server,nowait

    # -gdb tcp::23456 -S \
    # -curses \
    # -serial stdio \
