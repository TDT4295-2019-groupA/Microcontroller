#include <stdio.h>
#include <stdlib.h>
#include "em_device.h"
#include "em_cmu.h"
#include "input.h"
#include "usbhost.h"
#include "gpio.h"
#include "fpga.h"
#include "em_chip.h"
#include "timer.h"
//#include "interrupts.h"
#include "spi.h"
#include "midi.h"
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
			unsigned char *input = USBWaitForData();
			while(input[1] != 0) {
				MIDI_packet midi = convertToMidi(input);
				handleMIDIEvent(&midi, generator_states);
				input += (USB_OUTPUT_SIZE*sizeof(unsigned char));
			}
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
	CMU_ClockEnable(cmuClock_USART0, true);
}

