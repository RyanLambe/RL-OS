#include "../include/console.h"
#include "../include/interrupts.h"

struct __attribute__((packed)) MemoryMapEntry {
	unsigned long Base;
	unsigned long Length;
	unsigned int Type;
	unsigned int ACPI;
};

void KernelStart(){
    MemoryMapEntry* memMap = (MemoryMapEntry*)0x7000;
    SetupIDT();

    print("Welcome to RL-OS!\n\n");
    char buffer[1024];

    asm volatile ("int $0x3");
    asm volatile ("int $0x4");

    print("test");
}

extern "C" void CStart(){
    KernelStart();
}