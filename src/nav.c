#include "engines.h"
#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "sensors.h"
#include <time.h>
#include <unistd.h>
#include "position.h"
#include "nav.h"
#include "eye.h"

#define Sleep( msec ) usleep(( msec ) * 1000 )

void go_to_XY(float xb, float yb){
    // Get current positions
    float xa, ya, distance;
    int heading, rotation;
    printf("GO_TO_XY: Going toward X: %f Y: %f\n", xb,yb);
    // PHASE 1 - Orient toward the destination
    get_position_and_heading(&xa,&ya, &heading);
    get_dist_and_ang(xa,ya,xb,yb,heading,&distance,&rotation);
    printf("GO_TO_XY: Distance: %.2f Rotation: %d\n", distance, rotation);
    turn2(rotation);

    // PHASE 2 - Go to destination
    go_straight(0);
    //multi_set_tacho_speed_sp( sn_engineLR, MAX_SPEED);
    //multi_set_tacho_command_inx(sn_engineLR, TACHO_RUN_FOREVER);
    Sleep(100);
    /// PHASE 3 - Error correction
    while ( distance >= 10.0 ){
        get_position_and_heading(&xa,&ya, &heading);
        get_dist_and_ang(xa,ya,xb,yb,heading,&distance,&rotation);
        printf("GO_TO_XY: Distance: %.2f Rotation: %d\n", distance, rotation);

        if (rotation > 20 || rotation < -20){
            turn2(rotation);
        } else if ( rotation > 1 || rotation < -1 ){
            adjust_speed_by(rotation);
	}

        Sleep(100);
    }
    engine_stop();
}


void find_ball(){
    int init_head, head;
    float val;
    eye_start();
    while(!ball_inside){
        go_straight(350);
	init_head = get_heading();
        if(!ball_inside){
            turn2(75);
        }
        if(!ball_inside){
            turn2(-140);
        } 
        if(obstacle_detected(&val, &head)){
	    printf("Obstacle detected!\n");
            head = get_heading();
            turn2(-(init_head-head));
	    go_straight(750);
	}
        else{
            turn2(75);
        }
    }
    eye_stop();
}
