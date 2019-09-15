#include "declarations.h"
#include "em_gpio.h"
#include "em_cmu.h"

//integer for testing purposes
testy;

//timer interrupt every 2 seconds
int main(void)
{
	setupCMU();
	setupGPIO();
	setupTimer(2000);
	setupNVIC();
	while (1) {
		GPIO_PinOutToggle(gpioPortC, 3);
		for(volatile long i=0; i<100000; i++);
		GPIO_PinOutToggle(gpioPortC, 2);
	}
	return 0;
}

void setupCMU()
{
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_TIMER1, true);
	CMU_ClockEnable(cmuClock_DMA, true);
	CMU_ClockEnable(cmuClock_USART1, true);
}

