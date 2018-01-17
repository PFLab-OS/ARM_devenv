#pragma once

#include "elfhead.h"

typedef struct {
    const uint8_t* header;
    const Elf32_Ehdr* elf32_header;
    Elf32_Xword mem_size;
    Elf32_Xword align;
} Elf32Exec;

#define ELF_MEMBUF_PTR 0x200000

int parse_elf32(const void* elf_ptr, Elf32Exec* elf32_exec);
int load_elf32(const Elf32Exec* elf32_exec);
