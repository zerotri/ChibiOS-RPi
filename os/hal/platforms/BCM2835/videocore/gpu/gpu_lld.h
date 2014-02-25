#ifndef _VIDEOCORE_GPU_LLD_H_
#define _VIDEOCORE_GPU_LLD_H_
#include "bcm2835.h"
#include "chtypes.h"




/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

typedef struct {
    uint32_t screen_width;
    uint32_t screen_height;
    uint32_t virtual_width;
    uint32_t virtual_height;
    uint32_t pitch;
    uint32_t depth;
    uint32_t xoffset;
    uint32_t yoffset;
    uint32_t framePtr;
    uint32_t bufferSize;
    uint32_t valid;
}VideoCoreGPUFrameBufferInfo;

typedef enum {
    VideoCoreGPUError_None = 0,
    VideoCoreGPUError_MailboxError,
    VideoCoreGPUError_InvalidFramebuffer,
    VideoCoreGPUError_InvalidPitch,
    VideoCoreGPUError_Last
}VideoCoreGPUError;

#ifdef __cplusplus
extern "C" {
#endif

VideoCoreGPUError videocore_gpu_framebuffer_request(VideoCoreGPUFrameBufferInfo* request);

#ifdef __cplusplus
  }
#endif

#endif