#ifndef _VIDEOCORE_MAILBOX_LLD_H_
#define _VIDEOCORE_MAILBOX_LLD_H_

#include "bcm2835.h"
#include "chtypes.h"

/* Note: Also see https://github.com/dwelch67/raspberrypi for more information about interfacing to
   BCM2825 peripherals.*/

//#define REG(x) (*(volatile uint32_t *)(x))

// *****************************************************************************
//                        Mailbox
// *****************************************************************************

// -------- GPIO Registers --------
/* MAILBOX */
#define KERNEL_FB_LOC     (volatile unsigned int) 0x00002000
#define VC_MAIL_BASE 0x2000B880    // Base address for the mailbox registers

// This bit is set in the status register if there is no space to write into the mailbox
#define VC_MAIL_FULL 0x80000000
// This bit is set in the status register if there is nothing to read from the mailbox
#define VC_MAIL_EMPTY 0x40000000



#define VC_MAIL_READ REG(VC_MAIL_BASE)
#define VC_MAIL_STATUS REG(VC_MAIL_BASE + 0x18)
#define VC_MAIL_WRITE REG(VC_MAIL_BASE + 0x20)

#define VC_MAPPED_REGISTERS_BASE 0x20000000

// *****************************************************************************
//                 Support Functions
// *****************************************************************************

typedef enum {
	VideoCoreMailboxError_None = 0,
	VideoCoreMailboxError_MailboxEmpty,
	VideoCoreMailboxError_MailboxFull,
	VideoCoreMailboxError_NoMailOnRequestedChannel,
	VideoCoreMailboxError_Last
}VideoCoreMailboxError;

#ifdef __cplusplus
extern "C" {
#endif

  VideoCoreMailboxError videocore_mailbox_read(uint8_t channel, uint32_t* out);
  VideoCoreMailboxError videocore_mailbox_write(int8_t channel, uint32_t data);

#ifdef __cplusplus
  }
#endif

#endif
