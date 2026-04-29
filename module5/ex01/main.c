#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 115200
#define MYUBRR ((F_CPU / (8UL * BAUD)) - 1)  // U2X formula
//screen /dev/ttyUSB0 115200

//UART
void uart_init(unsigned int ubrr)
{
	UCSR0A = (1 << U2X0);
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_tx(unsigned char c)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

void uart_printstr(const char *str)
{
	while (*str)
		uart_tx(*str++);
}

//ADC
void adc_init()
{
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // ADEN = enable ADC ||| ADSPn = prescaler 128 p.249/p.259
}


uint8_t adc_read(uint8_t id)
{
	ADMUX = (1 << REFS0) | (1 << ADLAR) | (id & 0x0F);
	ADCSRA |= (1 << ADSC);         // Start conversion
	while (ADCSRA & (1 << ADSC));  // Wait until convertion is done
	return ADCH;                    // return result (only ADCH cause only 8 bit needed)
}

void init()
{
	uart_init(MYUBRR);
	adc_init();
}

void ft_8inttohex(uint8_t val)
{
	const char *hex = "0123456789abcdef";
	uart_tx(hex[val >> 4]); // first char
	uart_tx(hex[val & 0b00001111]); // second char
}

void main(void)
{
	init();
	while (1)
	{
		_delay_ms(20);
		ft_8inttohex(adc_read(0));
		uart_printstr(", ");
		ft_8inttohex(adc_read(1));
		uart_printstr(", ");
		ft_8inttohex(adc_read(2));
		uart_printstr("\r\n");	
	}
}