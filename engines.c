#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"

#define RIGHT 65
#define LEFT 68

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

/* Go straight for a specified amount of seconds */
void go_straight ( int seconds )
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
        set_tacho_command_inx( sn, TACHO_RUN_TIMED );
    }
    else {
        printf( "LEGO_EV3_M_MOTOR on port %c is NOT found\n", port );
    }
}

/* Go straight forever */
void go_straight ( void )
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
        set_tacho_command_inx( sn, TACHO_RUN_FOREVER );
    }
    else {
        printf( "LEGO_EV3_M_MOTOR on port %c is NOT found\n", port );
    }
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
}
