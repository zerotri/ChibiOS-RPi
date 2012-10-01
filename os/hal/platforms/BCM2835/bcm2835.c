#include "bcm2835.h"

/**
 * @brief Set GPIO pin function/mode. Used in multiple peripherals.
 *
 * @notapi
 */
void gpio_setmode(unsigned int gpio_pin, unsigned int mode)
{
  unsigned int gpfnbank = gpio_pin/10;
  unsigned int offset = (gpio_pin - (gpfnbank * 10)) * 3;
  volatile unsigned int *gpfnsel = &GPFSEL0 + gpfnbank;
  *gpfnsel |= (mode << offset);
}
