#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t rise = 1;

void    init()
{
    //timer0
    TCCR0A = (1 << WGM01);                          // turn on CTC
    TCCR0B = (1 << CS02); // prescaler 256
    OCR0A = 124;          // 2 ms
    TIMSK0 |= (1 << OCIE0A);             // enable intterupt on compare match with OCR0A
    //timer 1
    TCCR1A = (1 << COM1A1) | (1 << WGM11); //turn on led when count go back to 0 (p.140) 
    TCCR1B = (1 << WGM12) | (1 << WGM13); // turn on mode 14 (fast PWM with ICR1 as TOP) (p.142)
    OCR1A = 0; //at what count will led turn
    ICR1 = 255; // at what count will timer stop and restart (FCPU / (Prescaler(8) * frequence cible(1)))
    TCCR1B |= (1 << CS11); // reduce speed  with prescaler 8 (p.143)
    //LED
    DDRB |= ((1 << PB1));

    SREG |= (1 << 7);   // like sei , turn on interrupt
}

void TIMER0_COMPA_vect () __attribute__ ((signal, used, externally_visible));
void TIMER0_COMPA_vect ()
{
    if(rise == 0)
    {
        if(OCR1A > 0)
        {
            OCR1A --;
            if(OCR1A == 0)
                rise = 1;
        }
    }
    else
    {
        if(OCR1A < 255)
        {
            OCR1A++;
            if(OCR1A == 255)
                rise = 0;
        }
    }
} 

void main()
{
    init();
    while(1);
}