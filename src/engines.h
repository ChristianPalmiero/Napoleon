#ifndef ENGINES_H
#define ENGINES_H

enum { TURN_FORWARD, TURN_REVERSE };

int engine_init(void);
void engine_list(void);
void engine_reset(void);
void go_straight(int seconds);
void go_back(int seconds);
void engine_stop(void);
void turn(int angle, int direction);
void turn2(int angle);
void get_encoders_values(int * disp_left, int * disp_right);
void close_ball(void);
void open_ball(void);
void adjust_speed_by(int err);
//void go_to_XY(float xb, float yb);
#endif
