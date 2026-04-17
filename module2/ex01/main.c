#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 115200
#define MYUBRR ((F_CPU / (8UL * BAUD)) - 1)  // U2X formula
//screen /dev/ttyUSB0 115200
void USART_Init(unsigned int ubrr)
{
    TCCR1B = (1 << WGM12); // turn ON CTC on CR1A
    OCR1A = 31244; //set timer stop at 2s 
    UCSR0A = (1 << U2X0);                          // enable double speed
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    UCSR0B = (1 << TXEN0); //enable transmission
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);       //set 8N1
    TIMSK1 = (1 << OCIE1A); // turn timer1 interrupt on
    SREG |= (1 << 7);   // like sei , turn on interrupt
}

void USART_putchar(unsigned char c)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void USART_printstr(const char *str)
{
    while (*str)
        USART_putchar(*str++);
}

void TIMER1_COMPA_vect() __attribute__ ((signal, used));
void TIMER1_COMPA_vect()
{
    USART_printstr("Hello World!\r\n");
} 


int main(void)
{
    USART_Init(MYUBRR);
    while (1)
    {
    }
}