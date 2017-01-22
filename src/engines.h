#ifndef ENGINES_H
#define ENGINES_H
/**  @brief Definitions used by #turn function */
enum { TURN_FORWARD, TURN_REVERSE };
extern int stop_turn;

/** @brief Engine initialization function
 *
 * This function is doing all low-level magic to make lego tacho engines online.
 * @warning This function must be called, before and other engine-related functions.
 * @note All three engines must be plugged. Otherwise, this function will fail
 * @return 0 if engine initialization was sucessful. Otherwise abort() is called.
 */
int engine_init(void);

/** @brief Helper function displaying all currently plugged Lego engines */ 
void engine_list(void);

/** @brief Reset all plugged engines to default values */
void engine_reset(void);

/** @brief This function will make robot go straight for given ammount of time
 *  @param miliseconds Desired travel time. If equal 0 - go forever
 *  @note This function will block if @p miliseconds != 0
 */
void go_straight(int miliseconds);

/** @brief This function will make robot go back for given ammount of time
 *  @param seconds Desired travel time. If equal 0 - go forever
 *  @note This function will block if @p miliseconds != 0
 */
void go_back(int seconds);


/** @brief Send TACHO_STOP command to L and R engine.
 *  @warning Behavior may differ depending on the choosen stop action
 */
void engine_stop(void);

/** Turn robot by gven angle and direction 
 *  @param angle Turn angle. Positive - left, Negative - right
 *  @param direction 0 Forward 1 Reverse*/
void turn(int angle, int direction);

/** Turn robot in place by given angle 
 * @param angle Turn angle. Positive - lef, Negative - right
*/
void turn2(int angle);

/** @brief Read current encoder values from left and right engine 
 *  @param disp_left Buffer to store count of left engine encoder
 *  @param disp_right Buffer to store count of right engine encoder 
 */
void get_encoders_values(int * disp_left, int * disp_right);

/** @brief Close the ball cage */
void close_ball(void);

/** @brief Open the ball cage  */
void open_ball(void);

/** @brief Adjust current engine speed by giver @p err factor 
 *  @param err Value to add to the left engine current speed and to subtract from the right one
 *  @warning If the robot is at rest or the err value is too big, the behaviour is undefined 
 */
void adjust_speed_by(int err);
#endif
