/*
 * =====================================================================================
 *
 *       Filename:  sensors.c
 *
 *    Description:  EV3 sensors management library
 *
 *        Version:  1.0
 *        Created:  14.11.2016 14:01:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kamil Maslowski (), kmaslows@hotmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ev3.h"
#include "ev3_port.h"
#include "ev3_sensor.h"

const char const *color[] = { "?", "BLACK", "BLUE", "GREEN", "YELLOW", "RED", "WHITE", "BROWN" };
#define COLOR_COUNT  (( int )( sizeof( color ) / sizeof( color[ 0 ])))

uint8_t sn_touch = 0;
uint8_t sn_color = 0;
uint8_t sn_compass = 0;
uint8_t sn_sonar = 0;
uint8_t sn_mag = 0;
uint8_t sn_gyro = 0;

int sn_init(){

    printf("Initializing sensors...");
    if (!ev3_sensor_init()) {
        perror("ev3_sensor_init() failed!. Aborting....");
        abort();
    }

    if ( !ev3_search_sensor( LEGO_EV3_COLOR, &sn_color, 0 )) {
        printf("WARNING: COLOR sensor not found!");
    }

    if ( !ev3_search_sensor( HT_NXT_COMPASS, &sn_compass, 0 )) {
        printf("WARNING: COMPASS not found!");
    }

    if ( !ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyro, 0 )) {
        printf("WARNING: GYRO not found!");
    }
    
    if ( !ev3_search_sensor( LEGO_EV3_US, &sn_sonar, 0 )) {
        printf("WARNING: SONAR not found!");
    }
    if ( !ev3_search_sensor( NXT_ANALOG, &sn_mag, 0 )) {
        printf("WARNING: MAG sensor not found!");
    }

    printf("Sensors initialization finish");
    return 0;
}

void sn_lookup(){

    char s[256];
    int n;

    printf( "Found sensors:\n" );
    for ( int i = 0; i < DESC_LIMIT; i++ ) {
        if ( ev3_sensor[ i ].type_inx != SENSOR_TYPE__NONE_ ) {
            printf( "  type = %s\n", ev3_sensor_type( ev3_sensor[ i ].type_inx ));
            printf( "  port = %s\n", ev3_sensor_port_name( i, s ));
            if ( get_sensor_mode( i, s, sizeof( s ))) {
                printf( "  mode = %s\n", s );
            }
            if ( get_sensor_num_values( i, &n )) {
                for ( int ii = 0; ii < n; ii++ ) {
                    if ( get_sensor_value( ii, i, &val )) {
                        printf( "  value%d = %d\n", ii, val );
                    }
                }
            }
        }
    }
}

/*  COLOR SENSOR */

int sn_get_color_val(){
    int val;
    if ( sn_color != 0) {
        get_sensor_value( 0, sn_color, &val );
        return val;
    } else {
        perror("Attempt to read uninitialized Color sensor! Aborting...\n" );
        abort();
    }
}


int sn_color_set_mode(char* mode){
    if ( sn_color != 0 ) {
        set_sensor_mode(sn_color, mode);
        return 0;
    } else {
        perror("Attempt to set color mode of uninitialized COLOR sensor! Aborting...\n" );
        abort();
    }
}

/*  COMPASS */

int sn_get_compass_val(){
    float value; // TODO: CHECK IF THIS REALLY IS A FLOAT
    if ( sn_compass != 0){
        get_sensor_value0(sn_compass, &value );
        return value;
    } else {
        perror("Attempt to read uninitialized COMPASS! Aborting...\n" );
        abort();
    }
} 

/*  GYRO */

int sn_get_gyro_val(){
    int value;
    if (sn_gyro != 0){
        get_sensor_value0(sn_gyro, &value );
        return -value; // INVERTED - GYRO IS MOUNTED UPSIDE DOWN!
    } else {
        printf("Attempt to read uninitialized GYRO! Aborting...\n");
        abort();
    }
}

/* SONAR */

int sn_get_sonar_val(){
    int value;
    if (sn_sonar != 0){
        get_sensor_value0(sn_sonar, &value );
        return value;
    } else {
        printf("Attempt to read uninitialized SONAR! Aborting...\n");
        abort();
    }
}
/*  MAG */
int sn_get_mag_val(){
    int value;
    if (sn_mag != 0){
        get_sensor_value0(sn_mag, &value ) {
        return value;
    } else {
        printf("Attempt to read uninitialized MAG! Aborting...");
        abort();
    }
}
