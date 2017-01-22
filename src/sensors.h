#ifndef SENSORS_H
#define SENSORS_H


extern const char *color[];

/** @brief Initialization of the sensors */
int sn_init();

/** @brief Function printing types of connected sensors */ 
void sn_lookup();

/** @brief Function returning the color value from color sensor */
int sn_get_color_val();

/** @brief Function setting the color mode to color sensor 
 *  @param mode color mode name
 */
int sn_color_set_mode(char* mode);

/** @brief Function returning the compass value */
float sn_get_compass_val();

/** /** @brief Function returning the gyro value */
float sn_get_gyro_val();

/** @brief Function returning the sonar value */
float sn_get_sonar_val();

/** @brief Function returning the magnetnetic value  */
float sn_get_mag_val();

/** @brief Function setting the gyro mode */ 
int sn_gyro_set_mode();

#endif
