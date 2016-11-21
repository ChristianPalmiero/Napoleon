#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "sensors.c"

#define RIGHT 65
#define LEFT 68
#define BALL 67

// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>
// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )
//////////////////////////////////////////////////
#endif


/* Detect connected tachos
 Return the number of found tachos
 */
int engine_init( void )
{
#ifndef __ARM_ARCH_4T__
    printf( "The EV3 brick auto-detection is DISABLED,\nwaiting %s online with plugged tacho...\n", ev3_brick_addr );
#else
    printf( "Waiting tacho is plugged...\n" );
#endif
    printf( "Tacho motors initialization..\n" );

    return ev3_tacho_init();
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
    uint8_t sn;
    int port;
    
    port = RIGHT;
    if (ev3_search_tacho_plugged_in (port, 0, &sn, 0))
        set_tacho_command_inx( sn, TACHO_RESET );

    port = RIGHT;
    if (ev3_search_tacho_plugged_in (port, 0, &sn, 0))
        set_tacho_command_inx( sn, TACHO_RESET );
   
    port = BALL;
    if (ev3_search_tacho_plugged_in (port, 0, &sn, 0))
        set_tacho_command_inx( sn, TACHO_RESET );
    
    return;
}

/* Go straight for a specified amount of seconds */
void go_straight_limited ( int seconds )
{
    int port;
    uint8_t sn;
    
    port = RIGHT;
    if ( ev3_search_tacho_plugged_in( port, 0, &sn, 0 )) {
        int max_speed;
            
        printf( "LEGO_EV3_M_MOTOR on port %c is found, run for %d sec...\n",  port, seconds);
        get_tacho_max_speed( sn, &max_speed );
        printf( " max speed = %d\n", max_speed );
        set_tacho_stop_action_inx( sn, TACHO_COAST );
        set_tacho_speed_sp( sn, - max_speed / 3.3 );
        set_tacho_time_sp( sn, seconds * 1000 );
        set_tacho_ramp_up_sp( sn, 1000 );
        set_tacho_ramp_down_sp( sn, 1000 );
        set_tacho_position_sp( sn, 0 );
        set_tacho_command_inx( sn, TACHO_RUN_TIMED );
    }
    else {
        printf( "LEGO_EV3_M_MOTOR on port %c is NOT found\n", port );
    }
    
    port = LEFT;
    if ( ev3_search_tacho_plugged_in( port, 0, &sn, 0 )) {
        int max_speed;
        
        printf( "LEGO_EV3_M_MOTOR on port %c is found, run for %d sec...\n",  port, seconds);
        get_tacho_max_speed( sn, &max_speed );
        printf( " max speed = %d\n", max_speed );
        set_tacho_stop_action_inx( sn, TACHO_COAST );
        set_tacho_speed_sp( sn, - max_speed);
        set_tacho_time_sp( sn, seconds * 1000 );
        set_tacho_ramp_up_sp( sn, 1000 );
        set_tacho_ramp_down_sp( sn, 1000 );
        set_tacho_position_sp (sn, 0 );
        set_tacho_command_inx( sn, TACHO_RUN_TIMED );
    }
    else {
        printf( "LEGO_EV3_M_MOTOR on port %c is NOT found\n", port );
    }

    return;
}

/* Go straight forever */
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

/* Stop the engines */
void engine_stop ( void )
{
    int port;
    uint8_t sn;
    
    printf( "Stop the engines...\n" );
    port = RIGHT;
    if ( ev3_search_tacho_plugged_in( port, 0, &sn, 0 )) {
        set_tacho_stop_action_inx( sn, TACHO_COAST );
        set_tacho_ramp_down_sp( sn, 1000 );
        set_tacho_command_inx( sn, TACHO_STOP );
    }

    port = LEFT;
    if ( ev3_search_tacho_plugged_in( port, 0, &sn, 0 )) {
        set_tacho_stop_action_inx( sn, TACHO_COAST );
        set_tacho_ramp_down_sp( sn, 1000 );
        set_tacho_command_inx( sn, TACHO_STOP );
    }

    return;
}

/*Turn right of x degrees*/
void turn_right ( int x )
{
    int port, initial, amount;
    uint8_t sn, sn2;

    engine_stop();
    sn_init();

    initial = sn_get_compass_val();
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
        	set_tacho_relative_pos(-10);

		// Right track
		get_tacho_max_speed( sn2, &max_speed_2 );
                printf( " max speed = %d\n", max_speed_2 );
                set_tacho_speed_sp( sn2, - max_speed_2 / 2);
                set_tacho_ramp_up_sp( sn2, 0 );
                set_tacho_ramp_down_sp( sn2, 0 );
                set_tacho_relative_pos(10);

		amount = initial;
		while (  amount < (initial+x) ) {
            		set_tacho_command_inx( sn, TACHO_RUN_TO_REL_POS);
            		set_tacho_command_inx( sn2, TACHO_RUN_TO_REL_POS);
  			amount = sn_get_compass_val();
		}
		engine_stop();
	}
    }
    else {
        printf( "LEGO_EV3_M_MOTOR on port %c is NOT found\n", port );
    }
    
    return;
}

/* Turn left of x degrees*/
void turn_left ( int x )
{
    int port;
    uint8_t sn, sn2;

    engine_stop();

    port = RIGHT;
    if ( ev3_search_tacho_plugged_in( port, 0, &sn, 0 )) {
        printf( "LEGO_EV3_M_MOTOR on port %c is found, right...\n",  port);
        port = LEFT;
        if (ev3_search_tacho_plugged_in ( port, 0, &sn2, 0 )) {
                int max_speed;
                int max_speed_2;

                printf( "LEGO_EV3_M_MOTOR on port %c is found, right...\n",  port);

                // Right track
                get_tacho_max_speed( sn, &max_speed );
                printf( " max speed = %d\n", max_speed );
                set_tacho_speed_sp( sn, - max_speed / 2);
                set_tacho_ramp_up_sp( sn, 0 );
                set_tacho_ramp_down_sp( sn, 0 );
                // -x*10 = turn right of x°
                set_tacho_position_sp( sn, -x*10);

                // Left track
                get_tacho_max_speed( sn2, &max_speed_2 );
                printf( " max speed = %d\n", max_speed_2 );
                set_tacho_speed_sp( sn2, - max_speed_2 / 2);
                set_tacho_ramp_up_sp( sn2, 0 );
                set_tacho_ramp_down_sp( sn2, 0 );
                // x*10 = turn left of x°
                set_tacho_position_sp( sn2, x*10);

                for (int i=0; i<8; i++) {
                        set_tacho_command_inx( sn, TACHO_RUN_TO_REL_POS );
                        set_tacho_command_inx( sn2, TACHO_RUN_TO_REL_POS );
                }
        }
    }
    else {
        printf( "LEGO_EV3_M_MOTOR on port %c is NOT found\n", port );
    }

    sleep(3);
    engine_reset();

    return;
}

/*Open the engine for grabbing the ball*/
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

/*Close the engine for grabbing the ball*/
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

int main ( void ) {

    if ( ev3_init() == -1 ) return ( 1 );
    
    engine_init();
    engine_list();
    turn_right(45);
    //go_straight();
    sleep(2);
//    turn_left(45);
    //open_ball();
    sleep(1);
    //close_ball();
    engine_stop();
    
    return 0;
}
