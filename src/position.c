#include "position.h"
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// BASED ON: http://www.robotnav.com/position-estimation/


pthread_t position_tid;
bool position_terminate = false;

int update_interval = 100; // [ms]
int POS_X;
int POS_Y;

void * update_position(){
    for (int i = 0; i < 5; i++){
        if (!position_terminate){
            printf("Position thread: %d\n",i);
            sleep(1);
        }
    }
    return NULL;
}

void position_start(){
    POS_X = 0;
    POS_Y = 0;

    printf("Creating position tracking threat... ");
    pthread_create(&position_tid, NULL, update_position, NULL);
    printf("Done\n");
}

void position_stop(){
    printf("Waiting for position thread to terminate...\n");
    position_terminate = true;
    pthread_join(position_tid, NULL);
    printf("Done\n");
}





