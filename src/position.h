#ifndef POSITION_H
#define POSITION_H

#include <pthread.h>
#include <stdbool.h>
extern pthread_t position_tid;
extern bool position_terminate;

void position_start();
void position_stop();

#endif

