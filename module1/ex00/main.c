#include <avr/io.h>


void wait_ms(uint16_t _ms)
{
    while(_ms > 0)
    {
        volatile uint16_t i = F_CPU / 18000;
        while (i > 0)
            i--;
        _ms--;
    }
}

int main()
{
    DDRB |= (1 << PB0); // set PB5 as output pin
     while (1) {
        PORTB ^= (1 << PB0);
    wait_ms(500);
    }
}           