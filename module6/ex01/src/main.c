#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "i2c.h"
#include "utils.h"

int main(void)
{	
	uart_init(MYUBRR);
	i2c_init();
	_delay_ms(100);
	while (1)
	{
		i2c_start();
		if(!i2c_clean_write(0x38 << 1 | 0, 0x18))
		{
			i2c_write(0x1B);
			i2c_write(0x00);
			i2c_write(0x1C);
			i2c_write(0x00);
			i2c_write(0x1E);
			i2c_write(0x00);
		}
		_delay_ms(10);
		i2c_clean_write(0xAC);
		i2c_stop();
	}
}