#include <stdint.h>

#ifndef BTCLIENT_H
#define BTCLIENT_H

/*  Init */

int bt_init(int *s);
int bt_close(int *s);

/*  Sending */

ssize_t bt_send_ack(int s, uint16_t msgId, uint16_t ackId, uint8_t dest, uint8_t statusCode);
ssize_t bt_send_next(int s, uint16_t msgId, uint8_t ally);
ssize_t bt_send_position(int s, uint16_t msgId, int16_t x, int16_t y);
ssize_t bt_send_ball(int s, uint16_t msgId, uint8_t ally, uint8_t pick_notDrop, int16_t x, int16_t y);

/*  Receiving */

int bt_recv_ack(char * msg, ssize_t nbyte);
int bt_recv_next(char * msg, ssize_t nbyte);
int bt_recv_start(char * msg, ssize_t nbyte);
int bt_recv_stop(char * msg, ssize_t nbyte);
int bt_recv_kick(char * msg, ssize_t nbyte);
int bt_recv_ball(char * msg, ssize_t nbyte);

#endif
