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

bool connectToInput();
bool inputConnected();
MIDI_packet waitForInput();
void handleMultipleButtonPresses();

#endif /* INCLUDES_EFM32_HEADERS_INPUT_H_ */
