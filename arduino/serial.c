#include "serial.h"

void falling_edge(uint8_t pin)
{
	while(!digitalRead(pin)) {}
	while(digitalRead(pin)) {}
}

serial_packet_t packet_for(uint16_t controller)
{
	serial_packet_t packet;
	packet.fields.sync = 0x434e5953;
	packet.fields.data = 0xffff;
	packet.fields.controller_id = controller;
	return(packet);
}

serial_packet_t read_shift_register(uint16_t controller_id, uint8_t bit_count, uint8_t latch, uint8_t clock, uint8_t data)
{
	int i;
	serial_packet_t state = packet_for(controller_id);

	falling_edge(latch);
	for(i = 0; i < bit_count; i++)
	{
		falling_edge(clock);
		state.fields.data &= ~(uint16_t)(digitalRead(data) << i);
	}

	return(state);
}
