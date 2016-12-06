#include <stdlib.h>
#include "ev3.h"
#include "sensors.h"
#include "engines.h"
#include "position.h"
#include <unistd.h>
#include <signal.h>
#include "eye.h"
void intHandler() {
    engine_reset();
    //position_stop();
    eye_stop();
    exit(0);
}


int main ( void ) {

    signal(SIGINT, intHandler);

    if ( ev3_init() == -1 ) return ( 1 );
    engine_init();
    engine_list();
    sn_init();
//    position_start();
    eye_start();
    go_straight(7);
    turn(90,TURN_REVERSE);
    if (ball_inside){
        open_ball();
    }
    
    /*  go_straight(4);
    turn(90,TURN_FORWARD);
    go_straight(3);
    turn(-90,TURN_FORWARD);
    go_straight(2);
    turn(-90,TURN_FORWARD);
    go_straight(3);
    turn(90,TURN_FORWARD);
    go_straight(4);
*/   
   
   // position_stop();
    eye_stop();
    sleep(2);
    engine_reset();
    ev3_uninit();
}
