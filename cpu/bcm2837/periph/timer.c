#include "periph_conf.h"
#include "periph_cpu.h"
#include "periph/timer.h"

#define REG32       volatile unsigned int

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

unsigned int softick;
unsigned int compare;
unsigned int value;

void vTickISR(int nIRQ, void *pParam );

int timer_init(tim_t tim, unsigned long freq, timer_cb_t cb, void *arg)
{
  /* get the timers base register */
  //lpc23xx_timer_t *dev = get_dev(tim);

  /* make sure the timer device is valid */
  //if (dev == NULL) {
  //  return -1;
  //}

  /* save the callback */

  tim_t tim1;
  unsigned long freq1;
  tim1 = tim;
  tim = tim1;
  freq1 = freq;
  freq = freq1;

  isr_ctx.cb = cb;//cb to be called any time a timer expaiers
  isr_ctx.arg = arg;

  //hexstring(0x22222222);
  hexstring(freq);

  //                                        16   12      8    4    0
  timer->TCR = 0x003E0000;  //0000 0000 0011 1110 0000 0000 0000 0000
  timer->LOD = 0xffffffff;
  timer->RLD = 0xffffffff;
  //timer->PR = portTIMER_PRESCALE; //0xfa = 250 ,
  timer->PR = (250000000-freq)/freq;
  timer->CLI = 0;
  timer->TCR = 0x003E02A2;//0000 0000 0011 1110 0000 0010 1010 0010//15 bit counter, timer int enable, timer enabled

  RegisterInterrupt(64, vTickISR, 0);

  EnableInterrupt(64);

  //softick = (1 << TIMER_ACCURACY_SHIFT);
  softick = 0;
  //counter = 0;
  return 0;
}

void timer_vals(int* psoft, int* phard,int* pvalue){

  *psoft = softick;
  *phard = timer->TC;
  *pvalue = value;
}

unsigned int timer_read(tim_t tim)
{
  tim_t t = tim;
  tim = t;
  return (unsigned int) timer->TC;
}

int timer_set_absolute(tim_t tim, int channel, unsigned int val)
{
  //if (((unsigned) tim >= TIMER_NUMOF) || ((unsigned) channel >= TIMER_CHAN_NUMOF)) {
  //  return -1;
  //}

  tim_t tim1 = tim;
  tim = tim1;
  int c = channel;
  channel = c;

  if(val > timer->TC && val != 0xffffffff)
    timer->LOD = val-timer->TC;
  else
    timer->LOD = val;
  //timer->LOD = val;
  //value >>= TIMER_ACCURACY_SHIFT;
  //value <<= TIMER_ACCURACY_SHIFT;

  //timer->LOD = value;

  //compare = val;
  //hexstring(0x11111111);
  //hexstring(val);
  //hexstring(timer->TC);

  //lpc23xx_timer_t *dev = get_dev(tim);
  //dev->MR[channel] = value;
  //dev->MCR |= (1 << (channel * 3));

  return 0;
}

extern volatile unsigned int sched_context_switch_request;

__attribute__((no_instrument_function))
void vTickISR(int nIRQ, void *pParam )
{
  //vTaskIncrementTick();

  //#if configUSE_PREEMPTION == 1
  //vTaskSwitchContext();
  //#endif
  int a = nIRQ;
  nIRQ = a;
  void* b = pParam;
  pParam = b;

  timer->CLI = 0;			// Acknowledge the timer interrupt.
  //softick ++;//= TIMER_ACCURACY;
  //value = timer->VAL;
  //hexstring(0x12121234);


  //if(timer->TC == compare && compare){
  //timer->LOD = 0xffffffff;
    isr_ctx.cb(isr_ctx.arg,0);
    //  if(sched_context_switch_request == 1){
    //  thread_yield();
    //}

    //compare = 0;
    //}


}
