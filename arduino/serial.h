#pragma once
#include <stdint.h>
#include <Arduino.h>

#define BAUD_RATE 9600

typedef struct packet_t
{
	uint32_t sync;
	uint16_t controller_id;
	uint16_t data;
} packet_t;

typedef union serial_packet_t
{
	packet_t fields;
	char raw[8];
} serial_packet_t;

#ifdef __cplusplus
extern "C" {
#endif
void falling_edge(uint8_t pin);
serial_packet_t packet_for(uint16_t controller);
serial_packet_t read_shift_register(uint16_t controller_id, uint8_t bit_count, uint8_t latch, uint8_t clock, uint8_t data);
#ifdef __cplusplus
}
#endif
