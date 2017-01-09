#include "eye.h"
#include "engines.h"
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sensors.h"
#include "position.h"

#define Sleep( msec ) usleep(( msec ) * 1000 )
#define US_THRESHOLD 300

int eye_update_interval = 50; // [ms]
int smallest_head;
float smallest_val;
pthread_t eye_tid;
bool eye_terminate = false;
bool ball_inside = false;
pthread_mutex_t nav_mutex;

void reset_value(){
    smallest_head = 0;
    smallest_val = 9999.0;
}

void * eye_check(){
    int detected_color;
    float ball_distance;

    while(!eye_terminate){
        detected_color = (int) sn_get_color_val();
        ball_distance = sn_get_sonar_val();
	//printf("Detected distance: %f\n", ball_distance);
	if (ball_distance < US_THRESHOLD){
	    pthread_mutex_lock(&nav_mutex);
            smallest_val = ball_distance;
	    smallest_head = get_heading();
            pthread_mutex_unlock(&nav_mutex);
	}
 
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
    reset_value();
    pthread_mutex_init(&nav_mutex, NULL);
    printf("Creating THE EYE threat... ");
    pthread_create(&eye_tid, NULL, eye_check, NULL);
    printf("Done\n");
}

bool obstacle_detected(float *val, int *head){
    pthread_mutex_lock(&nav_mutex);       
    if(smallest_val<US_THRESHOLD){
        *val = smallest_val;
        *head = smallest_head;
    printf("\n\nSmallest Value: %f\n\n", smallest_val);
        reset_value();
 	pthread_mutex_unlock(&nav_mutex);       
        printf("\n\nValue: %f, Heading: %d\n\n", *val, *head);
        return true;
    }
    else{
        reset_value();
        pthread_mutex_unlock(&nav_mutex);
        return false;
    }
}

void eye_stop(){
    printf("Waiting for THE EYE thread to terminate...\n");
    eye_terminate = true;
    pthread_join(eye_tid, NULL);
    printf("Done\n");
}
