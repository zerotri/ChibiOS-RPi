#include "videocore/gpu/gpu_lld.h"
#include "videocore/mailbox/mailbox_lld.h"

#define ARM_BUS_LOCATION    (unsigned int) 0x40000000

uint32_t PHYSICAL_TO_BUS( uint32_t p ) {
  return (uint32_t)(p | ARM_BUS_LOCATION );
}

// And in those special cases, the RPI
// will return a value that ALSO needs
// to be 're-mapped'.
uint32_t BUS_TO_PHYSICAL( uint32_t p ) {
  if ( p > ARM_BUS_LOCATION ) 
    return (uint32_t)(p - ARM_BUS_LOCATION );
  return (uint32_t)p;
}

VideoCoreGPUError videocore_gpu_framebuffer_request(VideoCoreGPUFrameBufferInfo* request)
{

	// Snag the pointer value in uint32 form.
	uint32_t requestAddress = PHYSICAL_TO_BUS((uint32_t)request);

	// And then write a letter to our dearest GPU.
	VideoCoreMailboxError error = videocore_mailbox_write( 1, requestAddress );

	if(error != VideoCoreMailboxError_None)
		return VideoCoreGPUError_MailboxError;

	// And then read the response... Really read it.
	uint32_t response = 0xFF, explode = 100;
	do {
		error = videocore_mailbox_read( 1, &response);
		if(error != VideoCoreMailboxError_None)
			return VideoCoreGPUError_MailboxError;
	} while ( response != 0 && explode-- > 0 ) ;

	// Check if we've exploded.
	if ( explode <= 0 ) {
		//chprintf(chp, "Error! The mailbox didn't return a suitable value in a timely manner.\r\n" );
		request->valid = 0;
		return VideoCoreGPUError_MailboxError;// request;
	}

	// Check the meaning of the response.
	if ( request->framePtr == 0 ) {
		//chprintf(chp, "Error! The framebuffer returned is invalid. Aborting framebuffer acquisition\r\n" );
		request->valid = 0;
		return VideoCoreGPUError_InvalidFramebuffer;// request;
	}

	if ( request->pitch == 0 ) {
		//chprintf(chp, "Error! The pitch returned is invalid. Aborting framebuffer acquisition.\r\n" );
		request->valid = 0;
		return VideoCoreGPUError_InvalidPitch;// request;
	}

	// Initialize the success variables.
	request->valid = 1;
	request->framePtr = BUS_TO_PHYSICAL( request->framePtr );
	return VideoCoreGPUError_None;
}