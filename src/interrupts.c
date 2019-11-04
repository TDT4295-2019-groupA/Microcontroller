#include <stdbool.h>
#include "interrupts.h"
#include "gpio.h"
#include "em_timer.h"
#include "em_emu.h"



void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
    uint32_t DIN = *GPIO_IF;
    *GPIO_IFC = DIN;
    /*
    if(timerStarted == false){
    	timerStarted = true;
    	TIMER_Enable(TIMER1, true);
    }
    */
    handleButtons();
}

void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
    uint32_t DIN = *GPIO_IF;
    *GPIO_IFC = DIN;
    /*
    if(timerStarted == false){
    	timerStarted = true;
    	TIMER_Enable(TIMER1, true);
    }
    */
    handleButtons();
}

void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
    /*
	*TIMER1_IFC = 1;
    TIMER_Enable(TIMER1, false);
    timerStarted = false;
    if(testy == 0) {

    }else if(testy == 1) {
    	BSP_LedToggle(0);
    	for(volatile long i=0; i<100000; i++);
    	BSP_LedToggle(0);
    }else {
    	BSP_LedToggle(0);
    	BSP_LedToggle(1);
    	for(volatile long i=0; i<100000; i++);
    	BSP_LedToggle(0);
    	BSP_LedToggle(1);
    }
    testy = 0;
    */
}

void setupNVIC(void)
{
	*ISER0 |= 0x1<<12; // TIMER1
	*ISER0 |= 0x1<<11; // GPIO_ODD
	*ISER0 |= 0x1<< 1; // GPIO_EVEN

	*TIMER1_IFC = 1;
	*GPIO_IFC = *GPIO_IF;
}
