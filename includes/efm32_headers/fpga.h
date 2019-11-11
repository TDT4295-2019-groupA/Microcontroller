#ifndef SRC_FPGA_H_
#define SRC_FPGA_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "defines.h"
#include "midi.h"

typedef unsigned int    uint;
typedef uint8_t			byte;
typedef int8_t		    sbyte; // signed byte
typedef uint16_t        ushort;
typedef byte            NoteIndex;
typedef byte            ChannelIndex;
typedef byte          	Velocity; // goes from 0 to 127
typedef byte			Pitch;	  // also goes from 0 to 127, where 64(?) is default pitch
typedef int16_t			Sample;   // To represent a single audio "frame"
typedef unsigned int    Time;     // measured in n samples, meaning x second is represented as x * SAMPLE_RATE

#define N_GENERATORS    16 /*number of supported notes playing simultainiously  (polytones), \
                             subject to change, chisel and microcontroller code \
                             should scale from this single variable alone */

#define OVERRIDE_ON_FULL /* If this is defined the generator that has been on the longest \
 	 	 	 	 	 	 	will be overridden when a new generator is needed */

typedef struct Envelope { // either preset or controlled by knobs/buttons on the PCB
    Time   attack;
    Time   decay;
    Sample sustain; // 'percentage' of volume to sustain at, from 0 to 0x7FFF
    Time   release;
} __attribute__((packed)) Envelope;


// The following two structs represent the two packet types to be transmitted
// from the microcontroller to the FPGA:

typedef struct MicrocontrollerGlobalState {
    Velocity   master_volume;
    Envelope*  envelope;
    sbyte      pitchwheels [N_MIDI_CHANNELS];
} __attribute__((packed)) MicrocontrollerGlobalState;

typedef struct MicrocontrollerGeneratorState {
    // this is the state of a single fpga generator as seen on the microprocessor,
    // it is also the packet sent over SPI from the microcontroller to the FPGA
    // It represents the state of a single sound generator
    bool       enabled;           // whether the sound generator should be generating audio or not
    Instrument instrument;        // the index determining which waveform to use
    NoteIndex  note_index;        // to determine pitch/frequency
    byte       channel_index;     // to know which pitchwheel to use
    Velocity   velocity;          // to know which pitchwheel to use
} __attribute__((packed)) MicrocontrollerGeneratorState;

uint find_unused_generator_id(MicrocontrollerGeneratorState** generator_states);
uint find_longest_active_generator_id();
uint find_specific_generator_id(NoteIndex note_index, uint channel_index, MicrocontrollerGeneratorState** generator_states);
byte is_valid_generator_id(uint idx);
void update_generator_state(MicrocontrollerGeneratorState* generator_state, bool enabled, NoteIndex note_index, uint channel_index, Velocity velocity);
MicrocontrollerGeneratorState* generator_state_new(void);
MicrocontrollerGlobalState* global_state_new(void);

void handleMIDIEvent(MIDI_packet* m, MicrocontrollerGeneratorState** generator_states);

void microcontroller_send_global_state_update(const MicrocontrollerGlobalState* global_state);
void microcontroller_send_generator_update(ushort generator_index, bool reset_note_lifetime, const MicrocontrollerGeneratorState** generator_states);

#endif /* SRC_FPGA_H_ */
