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

void arena_small_finisher(){
    position_start(30,175,180);
    go_to_XY(30,100);
    go_to_XY(50, 100);
    find_ball();  
    go_to_XY(30,30);
}

void arena_small_beginner(){
    position_start(90,30,0);
    go_to_XY(90, 100);
    go_to_XY(70,100);
    sleep(4);
    open_ball();  
    go_back(1);
    go_to_XY(90,175);
}

void arena_big_beginner(int s){
    position_start(s*30.0, 19.0, 0);
    go_to_XY(s*100.0, 119.0);
    go_to_XY(s*100.0, 205.0);
    go_to_XY(s*45.0, 205.0);
    turn2(180);
    sleep(4);
    open_ball();
    bt_send_ball();
    go_back(1);
    turn2(s*90);
    go_to_XY(s*30.0, 380.0);
    bt_send_next();
}

void arena_big_finisher(int s){
    position_start(s*90.0, 380.0, -180);
    go_to_XY(s*35.0, 280.0);
    go_to_XY(s*35.0, 205.0);
    turn2(s*90);
    go_straight(500);
    find_ball();
    bt_send_ball();
    go_to_XY(s*100.0, 205.0);
    go_to_XY(s*100.0, 25.0);
    bt_send_next();
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
    engine_reset();
}
