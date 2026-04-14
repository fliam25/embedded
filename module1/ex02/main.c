#include <avr/io.h>

int main(void)
{
    DDRB |= (1 << PB1); // put led 2 as output

    TCCR1A = (1 << COM1A0); // Set compare match on pin PB1 (OC1A)     
    TCCR1B = (1 << WGM12); // turn ON CTC on CR1A
    OCR1A = 7811; // at what count will PB1 status will be inverted and where the timer will restart 
    TCCR1B |= (1 << CS12) | (1 << CS10); // reduce speed  with prescaler 1024

    while (1)
    {
    }
}