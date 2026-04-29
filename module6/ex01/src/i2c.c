
#include "i2c.h"
#include "utils.h"
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
	uint8_t status = (TWSR & 0xF8); // save status code
	ft_8inttohex(status , 1);
	return status;
}


void i2c_write(unsigned char data)
{
	//TWDR = 0x38 << 1 | 0; // SLA_W (cf adress in AHT20 datasheet p11) | 0 for write | 1 for read
	// TWCR = (1<<TWINT) | (1<<TWEN); // execute transsmision and keep i2c enabled
	// while (!(TWCR & (1<<TWINT)));
	// if (i2c_status() != 0x18) // check that SLA_W went good
	// 	return;
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	
}

void i2c_read(void)
{

}

uint8_t i2c_clean_write(unsigned char data, uint8_t code)
{
	i2c_write(data);
	if(i2c_status() != code)
		return 0;
	return 1;
}

void i2c_init()
{
	TWSR = 0; // no prescaler 
	TWBR = 72; //16Mhz / 16 + 2(72) x 1(no prescaler) = 100 000 (P.222)
	TWCR = (1 << TWEN); //enable I2C

}

void i2c_start()
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); // p.224/225 // TWSTA= Generate start condition , TWEN = keep I2c enabled and TWINT = execute and go to next operation
	while (!(TWCR & (1<<TWINT)));
	if (i2c_status() != 0x08) // check that start went good
		return;
}

void i2c_stop()
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO); // TWSTO stop communication
}