#include "position.h"
#include "engines.h"
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "sensors.h"
// BASED ON: http://www.robotnav.com/position-estimation/

#define Sleep( msec ) usleep(( msec ) * 1000 )


int update_interval = 100; // [ms]
#define  WHEEL_DIAMETER 5.6 
#define  TICKS_PER_REVOLUTION 360.0
const float ENCODER_SCALE_FACTOR = M_PI * WHEEL_DIAMETER / TICKS_PER_REVOLUTION;

float POS_X = 0.0;
float POS_Y = 0.0;
int HEADING = 0;

pthread_t position_tid;
pthread_mutex_t position_mutex;
bool position_terminate = false;


void * update_position(){

    int ticks_left_prev;
    int ticks_left;
    int ticks_right_prev;
    int ticks_right;
    int diff_left;
    int diff_right;
    int rotation = 0;
    int angle = 0;
    int angle_prev = 0;
    float displacement;

    // Get initial values

    get_encoders_values(&ticks_left_prev,&ticks_right_prev);
    angle_prev = sn_get_gyro_val();

    while(!position_terminate){
        get_encoders_values(&ticks_left,&ticks_right);
        // INVERT DISPLACMENT - DUE TO ENGINES BEING MOUNTED UPSIDE DOWN
	if(abs(ticks_left - ticks_left_prev) > 720 ){
            ticks_left=-ticks_left;
	}
	if(abs(ticks_right - ticks_right_prev) > 720 ){
	    ticks_right=-ticks_right;
	}
	diff_left  = (ticks_left - ticks_left_prev);
        diff_right = (ticks_right -ticks_right_prev);
	displacement = (diff_left+diff_right) * ENCODER_SCALE_FACTOR / 2.0;

        angle = sn_get_gyro_val();
        rotation = angle-angle_prev;
        angle_prev = angle;
        // CALCULATE NEW X,Y
        pthread_mutex_lock(&position_mutex);
        HEADING += rotation;
        POS_X += displacement*sin(angle*M_PI/180.0);
        POS_Y += displacement*cos(angle*M_PI/180.0);
        pthread_mutex_unlock(&position_mutex);

        //printf("POS: X:\t %.2f \t Y: \t %.2f A: %d D: %.02f, DiffL: %d, DiffR: %d\n",POS_X,POS_Y, HEADING, displacement, diff_left, diff_right); 
	
	//printf("Left ticks prev: %d, Left ticks: %d, Right ticks prev: %d, Right ticks: %d\n", ticks_left_prev, ticks_left, ticks_right_prev, ticks_right);	

        ticks_left_prev = ticks_left;
        ticks_right_prev = ticks_right;
        Sleep(update_interval);
    }
    return NULL;
}

void position_start(float x_start, float y_start){
    pthread_mutex_init(&position_mutex, NULL);
    pthread_mutex_lock(&position_mutex);
    POS_X = x_start;
    POS_Y = y_start;
    pthread_mutex_unlock(&position_mutex);

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

void get_dist_and_ang(float xa, float ya, float xb, float yb, int heading, float * out_dist, int * out_rotation){
    float x = xb-xa;
    float y = yb-ya;
    float angle = atan2(y,x)*180.0/M_PI;

    // TODO: CHECK THIS UNWRAPING
    heading = heading % 360;

    int rotation = ((int)roundf(angle))-(90-heading);

    if (rotation > 180){
        rotation -= 360;
    } else if ( rotation < -180 ) {
        rotation += 360;
    }

    *out_rotation = rotation;
    *out_dist = sqrt( (x*x) + (y*y));
}

void get_position_and_heading(float * x, float *y, int * heading){
    pthread_mutex_lock(&position_mutex);
    *x = POS_X;
    *y = POS_Y;
    *heading = HEADING;
    pthread_mutex_unlock(&position_mutex);
}

int get_heading(){
    static int heading;
    pthread_mutex_lock(&position_mutex);
    heading = HEADING;
    pthread_mutex_unlock(&position_mutex);
    return heading;
}
