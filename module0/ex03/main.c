#include <avr/io.h>
#include <util/delay.h>
int main()
{
 uint8_t pressed = 0;
 DDRB |= ((1 << PB0));
 DDRD &= ~(1 << PD2); //bitshift to put pd2 as input
 PORTD |= (1 << PD2); // turn on pull up resistor on pin PD2 (making the pin high by default when not pressed and avoiding incorrect reading cause of noise)
    while (1) {
        if (!(PIND & (1 << PD2)) && pressed == 0) //if button is pressed (pin PD2 is low)
        {
            pressed = 1;
            PORTB ^= (1 << PB0); //invert bit
        }
        else if(PIND & (1 << PD2))
            pressed = 0;
        _delay_ms(20);
    }
}
            