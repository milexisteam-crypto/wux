bits 64

global _start
extern wuxkrn
extern bss_start
extern bss_end

section .text

_start:
    cli

    ; 1. Włącz FPU/SSE
    mov     rax, cr0
    and     rax, ~(1 << 2)        ; CR0.EM = 0
    or      rax,  (1 << 1)        ; CR0.MP = 1
    mov     cr0, rax

    mov     rax, cr4
    or      rax,  (1 << 9)        ; CR4.OSFXSR = 1
    or      rax,  (1 << 10)       ; CR4.OSXMMEXCPT = 1
    mov     cr4, rax

    ; 2. Wyczyść .bss (prosto, bez SSE)
    mov     rdi, bss_start
    mov     rcx, bss_end
    sub     rcx, rdi
    xor     rax, rax

.clear_bss:
    test    rcx, rcx
    jz      .bss_done
    mov     byte [rdi], al
    inc     rdi
    dec     rcx
    jmp     .clear_bss

.bss_done:

    ; 3. Skok do kernela w C++
    call    wuxkrn

.hang:
    hlt
    jmp .hang