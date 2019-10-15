#include <stdio.h>
#include "em_device.h"
#include "em_cmu.h"
#include "segmentlcd.h"
#include "input.h"
#include "usbhost.h"
#include "gpio.h"
#include "fpga.h"
#include "interrupts.h"

void setupCMU(void);

int main(void)
{
	//CHIP_Init();
	setupCMU();
	setupGPIO();
	setupNVIC();

	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
	CMU_ClockEnable(cmuClock_GPIO, true);

	SegmentLCD_Init(false);
	SegmentLCD_Write("USBHOST");

	if(USBConnect()){
		SegmentLCD_Write("CONNECT");

		while(USBIsConnected()){
			MIDI_packet input = waitForInput();
			// TODO: do stuff with input
			handleMIDIEvent(&input);
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

