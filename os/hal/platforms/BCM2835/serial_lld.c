/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    templates/serial_lld.c
 * @brief   Serial Driver subsystem low level driver source template.
 *
 * @addtogroup SERIAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

SerialDriver SD1;

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 */
static const SerialConfig default_config = {
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void output_notify(GenericQueue *qp) {
    UNUSED(qp);

	while (1) {
    	chSysLockFromIsr();
    	int ch = sdRequestDataI(&SD1);
    	chSysUnlockFromIsr();
    	if (ch == Q_EMPTY) break;
    	mini_uart_send((unsigned int)ch);
    }
}

static unsigned int mini_uart_baudrate(unsigned int baudrate) {
	return (BCM2835_CLOCK_FREQ / (8 * baudrate)) - 1;
}


static unsigned int mini_uart_rx_interrupt_pending( void )
{
  unsigned int iir = AUX_MU_IIR_REG;
  if((iir & 1)==1) return 0;
  return  (iir & 6) == 4;
}

static void delay(unsigned int n)
{
	volatile unsigned int i = 0;
	for(i = 0; i < n; i++);
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

bool_t sd_lld_handle_interrupts( SerialDriver *sdp ) {
    if (mini_uart_rx_interrupt_pending()) {
		chSysLockFromIsr();
		do {			
			sdIncomingDataI(sdp, AUX_MU_IO_REG & 0xFF);
		} while (mini_uart_rx_interrupt_pending());
		chSysUnlockFromIsr();
		return TRUE;
	}
	
    return FALSE;
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level serial driver initialization.
 *
 * @notapi
 */
void sd_lld_init(void) {
  sdObjectInit(&SD1, NULL, output_notify);

  IRQ_DISABLE1 |= 1<<29;

  mini_uart_init(115200);

  IRQ_ENABLE1 |= 1<<29;
}

/**
 * @brief   Low level serial driver configuration and (re)start.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] config    the architecture-dependent serial driver configuration.
 *                      If this parameter is set to @p NULL then a default
 *                      configuration is used.
 *
 * @notapi
 */
void sd_lld_start(SerialDriver *sdp, const SerialConfig *config) {

  (void)sdp;
  if (config == NULL)
    config = &default_config;

}

/**
 * @brief   Low level serial driver stop.
 * @details De-initializes the USART, stops the associated clock, resets the
 *          interrupt vector.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 *
 * @notapi
 */
void sd_lld_stop(SerialDriver *sdp) {
  (void)sdp;
}

void mini_uart_init ( unsigned int baud )
{	
    AUX_ENABLES = 1;
  
  AUX_MU_IER_REG 	= 0x00;
  AUX_MU_CNTL_REG = 0x00;
  AUX_MU_LCR_REG 	= 0x03;
  AUX_MU_MCR_REG	= 0x00;
  AUX_MU_IER_REG 	= 0x05;
  AUX_MU_IIR_REG	= 0xC6; 

  // TODO make baud rate configurable
  AUX_MU_BAUD_REG = mini_uart_baudrate(baud);
  
  gpio_setmode(14, GPFN_ALT5);
  gpio_setmode(15, GPFN_ALT5);
  
  GPPUD = 0;
  delay(150);
  GPPUDCLK0 = (1<<14)|(1<<15);
  delay(150);
  GPPUDCLK0 = 0;
  
  AUX_MU_CNTL_REG = 0x03;
}

unsigned int mini_uart_recv ( void )
{
  while((AUX_MU_LSR_REG & 0x01) == 0);
  return(AUX_MU_IO_REG & 0xFF);
}

void mini_uart_send ( unsigned int c )
{
  while((AUX_MU_LSR_REG & 0x20) == 0);
  AUX_MU_IO_REG = c;
}

void mini_uart_sendhex ( unsigned int d )
{
  unsigned int rb;
  unsigned int rc;

  rb=32;
  while(1)
  {
    rb-=4;
    rc=(d>>rb)&0xF;
    if(rc>9)
      rc+=0x37; 
    else 
      rc+=0x30;
    mini_uart_send(rc);
    if(rb==0) break;
  }
  mini_uart_send(0x20);
}

void mini_uart_sendhxln ( unsigned int d )
{
  mini_uart_sendhex(d);
  mini_uart_send(0x0D);
  mini_uart_send(0x0A);
}

#endif /* HAL_USE_SERIAL */

/** @} */
