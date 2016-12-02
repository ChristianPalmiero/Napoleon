#ifndef SENSORS_H
#define SENSORS_H

int sn_init();
void sn_lookup();
float sn_get_color_val();
int sn_color_set_mode(char* mode);
float sn_get_compass_val();
float sn_get_gyro_val();
float sn_get_sonar_val();
float sn_get_mag_val();

//const char *color; 

#endif
