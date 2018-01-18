#include "elf_loader.h"
#include "uart.h"

extern char __start_elf_exec;
extern char __stop_elf_exec;

void c_entry(void)
{
    const void* elf_ptr = &__start_elf_exec;
    Elf32Exec elf32_exec;

    if (elf32_parse(elf_ptr, &elf32_exec) == -1) {
        print_str("Invalid file\n");
        return;
    }

    if (elf32_load(&elf32_exec) == -1) {
        print_str("Faile to load ELF\n");
        return;
    }
}
