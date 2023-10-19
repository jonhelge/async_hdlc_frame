#ifndef HDLC_H
#define HDLC_H

#include <stdlib.h>
#include <stdint.h>

#define FRAME_BOUNDARY 0x7E
#define ESC            0x7D
#define COMMAND_BYTE   1
#define LEN_BYTE       2

uint8_t hdlc_encode(uint8_t command, uint8_t *payload, uint8_t len, uint8_t *hdlc_buffer);
bool check_hdlc_frame(uint8_t *frame, uint8_t len);
void hdlc_strip_escape_characters(uint8_t *frame, uint8_t actual_len);
bool hdlc_decode(uint8_t *frame, uint8_t frame_len, uint8_t *command, uint8_t *payload, uint8_t *payload_len);

#endif // HDLC_H
