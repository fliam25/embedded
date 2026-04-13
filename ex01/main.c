#include <avr/io.h>

int main()
{
 DDRB |= (1 << PB4); // set PB5 as output pin
     while (1) {
       PORTB |= (1 << PB4); // set PB5 as high 
    }
}