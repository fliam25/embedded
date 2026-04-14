#include <avr/io.h>
#include <util/delay.h>
int main()
{
 DDRB |= ((1 << PB0));
 DDRD &= ~(1 << PD2); //bitshift to put pd2 as input
 PORTD |= (1 << PD2); // turn on pull up resistor on pin PD2 (making the pin high by default when not pressed and avoiding incorrect reading cause of noise)
    while (1) {
        if (!(PIND & (1 << PD2))) //if button is pressed (pin PD2 is low)
            PORTB |= (1 << PB0); // put pin pb0 as high (turn on led)
        else
            PORTB &= ~(1 << PB0); // put pin pb0 as low(turn off led)
        }   
}