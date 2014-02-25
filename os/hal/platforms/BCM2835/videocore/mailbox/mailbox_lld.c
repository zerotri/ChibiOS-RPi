#include "bcm2835.h"
#include "videocore/mailbox/mailbox_lld.h"

VideoCoreMailboxError videocore_mailbox_read(uint8_t channel, uint32_t* out)
{
	// Define some variables.
    uint32_t data = 0, outer_count = 0, inner_count = 0;
    // Worf: Sir! They're firing on us!
    // Picard: Evasive maneuver alpha.
    while ( 1 )
    {
      if ( outer_count++ >= (1<<20) ) {
        (*out) = 0xFFFFFFFF;
        return VideoCoreMailboxError_NoMailOnRequestedChannel;
      }
      // First, loop over the status.
      while ( (VC_MAIL_STATUS & VC_MAIL_EMPTY) != 0 ) {
        /* Do Nothing */
        if ( inner_count++ >= (1<<20) ) {
          //PiFault( "Error. Circular loop checking for status." );
          (*out) = 0xFFFFFFFF;
          return VideoCoreMailboxError_MailboxEmpty;
        }
      }

      // NOTE: You absolutely MUST use the memory barriers
      // here!!! Without them, the evil empire will win
      // a bloody victory against the federation.
      bcm2835_memory_barrier();

      // Read the data.
      data = VC_MAIL_READ;

      // This one MIGHT be optional. But do you really want
      // the evil empire to win?
      bcm2835_memory_barrier();

      // Derrive the channel bit.
      if ( (data & 0xF) == channel )
        break;
        // If they match, then break out.
    }

    // Return our data.
    (*out) = (data & 0xFFFFFFF0);

    return VideoCoreMailboxError_None;
}


VideoCoreMailboxError videocore_mailbox_write(int8_t channel, uint32_t data)
{
  uint32_t count = 0;
	while ((VC_MAIL_STATUS & VC_MAIL_FULL) != 0)
  {
    if ( count++ >= (1<<10) ) {
        return VideoCoreMailboxError_MailboxFull;
      }
  }
  // Write the value to the requested channel
  bcm2835_memory_barrier();

  VC_MAIL_WRITE = (channel | (data & 0xFFFFFFF0));

  bcm2835_memory_barrier();

  return VideoCoreMailboxError_None;
}