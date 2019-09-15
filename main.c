#include "declarations.h"
#include "em_gpio.h"

//integer for testing purposes
testy;

int main(void)
{
	setupGPIO();
	setupTimer(2734);
	setupNVIC();
	while (1) {
		GPIO_PinOutToggle(gpioPortC, 3);
		for(volatile long i=0; i<100000; i++);
		GPIO_PinOutToggle(gpioPortC, 2);
	}
	return 0;
}
