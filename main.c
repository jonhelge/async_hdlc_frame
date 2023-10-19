#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hdlc.h"

#define MY_COMMAND 0x01
#define CONTROL_COMMAND 0x02

int test_hdlc(uint8_t command, uint8_t *payload, uint8_t payload_len)
{
	uint8_t hdlc_buffer[255];
	uint8_t encoded_len = hdlc_encode(command, payload, payload_len, hdlc_buffer);

	// Print encoded frame
	printf("Encoded frame with command %02X: ", command);
	for (int i = 0; i < encoded_len; i++)
	{
		printf("%02X ", hdlc_buffer[i]);
	}
	printf("\n");

	// Decode the frame
	uint8_t decoded_command;
	uint8_t decoded_payload[255];
	uint8_t decoded_payload_len;

	if (hdlc_decode(hdlc_buffer, encoded_len, &decoded_command, decoded_payload, &decoded_payload_len))
	{
		printf("Decoded successfully!\n");
		printf("Decoded command: %02X\n", decoded_command);
		printf("Decoded payload: ");
		for (int i = 0; i < decoded_payload_len; i++)
		{
			printf("%02X ", decoded_payload[i]);
		}
		printf("\n");
		return 0;
	}
	else
	{
		printf("Failed to decode HDLC frame.\n");
		return -1;
	}
}

int main()
{
	uint8_t my_payload[] = {0x12, 0x34, 0x7E, 0x7D, 0x56, 0x78};
	uint8_t my_payload_len = 6;

	uint8_t control_payload[] = {0xAA, 0xBB, 0xCC, 0xDD};
	uint8_t control_payload_len = 4;

	uint8_t err;

	printf("Testing MY_COMMAND...\n");
	err = test_hdlc(MY_COMMAND, my_payload, my_payload_len);
	if (err)
	{
		return err;
	}

	printf("\nTesting CONTROL_COMMAND...\n");
	err = test_hdlc(CONTROL_COMMAND, control_payload, control_payload_len);

	return err;
}
