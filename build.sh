#! /bin/bash

clear

rm -r temp
rm -r build

# create paths
mkdir -p build
mkdir -p temp
mkdir -p temp/bin
mkdir -p temp/iso
mkdir -p temp/obj
mkdir -p temp/obj/kernel

# bootloader
nasm boot/boot.asm -f bin -o temp/bin/boot.bin

# kernel - asm
nasm boot/kernelEntry.asm -f elf64 -o temp/obj/kernelEntry.o
nasm kernel/src/interrupts.asm -f elf64 -o interrupts.asm.o

# kernel - c++
clang++ kernel/src/*.cpp -c -target x86_64-i386-elf
mv *.o temp/obj/kernel/

# kernel - link
ld.lld -o temp/bin/kernel.bin -Ttext 0x7E00 temp/obj/kernelEntry.o temp/obj/kernel/*.o --oformat binary

# combine project into img file
dd if=/dev/zero of=final.img bs=1024 count=1440
dd if=temp/bin/boot.bin of=final.img seek=0 count=1 conv=notrunc
dd if=temp/bin/kernel.bin of=final.img seek=1 conv=notrunc

# convert to iso
mv final.img temp/iso/
genisoimage -quiet -V 'RL-OS' -input-charset iso8859-1 -o build/output.iso -b final.img -hide final.img temp/iso/