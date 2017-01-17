#include <stdint.h>

#ifndef BTCLIENT_H
#define BTCLIENT_H

/*  Init */

int bt_init();
int bt_close();
int bt_check();

/*  Sending */

ssize_t bt_send_ack(uint16_t ackId, uint8_t dest, uint8_t statusCode);
ssize_t bt_send_next();
ssize_t bt_send_position();
ssize_t bt_send_ball();

/*  Receiving */

int bt_recv_ack(char * msg);
int bt_recv_next(char * msg);
int bt_recv_stop(char * msg);
int bt_recv_start(char * msg);
int bt_recv_kick(char * msg);
int bt_recv_ball(char * msg);

/* Thread */
void bt_start_transmit();
void bt_stop_transmit();

/* Variables */

//extern uint16_t msgId;
//extern int s;
extern uint8_t role, side, ally;

#endif
