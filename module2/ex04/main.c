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
void uart_init(unsigned int ubrr)
{
    UCSR0A = (1 << U2X0);                          // enable double speed
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);; //enable transmission and reception
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);       //set 8N1
    DDRB |= ((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4)); // set all led pin as output
}

int ft_strcmp(char *s1, char *s2)
{
    while (*s1 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return *s1 - *s2;
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

void uart_prompt(const char *prompt, char *str, uint8_t secret)
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
        else if(i < MAX_LEN - 1)
        {
            str[i++] = c;
            uart_tx(secret ? '*' : c);
        }
    } 
    str[i] = '\0';
    uart_printstr("\r\n");
    uart_printstr(str);
    uart_printstr("\r\n");
}
void led_effect(void)
{
    PORTB |= 0b00000001;
    _delay_ms(800);
    PORTB |= 0b0000011;
    _delay_ms(800);
    PORTB |= 0b0000111;
    _delay_ms(800);
    PORTB |= 0b0010111;
    _delay_ms(800);
    for(uint8_t i = 0; i < 10; i++)
    {
        PORTB &= ~0b00010111;
        _delay_ms(100);
        PORTB |= 0b00010111;
        _delay_ms(100);
    }
}
void main(void)
{
    uart_init(MYUBRR);
    char *username = USER;
    char *password = PASS;
    uint8_t logged = 0;
    while (1)
    {
        if(!logged)
        {
            char input_username[MAX_LEN];
            char input_password[MAX_LEN];
            uart_printstr("Enter your login:\r\n");
            uart_prompt("username: ",input_username, 0);
            uart_printstr(username);
            uart_printstr("\r\n");
            uart_prompt("password: ",input_password, 1);
            uart_printstr(password);
            uart_printstr("\r\n");
            if(ft_strcmp(input_username, username) == 0 && ft_strcmp(input_password, password) == 0)
            {
                logged = 1;
                uart_printstr("Welcome!\r\n");
                led_effect();
            } 
            else
                uart_printstr("Bad combinaison username/password\r\n");
        }
    }
}