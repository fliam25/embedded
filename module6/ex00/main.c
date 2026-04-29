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

//tools
void ft_8inttohex(uint8_t val, uint8_t prefix)
{
	if(prefix == 1)
		uart_printstr("0x");
	const char *hex = "0123456789abcdef";
	uart_tx(hex[val >> 4]); // first char
	uart_tx(hex[val & 0b00001111]); // second char
	uart_printstr("\r\n");
}

//I2C

/*status code 
0x08 = Start
0x10 = Repeated start (started again without stopping, common when switching from write to read)
0x18 = Sla_W(master will write to slave) transmitted and slave awnsered with ACK | 0x20 = Sla_W transmitted but slave didnt awnser (NACK)
0x40 = Sla_R(master will read from slave) transmitted and slave awnsered with ACK | 0x48 = Sla_R transmitted but slave didnt awnser (NACK)
0x28 = Data sent to slave and slave awnser with ACK | 0x30 Data sent to slave but no awnser (NACK)
0x50 = Master received data from slave and M ask for more (ACK)| 0x58 = Master received last byte of data from slave and it awnser with (NACK)
*/

/*Vocabulary
	TWI(I2c) = Two Wire Serial Interface (communication with two wire, one for clock one for data)
	TWSR = TWI Status Register (P.240)
	TWCR = TWI control register (P.239)
	TWDR = TWI Data Register (P.241)
	TWBR = TWI Bit Rate Register (P.239)
*/

uint8_t i2c_status()
{
	while (!(TWCR & (1<<TWINT)));
	uint8_t status = (TWSR & 0xF8); // save status code
	ft_8inttohex(status , 1);
	return status;
}

void i2c_init()
{
	TWSR = 0; // no prescaler 
	TWBR = 72; //16Mhz / 16 + 2(72) x 1(no prescaler) = 100 000 (P.222)
	TWCR = (1 << TWEN); //enable I2C

}

void i2c_write(unsigned char data)
{
	TWDR = 0x38 << 1 | 0; // SLA_W (cf adress in AHT20 datasheet p11) | 0 for write | 1 for read
	TWCR = (1<<TWINT) | (1<<TWEN); // execute transsmision and keep i2c enabled
	if (i2c_status() != 0x18)
		error;
}

void i2c_start()
{
	
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); // p.224/225 // TWSTA= Generate start condition , TWEN = keep I2c enabled and TWINT = execute and go to next operation
	if (i2c_status() != 0x08) // check that start went good
		return;
	

}

void i2c_stop()
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO); // TWSTO stop communication
}

void main(void)
{	
	uart_init(MYUBRR);
	i2c_init();
	i2c_start();
	i2c_stop();
	while (1)
	{
	}
}