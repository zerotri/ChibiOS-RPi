#ifndef _PERIPH_H_
#define _PERIPH_H_

#include "bcm2835.h"

void uart_init ( void );
void uart_flush ( void );
void uart_send ( unsigned int );
unsigned int uart_recv ( void );
unsigned int uart_data_ready( void );

unsigned int uart_rx_interrupt_pending( void );

void hexstring ( unsigned int );
void hexstrings ( unsigned int);

void gpio_setmode(unsigned int gpio_pin, unsigned int mode);
void gpio_setvalue(unsigned int gpio_pin, unsigned int value);

void  timer_init ( void );
unsigned int timer_ticks ( void );

void delay( unsigned int );

#endif

