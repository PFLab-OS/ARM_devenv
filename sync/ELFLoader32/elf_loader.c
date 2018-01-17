#include "elf_loader.h"
#include "string.h"
#include "uart.h"

static inline int is_elf(const void* e_hdr);
static inline int is_elf64(const void* e_hdr);

static inline int is_abi_sysv(const void* e_hdr);
static inline int is_abi_gnu(const void* e_hdr);

int parse_elf32(const void* const elf_ptr, Elf32Exec* elf32_exec)
{
    const uint8_t* hdr = (const uint8_t*)elf_ptr;
    const Elf32_Ehdr* e_hdr = (const Elf32_Ehdr*)elf_ptr;

    // Validate ELF header
    if (!is_elf(e_hdr) || is_elf64(e_hdr)
        || (!is_abi_sysv(e_hdr) && !is_abi_gnu(e_hdr)))
        return -1;

    if (e_hdr->e_type != ET_EXEC)
        return -1;

    // Segments
    Elf32_Xword mem_size = 0;
    Elf32_Xword align = sizeof(void*);

    for (int i = 0; i < e_hdr->e_phnum; i++) {
        const Elf32_Phdr* prog_hdr
            = (const Elf32_Phdr*)(hdr + e_hdr->e_phoff + e_hdr->e_phentsize * i);

        Elf32_Xword ms = prog_hdr->p_vaddr + prog_hdr->p_memsz;
        if (mem_size < ms)
            mem_size = ms;
        if (align < prog_hdr->p_align)
            align = prog_hdr->p_align;
    }

    elf32_exec->header = hdr;
    elf32_exec->elf32_header = e_hdr;
    elf32_exec->mem_size = mem_size;
    elf32_exec->align = align;

    return 0;
}

int load_elf32(const Elf32Exec* elf32_exec)
{
    const uint8_t* hdr = elf32_exec->header;
    const Elf32_Ehdr* e_hdr = elf32_exec->elf32_header;

    void* mem_buf = (void*)ELF_MEMBUF_PTR;

    for (int i = 0; i < e_hdr->e_phnum; i++) {
        const Elf32_Phdr* prog_hdr
            = (const Elf32_Phdr*)(hdr + e_hdr->e_phoff + e_hdr->e_phentsize * i);

        switch (prog_hdr->p_type) {
        case PT_LOAD:
            memcpy(mem_buf + prog_hdr->p_vaddr,
                &hdr[prog_hdr->p_offset],
                prog_hdr->p_memsz);
            memset(mem_buf + prog_hdr->p_vaddr + prog_hdr->p_filesz,
                0,
                prog_hdr->p_memsz - prog_hdr->p_filesz);
            break;
        }
    }

    // Clear bss section
    for (int i = 0; i < e_hdr->e_shnum; i++) {
        const Elf32_Shdr* sct_hdr
            = (const Elf32_Shdr*)(hdr
                                  + e_hdr->e_shoff
                                  + e_hdr->e_shentsize * i);
        if (sct_hdr->sh_type == SHT_NOBITS) {
            if ((sct_hdr->sh_flags & SHF_ALLOC) != 0)
                memset(mem_buf + sct_hdr->sh_addr, 0, sct_hdr->sh_size);
        }
    }

	for (volatile i = 0; i < (1 << 28); i++)
		;
    print_str("Executing...\n");
    int (*entry_point)() = mem_buf + e_hdr->e_entry;
    int rv = entry_point();
    print_str("Done. Return value: ");
    print_int(rv);
    print_char('\n');

    return 0;
}

#define EQ_ELF_IDENT(idx, val) e->e_ident[(idx)] == (val)

static inline int is_elf(const void* e_hdr)
{
    const Elf32_Ehdr* e = (const Elf32_Ehdr*)e_hdr;
    return EQ_ELF_IDENT(0, ELFMAG0)
           && EQ_ELF_IDENT(1, ELFMAG1)
           && EQ_ELF_IDENT(2, ELFMAG2)
           && EQ_ELF_IDENT(3, ELFMAG3);
}

static inline int is_elf64(const void* e_hdr)
{
    const Elf32_Ehdr* e = (const Elf32_Ehdr*)e_hdr;
    return EQ_ELF_IDENT(EI_CLASS, ELFCLASS64);
}

static inline int is_abi_sysv(const void* e_hdr)
{
    const Elf32_Ehdr* e = (const Elf32_Ehdr*)e_hdr;
    return EQ_ELF_IDENT(EI_OSABI, ELFOSABI_SYSV);
}

static inline int is_abi_gnu(const void* e_hdr)
{
    const Elf32_Ehdr* e = (const Elf32_Ehdr*)e_hdr;
    return EQ_ELF_IDENT(EI_OSABI, ELFOSABI_GNU);
}
