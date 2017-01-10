#ifndef EYE_H
#define EYE_H

#include <pthread.h>
#include <stdbool.h>
extern pthread_t eye_tid;
extern bool eye_terminate;
extern bool ball_inside;
#define US_THRESHOLD 300
void eye_start();
void eye_stop();
bool obstacle_detected(float *val, int *head);
#endif

