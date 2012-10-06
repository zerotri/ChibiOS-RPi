#include "bcm2835.h"

/**
 * @brief Set GPIO pin function/mode. Used in multiple peripherals.
 *
 * @param[in] gpio_pin       BCM2835 pin number
 * @param[in] mode          GPIO pin function
 *
 * @notapi
 */
void bcm2835_gpio_fnsel(unsigned int gpio_pin, unsigned int gpio_fn)
{
  unsigned int gpfnbank = gpio_pin/10;
  unsigned int offset = (gpio_pin - (gpfnbank * 10)) * 3;
  volatile unsigned int *gpfnsel = &GPFSEL0 + gpfnbank;
  *gpfnsel |= (gpio_fn << offset);
}
