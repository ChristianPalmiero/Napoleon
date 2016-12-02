#include <stdlib.h>
#include "ev3.h"
#include "sensors.h"
#include "engines.h"


int main ( void ) {

    if ( ev3_init() == -1 ) return ( 1 );
    engine_init();
    engine_list();
    sn_init();

    for (int i = 0; i < 1; i++){
    turn(360,TURN_FORWARD);
//    go_straight(5);
    //turn(90,TURN_FORWARD);
    //turn(-90,TURN_REVERSE);

    //turn(-90,TURN_FORWARD);
    //turn(90,TURN_REVERSE);
    }
    ev3_uninit();
}
