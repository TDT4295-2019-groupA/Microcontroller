#include <stdio.h>
#include <stdlib.h>
#include "em_device.h"
#include "em_cmu.h"
#include "input.h"
#include "timer.h"
#include "usbhost.h"
#include "gpio.h"
#include "fpga.h"
#include "em_chip.h"
//#include "interrupts.h"
#include "spi.h"
#include <stdbool.h>

#ifndef DEVICE_SADIE
#include "segmentlcd.h"
#endif

void setupCMU(void);

int main(void)
{
	CHIP_Init();
	setupCMU();
	setupGPIO();
	setupTimer(1);
	spi_init();
	setExtLed(true);
	pulse();
	setExtLed(false);

	while(!setDone());

	#ifndef DEVICE_SADIE
	SegmentLCD_Init(false);
	SegmentLCD_Write("USBHOST");
	#endif

	MicrocontrollerGeneratorState** generator_states = malloc(sizeof(MicrocontrollerGeneratorState*) * N_GENERATORS);
	for (uint8_t i = 0; i < N_GENERATORS; i++)
		generator_states[i] = generator_state_new();
	MicrocontrollerGlobalState* global_state = malloc(sizeof(global_state));
	global_state = global_state_new();

	if(USBConnect()){
#ifndef DEVICE_SADIE
		SegmentLCD_Write("CONNECT");
#endif

		while(USBIsConnected()){
			setExtLed(true);
			MIDI_packet input = waitForInput();
			// TODO: do stuff with input
			handleMIDIEvent(&input, generator_states);
		}
		// Connection removed
#ifndef DEVICE_SADIE
		SegmentLCD_Write("CON REM");
#endif
	}
	else{
		// Connection failed
#ifndef DEVICE_SADIE
		SegmentLCD_Write("CON FAIL");
#endif
	}
}

void setupCMU(void)
{
	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_WTIMER1, true);
	CMU_ClockEnable(cmuClock_LDMA, true);
	if (OUTPUT_CLOCK) {
	    CMU->CTRL = CMU->CTRL | CMU_CTRL_CLKOUTSEL1_HFXOQ;  // Set CMU_CLK1 to use HFRCO
	    CMU->HFPRESC = (CMU->HFPRESC & ~CMU_HFPRESC_PRESC_DEFAULT) | (0b11 << _CMU_HFPRESC_PRESC_SHIFT);
        CMU->ROUTEPEN = CMU->ROUTEPEN | CMU_ROUTEPEN_CLKOUT1PEN;  // Enable CMU_CLK1 out pin
        CMU->ROUTELOC0 = CMU->ROUTELOC0 | CMU_ROUTELOC0_CLKOUT1LOC_LOC2;  // Route CMU_CLK1 out of loc2 (PE12 on SADIE)
    } else {
#ifdef SPI_GPIO
        CMU_ClockEnable(cmuClock_USART0, true);
#endif
#ifdef SPI_FPGA
        CMU_ClockEnable(cmuClock_USART1, true);
#endif
    }
}

