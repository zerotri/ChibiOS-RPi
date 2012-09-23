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
#include "periph.h"

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

bool_t logged;
bool_t logged2;

static bool_t input_interrupt(SerialDriver* sdp) {
    UNUSED(sdp);

    while (uart_data_ready()) {
        chSysLockFromIsr();
        sdIncomingDataI(sdp, uart_recv());
        chSysUnlockFromIsr();
        return TRUE;
    }
	
    return FALSE;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

static void output_notify(GenericQueue *qp) {
    UNUSED(qp);

	while (1) {
    	chSysLockFromIsr();
    	int ch = sdRequestDataI(&SD1);
    	chSysUnlockFromIsr();
    	if (ch == Q_EMPTY) break;
    	uart_send((unsigned int)ch);
    }
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
  uart_init();
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

bool_t sd_lld_data_ready(void) {
  bool_t b;

  //CH_IRQ_PROLOGUE();

  b =  input_interrupt(&SD1);

  //CH_IRQ_EPILOGUE();

  return b;
}

#endif /* HAL_USE_SERIAL */

/** @} */
