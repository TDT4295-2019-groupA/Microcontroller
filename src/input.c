/*
 * input.c
 *
 *  Created on: 14. okt. 2019
 *      Author: Eilif
 *
 * 	Edited on: 18. okt. 2019
 * 		Author: Markus
 */

#include "input.h"
#include "gpio.h"
#include "usbhost.h"
#include "defines.h"
#include "fpga.h"

int octaveShiftValue = 0;
int MIDI_channelValue = 0;

int instrumentValue = 0;
int velocityValue = 0;

uint16_t ms_counter = 0;

int getInstrumentValue(){
	return instrumentValue;
}

int getVelocityValue(){
	return velocityValue;
}

//MIDI control package, standar for bytte av instrument, 0-127 valg. alt på channel 1 (0) for øyeblikket
static MIDI_cntrl_packet instruments[6] = {
		{{0xc0, 0x0a}},
		{{0xc0, 0x14}},
		{{0xc0, 0x1e}},
		{{0xc0, 0x28}},
		{{0xc0, 0x32}},
		{{0xc0, 0x3c}}
};

static bool last_button_state[GPIO_BTN_COUNT] = {0};
static MIDI_packet keydown_to_midi[GPIO_BTN_COUNT] = {
		{{0x90, MIDI_D4, 	0x7f}}, // 1 -> D
		{{0x90, MIDI_E4, 	0x7f}}, // 2 -> E
		{{0x90, MIDI_F4, 	0x7f}}, // 3 -> F
		{{0x90, MIDI_G4, 	0x7f}}, // 4 -> G
		{{0x90, MIDI_A4, 	0x7f}}, // 5 -> A
		{{0x90, MIDI_B4, 	0x7f}}, // 6 -> B
		{{0x00, 0x00, 		0x00}}, // 7 -> NO
		{{0x90, MIDI_CS4, 	0x7f}}, // 8 -> CS
		{{0x90, MIDI_DS4, 	0x7f}}, // 9 -> DS
		{{0x90, MIDI_FS4, 	0x7f}}, // 10 -> FS
		{{0x90, MIDI_GS4, 	0x7f}}, // 11 -> GS
		{{0x90, MIDI_AS4, 	0x7f}}, // 12 -> AS
		{{0x90, MIDI_C4, 	0x7f}}, // 13 -> C
		{{0x00, 0x00, 		0x00}}, // 14 -> NO
		{{0x00, 0x00, 		0x00}}, // 15 -> NO
		{{0x00, 0x00, 		0x00}}  // 16 -> NO
};
static MIDI_packet keyup_to_midi[GPIO_BTN_COUNT] = {
		{{0x80, MIDI_D4, 	0x00}}, // 1 -> D
		{{0x80, MIDI_E4, 	0x00}}, // 2 -> E
		{{0x80, MIDI_F4, 	0x00}}, // 3 -> F
		{{0x80, MIDI_G4, 	0x00}}, // 4 -> G
		{{0x80, MIDI_A4, 	0x00}}, // 5 -> A
		{{0x80, MIDI_B4, 	0x00}}, // 6 -> B
		{{0x00, 0x00, 		0x00}}, // 7 -> NO
		{{0x80, MIDI_CS4, 	0x00}}, // 8 -> CS
		{{0x80, MIDI_DS4, 	0x00}}, // 9 -> DS
		{{0x80, MIDI_FS4, 	0x00}}, // 10 -> FS
		{{0x80, MIDI_GS4, 	0x00}}, // 11 -> GS
		{{0x80, MIDI_AS4, 	0x00}}, // 12 -> AS
		{{0x80, MIDI_C4, 	0x00}}, // 13 -> C
		{{0x00, 0x00, 		0x00}}, // 14 -> NO
		{{0x00, 0x00, 		0x00}}, // 15 -> NO
		{{0x00, 0x00, 		0x00}}  // 16 -> NO
};

