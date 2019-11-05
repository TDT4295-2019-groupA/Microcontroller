#include <stdbool.h>
#include "interrupts.h"
#include "gpio.h"
#include "em_timer.h"
#include "em_emu.h"
#include "em_core.h"

void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	GPIO_IntClear(GPIO_IntGet());

    handleButtons();
}

void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
    GPIO_IntClear(GPIO_IntGet());

    handleButtons();
}


void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
    // Stuff
}


void setupNVIC(void)
{
	NVIC_Enable(GPIO_ODD_IRQn);
	NVIC_Enable(GPIO_EVEN_IRQn);

	NVIC_Enable(TIMER1_IRQn);

	GPIO_IntClear(GPIO_IntGet());
}
