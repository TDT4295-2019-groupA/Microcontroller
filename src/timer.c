#include <stdint.h>
#include <stdbool.h>
#include "em_cmu.h"
#include "em_timer.h"

TIMER_Init_TypeDef timerInit =
  {
    .enable     = true,
    .debugRun   = true,
    .prescale   = timerPrescale1024,
    .clkSel     = timerClkSelHFPerClk,
    .fallAction = timerInputActionNone,
    .riseAction = timerInputActionNone,
    .mode       = timerModeUp,
    .dmaClrAct  = false,
    .quadModeX4 = false,
    .oneShot    = false,
    .sync       = false,
  };

//delay in ms
void setupTimer(uint16_t delay)
{
	uint32_t top = (delay * CMU_ClockFreqGet(cmuClock_HFPER))/1024;
	TIMER_IntEnable(TIMER1, TIMER_IF_OF);
	TIMER_TopSet(TIMER1, top);
	TIMER_Init(TIMER1, &timerInit);
}


