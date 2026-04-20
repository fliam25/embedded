#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void init_rgb()
{
  DDRD |= ((1 << PD3) | (1 << PD5) | (1 << PD6)); // PD3 BLUE. PD5 RED, PD6 VERT
  TCCR0A = (1 << COM0A1) | (1 << COM0A0) | (1 << COM0B1) | (1 << COM0B0) | (1 << WGM01) | (1 << WGM00); // Set inverting compare match on pin PD6 (OC0A) and PD5(OC0B) and turn on FAST PWM mode on timer 0
  TCCR2A = (1 << COM2B1) | (1 << COM2B0) | (1 << WGM21) | (1 << WGM20); // set inverting compare match on PD3 (OC2B) and turn on FAST PWM on timer 2
  TCCR0B = (1 << CS01) | (1 << CS00); //prescaler
  TCCR2B = (1 << CS22);    //prescaler
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    OCR0B = 255 - r;
    OCR0A = 255 - g;
    OCR2B = 255 - b;
}

void wheel(uint8_t pos) {
  pos = 255 - pos;
  if (pos < 85) {
    set_rgb(255 - pos * 3, 0, pos * 3);
  }
  else if (pos < 170) {
    pos = pos - 85;
    set_rgb(0, pos * 3, 255 - pos * 3);
  } 
  else {
    pos = pos - 170;
    set_rgb(pos * 3, 255 - pos * 3, 0);
  }
}

int main(void)
{
    init_rgb();
    set_rgb(0, 0, 0);
    int i = 0;
    while(1)
    {
        wheel(i++);
        _delay_ms(30);
    }
    return 0;
}