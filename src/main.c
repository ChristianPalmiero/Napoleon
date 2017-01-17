#include <stdlib.h>
#include <stdio.h>
#include "ev3.h"
#include "sensors.h"
#include "engines.h"
#include "position.h"
#include <unistd.h>
#include <signal.h>
#include "eye.h"
#include "nav.h"
#include "btclient.h"

void intHandler() {
    engine_reset();
    //position_stop();
    eye_stop();
    exit(0);
}

void arena_small_beginner(){
    position_start(100.0,19.0,0);
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

void arena_big_beginner(int s){
    position_start(s*30.0, 19.0, 0);
    go_to_XY(s*100.0, 119.0);
    go_to_XY(s*100.0, 205.0);
    go_to_XY(s*45.0, 205.0);
    turn2(180);
    sleep(4);
    open_ball();
    go_back(1);
    turn2(s*90);
    go_to_XY(s*30.0, 380.0);
}

void arena_big_finisher(int s){
    position_start(s*90.0, 380.0, -180);
    go_to_XY(s*35.0, 280.0);
    go_to_XY(s*35.0, 205.0);
    turn2(s*90);
    go_straight(1000);
    find_ball();
    go_to_XY(s*100.0, 205.0);
    go_to_XY(s*110.0, 19.0);
}

int main ( void ) {
    signal(SIGINT, intHandler);

    if ( ev3_init() == -1 ) return ( 1 );
    engine_init();
    sn_init();

    /* if connected */
    if( bt_init() == 0 ) {
        printf("Connected!\n");
        bt_start_transmit();
        while(1){
	    bt_check();
        }
    }
    else {
        fprintf (stderr, "Failed to connect to server...\n");
        sleep (2);
        exit (EXIT_FAILURE);
    }
}
