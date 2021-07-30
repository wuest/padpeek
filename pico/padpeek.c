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

void pio_pulse_irh()
{
	// IRQ 0 => latch
	if(pio0_hw->irq & 1)
	{
		pio_sm_exec(pio0, DATA_SM, 0x0000 | (data_offset + 0x01));
	}
	// IRQ 1 => clock
	else if(pio0_hw->irq & 2)
	{
		pio_sm_exec(pio0, DATA_SM, 0x0000 | (data_offset + 0x04));
	}
}

int main()
{
	uint32_t frame;
	uint latch_offset, clock_offset;

	PIO pio = pio0;

	stdio_init_all();

	latch_offset = pio_add_program(pio, &pulse_program);
	clock_offset = pio_add_program(pio, &pulse_program);
	data_offset = pio_add_program(pio, &data_program);

	pulse_program_init(pio, LATCH_SM, latch_offset, INPUT_LATCH_PIN);
	pulse_program_init(pio, CLOCK_SM, clock_offset, INPUT_CLOCK_PIN);
	data_program_init(pio, DATA_SM, data_offset, INPUT_DATA_PIN);

	irq_set_exclusive_handler(PIO0_IRQ_0, pio_pulse_irh);
	irq_set_exclusive_handler(PIO0_IRQ_1, pio_pulse_irh);
	irq_set_enabled(PIO0_IRQ_0, true);
	irq_set_enabled(PIO0_IRQ_1, true);
	pio0_hw->inte0 = PIO_IRQ0_INTE_SM0_BITS | PIO_IRQ0_INTE_SM1_BITS | PIO_IRQ0_INTE_SM2_BITS;

	while(true)
	{
		frame = pio_sm_get_blocking(pio, DATA_SM);
		printf("Frame: %x\n", frame);
	}
}
