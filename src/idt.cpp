#include "idt.hpp"
#include "panic.hpp"
static IdtEntry idt[256];
static IdtPtr idt_ptr;

extern "C" void isr_default_stub();
extern "C" void isr_timer_stub();
asm(
    ".global isr_default_stub\n"
    "isr_default_stub:\n"
    "    iretq\n"
);

asm(
    ".global isr_timer_stub\n"
    "isr_timer_stub:\n"
    "    iretq\n"
);
static void idt_set(int i, uint64_t handler, uint16_t sel, uint8_t flags) {
    idt[i].offset_low  = handler & 0xFFFF;
    idt[i].selector    = sel;
    idt[i].ist         = 0;
    idt[i].type_attr   = flags; // 0x8E = present, ring0, interrupt gate
    idt[i].offset_mid  = (handler >> 16) & 0xFFFF;
    idt[i].offset_high = (handler >> 32) & 0xFFFFFFFF;
    idt[i].zero        = 0;
}

void idt_register_handler(int vec, void(*handler)()) {
    idt_set(vec, (uint64_t)handler, 0x08, 0x8E);
}

void idt_init() {
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base  = (uint64_t)&idt;

    // Jeśli chcesz ustawić wszystkie wektory:
    for (int i = 0; i < 31; ++i)
         idt_set(i, (uint64_t)isr_panic_stub, 0x08, 0x8E);

    // Timer IRQ
    idt_set(0x20, (uint64_t)isr_timer_stub, 0x08, 0x8E);

    idt_load((uint64_t)&idt_ptr);
}

// ===============================
//  OSADZONE STUBY ISR (64-bit)
// ===============================

