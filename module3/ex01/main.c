#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void display_color(uint8_t RED, uint8_t GREEN, uint8_t BLUE)
{
    PORTD &= ~((1 << PD3) | (1 << PD5) | (1 << PD6));
    if(RED)
      PORTD |= (1 << PD5);
    if(GREEN)
      PORTD |= (1 << PD6);
    if(BLUE)
      PORTD |= (1 << PD3);
}

void main(void)
{
    DDRD |= ((1 << PD3) | (1 << PD5) | (1 << PD6)); // PD3 BLUE. PD5 RED, PD6 VERT
    while (1) {
      // PRIMARY
      display_color(1, 0, 0); //RED
      _delay_ms(1000);
      display_color(0, 1, 0); //GREEN
      _delay_ms(1000);
      display_color(0, 0, 1); //BLUE
      _delay_ms(1000);
      // MIX
      display_color(1, 1, 0); //YELLOW
      _delay_ms(1000);
      display_color(0, 1, 1); //CYAN
      _delay_ms(1000);
      display_color(1, 0, 1); //PURPLE
      _delay_ms(1000);
      display_color(1, 1, 1); //WHITE
      _delay_ms(1000);
    }
}