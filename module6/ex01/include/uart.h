#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#define BAUD 115200
#define MYUBRR ((F_CPU / (8UL * BAUD)) - 1)  // U2X formula

void uart_init(unsigned int ubrr);
void uart_tx(unsigned char c);
void uart_printstr(const char *str);

#endif