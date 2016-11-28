#include <stdlib.h>
#include "ev3.h"
#include "sensors.h"
#include "engines.h"


int main ( void ) {

    if ( ev3_init() == -1 ) return ( 1 );
    engine_init();
    engine_list();
    sn_init();

    go_straight(3);
    turn(90);
    go_straight(3);
    turn(90);
    go_straight(3);
    turn(90);
    go_straight(3);
    turn(90);
    ev3_uninit();
}
