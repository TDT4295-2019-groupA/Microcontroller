#ifndef SRC_MIDI_H_
#define SRC_MIDI_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef unsigned char byte;
#define N_MIDI_KEYS     128
#define N_MIDI_CHANNELS 16

#define NOTES_IN_OCTAVE 12
#define MIDI_C4 	60
#define MIDI_CS4 	61
#define MIDI_D4 	62
#define MIDI_DS4 	63
#define MIDI_E4 	64
#define MIDI_F4 	65
#define MIDI_FS4 	66
#define MIDI_G4 	67
#define MIDI_GS4 	68
#define MIDI_A4 	69
#define MIDI_AS4 	70
#define MIDI_B4 	71

/*
typedef enum Instrument { // we can expand this as much as we want
    SQUARE   = 0,
    TRIANGLE = 1,
    SAWTOOTH = 2,
    SINE     = 3,
} Instrument;
*/
typedef uint8_t Instrument;

typedef struct MIDI_packet_info{
	byte packet_type;
	byte type_specifier;
} MIDI_packet_info;

typedef struct MIDI_packet{
	byte data[3];
} MIDI_packet;

typedef struct MIDI_cntrl_packet{
	byte data[2];
} MIDI_cntrl_packet;

MIDI_packet_info get_MIDI_packet_info(const byte* data);
bool validate_MIDI_packet(const byte* data, size_t length);
MIDI_packet *convertToMidi(unsigned char *usbpacket);

#endif /* SRC_MIDI_H_ */
