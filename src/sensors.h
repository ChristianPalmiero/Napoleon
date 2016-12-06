#ifndef SENSORS_H
#define SENSORS_H


extern const char *color[];

int sn_init();
void sn_lookup();
int sn_get_color_val();
int sn_color_set_mode(char* mode);
float sn_get_compass_val();
float sn_get_gyro_val();
float sn_get_sonar_val();
float sn_get_mag_val();
int sn_gyro_set_mode();

#endif
