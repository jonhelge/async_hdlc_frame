#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "hdlc.h"

#ifndef ZEPHYR_VERSION
#include <stdio.h>
#define LOG_INF(...)     \
	printf(__VA_ARGS__); \
	printf("\n")
#else
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(hdlc, LOG_LEVEL_DBG);
#endif

uint16_t crc16_ccitt(uint16_t crc, const void *buf, int len)
{
	const uint8_t *data = (const uint8_t *)buf;
	while (len--)
	{
		crc ^= (*data++) << 8;
		for (int i = 0; i < 8; i++)
		{
			if (crc & 0x8000)
			{
				crc = (crc << 1) ^ 0x1021;
			}
			else
			{
				crc = crc << 1;
			}
		}
	}
	return crc;
}

uint8_t hdlc_encode(uint8_t command, uint8_t *payload, uint8_t len, uint8_t *hdlc_buffer)
{
	hdlc_buffer[0] = FRAME_BOUNDARY;
	hdlc_buffer[COMMAND_BYTE] = command;
	hdlc_buffer[LEN_BYTE] = len;

	int hdlc_buffer_counter = LEN_BYTE + 1;

	for (int i = 0; i < len; i++)
	{
		if (payload[i] == FRAME_BOUNDARY || payload[i] == ESC)
		{
			hdlc_buffer[hdlc_buffer_counter++] = ESC;
			hdlc_buffer[hdlc_buffer_counter++] = payload[i] ^ 0x20; // Invert the 5th bit
		}
		else
		{
			hdlc_buffer[hdlc_buffer_counter++] = payload[i];
		}
	}
	hdlc_buffer[LEN_BYTE] = hdlc_buffer_counter - 3;

	uint16_t crc = crc16_ccitt(0, &hdlc_buffer[COMMAND_BYTE], hdlc_buffer_counter - COMMAND_BYTE);
	hdlc_buffer[hdlc_buffer_counter++] = (uint8_t)(crc >> 8);
	hdlc_buffer[hdlc_buffer_counter++] = (uint8_t)crc;
	hdlc_buffer[hdlc_buffer_counter++] = FRAME_BOUNDARY;
	return hdlc_buffer_counter;
}

bool check_hdlc_frame(uint8_t *frame, uint8_t len)
{
	if (len < 6)
	{
		LOG_INF("too short");
		return false;
	}
	if (len != frame[LEN_BYTE] + 6)
	{
		LOG_INF("wrong length");
		return false;
	}

	uint16_t crc = (frame[len - 3] << 8) | frame[len - 2];
	uint16_t calculated_crc = crc16_ccitt(0, &frame[COMMAND_BYTE], len - COMMAND_BYTE - 3);

	if (crc == calculated_crc)
	{
		LOG_INF("crc ok!");
		return true;
	}
	else
	{
		LOG_INF("wrong crc");
		return false;
	}
}

void hdlc_strip_escape_characters(uint8_t *frame, uint8_t actual_len)
{
	int write_index = LEN_BYTE + 1;
	int read_index = LEN_BYTE + 1;

	while (read_index < actual_len)
	{
		if (frame[read_index] == ESC)
		{
			frame[write_index++] = frame[++read_index] ^ 0x20;
			read_index++;
		}
		else
		{
			frame[write_index++] = frame[read_index++];
		}
	}

	frame[LEN_BYTE] = write_index - LEN_BYTE - 1;
}

bool hdlc_decode(uint8_t *frame, uint8_t frame_len, uint8_t *command, uint8_t *payload, uint8_t *payload_len)
{
	if (!check_hdlc_frame(frame, frame_len))
	{
		LOG_INF("Invalid HDLC frame");
		return false;
	}

	hdlc_strip_escape_characters(frame, frame_len);

	*command = frame[COMMAND_BYTE];
	*payload_len = frame[LEN_BYTE];

	memcpy(payload, &frame[LEN_BYTE + 1], *payload_len);

	return true;
}
