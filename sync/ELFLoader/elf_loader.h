#pragma once

#include "elfhead.h"

typedef struct {
    const uint8_t* header;
    const Elf64_Ehdr* elf64_header;
    Elf64_Xword mem_size;
    Elf64_Xword align;
} Elf64Exec;

#define ELF_MEMBUF_PTR 0x40200000

int parse_elf64(const void* elf_ptr, Elf64Exec* elf64_exec);
int load_elf64(const Elf64Exec* elf64_exec);
