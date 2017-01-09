#ifndef POSITION_H
#define POSITION_H

#include <pthread.h>
#include <stdbool.h>
extern bool position_terminate;

void position_start(float x_start, float y_start);
void position_stop();
void get_dist_and_ang(float xa, float ya, float xb, float yb, int heading, float * out_dist, int * out_rotation);
void get_position_and_heading(float * x, float *y, int * heading);
int get_heading();

#endif

