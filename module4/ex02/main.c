#include <avr/io.h>
#include <util/delay.h>

volatile uint8_t sw0_ovf_count = 0;
volatile uint8_t sw0_pressed = 0;
volatile uint8_t sw2_ovf_count = 0;
volatile uint8_t sw2_pressed = 0;
volatile uint8_t val = 0;

void    init()
{
    //timer0
    TCCR0A = 0;                          // normal mode
    TCCR0B = 0;                          // no clock source yet (timer stopped)
    TCNT0  = 0;                          // reset counter
    TIMSK0 |= (1 << TOIE0);             // enable overflow interrupt

    //timer2
    TCCR2A = 0;                          // normal mode
    TCCR2B = 0;                          // no clock source yet (timer stopped)
    TCNT2  = 0;                          // reset counter
    TIMSK2 |= (1 << TOIE2);             // enable overflow interrupt

    //LED
    DDRB |= ((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4));
    
    //BUTTON
    DDRD &= ~(1 << PD2); //bitshift to put pd2 as input
    DDRD &= ~(1 << PD4); //bitshift to put pd2 as input
    PORTD |= (1 << PD2); // turn on pull up resistor on pin PD2 (making the pin high by default when not pressed and avoiding incorrect reading cause of noise)
    PORTD |= (1 << PD4); // turn on pull up resistor on pin PD4 (making the pin high by default when not pressed and avoiding incorrect reading cause of noise)
    EICRA |= (1 << ISC00);  // Trigger interrupt on any change on INT0
    EIMSK |= (1 << INT0);   // Enable INT0
    PCICR |= (1 << PCIE2);   // Enable intterupt on any change between PIN 23-16 (Check datasheet)
    PCMSK2 |= (1 << PCINT20); // Specifficaly enable interrupt on PD4


    SREG |= (1 << 7);   // like sei , turn on interrupt
}


void INT0_vect() __attribute__ ((signal, used, externally_visible));
void INT0_vect()
{
    EIMSK &= ~(1 << INT0); //disable INT0
    if(sw0_pressed == 0)
    {
        val++;
        if(val > 15)
            val = 0;
        PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4));            
        PORTB |= (val & 7) | ((val & 8) << 1 ); // val & 00000111 + val & (00001000 << 1 = 00010000)
        sw0_pressed = 1;
    }
    else
        sw0_pressed = 0;
    EIFR |= (1 << INTF0);
    TCNT0 = 0; //reset counter
    TCCR0B |= (1 << CS02) | (1 << CS00); // prescaler 1024 (turn on timer)
} 

void PCINT2_vect() __attribute__ ((signal, used, externally_visible));
void PCINT2_vect()
{
    PCICR &= ~(1 << PCIE2); // Disable pin 23-16 interrupt
    if(sw2_pressed == 0)
    {
        if(val > 0)
            val--;
        PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4));
        PORTB |= (val & 7) | ((val & 8) << 1 );
        sw2_pressed = 1;
    }
    else
        sw2_pressed = 0;
    PCIFR |= (1 << PCIF2);// clear any pending PCINT2
    TCNT2 = 0; //reset counter
    TCCR2B |= (1 << CS02) | (1 << CS00); // prescaler 1024 (turn on timer)
} 

void TIMER0_OVF_vect() __attribute__ ((signal, used, externally_visible));
void TIMER0_OVF_vect()
{
    sw0_ovf_count++;
    if (sw0_ovf_count >= 3)
    {
        sw0_ovf_count = 0;
        TCCR0B = 0;                         // stop timer
        TCNT0 = 0;                          // reset counter
        EIFR |= (1 << INTF0);    // clear any pending INT0 flag
        EIMSK |= (1 << INT0);              // re-enable INT0
    }
} 

void TIMER2_OVF_vect() __attribute__ ((signal, used, externally_visible));
void TIMER2_OVF_vect()
{
    sw2_ovf_count++;
    if (sw2_ovf_count >= 3)
    {
        sw2_ovf_count = 0;
        TCCR2B = 0;                         // stop timer
        TCNT2 = 0;                          // reset counter
        PCIFR |= (1 << PCIF2);// clear any pending PCINT2
        PCICR |= (1 << PCIE2);   // Enable intterupt on any change between PIN 23-16 (Check datasheet)
    }
} 

int main()
{
    init();
    while (1) {
               
    }
}