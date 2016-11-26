#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "sensors.c"
#include <unistd.h>

#define RIGHT 67
#define LEFT 68
#define BALL 65
int MAX_SPEED = 500; // TODO: should be defined as const, but that makes too much compiler warnings -.-'

#define Sleep( msec ) usleep(( msec ) * 1000 )


uint8_t sn_engineR;
uint8_t sn_engineL;
uint8_t sn_engineLR[2];
uint8_t sn_engineM;
uint8_t sn_engineLRM[3];
/* Detect connected tachos
   Return the number of found tachos
   */
int engine_init( void ) {

    if (!ev3_tacho_init()){
        perror("ev3_tacho_init() failed! Aborting...");
        abort();
    }
    if (!ev3_search_tacho_plugged_in( LEFT, 0, &sn_engineL, 0 )) {
        perror("Left engine not found! Aborting...");
        abort();
    }
    if (!ev3_search_tacho_plugged_in( RIGHT, 0, &sn_engineR, 0)){
        perror("Right engine not found! Aborting...");
        abort();
    }
    if (!ev3_search_tacho_plugged_in( BALL, 0, &sn_engineM, 0)){
        perror("Middle engine not found! Aborting...");
        abort();
    } 
    sn_engineLR[0] = sn_engineL;
    sn_engineLR[1] = sn_engineR;
    sn_engineLRM[0] = sn_engineL;
    sn_engineLRM[1] = sn_engineR;
    sn_engineLRM[2] = sn_engineM;
    return 0;
}

/* Print connected tachos */
void engine_list ( void )
{
    int i;
    char s[256];

    printf( "Found tacho motors:\n" );
    for ( i = 0; i < DESC_LIMIT; i++ ) {
        if ( ev3_tacho[ i ].type_inx != TACHO_TYPE__NONE_ ) {
            printf( " type = %s\n", ev3_tacho_type( ev3_tacho[ i ].type_inx ));
            printf( " port = %s\n", ev3_tacho_port_name( i, s ));
            printf( " port = %d %d\n", ev3_tacho_desc_port(i), ev3_tacho_desc_extport(i));
        }
    }

    return;
}

/* Reset tachos */
void engine_reset ( void )
{
    for (int i = 0; i < 3; i++){
        set_tacho_command_inx( sn_engineLRM[i], TACHO_RESET );
    }
    return;
}

/* Go straight for a specified amount of seconds */
void go_straight_limited ( int seconds )
{
    for (int i = 0; i<2; i++){
        get_tacho_max_speed( sn_engineLR[i], &MAX_SPEED ); 
        set_tacho_stop_action_inx( sn_engineLR[i], TACHO_BRAKE );
        set_tacho_polarity_inx( sn_engineLR[i], TACHO_INVERSED);
        set_tacho_speed_sp( sn_engineLR[i], MAX_SPEED );
        set_tacho_time_sp( sn_engineLR[i], seconds * 1000 );
        set_tacho_ramp_up_sp( sn_engineLR[i], 1000 ); // TODO: CHECK IF THIS VALUE IS NOT TOO HIGH
        set_tacho_ramp_down_sp( sn_engineLR[i], 1000 ); // TODO: SAME ^
        //set_tacho_position_sp( sn, 0 );
    }
    set_tacho_command_inx( sn_engineL, TACHO_RUN_TIMED );
    set_tacho_command_inx( sn_engineR, TACHO_RUN_TIMED );
    sleep(seconds);
    return;
}

/* Go straight forever */
/*  
    void go_straight ( void )
    {
    int port;
    uint8_t sn;

    port = RIGHT;
    if ( ev3_search_tacho_plugged_in( port, 0, &sn, 0 )) {
    int max_speed;

    printf( "LEGO_EV3_M_MOTOR on port %c is found, run forever...\n",  port);
    get_tacho_max_speed( sn, &max_speed );
    printf( " max speed = %d\n", max_speed );
    set_tacho_stop_action_inx( sn, TACHO_COAST );
    set_tacho_speed_sp( sn, - max_speed / 3.3 );
    set_tacho_ramp_up_sp( sn, 1000 );
    set_tacho_ramp_down_sp( sn, 1000 );
    set_tacho_position_sp( sn, 0);
    set_tacho_command_inx( sn, TACHO_RUN_FOREVER );
    }
    else {
    printf( "LEGO_EV3_M_MOTOR on port %c is NOT found\n", port );
    }

    port = LEFT;
    if ( ev3_search_tacho_plugged_in( port, 0, &sn, 0 )) {
    int max_speed;

    printf( "LEGO_EV3_M_MOTOR on port %c is found, run forever...\n",  port);
    get_tacho_max_speed( sn, &max_speed );
    printf( " max speed = %d\n", max_speed );
    set_tacho_stop_action_inx( sn, TACHO_COAST );
    set_tacho_speed_sp( sn, - max_speed);
    set_tacho_ramp_up_sp( sn, 1000 );
    set_tacho_ramp_down_sp( sn, 1000 );
    set_tacho_position_sp ( sn, 0 );
    set_tacho_command_inx( sn, TACHO_RUN_FOREVER );
    }
    else {
    printf( "LEGO_EV3_M_MOTOR on port %c is NOT found\n", port );
    }

    return;
    }
    */
