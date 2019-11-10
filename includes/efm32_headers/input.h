/*
 * input.h
 *
 *  Created on: 14. okt. 2019
 *      Author: Eilif
 */

#ifndef INCLUDES_EFM32_HEADERS_INPUT_H_
#define INCLUDES_EFM32_HEADERS_INPUT_H_

#include <stdbool.h>
#include "midi.h"
#include "defines.h"

#define CHANGE_INSTRUMENT_BUTTON 7
#define OCTAVE_DOWN_BUTTON 13
#define OCTAVE_UP_BUTTON 14

#define ADC_PORT 3 // gpioPortD (ADC Channel 6 location #0: PD4)
#define TX_pin 0
#define ADC_pin 4 // ADC Channel 4

bool connectToInput();
bool inputConnected();
int getInstrumentValue();
int getVelocityValue();
MIDI_packet waitForInput();
void handleMultipleButtonPresses();
void handleAnalogWheel();

#endif /* INCLUDES_EFM32_HEADERS_INPUT_H_ */
