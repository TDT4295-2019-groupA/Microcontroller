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

void setupTimer(uint32_t delay)
{
	TIMER_IntEnable(TIMER1, TIMER_IF_OF);
	TIMER_TopSet(TIMER1, delay);
	TIMER_Init(TIMER1, &timerInit);
}


