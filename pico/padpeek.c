#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/pio.h"
#include "shiftregister.pio.h"

#define INPUT_BASE_PIN 18
#define INPUT_LATCH_PIN 20
#define SNES_POWER_PIN 16

int main()
{
	uint32_t frame;
	uint offset;

	PIO pio = pio0;
	uint sm = 0;

	stdio_init_all();

	offset = pio_add_program(pio, &shiftregister_program);
	shiftregister_program_init(pio, sm, offset, INPUT_BASE_PIN, INPUT_LATCH_PIN);

	while(true)
	{
		frame = pio_sm_get_blocking(pio, sm);
		printf("Frame: %x\n", frame);
	}
}
