#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 115200
#define MYUBRR ((F_CPU / (8UL * BAUD)) - 1)  // U2X formula
//screen /dev/ttyUSB0 115200
void uart_init(unsigned int ubrr)
{
    UCSR0A = (1 << U2X0);                          // enable double speed
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0); //enable transmission and reception and receptiuon intterupt
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);       //set 8N1
    SREG |= (1 << 7);   // like sei , turn on interrupt
}

void uart_tx(unsigned char c)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void USART_RX_vect() __attribute__ ((signal, used));
void USART_RX_vect()
{
    unsigned char c = UDR0;
    if(c >= 'a' && c <= 'z')
        c+= -32;
    else if(c >= 'A' && c <= 'Z')
        c+= 32;
    uart_tx(c);
} 


void uart_printstr(const char *str)
{
    while (*str)
        uart_tx(*str++);
}

void main(void)
{
    uart_init(MYUBRR);
    while (1)
    {
    }
}