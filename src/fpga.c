#include "fpga.h"
// #include "spi_master.h" I need this

uint find_unused_generator_id(const MicrocontrollerGeneratorState* generator_states)
{
	uint idx = 0;
	while (idx < N_GENERATORS && generator_states[idx].enabled) idx++;
	return idx;
}

uint find_specific_generator_id(NoteIndex note_index, uint channel_index, const MicrocontrollerGeneratorState* generator_states)
{
	uint idx = 0; // sound_generator_index
	while (idx < N_GENERATORS && !(
		generator_states[idx].enabled
		&& generator_states[idx].note_index    == note_index
		&& generator_states[idx].channel_index == channel_index
	)) idx++;
	return idx;
}

byte is_valid_generator_id(uint idx)
{
	return idx < N_GENERATORS;
}

void update_generator_state(MicrocontrollerGeneratorState* generator_state, bool enabled, NoteIndex note_index, uint channel_index, Velocity velocity)
{
	generator_state->enabled = enabled;
	generator_state->note_index = note_index;
	generator_state->channel_index = channel_index;
	generator_state->velocity = velocity;
}

void microcontroller_handle_midi_event(const byte *data, size_t length, MicrocontrollerGeneratorState* generator_states) {
    // The UART interrupt handler should call this function when it has recieved a full midi event

    MIDI_packet_info packet_info = get_MIDI_packet_info(data);

    // validate packet:
    if(!validate_MIDI_packet(data, length)) return; // ignore invalid packets

    // interpret and handle packet:
    switch (packet_info.packet_type) {
        // see https://www.midi.org/specifications-old/item/table-1-summary-of-midi-message
        break; case 0b1000: { // note-off event
            assert(length == 3);
            ChannelIndex   channel  = packet_info.type_specifier;
            NoteIndex      note     = data[1];
            Velocity       velocity = data[2];

            // find the sound generator currenty playing this note
            uint idx = find_specific_generator_id(note, channel, generator_states);
            if (!is_valid_generator_id(idx)) return; // none found, probably due to the note-on being ignored due to lack of generators

            update_generator_state(generator_states + idx, false, note, channel, velocity);
            microcontroller_send_generator_update(idx, false, generator_states);
        }
        break; case 0b1001: { // note-on event
            assert(length == 3);
            ChannelIndex   channel  = packet_info.type_specifier;
            NoteIndex      note     = data[1];
            Velocity       velocity = data[2];

            // find vacant sound generator
            uint idx = find_unused_generator_id(generator_states); // sound_generator_index
            if (!is_valid_generator_id(idx)) return; // out of sound generators, ignore

            update_generator_state(generator_states + idx, true, note, channel, velocity);
            microcontroller_send_generator_update(idx, true, generator_states);
        }
        break; case 0b1010: /*IGNORE*/ // Polyphonic Key Pressure (Aftertouch) event
        break; case 0b1011: /*IGNORE*/ // Control Change event
        break; case 0b1100: /*IGNORE*/ // Program Chang event
        break; case 0b1101: /*IGNORE*/ // Channel Pressure (After-touch) event
        break; case 0b1110: /*IGNORE*/ // Pitch Bend Change event
        break; case 0b1111: /*IGNORE*/ // System Exclusive event
        break; default: break;         // unknown - ignored
    }
}

void microcontroller_send_global_state_update(const MicrocontrollerGlobalState* global_state)
{
	byte data[1 + sizeof(MicrocontrollerGlobalState)];

	data[0] = 1; // global_state update

	memcpy(data+1, global_state, sizeof(MicrocontrollerGlobalState));

	spiDmaTransfer((uint8_t*)data, NULL,  sizeof(data));
}

void microcontroller_send_generator_update(ushort generator_index, byte reset_note_lifetime, const MicrocontrollerGeneratorState* generator_state)
{
	 // set reset_note_lifetime to true when sending note-on events
	byte data[2 + sizeof(ushort) + sizeof(MicrocontrollerGeneratorState)];

	data[0] = 2; // generator update

	data[1] = (byte) reset_note_lifetime;
	*(ushort*)(&data[2]) = generator_index;
	memcpy(data+2+sizeof(ushort), generator_state, sizeof(MicrocontrollerGeneratorState));

	spiDmaTransfer((uint8_t*)data, NULL,  sizeof(data));
}
