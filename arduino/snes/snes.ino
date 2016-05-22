#include <string.h>
#include <stdint.h>
#include "serial.h"

#define SNES_CONTROLLER_ID 1

// Pin -> button assignments as soldered on the teensy
// Modify to match the pin assignments
#define PIN_DATA 0
#define PIN_LATCH 1
#define PIN_CLOCK 2

void setup()
{
	Serial.begin(BAUD_RATE);
	pinMode(PIN_DATA, INPUT_PULLUP);
	pinMode(PIN_LATCH, INPUT_PULLUP);
	pinMode(PIN_CLOCK, INPUT_PULLUP);
}

void loop()
{
	packet_t state = get_buttons();
	char out[8];
	memcpy(out, &state, 8);

	Serial.write(out, 8);
}

packet_t get_buttons()
{
	packet_t state;
	state.pad = 0xffff;
	state.pad2 = 0;
	state.data = 0;
	state.controller_id = SNES_CONTROLLER_ID;

	while(!digitalRead(PIN_LATCH)) { }
	for(int i = 0; i < 12; i++)
	{
		while(!digitalRead(PIN_CLOCK)) { }
		state.data |= digitalRead(PIN_DATA) << i;
		while(digitalRead(PIN_CLOCK)) { }
	}
	while(digitalRead(PIN_LATCH)) { }

	return(state);
}
