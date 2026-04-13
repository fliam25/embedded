#include <avr/io.h>
#include <util/delay.h>
int main()
{
 int val = 0;
 int sw1_pressed = 0;
 int sw2_pressed = 0;
 DDRB |= (1 << PB0); //bitshift to put pb0 as output
 DDRB |= (1 << PB1); //bitshift to put pb1 as output
 DDRB |= (1 << PB2); //bitshift to put pb2 as output
 DDRB |= (1 << PB4); //bitshift to put pb4 as output
 DDRD &= ~(1 << PD2); //bitshift to put pd2 as input
 DDRD &= ~(1 << PD4); //bitshift to put pd2 as input
 PORTD |= (1 << PD2); // turn on pull up resistor on pin PD2 (making the pin high by default when not pressed and avoiding incorrect reading cause of noise)
 PORTD |= (1 << PD4); // turn on pull up resistor on pin PD2 (making the pin high by default when not pressed and avoiding incorrect reading cause of noise)

 uint8_t pins[4] = {PB0, PB1, PB2, PB4};
    while (1) {
        if (!(PIND & (1 << PD2)) && sw1_pressed == 0) //if button is pressed (pin PD2 is low)
        { 
            sw1_pressed = 1;  
            val++;
            if(val > 15)
                val = 0;
        }
        else
            sw1_pressed = 0;
        if (!(PIND & (1 << PD4)) && sw2_pressed == 0) //if button is pressed (pin PD2 is low)
        {
            sw2_pressed = 1;
                if(val > 0)   
                    val--;
        }
        else
            sw2_pressed = 0;
        for (int i = 0; i < 4; i++)
        {
            PORTB |= (1 << pins[i]); // put pin pb0 as high (turn on led)
            // int bit = (val >> i) & 1;
            // if(bit == 0)
            //     PORTB &= ~(1 << pins[i]); // put pin pb0 as low(turn off led)
            // else
            //     PORTB |= (1 << pins[i]); // put pin pb0 as high (turn on led)
        }
               
    }
}