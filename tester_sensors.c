/*
 * =====================================================================================
 *
 *       Filename:  tester_sensors.c
 *
 *    Description:  Main file to test sensors library
 *
 *        Version:  1.0
 *        Created:  14.11.2016 14:47:46
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
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include <unistd.h>

#include "sensors.c"

int main(){
	if ( ev3_init() == -1 ) return ( 1 );
	sn_init();
	sn_lookup();
	sn_set_color_mode();
	sn_lookup();
	while(1){
		float col = sn_get_sonar_val();
		printf( "\r(%f) \n", col);
		fflush( stdout );
		sleep(1);
	}
	ev3_uninit();

}

