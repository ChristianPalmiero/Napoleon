#include "engines.h"
#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "sensors.h"
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "position.h"
#define RIGHT 68
#define LEFT 67
#define BALL 65
const int MAX_SPEED = 500; 

#define Sleep( msec ) usleep(( msec ) * 1000 )

int stop_turn;
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
    multi_set_tacho_ramp_up_sp( sn_engineLR, 1000 ); // TODO: Check this value
    multi_set_tacho_ramp_down_sp( sn_engineLR, 0 ); // TODO: Check this value
    set_tacho_ramp_up_sp(sn_engineM, 1000);
    set_tacho_ramp_down_sp(sn_engineM, 1000);
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
void go_straight ( int mseconds )
{
    int sleep_time = 100; // [ms]
    multi_set_tacho_stop_action_inx( sn_engineLR, TACHO_BRAKE );
    multi_set_tacho_polarity_inx( sn_engineLR, TACHO_NORMAL);
    multi_set_tacho_speed_sp( sn_engineLR, MAX_SPEED );
    multi_set_tacho_time_sp( sn_engineLR, mseconds);

    if ( mseconds > 0 ) {
        int initial_angle = sn_get_gyro_val();
        int current_angle = initial_angle;
        int error;
        multi_set_tacho_command_inx( sn_engineLR, TACHO_RUN_TIMED );

        // TODO: REPLACE THIS WITH A PROPER PID CONTROLER!
        while (mseconds > 0){
            current_angle = sn_get_gyro_val();
            error = current_angle-initial_angle;
            if (error > 1 || error < -1) {
                set_tacho_speed_sp(sn_engineR, MAX_SPEED+(error*2));
                set_tacho_speed_sp(sn_engineL, MAX_SPEED-(error*2));
                multi_set_tacho_command_inx( sn_engineLR, TACHO_RUN_TIMED ); // TODO: FIX TIMED!
            }
            mseconds-=sleep_time;
            Sleep(sleep_time);        
        }
        multi_set_tacho_command_inx( sn_engineLR, TACHO_STOP );
    } else {
        multi_set_tacho_command_inx( sn_engineLR, TACHO_RUN_FOREVER );
    }
    return;
}

void go_back ( int seconds )
{
    int sleep_time = 100; // [ms]
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
            if (error > 1 || error < -1) {
                set_tacho_speed_sp(sn_engineR, MAX_SPEED+(error*2));
                set_tacho_speed_sp(sn_engineL, MAX_SPEED-(error*2));
                multi_set_tacho_command_inx( sn_engineLR, TACHO_RUN_TIMED ); // TODO: FIX TIMED!
            }
            mseconds-=sleep_time;
            Sleep(sleep_time);        
        }
        multi_set_tacho_command_inx( sn_engineLR, TACHO_STOP );
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
    printf("Turning by: %d\n", x);
    uint8_t sn_active;
    uint8_t sn_passive;

    if ( x > 0 && direction == TURN_FORWARD) {
        sn_active  = sn_engineR;
        sn_passive = sn_engineL;
        set_tacho_polarity_inx( sn_active, TACHO_NORMAL);
    } else if ( x < 0 && direction == TURN_FORWARD) {
        sn_active  = sn_engineL;
        sn_passive = sn_engineR;
        set_tacho_polarity_inx( sn_active, TACHO_NORMAL);
    } else  if ( x > 0 && direction == TURN_REVERSE ) {
        sn_active  = sn_engineL;
        sn_passive = sn_engineR;
        set_tacho_polarity_inx( sn_active, TACHO_INVERSED);
    } else  if ( x < 0 && direction == TURN_REVERSE ) {
        sn_active  = sn_engineR;
        sn_passive = sn_engineL;
        set_tacho_polarity_inx( sn_active, TACHO_INVERSED);
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

    int current_angle = -sn_get_gyro_val();
    int target_angle = current_angle + x;

    // Start the active engine
    set_tacho_command_inx( sn_active, TACHO_RUN_FOREVER );

    // Gyro control loop
    int deg_left = target_angle - current_angle;
    int deg_left_abs = abs(deg_left);
    int stage = 3;
    while ( (deg_left > 0 && x > 0 ) || ((deg_left < 0) && x < 0)) { // TODO: Check gyro value +/-
        current_angle = -sn_get_gyro_val();
        deg_left = target_angle - current_angle;
        deg_left_abs = abs(deg_left);

        printf("TURN: T: %d C: %d Deg_Left: %d\n",target_angle,current_angle,deg_left);

        if ( stage == 1 && 0 < deg_left_abs && deg_left_abs <= 6 ) {
            set_tacho_speed_sp( sn_active, 20);
            set_tacho_command_inx( sn_active, TACHO_RUN_FOREVER );
            stage--;
        } else if ( stage == 2 && deg_left_abs <= 15 ) {
            set_tacho_speed_sp( sn_active, 75);
            set_tacho_command_inx( sn_active, TACHO_RUN_FOREVER );
            stage--;
        } else if ( stage == 3 && deg_left_abs <= 30 ) {
            set_tacho_speed_sp( sn_active, 200);
            set_tacho_command_inx( sn_active, TACHO_RUN_FOREVER );
            stage--;
        }
        Sleep(50);
    }
    // HALT!
    set_tacho_command_inx(sn_active, TACHO_STOP);
    set_tacho_command_inx(sn_passive, TACHO_STOP);
    Sleep(500); 

    // TODO: Fix error
    //current_angle = sn_get_gyro_val();
    //int error = target_angle - current_angle;
    //if (error > 1 || error < -1){
    //    turn(error);
    //}

    return;
}

