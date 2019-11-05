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
	CORE_DECLARE_NVIC_ZEROMASK(mask);

	CORE_NvicMaskSetIRQ(GPIO_ODD_IRQn, 	&mask);
	CORE_NvicMaskSetIRQ(GPIO_EVEN_IRQn,	&mask);
	CORE_NvicMaskSetIRQ(TIMER1_IRQn,    &mask);

	CORE_NVIC_ENABLE(&mask);

	GPIO_IntClear(GPIO_IntGet());
}
