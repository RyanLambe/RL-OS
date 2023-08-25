#include "../include/console.h"
//#include <vector>

struct MemoryMapEntry {
	unsigned long Base;
	unsigned long Length;
	unsigned int Type;
	unsigned int ACPI;
};

struct IDTEntry {
   short offset_1;
   short selector;
   char  ist;
   char  type_attributes;
   short offset_2;
   int offset_3;
   int zero = 0;
};

// todo: look into "__attribute__ ((interrupt))" https://wiki.osdev.org/Interrupt_Service_Routines
void CreateInterrupt(void* pointer, short segmentSelector, char ist, bool trapGate, char DPL){
    IDTEntry entry;
    
    // split offset
    entry.offset_1 = static_cast<short>((long)pointer);
    entry.offset_2 = static_cast<short>((long)pointer >> 16);
    entry.offset_3 = static_cast<int>((long)pointer >> 32);

    // selector
    entry.selector = segmentSelector; // read more

    // ist only uses 3 bits
    entry.ist = ist & 0x07; // read more

    // P
    entry.type_attributes = 0x80;
    
    // trap gate = 0xF
    // interrupt gate = 0xE
    entry.type_attributes = trapGate ? 0xF : 0xE;

    // DPL
    entry.type_attributes = (DPL & 0x03) << 5;
}

void KernelStart(){
    char buffer[1024];
    MemoryMapEntry* memMap = (MemoryMapEntry*)0x7000;

    print("Hello :)\n\n");

    intToHexCharArray(memMap[0].Length, buffer);
    print("Length: ");
    print(buffer);
    print("\n");

    
    //CreateInterrupt((void*)0x0123456789abcdef);

    // interrupt stuff
    IDTEntry* idt = (IDTEntry*)0x4000;
}

extern "C" void CStart(){
    KernelStart();
}