//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#include "periph.h"

//-------------------------------------------------------------------------

void delay(unsigned int n)
{
	volatile unsigned int i = 0;
	for(i = 0; i < n; i++);
}

//------------------------------------------------------------------------
unsigned int uart_recv ( void )
{
    while((AUX_MU_LSR_REG & 0x01) == 0);
    return(AUX_MU_IO_REG & 0xFF);
}

//------------------------------------------------------------------------
void uart_send ( unsigned int c )
{
    while((AUX_MU_LSR_REG & 0x20) == 0);
    AUX_MU_IO_REG = c;
}

//------------------------------------------------------------------------
void uart_flush ( void )
{
    while(AUX_MU_LSR_REG & 0x100);
}

//------------------------------------------------------------------------

void gpio_setvalue(unsigned int gpio_pin, unsigned int value)
{
	if (value)
	{
    	unsigned int offset = gpio_pin / 32;
    	volatile unsigned int *gpset = &GPSET0 + offset;
    	unsigned int bit = gpio_pin - (offset * 32);   
    	unsigned int mask = 1 << bit;	
    	*gpset = mask;		
	}
	else 
	{
    	unsigned int offset = gpio_pin / 32;
    	volatile unsigned int *gpclr = &GPCLR0 + offset;
    	unsigned int bit = gpio_pin - (offset * 32); 
    	unsigned int mask = 1 << bit;	
    	*gpclr = mask;		
	}
}

void gpio_setmode(unsigned int gpio_pin, unsigned int mode)
{
    unsigned int gpfnbank = gpio_pin/10;
    unsigned int offset = (gpio_pin - gpfnbank * 10) * 3;
    volatile unsigned int *gpfnsel = &GPFSEL0 + offset;
	*gpfnsel = (*gpfnsel) & (mode << offset);
}

unsigned int uart_data_ready()
{
	return AUX_MU_LSR_REG & 0x01;
}

#define CLOCK_FREQ 250000000

unsigned int uart_baudrate(unsigned int baudrate) {
	return (CLOCK_FREQ / (8 * baudrate)) - 1;
}

void uart_init ( void )
{	
    AUX_ENABLES = 1;
    
    AUX_MU_IER_REG 	= 0x00;
    AUX_MU_CNTL_REG = 0x00;
    AUX_MU_LCR_REG 	= 0x03;
    AUX_MU_MCR_REG	= 0x00;
    AUX_MU_IER_REG 	= 0x05;
    AUX_MU_IIR_REG	= 0xC6; 
    AUX_MU_BAUD_REG = uart_baudrate(115200);
    
    gpio_setmode(14, GPFN_ALT5);
    gpio_setmode(15, GPFN_ALT5);
    
    GPPUD = 0;
    delay(150);
    GPPUDCLK0 = (1<<14)|(1<<15);
    delay(150);
    GPPUDCLK0 = 0;
    
    AUX_MU_CNTL_REG = 0x03;
}

unsigned int uart_rx_interrupt_pending( void )
{
  unsigned int iir = AUX_MU_IIR_REG;
  if((iir & 1)==1) return 0;
  return  (iir & 6) == 4;
}

//------------------------------------------------------------------------
void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x20);
}

//------------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}

//------------------------------------------------------------------------
void  timer_init ( void )
{
    //0xF9+1 = 250
    //250MHz/250 = 1MHz
    ARM_TIMER_CTL	= 0x00F90000;
    ARM_TIMER_CTL	= 0x00F90200;;
}
//-------------------------------------------------------------------------
unsigned int timer_ticks ( void )
{
    return(ARM_TIMER_CNT);
}

//-------------------------------------------------------------------------
//
// Copyright (c) 2012 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------
