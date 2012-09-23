#ifndef _PERIPH_H_
#define _PERIPH_H_

typedef volatile unsigned int *memory;

#define GPFN_IN			0x00
#define GPFN_OUT		0x01
#define GPFN_ALT0		0x04
#define GPFN_ALT1		0x05
#define GPFN_ALT2		0x06
#define GPFN_ALT3		0x07
#define GPFN_ALT4		0x03
#define GPFN_ALT5		0x02

#define GPIO16_PORT		&gpio_port_1
#define GPIO16_PAD		16

#define GPSET0  	*((memory)0x2020001C)
#define GPSET1  	*((memory)0x20200020)
#define GPCLR0  	*((memory)0x20200028)
#define GPCLR1  	*((memory)0x2020002C)
#define GPLEV0		*((memory)0x20200034)
#define GPLEV1		*((memory)0x20200038)

void uart_init ( void );
void uart_flush ( void );
void uart_send ( unsigned int );
unsigned int uart_recv ( void );
unsigned int uart_data_ready( void );

void hexstring ( unsigned int );
void hexstrings ( unsigned int);

void gpio_setmode(unsigned int gpio_pin, unsigned int mode);
void gpio_setvalue(unsigned int gpio_pin, unsigned int value);

void  timer_init ( void );
unsigned int timer_ticks ( void );

void delay( unsigned int );

#endif

