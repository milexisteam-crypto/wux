#include "gdt.hpp"


static GdtEntry gdt[9];   // 0..8 (7 i 8 = TSS)
static GdtPtr gdt_ptr;
static Tss tss;

static void gdt_set(int i, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[i].limit_low = limit & 0xFFFF;
    gdt[i].base_low  = base & 0xFFFF;
    gdt[i].base_mid  = (base >> 16) & 0xFF;
    gdt[i].access    = access;
    gdt[i].gran      = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[i].base_high = (base >> 24) & 0xFF;
}

static void gdt_set_tss(int i, uint64_t base, uint32_t limit) {
    // low 8 bytes
    gdt[i].limit_low = limit & 0xFFFF;
    gdt[i].base_low  = base & 0xFFFF;
    gdt[i].base_mid  = (base >> 16) & 0xFF;
    gdt[i].access    = 0x89; // available 64-bit TSS
    gdt[i].gran      = (limit >> 16) & 0x0F;
    gdt[i].base_high = (base >> 24) & 0xFF;

    // high 8 bytes
    uint32_t base_high = (base >> 32) & 0xFFFFFFFF;
    gdt[i+1].limit_low = base_high & 0xFFFF;
    gdt[i+1].base_low  = (base_high >> 16) & 0xFFFF;
    gdt[i+1].base_mid  = 0;
    gdt[i+1].access    = 0;
    gdt[i+1].gran      = 0;
    gdt[i+1].base_high = 0;
}

void gdt_init() {
    // wyczyść TSS
    /*for (int i = 0; i < (int)sizeof(Tss); ++i)
        ((uint8_t*)&tss)[i] = 0;
*/
    extern uint8_t kernel_stack_top[];
    tss.rsp0 = (uint64_t)kernel_stack_top;
    tss.io_map_base = sizeof(Tss);

    gdt_ptr.limit = sizeof(gdt) - 1;
    gdt_ptr.base  = (uint64_t)&gdt;

    // 0: null
    gdt_set(0, 0, 0, 0, 0);

    // 1: kernel code
    gdt_set(1, 0, 0, 0x9A, 0x20);

    // 2: kernel data
    gdt_set(2, 0, 0, 0x92, 0x00);

    // 3: user code
    gdt_set(3, 0, 0, 0xFA, 0x20);

    // 4: user data
    gdt_set(4, 0, 0, 0xF2, 0x00);

    // 7–8: TSS
    gdt_set_tss(7, (uint64_t)&tss, sizeof(Tss) - 1);

    gdt_flush((uint64_t)&gdt_ptr);
    tss_load();   // o ile w gdt.hpp masz już jej deklarację
}