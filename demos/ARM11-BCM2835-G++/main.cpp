/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ch.hpp"
#include "hal.h"
#include "shell.h"
#include "chprintf.h"
#include "videocore/mailbox/mailbox_lld.h"
#include "videocore/gpu/gpu_lld.h"


__attribute__ ((aligned (16))) VideoCoreGPUFrameBufferInfo g_fbInfo = {1920, 1080, 1920, 1080, 0, 24, 0, 0, 0, 0, 0};
/*typedef unsigned int uint32;
typedef char byte;


typedef struct __attribute__ ((aligned (16))){
  int width;
  int height;
  int vwidth;
  int vheight;
  int pitch;
  int depth;
  int x;
  int y;
  int pointer;
  int size;
} FrameBufferInfo;

struct GPU {
    uint32 screen_width;
    uint32 screen_height;
    uint32 virtual_width;
    uint32 virtual_height;
    uint32 pitch;
    uint32 depth;
    uint32 xoffset;
    uint32 yoffset;
    uint32 framePtr;
    uint32 bufferSize;
    bool valid;
  };

// MAILBOX
#define   KERNEL_FB_LOC     (volatile unsigned int) 0x00002000
#define MAIL_BASE 0xB880    // Base address for the mailbox registers

// This bit is set in the status register if there is no space to write into the mailbox
#define MAIL_FULL 0x80000000
// This bit is set in the status register if there is nothing to read from the mailbox
#define MAIL_EMPTY 0x40000000



#define MAIL_READ_OFFSET 0x00
#define MAIL_WRITE_OFFSET 0x20
#define MAIL_STATUS_OFFSET 0x18

#define MAPPED_REGISTERS_BASE 0x20000000

template<class T>
static T ReadMemMappedReg(size_t BaseAddress, size_t Offset)
{
    return *reinterpret_cast<const T *>(MAPPED_REGISTERS_BASE + BaseAddress + Offset);
}

template<class T>
static void WriteMemMappedReg(size_t BaseAddress, size_t Offset, T Data)
{
    *reinterpret_cast<T *>(MAPPED_REGISTERS_BASE + BaseAddress + Offset) = Data;
}

volatile uint32 GET32( uint32 addr ) {
  // Create a pointer to our location in memory.
  volatile uint32* ptr = (volatile uint32*)( addr );
  // Return the value.
  return (uint32)(*ptr);
}

volatile void PUT32( uint32 addr, uint32 value ) {
  // Create a pointer to our location in memory.
  volatile uint32* ptr = (volatile uint32*)( addr );
  // Set the value.
  *ptr = value;
}

volatile char GET4( uint32 addr ) {
  // Create a pointer to our location in memory.
  volatile char* ptr = (volatile char*)( addr );
  // Return the value.
  return (char)(*ptr);
}

volatile void PUT4( uint32 addr, char value ) {
  // Create a pointer to our location in memory.
  volatile char* ptr = (volatile char*)( addr );
  // Set the value.
  *ptr = value;
}
volatile void Barrier( void ) {
      asm volatile( "mcr p15, 0, ip, c7, c5, 0" );
      asm volatile( "mcr p15, 0, ip, c7, c5, 6" );
      asm volatile( "mcr p15, 0, ip, c7, c10, 4" );
      asm volatile( "mcr p15, 0, ip, c7, c5, 4" );
    }

uint32 ReadMailbox(byte channel)
{
    // Define some variables.
    uint32 data = 0, count = 0;
    // Worf: Sir! They're firing on us!
    // Picard: Evasive maneuver alpha.
    while ( true ) {
      // First, loop over the status.
      while ( (ReadMemMappedReg<uint32>(MAIL_BASE, MAIL_STATUS_OFFSET) & MAIL_EMPTY) != 0 ) {
        // Do Nothing
        if ( count++ >= (1<<20) ) {
          //PiFault( "Error. Circular loop checking for status." );
          return 0xFFFFFFFF;
        }
      }

      // NOTE: You absolutely MUST use the memory barriers
      // here!!! Without them, the evil empire will win
      // a bloody victory against the federation.
      Barrier();

      // Read the data.
      data = ReadMemMappedReg<uint32>(MAIL_BASE, MAIL_READ_OFFSET);

      // This one MIGHT be optional. But do you really want
      // the evil empire to win?
      Barrier();

      // Derrive the channel bit.
      if ( (data & 0xF) == channel )
        // If they match, then break out.
        break;
    }

    // Return our data.
    return (data & 0xFFFFFFF0);
}

void WriteMailbox(byte channel, uint32 data)
{
    while ((ReadMemMappedReg<uint32>(MAIL_BASE, MAIL_STATUS_OFFSET) & MAIL_FULL) != 0)
    {
        // Wait for space
    }
    // Write the value to the requested channel
    Barrier();
    WriteMemMappedReg<uint32>(MAIL_BASE, MAIL_WRITE_OFFSET, channel | (data & 0xFFFFFFF0) );
    Barrier();
}

*/


/* COMMANDS */


#define SHELL_WA_SIZE       THD_WA_SIZE(4096)
#define EXTENDED_SHELL

#ifdef EXTENDED_SHELL

#define TEST_WA_SIZE        THD_WA_SIZE(4096)


