#ifndef POSITION_H
#define POSITION_H

#include <pthread.h>
#include <stdbool.h>
/** @brief Global variable used to terminate the position thread
 *  @warning Only for use by #position_start and #position_stop functions.
 */
extern bool position_terminate;

/** @brief Initialization of the position tracking thread
 *  @param x_start initial point on x-axis of starting position
 *  @param y_start initial point on y-axis of starting position
 *  @param heading initial angle of heading
 */
void position_start(float x_start, float y_start, int heading);

/** @brief Function terminating position thread */
void position_stop();

/** @brief Calculate distance and rotation to get from point A to point B
 *  @param xa current position on x-axis
 *  @param ya current position on y-axis
 *  @param xb destination position on x-axis
 *  @param yb destination position on y-axis
 *  @param heading current heading
 *  @param out_dist pointer to a buffer to store calculated distance
 *  @param out_rotation pointer to a buffer to store calculated rotation
 */
void get_dist_and_ang(float xa, float ya, float xb, float yb, int heading, float * out_dist, int * out_rotation);

/** @brief Function returning current position on x and y axis
 *  @param x current position on x-axis
 *  @param y current position on y-axis
 *  @param heading current heading
 */
void get_position_and_heading(float * x, float *y, int * heading);

/** @brief Function returning current heading */
int get_heading();

#endif

