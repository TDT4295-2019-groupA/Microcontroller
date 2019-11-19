#include <stdio.h>
#include "fpga.h"
#include "spi.h"
#include "input.h"

uint find_unused_generator_id(MicrocontrollerGeneratorState** generator_states)
{
	uint idx = 0;
	while (idx < N_GENERATORS && generator_states[idx]->enabled) idx++;
	return idx;
}

static uint generator_activation = 0; // Total number of generator activations
static uint generator_activation_count[N_GENERATORS] = {0}; // Total count last time a generator was activated

uint find_longest_active_generator_id(){
	uint id = 0;
	uint lowest_count = (uint)-1;
	for(uint i = 0; i < N_GENERATORS; i++){
		if(generator_activation_count[i] < lowest_count){
			lowest_count = generator_activation_count[i];
			id = i;
		}
	}
	return id;
}

int find_vacant_generator_channel(MicrocontrollerGeneratorState** generator_states) {
    // we need to assign notes to generators in a round-robin fashion to avoid
    // overruling the generators which are still generating the release sound too much

    static int starting_pos = 0;
    int pos = starting_pos;
    while (generator_states[pos]->enabled) {
        pos++;
        if (pos >= N_GENERATORS) pos = 0;
        if (pos == starting_pos) return -1;

    }

    starting_pos = (pos+1) % N_GENERATORS;
    return pos;
}

uint find_specific_generator_id(NoteIndex note_index, uint channel_index, MicrocontrollerGeneratorState** generator_states)
{
	uint idx = 0; // sound_generator_index
	while (idx < N_GENERATORS && !(
		generator_states[idx]->enabled
		&& generator_states[idx]->note_index    == note_index
		&& generator_states[idx]->channel_index == channel_index
	)) idx++;
	return idx;
}

byte is_valid_generator_id(uint idx)
{
	return idx < N_GENERATORS;
}

void update_generator_state(MicrocontrollerGeneratorState* generator_state, bool enabled, NoteIndex note_index, uint channel_index, Velocity velocity)
{
	generator_activation++;
	generator_state->enabled = enabled;
	generator_state->note_index = note_index;
	generator_state->channel_index = channel_index;
	generator_state->velocity = velocity;
	generator_state->instrument = getInstrumentValue();
}

void handleMIDIEvent(MIDI_packet* m, MicrocontrollerGeneratorState** generator_states) {
	char converted[7];

	for(int i=0; i < 3; i++) {
		sprintf(&converted[i*2], "%02X", m->data[i]);
	}
	converted[6] = '\n';
    // The UART interrupt handler should call this function when it has recieved a full midi event
    MIDI_packet_info packet_info = get_MIDI_packet_info(m->data);

    // validate packet:
    //if(!validate_MIDI_packet(m->data, sizeof(m->data))) return; // ignore invalid packets
    // interpret and handle packet:
    switch (packet_info.packet_type) {
        // see https://www.midi.org/specifications-old/item/table-1-summary-of-midi-message
        break; case 0b1000: { note_off_event: ; // note-off event (our keyboard does not send this)
            //assert(length == 3);
            ChannelIndex   channel  = packet_info.type_specifier;
            NoteIndex      note     = m->data[1];
            Velocity       velocity = m->data[2];

            if (channel == 9) return; // ignore drums

            // find the sound generator currenty playing this note
			uint idx = find_specific_generator_id(note, channel, generator_states);
			if (!is_valid_generator_id(idx)) return; // none found, probably due to the note-on being ignored due to lack of generators

			update_generator_state(generator_states[idx], false, note, channel, velocity);
			microcontroller_send_generator_update(idx, false, generator_states);
        }
        break; case 0b1001: { // note-on event
            //assert(length == 3);
            ChannelIndex   channel  = packet_info.type_specifier;
            NoteIndex      note     = m->data[1];
            Velocity       velocity = m->data[2];

            if (channel == 9) return; // ignore drums
            if (velocity == 0) goto note_off_event; // people suck at following the midi standard

            // find vacant sound generator
			uint idx = find_unused_generator_id(generator_states); // sound_generator_index
			if (!is_valid_generator_id(idx)) { // out of generators, ignore
#ifdef OVERRIDE_ON_FULL
				idx = find_longest_active_generator_id();
#endif
            }

			update_generator_state(generator_states[idx], true, note, channel, velocity);
			microcontroller_send_generator_update(idx, true, generator_states);
			generator_activation_count[idx] = generator_activation;
        }
        break; case 0b1010:  // Polyphonic Key Pressure (Aftertouch) event
        break; case 0b1011:  // Control Change event
        break; case 0b1100:  // Program Chang event
        break; case 0b1101:  // Channel Pressure (After-touch) event
        break; case 0b1110: { // Pitch Bend Change event
        	Pitch pitch = m->data[2];
        }
        break; case 0b1111:  // System Exclusive event
        break; default: break;         // unknown - ignored
    }
}

void microcontroller_send_global_state_update(const MicrocontrollerGlobalState* global_state)
{
	byte data[1 + sizeof(MicrocontrollerGlobalState)]; // TODO remove the +1, its so we have room for a null byte

	data[0] = 1; // global_state update

	memcpy(data+1, global_state, sizeof(MicrocontrollerGlobalState));

	spi_transmit((byte*)data, sizeof(data));
}

void microcontroller_send_generator_update(ushort generator_index, bool reset_note_lifetime, const MicrocontrollerGeneratorState** generator_states)
{
	 // set reset_note_lifetime to true when sending note-on events
	byte data[2 + sizeof(ushort) + sizeof(MicrocontrollerGeneratorState)]; // TODO remove the +1, its so we have room for a null byte

	data[0] = 1; // here comes a package

	*(ushort*)(&data[1]) = generator_index;
	data[3] = (byte) reset_note_lifetime;

	memcpy(data+3+sizeof(ushort), generator_states[generator_index], sizeof(MicrocontrollerGeneratorState));

	spi_transmit((byte*)data, sizeof(data));
}

// TODO write code to free these structs, or else we will all suffer
MicrocontrollerGeneratorState* generator_state_new() {
	MicrocontrollerGeneratorState* generator_state = malloc(sizeof(MicrocontrollerGeneratorState));
	generator_state->channel_index = 0;
	generator_state->enabled = false;
	// TODO give reasonable defaultvalues here. i am lazy.
	generator_state->instrument = 0;
	generator_state->note_index = 0;
	generator_state->velocity = 0;
	return generator_state;

}

MicrocontrollerGlobalState* global_state_new(void) {
	MicrocontrollerGlobalState* global_state = malloc(sizeof(MicrocontrollerGlobalState));
	global_state->master_volume = 0;
	// TODO give reasonable default values here. i am lazy.
	global_state->envelope = NULL;
	for (uint i = 0; i < N_MIDI_CHANNELS; i++)
		global_state->pitchwheels[i] = 0;
	return global_state;
}

