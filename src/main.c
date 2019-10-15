#include <stdio.h>
#include "em_device.h"
#include "em_cmu.h"
#include "segmentlcd.h"
#include "input.h"
#include "gpio.h"
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

	if(connectToInput()){
		SegmentLCD_Write("CONNECT");
		char converted[7];
		while(inputConnected()){
			MIDI_packet input = waitForInput();
			// TODO: do stuff with input
			for(int i=0; i < 3; i++) {
				sprintf(&converted[i*2], "%02X", input.data[i]);
			}
			SegmentLCD_Write(converted);
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

