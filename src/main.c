#include <stdio.h>
#include <stdlib.h>
#include "em_device.h"
#include "em_cmu.h"
#include "input.h"
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

void set_decade(uint16_t val);
uint8_t conv_ascii(uint16_t val);

uint16_t ms_counter = 0;

// Array for displaying ADC result, initialize to "     mV"
uint8_t digit_array[7] = { 0x20, 0x20, 0x20, 0x20, 0x20, 'm', 'V' };

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

	uint8_t i;
	// Temp variable for storing ADC conversion results
	uint16_t adc_result = 0;
	uint32_t temp;


	/*Setup ADC
	Timebase bit field = 24, defines ADC warm up period (must be greater than or equal to number of clock cycles in 1us)
	Prescaler setting = 1: ADC clock = HFPERCLK/2 = 12MHz (ADC clock should be between 32kHz and 13MHz)
	Oversampling set to 2, no input filter, no need for Conversion Tailgating
	Warm-up mode = NORMAL (ADC is not kept warmed up between conversions)*/

	ADC0->CTRL = (24 << 16) | (1 << 8);

	ADC0->SINGLECTRL = (2 << 16) | (6 << 8);
	// Disable ADC interrupts
	ADC0->IEN = 0x0;

	// Setup Timer to trigger conversions
	// Set TOP value for Timer0
	TIMER0->TOP = 24000;
	// Enable Timer0 overflow interrupt
	TIMER0->IEN = 1;
	// Enable TIMER0 interrupt vector in NVIC
	NVIC_EnableIRQ(TIMER0_IRQn);
	// Start timer0
	TIMER0->CMD = 0x1;

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
			if(ms_counter == 500) {

				// Start Single Conversion
				ADC0->CMD = 0x1;
				// Wait for single conversion data to become valid
				while(!(ADC0->STATUS & (1 << 16)));
				// Store conversion result
				adc_result = ADC0->SINGLEDATA;

				// Change hex result to decimal result in mV
				temp = adc_result*3300;
				adc_result = temp/4095;

				//SegmentLCD_Write("%u",adc_result);

				ms_counter = 0; // reset counter
				}
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
	CMU_ClockEnable(cmuClock_USART0, true);
}

