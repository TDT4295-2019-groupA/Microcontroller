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
    .oneShot    = true,
    .sync       = false,
  };

//delay in ms
void setupTimer(uint16_t delay)
{
	unsigned int freq = CMU_ClockFreqGet(cmuClock_HFPER);
	uint32_t top = ((delay * freq)/1024) / 15;
	TIMER_TopSet(WTIMER1, top);
	TIMER_Init(WTIMER1, &timerInit);
}

void pulse(void)
{
	uint32_t top = TIMER_TopGet(WTIMER1);
	GPIO_PinOutSet(gpioPortE, 4);
	TIMER_Enable(WTIMER1, true);
	while(TIMER_CounterGet(WTIMER1)<top);
	GPIO_PinOutClear(gpioPortE, 4);
	TIMER_Enable(WTIMER1, false);
	TIMER_CounterSet(WTIMER1, 0);
}

bool setDone(void) {
	unsigned int flashed = GPIO_PinInGet(gpioPortC, 6);
	if(flashed == 1) {
		return true;
	} else {
		return false;
	}
}


