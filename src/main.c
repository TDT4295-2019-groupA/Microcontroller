#include <stdio.h>
#include <stdlib.h>
#include "em_device.h"
#include "em_cmu.h"
#include "segmentlcd.h"
#include "input.h"
#include "usbhost.h"
#include "gpio.h"
#include "fpga.h"
#include "interrupts.h"
#include "spi.h"

void setupCMU(void);

int main(void)
{
	//CHIP_Init();
	setupCMU();
	setupGPIO();
	setupNVIC();
	spi_init();

	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
	CMU_ClockEnable(cmuClock_GPIO, true);

	SegmentLCD_Init(false);
	SegmentLCD_Write("USBHOST");

	MicrocontrollerGeneratorState** generator_states = malloc(sizeof(MicrocontrollerGeneratorState*) * N_GENERATORS);
	for (uint8_t i = 0; i < N_GENERATORS; i++)
		generator_states[i] = generator_state_new();
	MicrocontrollerGlobalState* global_state = malloc(sizeof(global_state));
	global_state = global_state_new();

	if(USBConnect()){
		SegmentLCD_Write("CONNECT");

		while(USBIsConnected()){
			unsigned char *input = USBWaitForData();
			while(input[1] != 0) {
				MIDI_packet midi = convertToMidi(input);
				handleMIDIEvent(&midi, generator_states);
				input += (USB_OUTPUT_SIZE*sizeof(unsigned char));
			}
		}
		// Connection removed
		SegmentLCD_Write("CON REM");
	}
	else{
		// Connection failed
		SegmentLCD_Write("CON FAIL");
	}

}

void setupCMU(void)
{
	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_TIMER1, true);
	CMU_ClockEnable(cmuClock_DMA, true);
	CMU_ClockEnable(cmuClock_USART1, true);
}

