#include "../include/console.h"
#include "../include/interrupts.h"

struct __attribute__((packed)) MemoryMapEntry {
	unsigned long Base;
	unsigned long Length;
	unsigned int Type;
	unsigned int ACPI;
};

void KernelStart(){
    char buffer[1024];
    MemoryMapEntry* memMap = (MemoryMapEntry*)0x7000;

    print("Hello :)\n\n");

    intToHexCharArray(memMap[0].Length, buffer);
    print("Length: ");
    print(buffer);
    print("\n");

    // interrupt stuff
    setupIDT();

    asm volatile ("int $0x3");
}

extern "C" void CStart(){
    KernelStart();
}