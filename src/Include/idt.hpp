#pragma once
#include <stdint.h>

struct IdtEntry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  ist;
    uint8_t  type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed));

struct IdtPtr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));
void idt_register_handler(int vec, void(*handler)());
void idt_init();
extern "C" void idt_load(uint64_t idt_ptr_addr);
extern "C" void isr_timer_stub();
extern "C" void isr_default_stub();