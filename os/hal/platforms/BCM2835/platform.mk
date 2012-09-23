# List of all the BCM2835 platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/platforms/BCM2835/hal_lld.c \
              ${CHIBIOS}/os/hal/platforms/BCM2835/pal_lld.c \
             ${CHIBIOS}/os/hal/platforms/BCM2835/serial_lld.c \
             ${CHIBIOS}/os/hal/platforms/BCM2835/periph.c \


# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/platforms/BCM2835
