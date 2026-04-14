#include <avr/io.h>

int main()
{
 DDRB |= (1 << PB0); // set PB0 as output pin
     while (1) {
       PORTB |= (1 << PB0); // set PB0 as high 
    }
}