/*Turn by x degrees. Negative value will turn to the left*/
void turn2( int x)
{
    engine_stop();
    printf("Turning2 by: %d\n", x);
    stop_turn = 0;

    if ( x > 0 ) {
        set_tacho_polarity_inx( sn_engineL, TACHO_INVERSED);
        set_tacho_polarity_inx( sn_engineR, TACHO_NORMAL);
    } else  if ( x < 0 ) {
        set_tacho_polarity_inx( sn_engineL, TACHO_NORMAL);
        set_tacho_polarity_inx( sn_engineR, TACHO_INVERSED);
    } else {
        return;
    }

    multi_set_tacho_stop_action_inx( sn_engineLR, TACHO_HOLD );
    multi_set_tacho_speed_sp( sn_engineLR, 250);
    multi_set_tacho_command_inx( sn_engineLR, TACHO_STOP );

    // Active

    int current_angle = -sn_get_gyro_val();
    int target_angle = current_angle + x;

    // Start the active engine
    multi_set_tacho_command_inx( sn_engineLR, TACHO_RUN_FOREVER );

    // Gyro control loop
    int deg_left = target_angle - current_angle;
    int deg_left_abs = abs(deg_left);
    int stage = 3;
    while ( stop_turn==0 && ((deg_left > 0 && x > 0 ) || ((deg_left < 0) && x < 0))) { // TODO: Check gyro value +/-
        current_angle = -sn_get_gyro_val();
        deg_left = target_angle - current_angle;
        deg_left_abs = abs(deg_left);

        printf("TURN: T: %d C: %d Deg_Left: %d\n",target_angle,current_angle,deg_left);

        if ( stage == 1 && 0 < deg_left_abs && deg_left_abs <= 6 ) {
            multi_set_tacho_speed_sp( sn_engineLR, 20);
            multi_set_tacho_command_inx( sn_engineLR, TACHO_RUN_FOREVER );
            stage--;
        } else if ( stage == 2 && deg_left_abs <= 15 ) {
            multi_set_tacho_speed_sp( sn_engineLR, 75);
            multi_set_tacho_command_inx( sn_engineLR, TACHO_RUN_FOREVER );
            stage--;
        } else if ( stage == 3 && deg_left_abs <= 30 ) {
            multi_set_tacho_speed_sp( sn_engineLR, 200);
            multi_set_tacho_command_inx( sn_engineLR, TACHO_RUN_FOREVER );
            stage--;
        }
        Sleep(50);
    }
    // HALT!
    multi_set_tacho_polarity_inx( sn_engineLR, TACHO_NORMAL);
    multi_set_tacho_command_inx( sn_engineLR, TACHO_STOP);
    Sleep(500); 

    return;
}

/*Open the engine for grabbing the ball*/
void open_ball ( void )
{
    set_tacho_speed_sp(sn_engineM, 1000);
    set_tacho_command_inx(sn_engineM, TACHO_STOP);
    set_tacho_position_sp( sn_engineM, -40);
    set_tacho_command_inx( sn_engineM, TACHO_RUN_TO_REL_POS );

    return;
}

/*Close the engine for grabbing the ball*/
void close_ball ( void )
{
    engine_stop();
    set_tacho_ramp_up_sp(sn_engineM, 0);
    set_tacho_speed_sp(sn_engineM, 1000);
    set_tacho_command_inx(sn_engineM, TACHO_STOP);
    set_tacho_position_sp(sn_engineM, +40);
    set_tacho_command_inx(sn_engineM, TACHO_RUN_TO_REL_POS );
    printf("Stopping and closing ball engine!\n");

    return;
}

void get_encoders_values(int * disp_left, int * disp_right){
    get_tacho_position(sn_engineL, disp_left);
    get_tacho_position(sn_engineR, disp_right);
}

void adjust_speed_by(int err){
    set_tacho_speed_sp(sn_engineR, MAX_SPEED+(err*1));
    set_tacho_speed_sp(sn_engineL, MAX_SPEED-(err*1));
    multi_set_tacho_command_inx( sn_engineLR, TACHO_RUN_FOREVER );
}

/*
void go_to_XY(float xb, float yb){
    // Get current positions
    float xa, ya, distance;
    int heading, rotation;
    printf("GO_TO_XY: Going toward X: %f Y: %f\n", xb,yb);
    // PHASE 1 - Orient toward the destination
    get_position_and_heading(&xa,&ya, &heading);
    get_dist_and_ang(xa,ya,xb,yb,heading,&distance,&rotation);
    printf("GO_TO_XY: Distance: %.2f Rotation: %d\n", distance, rotation);
    turn2(rotation);

    // PHASE 2 - Go to destination
    multi_set_tacho_speed_sp( sn_engineLR, MAX_SPEED);
    multi_set_tacho_command_inx(sn_engineLR, TACHO_RUN_FOREVER);
    Sleep(100);
    /// PHASE 3 - Error correction
    while ( distance >= 10.0 ){
        get_position_and_heading(&xa,&ya, &heading);
        get_dist_and_ang(xa,ya,xb,yb,heading,&distance,&rotation);
        printf("GO_TO_XY: Distance: %.2f Rotation: %d\n", distance, rotation);

        if (rotation > 20 || rotation < -20){
            turn2(rotation);
        } else if ( rotation > 1 || rotation < -1 ){
            set_tacho_speed_sp(sn_engineR, MAX_SPEED+(rotation*1));
            set_tacho_speed_sp(sn_engineL, MAX_SPEED-(rotation*1));
            multi_set_tacho_command_inx( sn_engineLR, TACHO_RUN_FOREVER );
        }
       
	Sleep(100);
    }
    engine_stop();
}*/
