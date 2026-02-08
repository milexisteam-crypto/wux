bits 64

global gdt_flush
global tss_load

; void gdt_flush(uint64_t* gdt_ptr);
gdt_flush:
    lgdt [rdi]

    mov ax, 0x10          ; kernel data selector
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    push 0x08             ; kernel code selector
    lea rax, [rel .next]
    push rax
    retfq

.next:
    ret

; void tss_load();
tss_load:
    mov ax, 0x38          ; TSS selector (index 7 << 3)
    ltr ax
    ret