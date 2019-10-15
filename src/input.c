/*
 * input.c
 *
 *  Created on: 14. okt. 2019
 *      Author: Eilif
 */

#include "input.h"
#include "gpio.h"
#include "usbhost.h"
#include "defines.h"

static bool last_button_state[GPIO_BTN_COUNT] = {0};
static MIDI_packet keydown_to_midi[16] = {
		{{0x90, 0x30, 0x7f}},
		{{0x90, 0x31, 0x7f}},
		{{0x90, 0x32, 0x7f}},
		{{0x90, 0x33, 0x7f}},
		{{0x90, 0x34, 0x7f}},
		{{0x90, 0x35, 0x7f}},
		{{0x90, 0x36, 0x7f}},
		{{0x90, 0x37, 0x7f}},
		{{0x90, 0x38, 0x7f}},
		{{0x90, 0x39, 0x7f}},
		{{0x90, 0x3a, 0x7f}},
		{{0x90, 0x3b, 0x7f}},
		{{0x90, 0x3c, 0x7f}},
		{{0x90, 0x3d, 0x7f}},
		{{0x90, 0x3e, 0x7f}},
		{{0x90, 0x3f, 0x7f}}
};
static MIDI_packet keyup_to_midi[16] = {
		{{0x80, 0x30, 0x00}},
		{{0x80, 0x31, 0x00}},
		{{0x80, 0x32, 0x00}},
		{{0x80, 0x33, 0x00}},
		{{0x80, 0x34, 0x00}},
		{{0x80, 0x35, 0x00}},
		{{0x80, 0x36, 0x00}},
		{{0x80, 0x37, 0x00}},
		{{0x80, 0x38, 0x00}},
		{{0x80, 0x39, 0x00}},
		{{0x80, 0x3a, 0x00}},
		{{0x80, 0x3b, 0x00}},
		{{0x80, 0x3c, 0x00}},
		{{0x80, 0x3d, 0x00}},
		{{0x80, 0x3e, 0x00}},
		{{0x80, 0x3f, 0x00}}
};


bool connectToInput(){

#if defined(INPUT_BUTTONS) && defined(INPUT_USB) || (!defined(INPUT_BUTTONS) && !defined(INPUT_USB))
	assert(0); // Must choose 1 input device
#endif

#ifdef INPUT_BUTTONS
	return true; // No setup for buttons
#endif

#ifdef INPUT_USB
	return USBConnect();
#endif
}

bool inputConnected(){
#ifdef INPUT_BUTTONS
	return true; // Always connected
#endif

#ifdef INPUT_USB
	return USBIsConnected();
#endif
}

MIDI_packet waitForInputButtons(){
	while(true){
		for(int i = 0; i < GPIO_BTN_COUNT; i++){
			if(isButtonDown(i) != last_button_state[i]){
				last_button_state[i] = isButtonDown(i);
				if(isButtonDown(i))
					return keydown_to_midi[i];
				else
					return keyup_to_midi[i];
			}
		}
	}
	MIDI_packet def = {0};
	return def;
}

MIDI_packet waitForInputUSB(){
	USB_output usb_out = USBWaitForData();
	MIDI_packet midi_out;
	midi_out.data[0] = usb_out.data[1];
	midi_out.data[1] = usb_out.data[2];
	midi_out.data[2] = usb_out.data[3];
	return midi_out;
}

MIDI_packet waitForInput(){
#ifdef INPUT_BUTTONS
	return waitForInputButtons();
#endif

#ifdef INPUT_USB
	return waitForInputUSB();
#endif
}
