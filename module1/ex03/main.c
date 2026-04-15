#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    uint8_t sw1_pressed = 0;
    uint8_t sw2_pressed = 0;
    DDRB |= (1 << PB1); 

    DDRD &= ~(1 << PD2); //bitshift to put pd2 as input
    DDRD &= ~(1 << PD4); //bitshift to put pd4 as input
    PORTD |= (1 << PD2); // turn on pull up resistor on pin PD2 (making the pin high by default when not pressed and avoiding incorrect reading cause of noise)
    PORTD |= (1 << PD4); // turn on pull up resistor on pin PD4 (making the pin high by default when not pressed and avoiding incorrect reading cause of noise)

    TCCR1A = (1 << COM1A1) | (1 << WGM11); //turn on led when count go back to 0 (p.140) 
    TCCR1B = (1 << WGM12) | (1 << WGM13); // turn on mode 14 (fast PWM with ICR1 as TOP) (p.142)
    OCR1A = 1562; //at what count will led turn off = 15625 / 10
    ICR1 = 15625; // at what count will timer stop and restart (FCPU / (Prescaler(1024) * frequence cible(1)))
    TCCR1B |= (1 << CS12) | (1 << CS10); // reduce speed  with prescaler 1024 (p.143)

    while (1)
    {
         if (!(PIND & (1 << PD2)) && sw1_pressed == 0 && OCR1A < 15620) //if button is pressed (pin PD2 is low)
        { 
                sw1_pressed = 1;
                OCR1A += 1562;
        }
        else if((PIND & (1 << PD2)))
            sw1_pressed = 0;
        _delay_ms(20);
        if (!(PIND & (1 << PD4))&& sw2_pressed == 0 && OCR1A > 1562) //if button is pressed (pin PD2 is low)
        {
                sw2_pressed = 1;
                OCR1A -= 1562;
        }
        else if((PIND & (1 << PD4)))
            sw2_pressed = 0;
        _delay_ms(20);
    }
}