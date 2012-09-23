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
 * @file    templates/pal_lld.c
 * @brief   PAL subsystem low level driver template.
 *
 * @addtogroup PAL
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "periph.h"

#if HAL_USE_PAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

gpio_port_t gpio_port_1;
gpio_port_t gpio_port_2;

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

void _pal_lld_init(const PALConfig* config) {

	(void)config;
	
	gpio_port_1.gpset = &GPSET0;
	gpio_port_1.gpclr = &GPCLR0;
	gpio_port_1.gplev = &GPLEV0;

	gpio_port_2.gpset = &GPSET1;
	gpio_port_2.gpclr = &GPCLR1;
	gpio_port_2.gplev = &GPLEV1;
	gpio_port_2.pin_base = 32;
}

void _pal_lld_setgroupmode(ioportid_t port, ioportmask_t mask, uint32_t mode) {
  int i;
  switch (mode) {
	  case PAL_MODE_INPUT:
		for (i = 0; i < 32; i++) {
		   unsigned int bit = mask & 1;
		   if (bit) gpio_setmode(i + port->pin_base, GPFN_IN);
		   mask >>= 1;
		}
		break;
	  case PAL_MODE_OUTPUT_PUSHPULL:
	  case PAL_MODE_OUTPUT_OPENDRAIN:
		for (i = 0; i < 32; i++) {
		   unsigned int bit = mask & 1;
		   if (bit) gpio_setmode(i + port->pin_base, GPFN_OUT);
		   mask >>= 1;
		}
		break;
    }
}

void _pal_lld_writeport(ioportid_t port, ioportmask_t bits) {
	*(port->gpset) = port->latch = bits;
	*(port->gpclr) = ~bits;
}

#endif /* HAL_USE_PAL */

/** @} */
