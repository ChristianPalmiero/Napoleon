#include <stdint.h>

#ifndef BTCLIENT_H
#define BTCLIENT_H

int read_from_server (int sock, char *buffer, size_t maxSize);
int bt_init(int *s);
int bt_close(int *s);
ssize_t bt_send_ack(int s, uint16_t msgId, uint16_t ackId, uint8_t dest, uint8_t statusCode);
ssize_t bt_send_next(int s, uint16_t msgId, uint8_t ally);
ssize_t bt_start(int s, uint8_t *role, uint8_t *side, uint8_t *ally);
ssize_t bt_stop(int s);
ssize_t bt_kick(int s);
ssize_t bt_send_position(int s, uint16_t msgId, int16_t x, int16_t y);
ssize_t bt_send_ball(int s, uint16_t msgId, uint8_t ally, uint8_t pick_notDrop, int16_t x, int16_t y);

#endif
