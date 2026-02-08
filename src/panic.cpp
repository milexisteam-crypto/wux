extern "C" void kernelpanic(const char* msg);
extern "C" void isr_panic_stub();
extern "C" void kernelpanic(const char* msg) {
    asm volatile("cli");

    // tu możesz użyć swojego draw_string, framebuffer itd.
    // np:
    // draw_string(fb, hdr, glyphs, 50, 50, msg, kolor);

    for (;;)
        asm volatile("hlt");
}
asm(
    ".global isr_panic_stub\n"
    "isr_panic_stub:\n"
    "    cli\n"
    "    lea panic_msg(%rip), %rdi\n"   // RDI = const char* msg
    "    call kernelpanic\n"
    "1:  hlt\n"
    "    jmp 1b\n"
    "panic_msg:\n"
    "    .asciz \"KERNEL PANIC: unhandled exception\"\n"
);
