.global _start

_start:
    ldr sp, =stack_top
    bl c_entry
    b .
