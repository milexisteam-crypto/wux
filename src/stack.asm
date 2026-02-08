bits 64

global kernel_stack_top

section .bss
align 16
kernel_stack:
    resb 8192        ; 8 KB stack

kernel_stack_top: