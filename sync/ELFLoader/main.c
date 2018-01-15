#include "elf_loader.h"
#include "uart.h"

extern char __start_elf_exec;
extern char __stop_elf_exec;

void c_entry(void)
{
    const void* elf_ptr = &__start_elf_exec;
    Elf64Exec elf64_exec;

    if (parse_elf64(elf_ptr, &elf64_exec) == -1) {
        print_str("Invalid file\n");
        return;
    }

    if (load_elf64(&elf64_exec) == -1) {
        print_str("Faile to load ELF\n");
        return;
    }
}
