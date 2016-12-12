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


void test1(){
    position_start(100.0,0.0);
    go_to_XY(100.0, 175.0);
}
void test2(){
    position_start(100.0,10.0);
    go_to_XY(100.0, 90.0);
    go_to_XY(90.0, 90.0);
}
void test3(){

}
void test4(){

}
void test5(){

}


int main ( void ) {

    signal(SIGINT, intHandler);

    if ( ev3_init() == -1 ) return ( 1 );
    engine_init();
    engine_list();
    sn_init();
    //position_start(0.0,0.0);
    //eye_start();

    test2();

    position_stop();
    //eye_stop();
    sleep(1);
    engine_reset();
    ev3_uninit();
}
