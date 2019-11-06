#include <stdbool.h>
#include "interrupts.h"
#include "em_gpio.h"
#include "gpio.h"
#include "em_timer.h"
#include "em_emu.h"
#include "em_core.h"
#include "gpiointerrupt.h"
#include "em_int.h"

/*void GPIO_EVEN_IRQHandler()
{
	GPIO_IntClear(GPIO_IntGet());

	setExtLed(true);
    //handleButtons();
}

void GPIO_ODD_IRQHandler()
{
    GPIO_IntClear(GPIO_IntGet());

    //setExtLed(false);
    //handleButtons();
}*/



void setupNVIC(void)
{
	NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
	//NVIC_EnableIRQ(TIMER1_IRQn);

	GPIO_IntClear(GPIO_IntGet());
}

void inthandler(void){
	setExtLed(true);
}
void setupBtnInterrupts(void){
	GPIOINT_Init();
	GPIOINT_CallbackRegister(2, inthandler);
	GPIOINT_CallbackRegister(3, inthandler);
	GPIOINT_CallbackRegister(4, inthandler);
	GPIOINT_CallbackRegister(5, inthandler);
	GPIOINT_CallbackRegister(6, inthandler);
	GPIOINT_CallbackRegister(7, inthandler);
	GPIOINT_CallbackRegister(8, inthandler);
	GPIOINT_CallbackRegister(11, inthandler);
	GPIOINT_CallbackRegister(12, inthandler);
	GPIOINT_CallbackRegister(13, inthandler);
	GPIOINT_CallbackRegister(14, inthandler);
	GPIO_IntEnable(1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<6 | 1<<7 | 1<<8 | 1<<11 | 1<<12 | 1<<13 | 1<<14);
}
