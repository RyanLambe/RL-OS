#include "../include/console.h"
#include "../include/interrupts.h"
#include "../include/Time.h"

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
    print("20Hz: ");

    Time::SetupTime(20);
}

extern "C" void CStart(){
    KernelStart();
}