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
    // PHASE 1 - Orient toward the destination
    get_position_and_heading(&xa,&ya, &heading);
    printf("GO_TO_XY: Going toward X: %f Y: %f from X:%f Y:%f\n", xb,yb, xa, ya);
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
        printf("GO_TO_XY: Distance: %.2f Rotation: %d towards X: %f Y:%f from X: %f Y:%f\n", distance, rotation, xb, yb, xa, ya);

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
    int init_head, head, head_us, right_t, angle=75;
    float val;
    eye_start();
    while(!ball_inside){
        
	init_head = get_heading();
	printf("FIND BALL: Initial heading: %d\n", init_head);
        if(!ball_inside){
            turn2(angle);
        }
        if(!ball_inside){
	    head = get_heading();
	    printf("FIND BALL: heading: %d, turning by: %d\n", head, -2*(init_head-head));
            turn2(-2*(init_head-head));
        } 
	
        if(!ball_inside && obstacle_detected(&val, &head_us)){
	    printf("Obstacle detected!\n");
            head = get_heading();
	    printf("FIND BALL: head_us %d, head %d\n", head_us, head);
            turn2(-(head_us-head));
	    right_t = (int)((val-45)/290.0*1000);
	    printf("FIND BALL: Right_t: %d\n", right_t);
	    go_straight(right_t);
	    angle = 30+45*((val-45)/(US_THRESHOLD-45));
	}
        else if(!ball_inside){
            head = get_heading();
            turn2(-(init_head-head));
            go_straight(350);
        }
    }
    eye_stop();
}
