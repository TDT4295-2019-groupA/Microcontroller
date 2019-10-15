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


bool connectToInput();
bool inputConnected();
MIDI_packet waitForInput();


#endif /* INCLUDES_EFM32_HEADERS_INPUT_H_ */