static void cmd_graphics_init(BaseSequentialStream *chp, int argc, char *argv[]) {
  VideoCoreGPUFrameBufferInfo* request = (VideoCoreGPUFrameBufferInfo*)(0x00002000);
  request->screen_width = 1920;
  request->screen_height = 1080;
  request->virtual_width = 1920;
  request->virtual_height = 1080;
  request->pitch = 0;
  request->depth = 24;
  request->xoffset = 0;
  request->yoffset = 0;
  request->framePtr = 0;
  request->bufferSize = 0;
  request->valid = false;

  VideoCoreGPUError error = videocore_gpu_framebuffer_request(request);

  if(error != VideoCoreGPUError_None)
  {
    switch( error )
    {
      case VideoCoreGPUError_MailboxError:
        chprintf(chp, "Error! The mailbox didn't return a suitable value in a timely manner.\r\n" );
        break;

      case VideoCoreGPUError_InvalidFramebuffer:
      chprintf(chp, "Error! The framebuffer returned is invalid. Aborting framebuffer acquisition\r\n" );
        break;

      case VideoCoreGPUError_InvalidPitch:
      chprintf(chp, "Error! The pitch returned is invalid. Aborting framebuffer acquisition.\r\n" );
        break;
      default:
        break;
    }
  }

  //graphics clear
  if ( !request->valid ) {
    return;
  }
  chprintf(chp, "All Good!\r\n");
  // Setup the variables.
  uint32_t x = 0, y = 0, y_offset = 0, bufferOffset;
  //if ( !this->switched && this->enableDoubleBuffer )
    //y_offset = request->screen_height;

  uint32_t color = 0x00FF00;
  // Calculate the colors.
  char r = ( color & 0xFF0000 ) >> 16;
  char g = ( color & 0x00FF00 ) >> 8;
  char b = ( color & 0x0000FF );

  // Iterate
  for ( y = 0; y < request->screen_height; y++ ) {
    for ( x = 0; x < request->screen_width; x++ ) {

      // Calculate the location in memory.
      bufferOffset = ( ( y + y_offset ) * request->pitch ) + ( x * 3 );

      // Update the data.
      *( (char*)( request->framePtr + bufferOffset ) ) = r;
      *( (char*)( request->framePtr + bufferOffset + 1 ) ) = g;
      *( (char*)( request->framePtr + bufferOffset + 2 ) ) = b;

    }
  }
}



static void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[]) {
  size_t n, size;

  UNUSED(argv);
  if (argc > 0) {
    chprintf(chp, "Usage: mem\r\n");
    return;
  }
  n = chHeapStatus(NULL, &size);
  chprintf(chp, "core free memory : %u bytes\r\n", chCoreStatus());
  chprintf(chp, "heap fragments   : %u\r\n", n);
  chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

static void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[]) {
  static const char *states[] = {THD_STATE_NAMES};
  Thread *tp;

  UNUSED(argv);
  if (argc > 0) {
    chprintf(chp, "Usage: threads\r\n");
    return;
  }
  chprintf(chp, "    addr    stack prio refs     state time    name\r\n");
  tp = chRegFirstThread();
  do {
    chprintf(chp, "%.8lx %.8lx %4lu %4lu %9s %-8lu %s\r\n",
            (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
            (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
       states[tp->p_state], (uint32_t)tp->p_time, tp->p_name);
    tp = chRegNextThread(tp);
  } while (tp != NULL);
}

#endif // EXTENDED_SHELL

static void cmd_reboot(BaseSequentialStream *chp, int argc, char *argv[]) {
  UNUSED(argv);
  if (argc > 0) {
    chprintf(chp, "Usage: reboot\r\n");
    return;
  }

  /* Watchdog will cause reset after 1 tick.*/
  watchdog_start(1);
}

static const ShellCommand commands[] = {
#ifdef EXTENDED_SHELL
  {"mem", cmd_mem},
  {"threads", cmd_threads},
  {"graphics_init",cmd_graphics_init},
#endif
  {"reboot", cmd_reboot},
  {NULL, NULL}
};

static const ShellConfig shell_config = {
  (BaseSequentialStream *)&SD1,
  commands
};

static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *p) {
  (void)p;
  chRegSetThreadName("blinker");
  while (TRUE) {
    palClearPad(ONBOARD_LED_PORT, ONBOARD_LED_PAD);
    chThdSleepMilliseconds(100);
    palSetPad(ONBOARD_LED_PORT, ONBOARD_LED_PAD);
    chThdSleepMilliseconds(900);
  }
  return 0;
}

/*
 * Application entry point.
 */
int main(void) {
  halInit();
  chSysInit();

  /*
   * Serial port initialization.
   */
  sdStart(&SD1, NULL); 
  chprintf((BaseSequentialStream *)&SD1, "Main (SD1 started)\r\n");

  /*
   * Shell initialization.
   */
  shellInit();
  shellCreate(&shell_config, SHELL_WA_SIZE, NORMALPRIO + 1);

  /*
   * Set mode of onboard LED
   */
  palSetPadMode(ONBOARD_LED_PORT, ONBOARD_LED_PAD, PAL_MODE_OUTPUT);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Events servicing loop.
   */
  chThdWait(chThdSelf());

  return 0;
}
