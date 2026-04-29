#ifndef I2C_H
#define I2C_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_write(unsigned char data);
uint8_t i2c_status();
void	i2c_read(void);
uint8_t i2c_clean_write(unsigned char data, uint8_t code);

#endif