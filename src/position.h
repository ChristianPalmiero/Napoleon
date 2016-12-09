#ifndef POSITION_H
#define POSITION_H

#include <pthread.h>
#include <stdbool.h>
extern pthread_t position_tid;
extern bool position_terminate;

void position_start();
void position_stop();
void get_dist_and_ang(float xa, float ya, float xb, float yb, float heading, float * out_dist, float * out_rotation);
void get_position_and_heading(float * x, float *y, int * heading);

#endif