/* Stop the engines */
void engine_stop ( void )
{
    printf( "Stop the engines...\n" );
    for (int i = 0; i < 2; i++){
        set_tacho_stop_action_inx( sn_engineLR[i], TACHO_BRAKE );
        set_tacho_ramp_down_sp( sn_engineLR[i], 0 );
        set_tacho_command_inx( sn_engineLR[i], TACHO_STOP );
    }
    return;
}

/*Turn by x degrees. Negative value will turn to the left*/
/*
   void turn( int x )
   {
   int port, initial, amount;
   uint8_t snL, snR;

   engine_stop();

   port = LEFT;
   if ( ev3_search_tacho_plugged_in( port, 0, &sn, 0 )) {
   printf( "LEGO_EV3_M_MOTOR on port %c is found, right...\n",  port);
   port = RIGHT;
   if (ev3_search_tacho_plugged_in ( port, 0, &sn2, 0 )) {
   int max_speed;
   int max_speed_2;

   printf( "LEGO_EV3_M_MOTOR on port %c is found, right...\n",  port);

// Left track
get_tacho_max_speed( sn, &max_speed );
printf( " max speed = %d\n", max_speed );
set_tacho_speed_sp( sn, - max_speed / 2);
set_tacho_ramp_up_sp( sn, 0 );
set_tacho_ramp_down_sp( sn, 0 );
set_tacho_position_sp(sn, -10);

// Right track
get_tacho_max_speed( sn2, &max_speed_2 );
printf( " max speed = %d\n", max_speed_2 );
set_tacho_speed_sp( sn2, - max_speed_2 / 2);
set_tacho_ramp_up_sp( sn2, 0 );
set_tacho_ramp_down_sp( sn2, 0 );
set_tacho_position_sp(sn2, 10);

amount = initial;
int err=10;
while (  amount < (initial+x-err) ) {
set_tacho_command_inx( sn, TACHO_RUN_TO_REL_POS);
set_tacho_command_inx( sn2, TACHO_RUN_TO_REL_POS);
amount = sn_get_compass_val();
printf("%d\t", amount);
}
engine_stop();
}
}
else {
printf( "LEGO_EV3_M_MOTOR on port %c is NOT found\n", port );
}

return;
}
*/
/*Open the engine for grabbing the ball*/
/* 
   void open_ball ( void )
   {
   int port;
   uint8_t sn;

   engine_stop();

   port = BALL;
   if ( ev3_search_tacho_plugged_in( port, 0, &sn, 0 )) {
   printf( "LEGO_EV3_M_MOTOR on port %c is found, right...\n",  port);
   int max_speed;

// Front engine
get_tacho_max_speed( sn, &max_speed );
printf( " max speed = %d\n", max_speed );
set_tacho_speed_sp( sn, - max_speed / 2);
set_tacho_ramp_up_sp( sn, 0 );
set_tacho_ramp_down_sp( sn, 0 );
set_tacho_position_sp( sn, -40);

for (int i=0; i<8; i++) {
set_tacho_command_inx( sn, TACHO_RUN_TO_REL_POS );
}
}
else {
printf( "LEGO_EV3_M_MOTOR on port %c is NOT found\n", port );
}

return;
}
*/

/*Close the engine for grabbing the ball*/
/* 
   void close_ball ( void )
   {
   int port;
   uint8_t sn;

   engine_stop();

   port = BALL;
   if ( ev3_search_tacho_plugged_in( port, 0, &sn, 0 )) {
   printf( "LEGO_EV3_M_MOTOR on port %c is found, right...\n",  port);
   int max_speed;

// Front engine
get_tacho_max_speed( sn, &max_speed );
printf( " max speed = %d\n", max_speed );
set_tacho_speed_sp( sn, - max_speed / 3);
set_tacho_ramp_up_sp( sn, 0 );
set_tacho_ramp_down_sp( sn, 0 );
set_tacho_position_sp( sn, +37);

for (int i=0; i<8; i++) {
set_tacho_command_inx( sn, TACHO_RUN_TO_ABS_POS );
}
}
else {
printf( "LEGO_EV3_M_MOTOR on port %c is NOT found\n", port );
}

return;
}
*/
int main ( void ) {

    if ( ev3_init() == -1 ) return ( 1 );
    engine_init();
    engine_list();
    sn_init();
    go_straight_limited(3);
    engine_stop();

    return 0;
}

