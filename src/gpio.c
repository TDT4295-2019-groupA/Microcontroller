#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "gpio.h"
#include "em_gpio.h"
#include "gpiointerrupt.h"
#include "em_int.h"
#include "em_cmu.h"
#ifndef DEVICE_SADIE
#include "bsp.h"
#include "bsp_trace.h"
#endif
#include "input.h"

#ifdef DEVICE_GECKO_STARTER_KIT
static unsigned int gpio_btn_index_to_pin[] = {
	9, 10
};
static unsigned int gpio_btn_index_to_port[] = {
	gpioPortB,	//0
	gpioPortB,	//1
};
#endif

#ifdef DEVICE_SADIE
static unsigned int gpio_btn_index_to_pin[] = {
	2, 3, 4, 5, 3, 4, 5, 6, 4, 5, 7, 8, 12, 13, 14, 11
};
static unsigned int gpio_btn_index_to_port[] = {
	gpioPortA,	//0
	gpioPortA,	//1
	gpioPortA,	//2
	gpioPortA,	//3
	gpioPortB,	//4
	gpioPortB,	//5
	gpioPortB,	//6
	gpioPortB,	//7
	gpioPortC,	//8
	gpioPortC,	//9
	gpioPortB,	//10
	gpioPortB,	//11
	gpioPortA,	//12
	gpioPortA,	//13
	gpioPortA,	//14
	gpioPortB	//15
};
#endif

static bool button_state[GPIO_BTN_COUNT] = {0};

static bool led_toggle = false; // debug

void handleButtons()
{
    for(int i = 0; i < GPIO_BTN_COUNT; i++){
        button_state[i] = GPIO_PinInGet(gpio_btn_index_to_port[i], gpio_btn_index_to_pin[i]);
    }
    led_toggle = !led_toggle;
    setExtLed(led_toggle);
    handleMultipleButtonPresses();
}

void pulse_reset() {
    if (GPIO_PinInGet(gpioPortA, 13)) {
        GPIO_PinOutSet(gpioPortE, 15); // Arbitrary GPIO pin
        GPIO_PinOutSet(gpioPortE, 4); // FPGA Soft reset
    } else {
        GPIO_PinOutClear(gpioPortE, 15);
        GPIO_PinOutClear(gpioPortE, 4);
    }
}

void setupGPIO(void)
{
    for(int i = 0; i < GPIO_BTN_COUNT; i++){
		GPIO_PinModeSet(gpio_btn_index_to_port[i], gpio_btn_index_to_pin[i], gpioModeWiredOrPullDown , 0);
		GPIO_ExtIntConfig(gpio_btn_index_to_port[i], gpio_btn_index_to_pin[i], gpio_btn_index_to_pin[i], true, true, true);
	}

    GPIOINT_Init();
    GPIOINT_CallbackRegister(2, handleButtons);
    GPIOINT_CallbackRegister(3, handleButtons);
    GPIOINT_CallbackRegister(4, handleButtons);
    GPIOINT_CallbackRegister(5, handleButtons);
    GPIOINT_CallbackRegister(6, handleButtons);
    GPIOINT_CallbackRegister(7, handleButtons);
    GPIOINT_CallbackRegister(8, handleButtons);
    GPIOINT_CallbackRegister(11, handleButtons);
    GPIOINT_CallbackRegister(12, pulse_reset);
    GPIOINT_CallbackRegister(13, pulse_reset);
    GPIOINT_CallbackRegister(14, pulse_reset);

	// turn on Softmute
	GPIO_PinModeSet(gpioPortA, 1, gpioModePushPull, 1);
	//GPIO_DriveStrengthSet(gpioPortA,gpioDriveStrengthStrongAlternateStrong);
	// fpga_ready
	GPIO_PinModeSet(gpioPortC, 6, gpioModeWiredOrPullDown, 1);
	// fpga_resetpNVIC()
	GPIO_PinModeSet(gpioPortC, 7, gpioModePushPull, 1);
	GPIO_PinModeSet(gpioPortE, 4, gpioModePushPull, 0);
	GPIO_PinModeSet(gpioPortE, 14, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortE, 15, gpioModePushPull, 0);
    setSoftMute(true);
}

void led()
{
	BSP_LedToggle(0);
}

bool isButtonDown(unsigned int index){
	assert(index < GPIO_BTN_COUNT);
	return button_state[index];
}

void setSoftMute(bool high) {
	if (high){
		GPIO_PinOutSet(gpioPortA, 1);
	}
	else{
		GPIO_PinOutClear(gpioPortA, 1);
	}

}

void setExtLed(bool high){
	if (high){
		GPIO_PinOutSet(gpioPortE, 14);
	}
	else{
		GPIO_PinOutClear(gpioPortE, 14);
	}
}
