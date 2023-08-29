#include "../include/interrupts.h"

IDTEntry* idt = (IDTEntry*)0x4000;
IDTPointer idtp;

extern "C" void idt_flush(unsigned int);

void SetInterrupt(int index, unsigned long pointer, GDTSegment segment, unsigned char ist, bool trapGate, PrivilegeLevel privilege);

// todo: look into "__attribute__ ((interrupt))" https://wiki.osdev.org/Interrupt_Service_Routines
void setupIDT(){
    idtp.limit = sizeof(IDTEntry) * 256 - 1;
    idtp.base = (unsigned long)&idt;

    // empty idt
    long* toClear = (long*)idt;
    for(int i = 0; i < sizeof(IDTEntry) / sizeof(long) * 256; i++)
        toClear[i] = 0;

    // fill first 32 interrupts
    // no IST = 0x00
    SetInterrupt(0, (unsigned long)isr0, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);

    idt_flush((unsigned long)&idtp);
}

void SetInterrupt(int index, unsigned long pointer, GDTSegment segment, unsigned char ist, bool trapGate, PrivilegeLevel privilege){
    // split offset
    idt[index].offset_1 = pointer & 0xFFFF;
    idt[index].offset_2 = (pointer >> 16) & 0xFFFF;
    idt[index].offset_3 = (pointer >> 32) & 0xFFFFFFFF;

    // segment
    idt[index].selector = segment;

    // ist only uses 3 bits
    idt[index].ist = ist & 0x07; // todo: look into

    // P
    idt[index].type_attributes = 0x80;

    // trap gate = 0xF
    // interrupt gate = 0xE
    idt[index].type_attributes = trapGate ? 0xF : 0xE;

    // DPL
    idt[index].type_attributes = ((unsigned char)privilege & 0x03) << 5;
}

// This gets called from our ASM interrupt handler stub.
extern "C" void isr_handler(registers regs)
{
    *(char*)0xb8000 = 'Q';
}