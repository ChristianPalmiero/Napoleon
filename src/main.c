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
    position_start(100.0,19.0);
    go_to_XY(100.0, 177.0);
    position_stop();
}

void test2(){
    position_start(100.0,19.0);
    go_to_XY(100.0, 98.0);
    go_to_XY(74.0, 98.0);
    sleep(2);
    open_ball();
    position_stop();    
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
    position_stop();
}

// START WITH BALL ENGINE OPEN!!!!!!!!
void test5(){
    eye_start();
    go_straight(2);
    eye_stop();
}


int main ( void ) {

    signal(SIGINT, intHandler);

    if ( ev3_init() == -1 ) return ( 1 );
    engine_init();
    engine_list();
    sn_init();
    test4();
    sleep(1);
    engine_reset();
    ev3_uninit();
}
