#include "utils.h"
#include "uart.h"

void ft_8inttohex(uint8_t val, uint8_t prefix)
{
	if(prefix == 1)
		uart_printstr("0x");

	const char *hex = "0123456789abcdef";

	uart_tx(hex[val >> 4]);
	uart_tx(hex[val & 0x0F]);

	uart_printstr("\r\n");
}