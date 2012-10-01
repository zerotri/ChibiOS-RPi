#ifndef _BCM2835_H_
#define _BCM2835_H_

/* Note: Also see https://github.com/dwelch67/raspberrypi for more information about interfacing to
   BCM2825 peripherals.*/

#define REG(x) (*(volatile unsigned int *)(x))
#define BIT(n) (1 << (n))

#define BCM2835_CLOCK_FREQ 250000000

// *****************************************************************************
//                        General Purpose I/O (GPIO)
// *****************************************************************************

// -------- GPIO Registers --------
#define GPSET0  	REG(0x2020001C)
#define GPSET1  	REG(0x20200020)
#define GPCLR0  	REG(0x20200028)
#define GPCLR1  	REG(0x2020002C)
#define GPLEV0		REG(0x20200034)
#define GPLEV1		REG(0x20200038)

#define GPFSEL0 	REG(0x20200000)
#define GPFSEL1 	REG(0x20200004)
#define GPFSEL2 	REG(0x20200008)
#define GPFSEL3 	REG(0x2020000C)
#define GPFSEL4 	REG(0x20200010)
#define GPFSEL6 	REG(0x20200014)

#define GPPUD       REG(0x20200094)
#define GPPUDCLK0   REG(0x20200098)

// -------- GPIO Functions --------
#define GPFN_IN	  	0x00
#define GPFN_OUT   	0x01
#define GPFN_ALT0  	0x04
#define GPFN_ALT1  	0x05
#define GPFN_ALT2  	0x06
#define GPFN_ALT3  	0x07
#define GPFN_ALT4  	0x03
#define GPFN_ALT5  	0x02

// ----- GPIO - Onboard LED -------
#define GPIO16_PORT		&gpio_port_1
#define GPIO16_PAD		16

void gpio_setmode(unsigned int gpio_pin, unsigned int mode);

// *****************************************************************************
//                          Timer (ARM Side)
// *****************************************************************************

#define ARM_TIMER_LOD REG(0x2000B400)
#define ARM_TIMER_VAL REG(0x2000B404)
#define ARM_TIMER_CTL REG(0x2000B408)
#define ARM_TIMER_CLI REG(0x2000B40C)
#define ARM_TIMER_RIS REG(0x2000B410)
#define ARM_TIMER_MIS REG(0x2000B414)
#define ARM_TIMER_RLD REG(0x2000B418)
#define ARM_TIMER_DIV REG(0x2000B41C)
#define ARM_TIMER_CNT REG(0x2000B420)

// *****************************************************************************
//                        System Timer
// *****************************************************************************

#define SYSTIMER_CS    REG(0x20003000)
#define SYSTIMER_CLO   REG(0x20003004)

// *****************************************************************************
//                         AUX Registers
// *****************************************************************************

#define AUX_ENABLES     REG(0x20215004)

// --- Mini UART Registers -----
#define AUX_MU_IO_REG   REG(0x20215040)
#define AUX_MU_IER_REG  REG(0x20215044)
#define AUX_MU_IIR_REG  REG(0x20215048)
#define AUX_MU_LCR_REG  REG(0x2021504C)
#define AUX_MU_MCR_REG  REG(0x20215050)
#define AUX_MU_LSR_REG  REG(0x20215054)
#define AUX_MU_MSR_REG  REG(0x20215058)
#define AUX_MU_SCRATCH  REG(0x2021505C)
#define AUX_MU_CNTL_REG REG(0x20215060)
#define AUX_MU_STAT_REG REG(0x20215064)
#define AUX_MU_BAUD_REG REG(0x20215068)

// *****************************************************************************
//                        Interrupts
// *****************************************************************************

#define IRQ_BASIC        REG(0x2000B200)
#define IRQ_PEND1        REG(0x2000B204)
#define IRQ_PEND2        REG(0x2000B208)
#define IRQ_FIQ_CONTROL  REG(0x2000B210)
#define IRQ_ENABLE1      REG(0x2000B210)
#define IRQ_ENABLE2      REG(0x2000B214)
#define IRQ_ENABLE_BASIC REG(0x2000B218)
#define IRQ_DISABLE1     REG(0x2000B21C)
#define IRQ_DISABLE2     REG(0x2000B220)

// *****************************************************************************
//                 Broadcom Serial Controllers (I2C)
// *****************************************************************************

struct bscdevice_t {
  volatile unsigned int control;
  volatile unsigned int status; 
  volatile unsigned int dataLength;
  volatile unsigned int slaveAddress;
  volatile unsigned int dataFifo;
  volatile unsigned int clockDivider;
  volatile unsigned int dataDelay;
  volatile unsigned int clockStretchTimeout;
};

typedef struct bscdevice_t bscdevice_t;

/* Only BSC0 is accessible from the RPi pi header.*/
#define BSC0_ADDR ((bscdevice_t *)0x20205000)
#define BSC1_ADDR ((bscdevice_t *)0x20804000)
#define BSC2_ADDR ((bscdevice_t *)0x20805000)

/* I2C control flags */
#define BSC_I2CEN BIT(15)
#define BSC_INTR  BIT(10)
#define BSC_INTT  BIT(9)
#define BSC_INTD  BIT(8)
#define BSC_ST    BIT(7)
#define BSC_CLEAR BIT(4)
#define BSC_READ  BIT(0)

/* I2C status flags */
#define BSC_TA   BIT(0) /** @brief Transfer active.*/
#define BSC_DONE BIT(1) /** @brief Transfer done.*/
#define BSC_TXW  BIT(2) /** @brief FIFO needs writing.*/
#define BSC_RXR  BIT(3) /** @brief FIFO needs reading.*/
#define BSC_TXD  BIT(4) /** @brief FIFO can accept data.*/
#define BSC_RXD  BIT(5) /** @brief FIFO contains data.*/
#define BSC_TXE  BIT(6) /** @brief FIFO empty.*/
#define BSC_RXF  BIT(7) /** @brief FIFO full.*/
#define BSC_ERR  BIT(8) /** @brief ACK error.*/
#define BSC_CLKT BIT(9) /** @brief Clock stretch timeout.*/

#define CLEAR_STATUS  BSC_CLKT|BSC_ERR|BSC_DONE

#define START_READ    BSC_I2CEN|BSC_ST|BSC_CLEAR|BSC_READ
#define START_WRITE   BSC_I2CEN|BSC_ST

#endif
