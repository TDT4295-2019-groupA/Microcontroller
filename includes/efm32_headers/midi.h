#ifndef SRC_MIDI_H_
#define SRC_MIDI_H_

#include <stdbool.h>
#include <stddef.h>

typedef unsigned char byte;
#define N_MIDI_KEYS     128
#define N_MIDI_CHANNELS 16
#define MIDI_A3_INDEX   45    /* see https://www.noterepeat.com/articles/how-to/213-midi-basics-common-terms-explained */
#define MIDI_A3_FREQ    440.0 /* no, i won't listen to your A=432Hz bullshit */

typedef enum Instrument { // we can expand this as much as we want
    SQUARE   = 0,
    TRIANGLE = 1,
    SAWTOOTH = 2,
    SINE     = 3,
} Instrument;

typedef struct MIDI_packet_info{
	byte packet_type;
	byte type_specifier;
} MIDI_packet_info;

typedef struct MIDI_packet{
	byte data[3];
} MIDI_packet;

MIDI_packet_info get_MIDI_packet_info(const byte* data);
bool validate_MIDI_packet(const byte* data, size_t length);

#endif /* SRC_MIDI_H_ */
