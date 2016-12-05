#include <stdlib.h>
#include "ev3.h"
#include "sensors.h"
#include "engines.h"
#include "position.h"
#include <unistd.h>
int main ( void ) {

    if ( ev3_init() == -1 ) return ( 1 );
    engine_init();
    //    engine_list();
    sn_init();
    position_start();
    sleep(1);
    for (int i = 0; i < 4; i++){

        //go_straight(2);
        //turn(90,TURN_FORWARD);
    }
    go_straight(6);
    turn(90,TURN_FORWARD);
    go_straight(2);
    turn(90,TURN_FORWARD);
    go_straight(6);
    turn(90,TURN_FORWARD);
    go_straight(2);
    turn(90,TURN_FORWARD);
    sleep(1);
    position_stop();

    ev3_uninit();
}
