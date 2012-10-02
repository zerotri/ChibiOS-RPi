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
 * @file    BCM2835/i2c_lld.c
 * @brief   I2C Driver subsystem low level driver source template.
 *
 * @addtogroup I2C
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "chprintf.h"

#if HAL_USE_I2C || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

I2CDriver I2C0;

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void i2c_init(I2CDriver *i2c_driver, bscdevice_t *device_address) {
  i2c_driver->callback = NULL;
  i2c_driver->device = device_address;
  i2cObjectInit(&I2C0);
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level I2C driver initialization.
 *
 * @notapi
 */
void i2c_lld_init(void) {
  i2c_init(&I2C0, BSC0_ADDR);
}

/**
 * @brief   Configures and activates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_start(I2CDriver *i2cp) {
  if (i2cp->state == I2C_STOP) {
    /* Configuration */
    /* Set up GPIO pins for I2C */
    gpio_setmode(i2cp->config->ic_pin, GPFN_ALT0);
    gpio_setmode(i2cp->config->ic_pin + 1, GPFN_ALT0);
    i2cp->device->control |= BSC_I2CEN;
  }
}

/**
 * @brief   Deactivates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_stop(I2CDriver *i2cp) {
  if (i2cp->state == I2C_READY) {
    /* Set GPIO pin function to default */
    gpio_setmode(i2cp->config->ic_pin, GPFN_IN);
    gpio_setmode(i2cp->config->ic_pin + 1, GPFN_IN);
    i2cp->device->control &= ~BSC_I2CEN;
  }
}

/**
 * @brief   Master transmission.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] addr      slave device address (7 bits) without R/W bit
 * @param[in] txbuf     transmit data buffer pointer
 * @param[in] txbytes   number of bytes to be transmitted
 * @param[out] rxbuf     receive data buffer pointer
 * @param[in] rxbytes   number of bytes to be received
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 *
 * @notapi
 */
msg_t i2c_lld_master_transmit_timeout(I2CDriver *i2cp, i2caddr_t addr, 
                                       const uint8_t *txbuf, size_t txbytes, 
                                       uint8_t *rxbuf, const uint8_t rxbytes, 
                                       systime_t timeout) {
  if (i2cp->state == I2C_READY) {
    size_t i = 0;
    const uint8_t *b = txbuf;

    i2cp->device->slaveAddress = addr;
    i2cp->device->clockStretchTimeout = 100000;
    i2cp->device->dataLength = txbytes;

    while (i++ < txbytes) {
      while (!(i2cp->device->status & BSC_TXD));
      i2cp->device->dataFifo = *(b++);
    }

    i2cp->device->status = CLEAR_STATUS; // handle enable bit better
    i2cp->device->control = START_WRITE;

    while (!(i2cp->device->status & BSC_DONE));
    i2cp->device->status |= BSC_DONE;

    if (rxbytes > 0) {
      i2c_lld_master_receive_timeout(i2cp, addr, rxbuf, rxbytes, timeout);
    }
  }

  return 0;
}


/**
 * @brief   Master receive.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] addr      slave device address (7 bits) without R/W bit
 * @param[out] rxbuf     receive data buffer pointer
 * @param[in] rxbytes   number of bytes to be received
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 *
 * @notapi
 */
msg_t i2c_lld_master_receive_timeout(I2CDriver *i2cp, i2caddr_t addr, 
                                       uint8_t *rxbuf, size_t rxbytes, 
                                       systime_t timeout) {
  UNUSED(addr);

  if (i2cp->state == I2C_READY) {
    size_t i = 0;
    uint8_t *b = rxbuf;

    i2cp->device->slaveAddress = addr;
    i2cp->device->clockStretchTimeout = 100000;
    i2cp->device->dataLength = rxbytes;

    i2cp->device->status = CLEAR_STATUS;
    i2cp->device->control = START_READ;
    while (!(i2cp->device->status & BSC_DONE));

    systime_t max_time = chTimeNow() + timeout;
    while (i++ < rxbytes) {
      while (!(i2cp->device->status & BSC_RXD)) {
        if (chTimeNow() > max_time) {
          return 0;
	}
      }
      *(b++) = i2cp->device->dataFifo;
    }
  }

  return 0;
}

#endif /* HAL_USE_I2C */

/** @} */
