bits 64

global idt_load
global isr_timer_stub

section .text

idt_load:
    lidt [rdi]
    ret

