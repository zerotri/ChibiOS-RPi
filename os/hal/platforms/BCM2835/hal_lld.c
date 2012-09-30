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
 * @file    templates/hal_lld.c
 * @brief   HAL Driver subsystem low level driver source template.
 *
 * @addtogroup HAL
 * @{
 */

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief Start the system timer
 *
 * @notapi
 */
static void systimer_init( void )
{
  // 1 MHz clock, Counter=1000, 1 ms tick
  ARM_TIMER_CTL = 0x003E0000;
  ARM_TIMER_LOD = 1000-1;
  ARM_TIMER_RLD = 1000-1;
  ARM_TIMER_DIV = 0x000000F9;
  ARM_TIMER_CLI = 0;
  ARM_TIMER_CTL = 0x003E00A2;

  IRQ_ENABLE_BASIC = 1;
}

/**
 * @brief Process system timer interrupts, if present.
 *
 * @notapi
 */
static void systimer_handle_interrupts( void )
{
  // Update the system time
  chSysLockFromIsr();
  chSysTimerHandlerI();
  chSysUnlockFromIsr();

  // Clear timer interrupt
  ARM_TIMER_CLI = 0;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief Interrupt handler
 *
 */
CH_IRQ_HANDLER(IrqHandler)
{
  CH_IRQ_PROLOGUE();

  sd_lld_handle_interrupts(&SD1);
  systimer_handle_interrupts();

  CH_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level HAL driver initialization.
 *
 * @notapi
 */
void hal_lld_init(void) {
  systimer_init();
}

/** @} */
