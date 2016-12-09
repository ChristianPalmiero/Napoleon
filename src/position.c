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

// TODO: Add mutex!
float POS_X;
float POS_Y;
int HEADING = 0;

pthread_t position_tid;
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
        diff_left  = (ticks_left - ticks_left_prev);
        diff_right = (ticks_right -ticks_right_prev);
        displacement = (diff_left+diff_right) * ENCODER_SCALE_FACTOR / 2.0;

        angle = sn_get_gyro_val();
        rotation = angle-angle_prev;
        HEADING += rotation;

        // CALCULATE NEW X,Y

        POS_X += displacement*sin(angle*M_PI/180.0);
        POS_Y += displacement*cos(angle*M_PI/180.0);

        printf("Position thread: \n");
        printf("LEFT \tOLD: %d\t NEW: %d\t DISP: %d\n", ticks_left_prev, ticks_left, diff_left);
        printf("RIGHT \tOLD: %d\t NEW: %d\t DISP: %d\n", ticks_right_prev, ticks_right, diff_right);
        printf("ANGLE: \t %.0f \t DISPLACEMENT: %.02f\n", angle, displacement);
        printf("X:\t %.2f \t Y: \t %.2f\n",POS_X,POS_Y); 

        ticks_left_prev = ticks_left;
        ticks_right_prev = ticks_right;
        Sleep(update_interval);
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

void get_dist_and_ang(float xa, float ya, float xb, float yb, int heading, float * out_dist, int * out_rotation){
    float x = xb-xa;
    float y = yb-ya;
    float angle = atan2(y,x)*180.0/M_PI;

    // TODO: CHECK THIS UNWRAPING
    heading = heading % 360;

    int rotation = ((int)roundf(angle))-(90-heading);

    if (rotation > 180){
        rotation = -360+(rotation);
    } else if ( rotation < -180 ) {
        rotation = 360+*rotation;
    }roundf()

    *out_rotation = rotation;
    *out_dist = sqrt( (x*x) + (y*y));
}

void get_position_and_heading(float * x, float *y, int * heading){
    // TODO: Add mutex!
    *x = POS_X;
    *y = POS_Y;
    *heading = HEADING;
}
