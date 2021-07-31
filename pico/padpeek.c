#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/irq.h"
#include "hardware/pio.h"
#include "shiftregister.pio.h"

#define SNES_POWER_PIN 16
#define INPUT_CLOCK_PIN 18
#define INPUT_DATA_PIN 19
#define INPUT_LATCH_PIN 20

#define LATCH_SM 0
#define CLOCK_SM 1
#define DATA_SM 2

volatile uint data_offset;

void __not_in_flash_func(pio_latch_irh)()
{
	pio_sm_exec(pio0, DATA_SM, 0x0000 | (data_offset + 0x01));
}

void __not_in_flash_func(pio_clock_irh)()
{
	pio_sm_exec(pio0, DATA_SM, 0x0000 | (data_offset + 0x04));
}

int main()
{
	uint16_t frame;
	uint latch_offset, clock_offset;

	PIO pio = pio0;

	stdio_init_all();

	latch_offset = pio_add_program(pio, &latch_program);
	clock_offset = pio_add_program(pio, &clock_program);
	data_offset = pio_add_program(pio, &data_program);

    irq_set_exclusive_handler(PIO0_IRQ_0, pio_latch_irh);
    irq_set_enabled(PIO0_IRQ_0, true);
    irq_set_exclusive_handler(PIO0_IRQ_1, pio_clock_irh);
    irq_set_enabled(PIO0_IRQ_1, true);
    pio_set_irq0_source_enabled(pio, pis_interrupt0, true);
    pio_set_irq1_source_enabled(pio, pis_interrupt1, true);

    hw_clear_bits(&pio->inte0, 1u << LATCH_SM);
    hw_clear_bits(&pio->inte1, 1u << CLOCK_SM);
    hw_clear_bits(&pio->inte1, 1u << DATA_SM);
	pio->irq = 0x7;

	data_program_init(pio, DATA_SM, data_offset, INPUT_DATA_PIN);
	latch_program_init(pio, LATCH_SM, latch_offset, INPUT_LATCH_PIN);
	clock_program_init(pio, CLOCK_SM, clock_offset, INPUT_CLOCK_PIN);
	while(true)
	{
		frame = pio_sm_get_blocking(pio, DATA_SM);
		printf("Frame: %x\n", frame);
	}
}
