/*
 * =====================================================================================
 *
 *       Filename:  sensors.h
 *
 *    Description:  EV3 sensors management librar 
 *
 *        Version:  1.0
 *        Created:  28.11.2016 19:09:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kamil Maslowski (), kmaslows@hotmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

int sn_init();
void sn_lookup();
int sn_get_color_val();
int sn_color_set_mode(char* mode);
int sn_get_compass_val();
int sn_get_gyro_val();
int sn_get_sonar_val();
int sn_get_mag_val();

const char *color[] = { "?", "BLACK", "BLUE", "GREEN", "YELLOW", "RED", "WHITE", "BROWN" };