static int octave_shift_min = -4;
static int octave_shift = 0;
static int octave_shift_max = 3;

// Yes this is ugly thank you
static int button_is_on_keyboard[GPIO_BTN_COUNT] = {1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0};

MIDI_packet waitForInput(){
	USB_output usb_out = USBWaitForData();
	MIDI_packet midi_out;
	midi_out.data[0] = usb_out.data[1];
	midi_out.data[1] = usb_out.data[2];
	midi_out.data[2] = usb_out.data[3];
	return midi_out;
}

void handleAnalogWheel(){
		uint16_t adc_result = 0; // Temp variable for storing ADC conversion results
		uint32_t temp;

		// Setup ADC
		// Timebase bit field = 24, defines ADC warm up period (must be greater than or equal to number of clock cycles in 1us)
		// Prescaler setting = 1: ADC clock = HFPERCLK/2 = 12MHz (ADC clock should be between 32kHz and 13MHz)
		// Oversampling set to 2, no input filter, no need for Conversion Tailgating
		// Warm-up mode = NORMAL (ADC is not kept warmed up between conversions)
		ADC0->CTRL = (24 << 16) | (1 << 8);

		// Can use single-cycle acquisition time since we are spacing out our conversions using a timer
		// Use buffered Vdd as reference, use Channel 6 as input to single conversion
		// 12-bit resolution, right-justified, single-ended input, single conversion
		//Might have to change (4 << 8) to (6 << 8)
		ADC0->SINGLECTRL = (2 << 16) | (4 << 8);
		ADC0->IEN = 0x0; // Disable ADC interrupts

		// Setup Timer to trigger conversions
		TIMER0->TOP = 24000;							// Set TOP value for Timer0
		TIMER0->IEN = 1;								// Enable Timer0 overflow interrupt
		NVIC_EnableIRQ(TIMER0_IRQn);					// Enable TIMER0 interrupt vector in NVIC
		TIMER0->CMD = 0x1;								// Start timer0

		while(1) {
			if(ms_counter == 500) {

				ADC0->CMD = 0x1;						// Start Single Conversion
				while(!(ADC0->STATUS & (1 << 16)));		// Wait for single conversion data to become valid
				adc_result = ADC0->SINGLEDATA;			// Store conversion result

				// Change hex result to decimal result in V
				temp = adc_result*3300;
				adc_result = temp/4095;
				velocityValue = ((adc_result/1000)*38)+1;
				ms_counter = 0; // reset counter
			}
		  }
}

void handleMultipleButtonPresses(MicrocontrollerGeneratorState** generator_states){
	for(int i = 0; i < GPIO_BTN_COUNT; i++){
		if(last_button_state[i] != isButtonDown(i)){
			last_button_state[i] = isButtonDown(i);
			if(button_is_on_keyboard[i]){ // Handle buttonkeyboard events
				MIDI_packet packet_to_send;
				if(isButtonDown(i))
					packet_to_send = keydown_to_midi[i];
				else
					packet_to_send = keyup_to_midi[i];

				// Change octave of packet
				packet_to_send.data[1] += octave_shift * NOTES_IN_OCTAVE;

				handleMIDIEvent(&packet_to_send, generator_states);
			}
			else{ // Handle buttonmenu events
				if(isButtonDown(i)){
					if(i == CHANGE_INSTRUMENT_BUTTON){
						// Change instrument
						if(instrumentValue <= 3)
							instrumentValue++;
						if(instrumentValue >3)
							instrumentValue = 0;
					}
					else if(i == OCTAVE_DOWN_BUTTON){
						octave_shift--;
						if (octave_shift < octave_shift_min) octave_shift = octave_shift_min;
					}
					else if(i == OCTAVE_UP_BUTTON){
						octave_shift++;
						if (octave_shift > octave_shift_max) octave_shift = octave_shift_max;
					}

				}
			}
		}
	}
}

