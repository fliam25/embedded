#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void main(void)
{
    DDRD |= ((1 << PD3) | (1 << PD5) | (1 << PD6)); // PD3 BLUE. PD5 RED, PD6 VERT
    while (1) {
      PORTD |= (1 << PD5);
      _delay_ms(1000);
      PORTD &= ~(1 << PD5);
      PORTD |= (1 << PD6);
      _delay_ms(1000);
      PORTD &= ~(1 << PD6);
      PORTD |= (1 << PD3);
      _delay_ms(1000);
      PORTD &= ~(1 << PD3);
    }
}