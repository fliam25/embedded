#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t ovf_count = 0;

void    init()
{
    //timer0
    TCCR0A = 0;                          // normal mode
    TCCR0B = 0;                          // no clock source yet (timer stopped)
    TCNT0  = 0;                          // reset counter
    TIMSK0 |= (1 << TOIE0);             // enable overflow interrupt
    //LED
    DDRB |= ((1 << PB0));
    //BUTTON
    DDRD &= ~(1 << PD2); //bitshift to put pd2 as input
    PORTD |= (1 << PD2); // turn on pull up resistor on pin PD2 (making the pin high by default when not pressed and avoiding incorrect reading cause of noise)
    EICRA &= ~(1 << ISC00);  // clear ISC00
    EICRA |= (1 << ISC01);  // Trigger on falling edge
    EIMSK |= (1 << INT0);   // Enable INT0

    SREG |= (1 << 7);   // like sei , turn on interrupt
}


void INT0_vect() __attribute__ ((signal, used, externally_visible));
void INT0_vect()
{
    EIMSK &= ~(1 << INT0); //disable INT0
    EIFR |= (1 << INTF0);
    TCNT0 = 0; //reset counter
    PORTB ^= (1 << PB0); //invert bit
    TCCR0B |= (1 << CS02) | (1 << CS00); // prescaler 1024 (turn on timer)
} 


void TIMER0_OVF_vect() __attribute__ ((signal, used, externally_visible));
void TIMER0_OVF_vect()
{
    ovf_count++;
    if (ovf_count >= 10)
    {
        ovf_count = 0;
        TCCR0B = 0;                         // stop timer
        TCNT0 = 0;                          // reset counter
        EIFR |= (1 << INTF0);    // clear any pending INT0 flag
        EIMSK |= (1 << INT0);              // re-enable INT0
    }
} 

void main()
{
    init();
    while(1);
}