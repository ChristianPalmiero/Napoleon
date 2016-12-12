#ifndef EYE_H
#define EYE_H

#include <pthread.h>
#include <stdbool.h>
extern pthread_t eye_tid;
extern bool eye_terminate;
extern bool ball_inside;
void eye_start();
void eye_stop();

#endif

