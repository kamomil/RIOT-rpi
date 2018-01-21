#include "periph_conf.h"
#include "periph_cpu.h"
#include "periph/timer.h"

#define REG32       volatile unsigned int

/*the timer uses the arm timer described in page 196 in the broadcom datasheet*/

typedef struct {
  REG32   LOD;             /**< interrupt register */
  REG32   VAL;
  REG32   TCR;            /**< timer control register */
  REG32   CLI;             /**< irq clear */
  REG32   RIS;//raw irq
  REG32   MIS;//masked irqb
  REG32   RLD;//reload
  REG32   PR;//pre divider (prescale counter)
  REG32   TC;//free running counter
} bcm235_timer_t;

#define portTIMER_BASE                    		( (unsigned long ) 0x3f00B400 )

static volatile bcm235_timer_t* const timer = (bcm235_timer_t*) (portTIMER_BASE);

/**
 * @brief   Interrupt context information for configured timers
 */
static timer_isr_ctx_t isr_ctx;

#define portTIMER_PRESCALE 						( ( unsigned long ) 0xFA )


void vTickISR(int nIRQ, void *pParam );

int timer_init(tim_t tim, unsigned long freq, timer_cb_t cb, void *arg)
{

  //stupid things for getting rid of unused var warnings
  //since there is only one timer, there is no use of this arg
  (void)tim;


  isr_ctx.cb = cb;//cb to be called any time a timer expaiers
  isr_ctx.arg = arg;

  //hexstring(0x22222222);
  hexstring(freq);

  //                                           16   12    8    4    0
  timer->TCR = 0x003E0000;  //0000 0000 0011 1110 0000 0000 0000 0000

  //the timer interrupt will fire when the VAL register which is initialized from the LOD register
  //is equal 0. It is decremented in a rate eccording to the frequency of the timer.
  timer->LOD = 0xffffffff;
  timer->RLD = 0xffffffff;

  //according to the datasheet
  timer->PR = (250000000-freq)/freq;
  timer->CLI = 0;
  timer->TCR = 0x003E02A2;//0000 0000 0011 1110 0000 0010 1010 0010//15 bit counter, timer int enable, timer enabled

  RegisterInterrupt(64, vTickISR, 0);

  EnableInterrupt(64);

  return 0;
}

unsigned int timer_read(tim_t tim)
{
  (void)tim;
  //TC is a free running counter incremented according to the timer frequency
  return (unsigned int) timer->TC;
}

//when calling xtimer_sleep(sec), then eventually this function is called
//with a value that is equal to the current timer read + sec*freq
//since the LOD register is set to a count down value, we need to set it to
//val-TC
int timer_set_absolute(tim_t tim, int channel, unsigned int val)
{
  (void)tim;
  (void)channel;

  if(val > timer->TC && val != 0xffffffff)
    timer->LOD = val-timer->TC;
  else
    timer->LOD = val;
  return 0;
}

extern volatile unsigned int sched_context_switch_request;

__attribute__((no_instrument_function))
void vTickISR(int nIRQ, void *pParam )
{
  (void)nIRQ;
  (void)pParam;

  timer->CLI = 0;// Acknowledge the timer interrupt.

  //the callback is called when a timer expierse.
  //this, since the vTickISR hanleder fires only when the LOD is counted down to 0
  //we should always call cb from here
  isr_ctx.cb(isr_ctx.arg,0);
}
