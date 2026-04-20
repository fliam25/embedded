#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 115200
#define MYUBRR ((F_CPU / (8UL * BAUD)) - 1)  // U2X formula
#define PASS "mdp"
#define USER "user"
#define MAX_LEN 30
//screen /dev/ttyUSB0 115200

void init_rgb()
{
    DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6); // PD3 BLUE. PD5 RED, PD6 VERT
    TCCR0A = (1 << COM0A1) | (1 << COM0A0)         // Set inverting compare match on pin PD6 (OC0A)
           | (1 << COM0B1) | (1 << COM0B0)         // Set inverting compare match on pin PD5 (OC0B)
           | (1 << WGM01)  | (1 << WGM00);         // turn on FAST PWM mode on timer 0
    TCCR2A = (1 << COM2B1) | (1 << COM2B0)         // set inverting compare match on PD3 (OC2B)
           | (1 << WGM21)  | (1 << WGM20);         // turn on FAST PWM on timer 2
    TCCR0B = (1 << CS01) | (1 << CS00);            // prescaler
    TCCR2B = (1 << CS22);                          // prescaler
}

void uart_init(unsigned int ubrr)
{
    UCSR0A = (1 << U2X0);                          // enable double speed
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);; //enable transmission and reception
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);       //set 8N1
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    OCR0B = 255 - r;
    OCR0A = 255 - g;
    OCR2B = 255 - b;
}

unsigned char uart_rx(void)
{
    while ( !(UCSR0A & (1<<RXC0)) );
    return UDR0;
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

uint8_t ft_strcheck(char *str)
{
  uint8_t i = 0;
  while(str[i])
  {
    if(i > 0 && !((str[i] >= 'a' && str[i] <= 'f') || (str[i] >= 'A' && str[i] <= 'F') || (str[i] >= '0' && str[i] <= '9')))
      return 0;
    i++;
  }
  return i;
}

uint8_t hex_to_int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

void process_imput(char *input)
{
  uint8_t r , g , b;
  if(ft_strcheck(input) != 7 || input[0] != '#')
  {
    uart_printstr("Wrong color format");
    uart_printstr("\r\n");
    return;
  }
  r = (hex_to_int(input[1]) << 4) | (hex_to_int(input[2]));
  g = (hex_to_int(input[3]) << 4) | (hex_to_int(input[4]));
  b = (hex_to_int(input[5]) << 4) | (hex_to_int(input[6]));
  set_rgb(r, g , b);
}

void uart_prompt(const char *prompt, char *str)
{
    uart_printstr(prompt);
    char c;
    uint8_t i = 0;
    while ((c = uart_rx()) != '\r')
    {
        if(c == 127 || c == '\b')
        {
            if (i > 0)
            {
                i--;
                uart_printstr("\b \b");
            }
        }
        else if (c >= 0x20 && c <= 0x7E && i < 8 - 1)
        {
            str[i++] = c;
            uart_tx(c);
        }
    } 
    str[i] = '\0';
    uart_printstr("\r\n");
}

void main(void)
{
    init_rgb();
    set_rgb(0, 0, 0);
    uart_init(MYUBRR);
    while (1)
    {
      char colorcode_hexa[8];
      uart_prompt("Enter a color code (#RRGGBB): ",colorcode_hexa);
      process_imput(colorcode_hexa);
    }
}