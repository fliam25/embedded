#include <avr/io.h>

int main(void)
{
    DDRB |= (1 << PB1); 

    TCCR1A = (1 << COM1A1) | (1 << WGM11); //turn on led when count go back to 0 (p.140)
    TCCR1B = (1 << WGM12) | (1 << WGM13); // turn on mode 14 (fast PWM with ICR1 as TOP) (p.142)
    OCR1A = 1562; //at what count will led turn off = 15625 / 10
    ICR1 = 15625; // at what count will timer stop and restart (FCPU / (Prescaler(1024) * frequence cible(1)))
    TCCR1B |= (1 << CS12) | (1 << CS10); // reduce speed  with prescaler 1024 (p.143)

    while (1)
    {
    }
}