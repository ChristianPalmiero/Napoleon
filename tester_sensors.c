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
	sn_lookup();
	int col;
	while(1){
		sn_color_set_mode("COL-COLOR");
		col = sn_get_color_val();
		printf("%s \t\t\t\n", color[col]);
/*		
		sn_color_set_mode("COL-REFLECT");
		sleep(1);
		col = sn_get_color_val();
		printf( "%d \n", col);
*/		
		fflush( stdout );
//		sleep(1);
	}
	ev3_uninit();

}

