#include "eye.h"
#include "engines.h"
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sensors.h"

#define Sleep( msec ) usleep(( msec ) * 1000 )


int eye_update_interval = 50; // [ms]

pthread_t eye_tid;
bool eye_terminate = false;
bool ball_inside = false;

void * eye_check(){

    int detected_color;
    while(!eye_terminate){
        detected_color = (int) sn_get_color_val();
        //printf("Detected color idx: %d\n", detected_color);
        //printf("Detected color: %s\n", color[detected_color]);

        if (detected_color == 5) {
            close_ball();
            ball_inside = true;
            return NULL;
        }
        Sleep(eye_update_interval);
    }
    return NULL;
}


void eye_start(){

    printf("Creating THE EYE threat... ");
    pthread_create(&eye_tid, NULL, eye_check, NULL);
    printf("Done\n");
}

void eye_stop(){
    printf("Waiting for THE EYE thread to terminate...\n");
    eye_terminate = true;
    pthread_join(eye_tid, NULL);
    printf("Done\n");
}





