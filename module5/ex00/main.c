#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define BAUD 115200

//screen /dev/ttyUSB0 115200

void init()
{
    ADCSRA |= (1 << ADEN); //Enable ADC p.259
    ADMUX = (1 << REFS0) | (1 << ADLAR); // use AVCC p.257 and align result on the left to be on 8 bit
    
}


void main(void)
{
    init();
    while (1)
    {
    }
}