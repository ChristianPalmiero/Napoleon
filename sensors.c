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



#include "sensors.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_sensor.h"

uint8_t sn_touch;
uint8_t sn_color;
uint8_t sn_compass;
uint8_t sn_sonar;
uint8_t sn_mag;
uint8_t sn_gyro;


float value;
const char *color[] = { "?", "BLACK", "BLUE", "GREEN", "YELLOW", "RED", "WHITE", "BROWN" };
int sn_init(){

    printf("Initializing sensors...");
    if (!ev3_sensor_init()) {
        perror("ev3_sensor_init() failed!. Aborting....");
        abort();
    }

    if ( !ev3_search_sensor( LEGO_EV3_COLOR, &sn_color, 0 )) {
        sn_color = DESC_LIMIT;
        printf("WARNING: COLOR sensor not found!\n");
    }

    if ( !ev3_search_sensor( HT_NXT_COMPASS, &sn_compass, 0 )) {
        sn_compass = DESC_LIMIT;
        printf("WARNING: COMPASS not found!\n");
    }

    if ( !ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyro, 0 )) {
        sn_gyro = DESC_LIMIT;
        printf("WARNING: GYRO not found!\n");
    }

    if ( !ev3_search_sensor( LEGO_EV3_US, &sn_sonar, 0 )) {
        sn_sonar = DESC_LIMIT;
        printf("WARNING: SONAR not found!\n");
    }
    if ( !ev3_search_sensor( NXT_ANALOG, &sn_mag, 0 )) {
        sn_mag = DESC_LIMIT;
        printf("WARNING: MAG sensor not found!\n");
    }

    printf("Sensors initialization finish\n");
    return 0;
}

void sn_lookup(){

    char s[256];
    unsigned int n;
    int value;

    printf( "Found sensors:\n" );
    for ( int i = 0; i < DESC_LIMIT; i++ ) {
        if ( ev3_sensor[ i ].type_inx != SENSOR_TYPE__NONE_ ) {
            printf( "  type = %s\n", ev3_sensor_type( ev3_sensor[ i ].type_inx ));
            printf( "  port = %s\n", ev3_sensor_port_name( i, s ));
            if ( get_sensor_mode( i, s, sizeof( s ))) {
                printf( "  mode = %s\n", s );
            }
            if ( get_sensor_num_values( i, &n )) {
                for ( unsigned int ii = 0; ii < n; ii++ ) {
                    if ( get_sensor_value( ii, i, &value )) {
                        printf( "  value%d = %d\n", ii, value );
                    }
                }
            }
        }
    }
}

/*  COLOR SENSOR */

float sn_get_color_val(){
    int val;
    if ( sn_color != DESC_LIMIT) {
        get_sensor_value( 0, sn_color, &val );
        return val;
    } else {
        perror("Attempt to read uninitialized Color sensor! Aborting...\n" );
        abort();
    }
}


int sn_color_set_mode(char* mode){
    if ( sn_color != DESC_LIMIT ) {
        set_sensor_mode(sn_color, mode);
        return 0;
    } else {
        perror("Attempt to set color mode of uninitialized COLOR sensor! Aborting...\n" );
        abort();
    }
}

/*  COMPASS */

float sn_get_compass_val(){
    if ( sn_compass != DESC_LIMIT){
        get_sensor_value0(sn_compass, &value );
        return value;
    } else {
        perror("Attempt to read uninitialized COMPASS! Aborting...\n" );
        abort();
    }
} 

/*  GYRO */

float sn_get_gyro_val(){
    if (sn_gyro != DESC_LIMIT){
        get_sensor_value0(sn_gyro, &value );
        return -value; // INVERTED - GYRO IS MOUNTED UPSIDE DOWN!
    } else {
        printf("Attempt to read uninitialized GYRO! Aborting...\n");
        abort();
    }
}

/* SONAR */

float sn_get_sonar_val(){
    if (sn_sonar != DESC_LIMIT){
        get_sensor_value0(sn_sonar, &value );
        return value;
    } else {
        printf("Attempt to read uninitialized SONAR! Aborting...\n");
        abort();
    }
}

/*  MAG */
float sn_get_mag_val(){
    if (sn_mag != DESC_LIMIT){
        get_sensor_value0(sn_mag, &value );
        return value;
    } else {
        printf("Attempt to read uninitialized MAG! Aborting...");
        abort();
    }
}
