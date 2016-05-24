#include <string.h>
#include <stdint.h>
#include "serial.h"

#define SNES_CONTROLLER_ID 1
#define BIT_COUNT 12

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
	serial_packet_t state = read_shift_register(SNES_CONTROLLER_ID, BIT_COUNT, PIN_LATCH, PIN_CLOCK, PIN_DATA);
	Serial.write(state.raw, 8);
}
