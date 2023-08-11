#! /bin/bash

# create paths
mkdir -p build
mkdir -p temp
mkdir -p temp/bin
mkdir -p temp/obj
mkdir -p temp/iso

# bootloader
nasm src/boot.asm -f bin -o temp/bin/boot.bin

# kernel - asm
nasm src/kernelEntry.asm -f elf32 -o temp/obj/kernelEntry.o

# kernel - c
clang src/kernel.c -o temp/obj/kernel.o -c -target i386-elf

# kernel - link
ld.lld -o temp/bin/kernel.bin -Ttext 0x1000 temp/obj/kernelEntry.o temp/obj/kernel.o --oformat binary -m elf_i386


# combine project into img file
dd if=/dev/zero of=final.img bs=1024 count=1440
dd if=temp/bin/boot.bin of=final.img seek=0 count=1 conv=notrunc
dd if=temp/bin/kernel.bin of=final.img seek=1 conv=notrunc

# convert to iso
cp final.img temp/iso/
genisoimage -quiet -V 'MYOS' -input-charset iso8859-1 -o build/output.iso -b final.img -hide final.img temp/iso/
rm final.img