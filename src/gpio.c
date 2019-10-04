#include <stdint.h>
#include <stdbool.h>
#include "../headers/gpio.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "bsp.h"
#include "bsp_trace.h"

void setupGPIO(void)
{
	for(unsigned int i=4; i<16; i++) {
		GPIO_PinModeSet(gpioPortC, i, gpioModeInputPullFilter, 1);
		GPIO_ExtIntConfig(gpioPortC, i, i, true, false, true);
	}
}

void led()
{
	BSP_LedToggle(0);
}

void handleButtons()
{
	bool bt1  = GPIO_PinInGet(gpioPortC, 4);
	bool bt2  = GPIO_PinInGet(gpioPortC, 5);
	bool bt3  = GPIO_PinInGet(gpioPortC, 6);
	bool bt4  = GPIO_PinInGet(gpioPortC, 7);
	bool bt5  = GPIO_PinInGet(gpioPortC, 8);
	bool bt6  = GPIO_PinInGet(gpioPortC, 9);
	bool bt7  = GPIO_PinInGet(gpioPortC, 10);
	bool bt8  = GPIO_PinInGet(gpioPortC, 11);
	bool bt9  = GPIO_PinInGet(gpioPortC, 12);
	bool bt10 = GPIO_PinInGet(gpioPortC, 13);
	bool bt11 = GPIO_PinInGet(gpioPortC, 14);
	bool bt12 = GPIO_PinInGet(gpioPortC, 15);

	if (bt1) {
		led();
	}
	if (bt2) {
		led();
	}
	if (bt3) {
		led();
	}
	if (bt4) {
		led();
	}
	if (bt5) {
		led();

	}
	if (bt6) {
		led();

	}
	if (bt7) {
		led();

	}
	if (bt8) {
		led();

	}
	if (bt9) {
		led();

	}
	if (bt10) {
		led();

	}
	if (bt11) {
		led();

	}
	if (bt12) {
		led();

	}
}
