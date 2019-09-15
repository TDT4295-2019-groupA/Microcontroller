#include <stdint.h>
#include <stdbool.h>
#include "declarations.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "bsp.h"
#include "bsp_trace.h"

void setupGPIO()
{
	CMU_ClockEnable(cmuClock_GPIO, true);
	for(unsigned int i=0; i<12; i++) {
		GPIO_PinModeSet(gpioPortC, i, gpioModeInputPullFilter, 1);
		GPIO_ExtIntConfig(gpioPortC, i, i, true, false, true);
	}
}

void handleButtons(uint32_t DIN)
{
	DIN = ~DIN;
	bool sw1  = (DIN & (0x1<<0));
	bool sw2  = (DIN & (0x1<<1));
	bool sw3  = (DIN & (0x1<<2));
	bool sw4  = (DIN & (0x1<<3));
	bool sw5  = (DIN & (0x1<<4));
	bool sw6  = (DIN & (0x1<<5));
	bool sw7  = (DIN & (0x1<<6));
	bool sw8  = (DIN & (0x1<<7));
	bool sw9  = (DIN & (0x1<<8));
	bool sw10 = (DIN & (0x1<<9));
	bool sw11 = (DIN & (0x1<<10));
	bool sw12 = (DIN & (0x1<<11));

	if (sw1) {
		//led();
		testy += 1;
	}
	if (sw2) {
		//led();
		testy += 1;
	}
	if (sw3) {
		//led();
		testy += 1;
	}
	if (sw4) {
		//led();
		testy += 1;
	}
	if (sw5) {
		//led();
		testy += 1;
	}
	if (sw6) {
		//led();
		testy += 1;
	}
	if (sw7) {
		//led();
		testy += 1;
	}
	if (sw8) {
		//led();
		testy += 1;
	}
	if (sw9) {
		//led();
		testy += 1;
	}
	if (sw10) {
		//led();
		testy += 1;
	}
	if (sw11) {
		//led();
		testy += 1;
	}
	if (sw12) {
		//led();
		testy += 1;
	}
}
