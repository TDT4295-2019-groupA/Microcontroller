#include "timer.h"

TIMER_Init_TypeDef timerInit =
  {
    .enable     = false,
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
	TIMER_TopSet(TIMER1, top);
	TIMER_Init(TIMER1, &timerInit);
}

void pulse(void)
{
	uint32_t top = TIMER_TopGet(TIMER1);
	GPIO_PinOutSet(gpioPortE, 4);
	TIMER_Enable(TIMER1, true);
	while(TIMER_CounterGet(TIMER1)<top) {

	}
	GPIO_PinOutClear(gpioPortE, 4);
	TIMER_Enable(TIMER1, false);
	TIMER_CounterSet(TIMER1, 0);
}

bool setDone(void) {
	unsigned int flashed = GPIO_PinInGet(gpioPortC, 6);
	if(flashed == 1) {
		pulse();
		return true;
	} else {
		return false;
	}
}


