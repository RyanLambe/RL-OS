#include "../include/interrupts.h"
#include "../include/types.h"
#include "../include/console.h"
#include "../include/cpu.h"
#include "../include/Time.h"

IDTEntry* idt = (IDTEntry*)0x4000;

void SetInterrupt(int index, unsigned long pointer, GDTSegment segment, unsigned char ist, bool trapGate, PrivilegeLevel privilege);
void SetupISRs();
void SetupIRQs();

void SetupIDT(){

    // empty idt
    long* toClear = (long*)idt;
    for(int i = 0; i < sizeof(IDTEntry) / sizeof(long) * 256; i++)
        toClear[i] = 0;

    SetupIRQs();
    SetupISRs();
}

void SetupISRs(){
    // no IST = 0x00
    SetInterrupt(0, (unsigned long)isr0, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(1, (unsigned long)isr1, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(2, (unsigned long)isr2, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(3, (unsigned long)isr3, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(4, (unsigned long)isr4, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(5, (unsigned long)isr5, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(6, (unsigned long)isr6, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(7, (unsigned long)isr7, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(8, (unsigned long)isr8, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(9, (unsigned long)isr9, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(10, (unsigned long)isr10, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(11, (unsigned long)isr11, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(12, (unsigned long)isr12, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(13, (unsigned long)isr13, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(14, (unsigned long)isr14, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(15, (unsigned long)isr15, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(16, (unsigned long)isr16, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(17, (unsigned long)isr17, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(18, (unsigned long)isr18, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(19, (unsigned long)isr19, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(20, (unsigned long)isr20, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(21, (unsigned long)isr21, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(22, (unsigned long)isr22, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(23, (unsigned long)isr23, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(24, (unsigned long)isr24, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(25, (unsigned long)isr25, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(26, (unsigned long)isr26, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(27, (unsigned long)isr27, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(28, (unsigned long)isr28, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(29, (unsigned long)isr29, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(30, (unsigned long)isr30, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(31, (unsigned long)isr31, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
}

void SetupIRQs(){

    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    // no IST = 0x00
    SetInterrupt(32, (unsigned long)irq0, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(33, (unsigned long)irq1, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(34, (unsigned long)irq2, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(35, (unsigned long)irq3, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(36, (unsigned long)irq4, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(37, (unsigned long)irq5, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(38, (unsigned long)irq6, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(39, (unsigned long)irq7, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(40, (unsigned long)irq8, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(41, (unsigned long)irq9, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(42, (unsigned long)irq10, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(43, (unsigned long)irq11, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(44, (unsigned long)irq12, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(45, (unsigned long)irq13, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(46, (unsigned long)irq14, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);
    SetInterrupt(47, (unsigned long)irq15, GDTSegment::Code, 0x00, false, PrivilegeLevel::Kernel);

}

void SetInterrupt(int index, unsigned long pointer, GDTSegment segment, unsigned char ist, bool trapGate, PrivilegeLevel privilege){
    // split offset
    idt[index].pointerLow = pointer & 0xFFFF;
    idt[index].pointerMid = (pointer >> 16) & 0xFFFF;
    idt[index].pointerHigh = (pointer >> 32) & 0xFFFFFFFF;

    // segment
    idt[index].selector = 0x10;//segment;

    // ist only uses 3 bits
    idt[index].ist = ist & 0x07; // todo: look into

    // P
    idt[index].flags = 0x80;

    // trap gate = 0xF
    // interrupt gate = 0xE
    idt[index].flags |= trapGate ? 0xF : 0xE;

    // DPL
    idt[index].flags |= ((unsigned char)privilege & 0x03) << 5;
    idt[index].zero = 0;
}

// called from asm
extern "C" void ISRHandler(Registers registers)
{
    char buffer[1024];
    intToHexCharArray(registers.interrupt, buffer);

    print("Unhandled interrupt: ");
    print(buffer);

    switch(registers.interrupt){
        case 8:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:

        intToHexCharArray(registers.errorCode, buffer);

        print(" - Error code: ");
        print(buffer);

        break;
    }

    print("\n");
}

// todo: create standard library and move this to it
extern "C" void memcpy(void *dest, const void *src, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++)
    {
        ((char*)dest)[i] = ((char*)src)[i];
    }
}

// called from asm
extern "C" void IRQHandler(Registers registers)
{
    // send received message to pic
    if (registers.interrupt >= 40)
    {
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);

    // handle interrupt
    switch (registers.interrupt) {
        // pit
        case 32:
            Time::tick(&registers);
            break;

        // todo: keyboard
        case 33:
            print("Keyboard");
            break;

        // todo: mouse
        case 44:
            print("Mouse");
            break;
    }
}