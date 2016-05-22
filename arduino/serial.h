#pragma once

#define BAUD_RATE 9600

typedef struct packet_t
{
	uint16_t pad;
	uint16_t controller_id;
	uint16_t data;
	uint16_t pad2;
} packet_t;

