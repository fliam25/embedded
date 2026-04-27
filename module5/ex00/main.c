#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define BAUD 115200

//screen /dev/ttyUSB0 115200

void init()
{
    ADCSRA |= (1 << ADEN); //Enable ADC p.259
    ADMUX |= (1 << REFS0); // use AVCC p.257
    
}


void main(void)
{
    init();
    while (1)
    {
    }
}