#include <stdbool.h>
#include "interrupts.h"
#include "gpio.h"
#include "em_timer.h"
#include "em_emu.h"
#include "em_core.h"

void GPIO_EVEN_IRQHandler()
{
	GPIO_IntClear(GPIO_IntGet());

	setExtLed(true);
    //handleButtons();
}

void GPIO_ODD_IRQHandler()
{
    GPIO_IntClear(GPIO_IntGet());

    setExtLed(false);
    //handleButtons();
}



void setupNVIC(void)
{
	NVIC_EnableIRQ(GPIO_ODD_IRQn);
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
	//NVIC_EnableIRQ(TIMER1_IRQn);

	GPIO_IntClear(GPIO_IntGet());
}
