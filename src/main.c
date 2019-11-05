#include <stdio.h>
#include <stdlib.h>
#include "em_device.h"
#include "em_cmu.h"
#include "input.h"
#include "usbhost.h"
#include "gpio.h"
#include "fpga.h"
#include "interrupts.h"
#include "spi.h"
#include <stdbool.h>

#ifndef DEVICE_SADIE
#include "segmentlcd.h"
#endif

void setupCMU(void);

int main(void)
{
	//CHIP_Init();
	setupCMU();
	setupGPIO();
	//setupNVIC();
	spi_init();

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
	CMU_ClockEnable(cmuClock_TIMER1, true);
	CMU_ClockEnable(cmuClock_LDMA, true);
	CMU_ClockEnable(cmuClock_USART1, true);
}

