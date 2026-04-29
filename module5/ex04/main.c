#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 115200
#define MYUBRR ((F_CPU / (8UL * BAUD)) - 1)  // U2X formula
//screen /dev/ttyUSB0 115200

//LED
void led_init()
{
	DDRB |= ((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4)); // set all led pin as output
}

void ft_gauge_led(uint8_t val)
{
	PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4));
	if(val < (64))
		return;
	PORTB |= (1 << PB0);
	if(val < (128))
		return;
	PORTB |= (1 << PB1);	
	if(val < (192))
		return;
	PORTB |= (1 << PB2);
	if(val < (255))
		return;
	PORTB |= (1 << PB4);
}

//RGB
void rgb_init()
{
	DDRD |= ((1 << PD3) | (1 << PD5) | (1 << PD6)); // PD3 BLUE. PD5 RED, PD6 VERT
	TCCR0A = (1 << COM0A1) | (1 << COM0A0) | (1 << COM0B1) | (1 << COM0B0) | (1 << WGM01) | (1 << WGM00); // Set inverting compare match on pin PD6 (OC0A) and PD5(OC0B) and turn on FAST PWM mode on timer 0
	TCCR2A = (1 << COM2B1) | (1 << COM2B0) | (1 << WGM21) | (1 << WGM20); // set inverting compare match on PD3 (OC2B) and turn on FAST PWM on timer 2
	TCCR0B = (1 << CS01) | (1 << CS00); //prescaler
	TCCR2B = (1 << CS22);//prescaler
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

//UART
// void uart_init(unsigned int ubrr)
// {
// 	UCSR0A = (1 << U2X0);
// 	UBRR0H = (unsigned char)(ubrr >> 8);
// 	UBRR0L = (unsigned char)ubrr;
// 	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
// 	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
// }

// void uart_tx(unsigned char c)
// {
// 	while (!(UCSR0A & (1 << UDRE0)));
// 	UDR0 = c;
// }

// void uart_printstr(const char *str)
// {
// 	while (*str)
// 		uart_tx(*str++);
// }

// void uart_putnbr(int16_t n)
// {
// 	if (n < 0)
// 	{
// 		uart_tx('-');
// 		n = -n;
// 	}
// 	if (n >= 10)
// 		uart_putnbr(n / 10);
// 	uart_tx('0' + (n % 10));
// }

//ADC
void	adc_switch(uint8_t id)
{
	ADMUX = (1 << ADLAR) | (1 << REFS0) | (id & 0x0f);
}

void adc_init()
{
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADATE); // ADEN = enable ADC || ADSPn = prescaler 128 p.249/p.259 || ADATE = Auto trigger > allow free running mode (constant conversion) (p.258)
	ADCSRB = 0;          // turn off ADCSRB to make sure free running mode is on;
	ADCSRA |= (1 << ADSC); //start first conversion
}

void init()
{
	// uart_init(MYUBRR);
	adc_init();
	rgb_init();
	adc_switch(0);
}

void main(void)
{
	init();
	
	while (1)
	{
		uint8_t val = ADCH;
		// uart_putnbr(res);
		// uart_printstr("\r\n");
		wheel(val);
		ft_gauge_led(val);
	}
}