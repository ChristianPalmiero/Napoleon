#include <stdlib.h>
#include "ev3.h"
#include "sensors.h"
#include "engines.h"
#include "position.h"
#include <unistd.h>
#include <signal.h>
#include "eye.h"
#include "nav.h"
#include "bluetooth.h"

void intHandler() {
    engine_reset();
    //position_stop();
    eye_stop();
    exit(0);
}

void test1(){
    position_start(100.0,19.0);
    go_to_XY(100.0, 177.0);
    position_stop();
}

void arena_small_beginner(){
    go_to_XY(100.0, 98.0);
    go_to_XY(74.0, 98.0);
    find_ball();  
    go_to_XY(100.0,177.0);
}

void arena_small_finisher(){
    go_to_XY(62.0, 107.0);
    sleep(4);
    open_ball();  
    go_back(1);
    go_to_XY(100.0,19.0);
}

void test3(){
    position_start(100.0, 19.0);
    //1 m forward
    go_to_XY(100.0, 119.0);
    //70 cm to the left
    go_to_XY(30.0, 119.0);
    //90 cm forward
    go_to_XY(30.0, 209.0);
    //65 cm to the right
    go_to_XY(95.0, 209.0);
    //1.73 m forward
    go_to_XY(95.0, 382.0);
    position_stop();
}

void test4(){
    position_start(100.0, 19.0);
    //1 m forward
    go_to_XY(100.0, 119.0);
    //75 cm to the left
    go_to_XY(25.0, 119.0);
    //83 cm forward
    go_to_XY(25.0, 202.0);
    //28 cm to the right
    go_to_XY(53.0, 202.0);
    sleep(3);
    open_ball();
    //sleep(2);
    //go_back(2);
    position_stop();
}

// START WITH BALL ENGINE OPEN!!!!!!!!
void test5(){
    eye_start();
    while(!ball_inside){
    	go_straight(350);
    	if(!ball_inside){
	    turn2(60);
	}
	if(!ball_inside){
	    turn2(-120);
	}
	if(!ball_inside){
	    turn2(60);
	}
    }
    eye_stop();
}


int main ( void ) {

    signal(SIGINT, intHandler);

    if ( ev3_init() == -1 ) return ( 1 );
    engine_init();
    sn_init();
    bt_start();
    

    while(1){
    
    }


    position_start(100.0,19.0);
    while(1){
	arena_small_beginner();
        arena_small_finisher();
        ball_inside = false;
        sleep(1);
    }
    position_stop();
    engine_reset();
    ev3_uninit();
}
