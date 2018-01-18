#pragma once

#include "elfhead.h"

typedef struct {
    const uint8_t* header;
    const Elf64_Ehdr* elf64_header;
    Elf64_Xword mem_size;
    Elf64_Xword align;
} Elf64Exec;

#define ELF_MEMBUF_PTR 0x40200000

int elf64_parse(const void* elf_ptr, Elf64Exec* elf64_exec);
int elf64_load(const Elf64Exec* elf64_exec);
