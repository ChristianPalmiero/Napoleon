#ifndef ENGINES_H
#define ENGINES_H

enum { TURN_FORWARD, TURN_REVERSE };

int engine_init(void);
void engine_list(void);
void engine_reset(void);
void go_straight(int seconds);
void engine_stop(void);
void turn(int angle, int direction);

void close_ball(void);
void open_ball(void);

#endif
