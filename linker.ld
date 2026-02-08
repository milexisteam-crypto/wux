ENTRY(_start)

KERNEL_VMA = 0xffffffff80000000;

SECTIONS
{
    /* Linkujemy kernel w higher-half */
    . = KERNEL_VMA;

    /* --- Kod --- */
    text_start = .;
    .text ALIGN(0x1000) : AT(ADDR(.text) - KERNEL_VMA) {
        *(.text*)
    }
    text_end = .;

    /* --- Dane tylko do odczytu --- */
    rodata_start = .;
    .rodata ALIGN(0x1000) : AT(ADDR(.rodata) - KERNEL_VMA) {
        *(.rodata*)
    }
    rodata_end = .;

    /* --- Dane inicjalizowane --- */
    data_start = .;
    .data ALIGN(0x1000) : AT(ADDR(.data) - KERNEL_VMA) {
        *(.data*)
    }
    data_end = .;

    /* --- Dane niezainicjalizowane --- */
    bss_start = .;
    .bss ALIGN(0x1000) : AT(ADDR(.bss) - KERNEL_VMA) {
        *(COMMON)
        *(.bss*)
    }
    bss_end = .;

    kernel_end = .;
}