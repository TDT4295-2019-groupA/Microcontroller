#ifndef SRC_FPGA_H_
#define SRC_FPGA_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#include "midi.h"

typedef unsigned int    uint;
typedef unsigned char   byte;
typedef signed char     sbyte; // signed byte
typedef unsigned short  ushort;
typedef byte            NoteIndex;
typedef byte            ChannelIndex;
typedef byte            Velocity; // goes from 0 to 127
typedef signed short    Sample;   // To represent a single audio "frame"
typedef unsigned int    Time;     // measured in n samples, meaning x second is represented as x * SAMPLE_RATE

#define N_GENERATORS    8 /* number of supported notes playing simultainiously  (polytones), \
                             subject to change, chisel and microcontroller code \
                             should scale from this single variable alone */

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
    Envelope   envelope;
    sbyte      pitchwheels [N_MIDI_CHANNELS];
} __attribute__((packed)) MicrocontrollerGlobalState;

typedef struct MicrocontrollerGeneratorState {
    // this is the state of a single fpga generator as seen on the microprocessor,
    // it is also the packet sent over SPI from the microcontroller to the FPGA
    // It represents the state of a single sound generator
    bool       enabled;           // whether the sound generator should be generating audio or not
    Instrument instrument;        // the index determining which waveform to use
    NoteIndex  note_index;        // to determine pitch/frequency
    uint       channel_index;     // to know which pitchwheel to use
    Velocity   velocity;          // to know which pitchwheel to use
} __attribute__((packed)) MicrocontrollerGeneratorState;

uint find_unused_generator_id(const MicrocontrollerGeneratorState* generator_states);
uint find_specific_generator_id(NoteIndex note_index, uint channel_index, const MicrocontrollerGeneratorState* generator_states);
byte is_valid_generator_id(uint idx);
void update_generator_state(MicrocontrollerGeneratorState* generator_state, bool enabled, NoteIndex note_index, uint channel_index, Velocity velocity);

void handleMIDIEvent(MIDI_packet* m);

void microcontroller_send_global_state_update(const MicrocontrollerGlobalState* global_state);
void microcontroller_send_generator_update(ushort generator_index, byte reset_note_lifetime, const MicrocontrollerGeneratorState* generator_state);

#endif /* SRC_FPGA_H_ */
