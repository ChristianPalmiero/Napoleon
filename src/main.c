#include <stdlib.h>
#include "ev3.h"
#include "sensors.h"
#include "engines.h"
#include "position.h"
#include <unistd.h>
int main ( void ) {

    if ( ev3_init() == -1 ) return ( 1 );
    engine_init();
    engine_list();
    sn_init();

    for (int i = 0; i < 1; i++){
    
        position_start();
        sleep(2);
        position_stop();
    }
    ev3_uninit();
}
