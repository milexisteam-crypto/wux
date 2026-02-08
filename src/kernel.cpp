#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "limine.h"
#include <gdt.hpp>
#include <idt.hpp>
// Linker symbols
extern "C" {
    extern char bss_start[];
    extern char bss_end[];
}

// RGB → format framebufferu Limine
static inline uint32_t nrgb(uint8_t r, uint8_t g, uint8_t b,
                            const limine_framebuffer* fb) {
    return ((uint32_t)r << fb->red_mask_shift) |
           ((uint32_t)g << fb->green_mask_shift) |
           ((uint32_t)b << fb->blue_mask_shift);
}

// Limine requests (Twoja wersja limine.h)
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

static volatile struct limine_module_request module_request = {
    .id = LIMINE_MODULE_REQUEST_ID,
    .revision = 0
};

static void hang() {
    for (;;) asm volatile("hlt");
}

// PSF2 header
struct __attribute__((packed)) PSF2_Header {
    uint32_t magic;
    uint32_t version;
    uint32_t headersize;
    uint32_t flags;
    uint32_t length;
    uint32_t charsize;
    uint32_t height;
    uint32_t width;
};

static void draw_char_psf2(limine_framebuffer* fb,
                           PSF2_Header* hdr,
                           uint8_t* glyphs,
                           int x, int y,
                           char c,
                           uint32_t color) {

    uint8_t* glyph = glyphs + hdr->charsize * (uint8_t)c;

    for (uint32_t row = 0; row < hdr->height; row++) {
        for (uint32_t col = 0; col < hdr->width; col++) {

            uint32_t bit_index  = row * hdr->width + col;
            uint32_t byte_index = bit_index / 8;
            uint32_t bit        = 7 - (bit_index % 8);

            if (glyph[byte_index] & (1u << bit)) {
                uint32_t* pixel = (uint32_t*)((uint8_t*)fb->address +
                    (y + (int)row) * fb->pitch +
                    (x + (int)col) * 4);

                *pixel = color;
            }
        }
    }
}

static void draw_string(limine_framebuffer* fb,
                        PSF2_Header* hdr,
                        uint8_t* glyphs,
                        int x, int y,
                        const char* str,
                        uint32_t color) {

    int cx = x;
    while (*str) {
        draw_char_psf2(fb, hdr, glyphs, cx, y, *str, color);
        cx += hdr->width;
        str++;
    }
}

extern "C" void wuxkrn() {
    /*
    // Clear .bss
    for (char* p = bss_start; p < bss_end; ++p)
        *p = 0;

    // Framebuffer
    if (!framebuffer_request.response ||
        framebuffer_request.response->framebuffer_count < 1)
        hang();

    limine_framebuffer* fb = framebuffer_request.response->framebuffers[0];

    uint64_t width  = fb->width;
    uint64_t height = fb->height;
    uint64_t pitch  = fb->pitch;

    // Background
    uint32_t bg = nrgb(40, 44, 255, fb);

    for (uint64_t y = 0; y < height; y++) {
        uint32_t* row = (uint32_t*)((uint8_t*)fb->address + y * pitch);
        for (uint64_t x = 0; x < width; x++)
            row[x] = bg;
    }

    // Modules
    if (!module_request.response ||
        module_request.response->module_count < 1)
        hang();

    // Bierzemy pierwszy moduł — bez sprawdzania nazwy
    limine_file* mod = module_request.response->modules[0];
    uint8_t* font_data = (uint8_t*)mod->address;

    if (!font_data)
        hang();

    // PSF2
    PSF2_Header* hdr = (PSF2_Header*)font_data;

    if (hdr->magic != 0x864ab572)
        hang();

    uint8_t* glyphs = font_data + hdr->headersize;

    uint32_t fg = nrgb(255, 255, 255, fb);

    draw_string(fb, hdr, glyphs, 50, 50, "hello world", fg);

    */
       for (char* p = bss_start; p < bss_end; ++p)
        *p = 0;
        asm volatile("cli");


gdt_init();
idt_init();
//for (;;) asm volatile("hlt");


asm volatile("sti");
    // Clear .bss


    // Framebuffer
    if (!framebuffer_request.response ||
        framebuffer_request.response->framebuffer_count < 1)
        hang();

    limine_framebuffer* fb = framebuffer_request.response->framebuffers[0];

    uint64_t width  = fb->width;
    uint64_t height = fb->height;
    uint64_t pitch  = fb->pitch;

    // Background
    uint32_t bg = nrgb(40, 44, 255, fb);

    for (uint64_t y = 0; y < height; y++) {
        uint32_t* row = (uint32_t*)((uint8_t*)fb->address + y * pitch);
        for (uint64_t x = 0; x < width; x++)
            row[x] = bg;
    }

    // Modules
    if (!module_request.response ||
        module_request.response->module_count < 1)
        hang();

    // Bierzemy pierwszy moduł — bez sprawdzania nazwy
    limine_file* mod = module_request.response->modules[0];
    uint8_t* font_data = (uint8_t*)mod->address;

    if (!font_data)
        hang();

    // PSF2
    PSF2_Header* hdr = (PSF2_Header*)font_data;

    if (hdr->magic != 0x864ab572)
        hang();

    uint8_t* glyphs = font_data + hdr->headersize;

    uint32_t fg = nrgb(255, 255, 255, fb);

    draw_string(fb, hdr, glyphs, 0, 0, "hello world", fg);
    draw_string(fb, hdr, glyphs, 0, 16, "tyle", fg);

}