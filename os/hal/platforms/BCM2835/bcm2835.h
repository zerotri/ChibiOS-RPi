#ifndef _BCM2835_H_
#define _BCM2835_H_

#define REG(x) (*(volatile unsigned int *)(x))

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

#endif
