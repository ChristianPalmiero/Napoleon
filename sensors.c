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

int i;
uint8_t sn;
FLAGS_T state;
uint8_t sn_touch;
uint8_t sn_color;
uint8_t sn_compass;
uint8_t sn_sonar;
uint8_t sn_mag;
char s[ 256 ];
int val;
float value;
uint32_t n, ii;

int sn_init(){
	return ev3_sensor_init();
}

void sn_lookup(){
	printf( "Found sensors:\n" );
	for ( i = 0; i < DESC_LIMIT; i++ ) {
		if ( ev3_sensor[ i ].type_inx != SENSOR_TYPE__NONE_ ) {
			printf( "  type = %s\n", ev3_sensor_type( ev3_sensor[ i ].type_inx ));
			printf( "  port = %s\n", ev3_sensor_port_name( i, s ));
			if ( get_sensor_mode( i, s, sizeof( s ))) {
				printf( "  mode = %s\n", s );
			}
			if ( get_sensor_num_values( i, &n )) {
				for ( ii = 0; ii < n; ii++ ) {
					if ( get_sensor_value( ii, i, &val )) {
						printf( "  value%d = %d\n", ii, val );
					}
				}
			}
		}
	}
}

int sn_get_color_val(){
	if ( ev3_search_sensor( LEGO_EV3_COLOR, &sn_color, 0 )) {
		if ( !get_sensor_value( 0, sn_color, &val ) || ( val < 0 ) || ( val >= COLOR_COUNT )) {
			val = 0;
		}
		return val;
	} else {
		printf( "COLOR sensor was NOT FOUND!\n" );
		return -1;
	}
}

float sn_get_compas_val(){
	if (ev3_search_sensor(HT_NXT_COMPASS, &sn_compass,0)){
		if ( !get_sensor_value0(sn_compass, &value )) {
			value = 0;
		}
		return value;
	} else {
		printf("COMPASS sensor was NOT FOUND!\n");
		return -1;
	}
}

float sn_get_sonar_val(){
	if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0)){
		if ( !get_sensor_value0(sn_sonar, &value )) {
			value = 0;
		}
		return value;
	} else {
		printf("SONAR sensor was NOT FOUND!\n");
		return -1;
	}
}

float sn_get_mag_val(){
	if (ev3_search_sensor(NXT_ANALOG, &sn_mag,0)){
		if ( !get_sensor_value0(sn_mag, &value )) {
			value = 0;
		}
		return value;
	} else {
		printf("Magnetic sensor was NOT FOUND!\n");
		return -1;
	}
}










