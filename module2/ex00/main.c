#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 115200
#define MYUBRR ((F_CPU / (8UL * BAUD)) - 1)  // U2X formula
//screen /dev/ttyUSB0 115200
void USART_Init(unsigned int ubrr)
{
    UCSR0A = (1 << U2X0);                          // enable double speed
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << TXEN0); //TRANSMITTER
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);       // 8N1
}

void USART_Transmit(unsigned char data)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

int main(void)
{
    USART_Init(MYUBRR);
    while (1)
    {
        USART_Transmit('Z');
        _delay_ms(1000);
    }
}