#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/pio.h"
#include "shiftregister.pio.h"

#define SNES_POWER_PIN 16
#define INPUT_DATA_PIN 18
#define INPUT_CLOCK_PIN 19
#define INPUT_LATCH_PIN 20

#define LATCH_SM 0
#define CLOCK_SM 1

volatile uint clock_offset;

void __not_in_flash_func(pio_latch_irh)()
{
	pio_sm_exec(pio0, CLOCK_SM, 0x0000 | (clock_offset + 0x05));
	pio_interrupt_clear(pio0, 0);
}

void init()
{
	uint16_t latch_offset;
	PIO pio = pio0;

	latch_offset = pio_add_program(pio, &latch_program);
	clock_offset = pio_add_program(pio, &clock_program);

    irq_set_exclusive_handler(PIO0_IRQ_0, pio_latch_irh);
    irq_set_enabled(PIO0_IRQ_0, true);
    pio_set_irq0_source_enabled(pio, pis_interrupt0, true);

    hw_clear_bits(&pio->inte0, 1u << LATCH_SM);
    hw_clear_bits(&pio->inte0, 1u << CLOCK_SM);
	pio->irq = 0x3;

	latch_program_init(pio, LATCH_SM, latch_offset, INPUT_LATCH_PIN);
	clock_program_init(pio, CLOCK_SM, clock_offset, INPUT_DATA_PIN);
}

int main()
{
	uint32_t frame;
	uint8_t count = 0;

	stdio_init_all();
	init();

	while(true)
	{
		frame = pio_sm_get_blocking(pio0, CLOCK_SM);
		fprintf(stdout, "|%02x-%08x|", count++, frame);
		fflush(stdout);
	}
}
