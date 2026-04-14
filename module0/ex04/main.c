#include <avr/io.h>
#include <util/delay.h>
int main()
{
    
 uint8_t  val = 0;
 uint8_t sw1_pressed = 0;
 uint8_t sw2_pressed = 0;
 DDRB |= ((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4));
 DDRD &= ~(1 << PD2); //bitshift to put pd2 as input
 DDRD &= ~(1 << PD4); //bitshift to put pd2 as input
 PORTD |= (1 << PD2); // turn on pull up resistor on pin PD2 (making the pin high by default when not pressed and avoiding incorrect reading cause of noise)
 PORTD |= (1 << PD4); // turn on pull up resistor on pin PD2 (making the pin high by default when not pressed and avoiding incorrect reading cause of noise)
    while (1) {
        if (!(PIND & (1 << PD2)) && sw1_pressed == 0) //if button is pressed (pin PD2 is low)
        { 
                sw1_pressed = 1;
                val++;
                if(val > 15)   
                    val = 0;
        }
        else if((PIND & (1 << PD2)))
            sw1_pressed = 0;
        _delay_ms(20);
        if (!(PIND & (1 << PD4))&& sw2_pressed == 0) //if button is pressed (pin PD2 is low)
        {
                sw2_pressed = 1;
                    if(val > 0)   
                        val--;
        }
        else if((PIND & (1 << PD4)))
            sw2_pressed = 0;
        _delay_ms(20);
        for (int i = 0; i < 4; i++)
        {
            int bit = (val >> i) & 1;
            switch (i) {
                case 0:
                    if(bit == 0)
                        PORTB &= ~(1 << PB0); // put pin pb0 as low(turn off led)
                    else
                        PORTB |= (1 << PB0); // put pin pb0 as high (turn on led)
                    break;
                case 1:
                    if(bit == 0)
                        PORTB &= ~(1 << PB1); // put pin pb0 as low(turn off led)
                    else
                        PORTB |= (1 << PB1); // put pin pb0 as high (turn on led)
                    break;
                case 2:
                    if(bit == 0)
                        PORTB &= ~(1 << PB2); // put pin pb0 as low(turn off led)
                    else
                        PORTB |= (1 << PB2); // put pin pb0 as high (turn on led)
                    break;
                case 3:
                    if(bit == 0)
                        PORTB &= ~(1 << PB4); // put pin pb0 as low(turn off led)
                    else
                        PORTB |= (1 << PB4); // put pin pb0 as high (turn on led)
                    break;
            }
        }
               
    }
}