#include "bcm2835.h"

/**
 * @brief Set GPIO pin function/mode. Used in multiple peripherals.
 *
 * @param[in] gpio_pin       BCM2835 pin number
 * @param[in] mode          GPIO pin function
 *
 * @notapi
 */
void bcm2835_gpio_fnsel(uint32_t gpio_pin, uint32_t gpio_fn)
{
  uint32_t gpfnbank = gpio_pin/10;
  uint32_t offset = (gpio_pin - (gpfnbank * 10)) * 3;
  volatile uint32_t *gpfnsel = &GPFSEL0 + gpfnbank;
  *gpfnsel |= (gpio_fn << offset);
}
