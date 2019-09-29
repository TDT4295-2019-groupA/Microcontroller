#include "midi.h"

MIDI_packet_info get_MIDI_packet_info(const byte* data)
{
	MIDI_packet_info packet_info;
	byte status_byte    = data[0];
	packet_info.packet_type    = (status_byte & 0xF0) >> 4;
	packet_info.type_specifier = (status_byte & 0x0F);

	return packet_info;
}

bool validate_MIDI_packet(const byte* data, size_t length)
{
	for (size_t i = 1; i < length; i++)
		if (((bool)(data[i] & 0x80)) == (i==0))
			return false;
	return true;
}
