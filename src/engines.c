#include "engines.h"
#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "sensors.h"
#include <unistd.h>

#define RIGHT 67
#define LEFT 68
#define BALL 65
const int MAX_SPEED = 500; 

#define Sleep( msec ) usleep(( msec ) * 1000 )


uint8_t sn_engineR;
uint8_t sn_engineL;
uint8_t sn_engineLR[3];
uint8_t sn_engineM;
uint8_t sn_engineLRM[4];
int position_engineM;
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
    sn_engineLR[2] = DESC_LIMIT;

    sn_engineLRM[0] = sn_engineL;
    sn_engineLRM[1] = sn_engineR;
    sn_engineLRM[2] = sn_engineM;
    sn_engineLRM[3] = DESC_LIMIT;

    // Set default value of ramp up/down speed
    multi_set_tacho_ramp_up_sp( sn_engineLRM, 1000 ); // TODO: Check this value
    multi_set_tacho_ramp_down_sp( sn_engineLRM, 0 ); // TODO: Check this value
    multi_set_tacho_stop_action_inx( sn_engineLRM, TACHO_HOLD );

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
    multi_set_tacho_command_inx( sn_engineLRM, TACHO_RESET );
    return;
}

/* Go straight for a specified amount of seconds.
 * Passing 0 as number of seconds means go forever */
void go_straight ( int seconds )
{
    multi_set_tacho_stop_action_inx( sn_engineLR, TACHO_BRAKE );
    multi_set_tacho_polarity_inx( sn_engineLR, TACHO_INVERSED);
    multi_set_tacho_speed_sp( sn_engineLR, MAX_SPEED );
    multi_set_tacho_time_sp( sn_engineLR, seconds * 1000 );

    if ( seconds > 0 ) {
        int initial_angle = sn_get_gyro_val();
        int current_angle = initial_angle;
        int mseconds = seconds * 1000;
        int error;
        multi_set_tacho_command_inx( sn_engineLR, TACHO_RUN_TIMED );

        // TODO: REPLACE THIS WITH A PROPER PID CONTROLER!
        while (mseconds > 0){
            current_angle = sn_get_gyro_val();
            error = current_angle-initial_angle;
            if (error > 1 || error < 1) {
                set_tacho_speed_sp(sn_engineR, MAX_SPEED-(error*10));
                set_tacho_speed_sp(sn_engineL, MAX_SPEED+(error*10));
                multi_set_tacho_command_inx( sn_engineLR, TACHO_RUN_TIMED ); // TODO: FIX TIMED!
            }        


        }



        sleep(seconds);
    } else {
        multi_set_tacho_command_inx( sn_engineLR, TACHO_RUN_FOREVER );
    }
    return;
}

/* Stop the engines */
void engine_stop ( void )
{
    multi_set_tacho_command_inx( sn_engineLR, TACHO_STOP );
    return;
}

/*Turn by x degrees. Negative value will turn to the left*/
void turn( int x , int direction)
{
    engine_stop();

    uint8_t sn_active;
    uint8_t sn_passive;

    if ( x > 0 && direction == TURN_FORWARD) {
        sn_active  = sn_engineL;
        sn_passive = sn_engineR;
        set_tacho_polarity_inx( sn_active, TACHO_INVERSED);
    } else if ( x < 0 && direction == TURN_FORWARD) {
        sn_active  = sn_engineR;
        sn_passive = sn_engineL;
        set_tacho_polarity_inx( sn_active, TACHO_INVERSED);
    } else  if ( x > 0 && direction == TURN_REVERSE ) {
        sn_active  = sn_engineR;
        sn_passive = sn_engineL;
        set_tacho_polarity_inx( sn_active, TACHO_NORMAL);
    } else  if ( x < 0 && direction == TURN_REVERSE ) {
        sn_active  = sn_engineL;
        sn_passive = sn_engineR;
        set_tacho_polarity_inx( sn_active, TACHO_NORMAL);
    } else {
        return;
    }

    // Passive
    set_tacho_stop_action_inx( sn_passive, TACHO_HOLD );
    set_tacho_speed_sp( sn_passive, MAX_SPEED);
    set_tacho_command_inx( sn_passive, TACHO_STOP );

    // Active
    set_tacho_speed_sp( sn_active, MAX_SPEED);
    set_tacho_stop_action_inx( sn_active, TACHO_HOLD );

    int current_angle = sn_get_gyro_val();
    int target_angle = current_angle + x;

    // Start the active engine
    set_tacho_command_inx( sn_active, TACHO_RUN_FOREVER );

    // Gyro control loop
    int deg_left;
    int stage = 3;
    if ( target_angle > current_angle ){
        while ( current_angle < target_angle ) {
            current_angle = sn_get_gyro_val();

            deg_left = target_angle - current_angle;
            if ( stage == 1 && 0 < deg_left && deg_left <= 6 ) {
                set_tacho_speed_sp( sn_active, 10);
                set_tacho_command_inx( sn_active, TACHO_RUN_FOREVER );
                stage--;
            } else if ( stage == 2 && deg_left <= 15 ) {
                set_tacho_speed_sp( sn_active, 75);
                set_tacho_command_inx( sn_active, TACHO_RUN_FOREVER );
                stage--;
            } else if ( stage == 3 && deg_left <= 30 ) {
                set_tacho_speed_sp( sn_active, 200);
                set_tacho_command_inx( sn_active, TACHO_RUN_FOREVER );
                stage--;
            }
            Sleep(25);
        }
    } else if ( target_angle < current_angle ) {
        while ( current_angle > target_angle ) {
            current_angle = sn_get_gyro_val();

            deg_left = current_angle - target_angle;
            if ( stage == 1 && 0 < deg_left && deg_left <= 6 ) {
                set_tacho_speed_sp( sn_active, 10);
                set_tacho_command_inx( sn_active, TACHO_RUN_FOREVER );
                stage--;
            } else if ( stage == 2 && deg_left <= 15 ) {
                set_tacho_speed_sp( sn_active, 75);
                set_tacho_command_inx( sn_active, TACHO_RUN_FOREVER );
                stage--;
            } else if ( stage == 3 && deg_left <= 30 ) {
                set_tacho_speed_sp( sn_active, 200);
                set_tacho_command_inx( sn_active, TACHO_RUN_FOREVER );
                stage--;
            }
            Sleep(25);
        }
    }
    // HALT!
    set_tacho_command_inx(sn_active, TACHO_STOP);

    // Sleep(500); 

    // TODO: Fix error
    //current_angle = sn_get_gyro_val();
    //int error = target_angle - current_angle;
    //if (error > 1 || error < -1){
    //    turn(error);
    //}

    return;
}
/*Open the engine for grabbing the ball*/
void open_ball ( void )
{
    set_tacho_command_inx(sn_engineM, TACHO_STOP);
    set_tacho_position_sp( sn_engineM, -40);
    set_tacho_command_inx( sn_engineM, TACHO_RUN_TO_REL_POS );

    return;
}

/*Close the engine for grabbing the ball*/
void close_ball ( void )
{
    set_tacho_command_inx(sn_engineM, TACHO_STOP);
    set_tacho_position_sp(sn_engineM, +40);
    set_tacho_command_inx(sn_engineM, TACHO_RUN_TO_REL_POS );

    return;
}

