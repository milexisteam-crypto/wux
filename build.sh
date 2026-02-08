#!/usr/bin/env bash
set -e

# Wyczyść i utwórz obj/
rm -rf obj
mkdir -p obj/iso/EFI/BOOT

cd src

# Znajdź wszystkie pliki .c i .cpp
sourcesC=$(find . -maxdepth 1 -name "*.c")
sourcesCpp=$(find . -maxdepth 1 -name "*.cpp")
sourcesAsm=$(find . -maxdepth 1 -name "*.asm")
# Kompilacja C++
for f in $sourcesCpp; do
    base=$(basename "$f" .cpp)
    x86_64-elf-g++ -c "$f" -o "../obj/$base.o" -IInclude \
        -ffreestanding -fno-exceptions -fno-rtti -O2 \
        -mno-red-zone -mcmodel=large -fno-pic -fno-pie
done

# Kompilacja C
for f in $sourcesC; do
    base=$(basename "$f" .c)
    x86_64-elf-gcc -c "$f" -o "../obj/$base.o" -IInclude \
        -ffreestanding -O2 \
        -mno-red-zone -mcmodel=large -fno-pic -fno-pie
done
for f in $sourcesAsm; do
    base=$(basename "$f" .asm)
    nasm -f elf64 "$f" -o "../obj/$base.o"
done
cd ..
# Linkowanie
objects=$(find obj -name "*.o")

x86_64-elf-ld \
    -nostdlib \
    -z max-page-size=0x1000 \
    -T linker.ld \
    -o obj/kernel.elf \
    $objects

# Kopiowanie plików do ISO
cp limine.conf obj/iso/
cp BOOTX64.EFI obj/iso/EFI/BOOT
cp limine-uefi-cd.bin obj/iso/
cp obj/kernel.elf obj/iso/
cp font.psfu obj/iso/
# Tworzenie ISO
sudo xorriso -as mkisofs \
  -iso-level 3 \
  -full-iso9660-filenames \
  -volid "WUX_OS" \
  -eltorito-alt-boot \
  --efi-boot limine-uefi-cd.bin \
  -no-emul-boot \
  -efi-boot-part \
  --efi-boot-image \
  -append_partition 2 0xef limine-uefi-cd.bin \
  -partition_cyl_align off \
  -o wux.iso \
  obj/iso