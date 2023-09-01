#include "../include/Time.h"
#include "../include/console.h"
#include "../include/cpu.h"

bool globalTimeStarted = false;
unsigned long globalTime = 0;

void Time::tick(Registers* registers){
    print("-");
    globalTime++;
}

// todo: improve and move to better spot
short clamp(unsigned short value, unsigned short min, unsigned short max){
    const unsigned short t = value < min ? min : value;
    return t > max ? max : t;
}

void Time::SetupTime(unsigned short frequency){
    if(!globalTimeStarted){
        // Square wave
        outb(0x43, 0x36);

        // set frequency of IRQ
        unsigned short divisor = clamp(1193182 / frequency, 1, 65535);
        outb(0x40, (unsigned char)divisor);
        outb(0x40, (unsigned char)(divisor >> 8));
    }
}
