/*
 * driver_aktorik.h
 *
 *  Created on: Oct 28, 2024
 *      Author: leo
 */

#ifndef DL_DRIVER_AKTORIK_H_
#define DL_DRIVER_AKTORIK_H_

void Driver_SetSteering(signed char steer_angle_pc);
void Driver_SteeringInit(void);
void Driver_SetThrottle(signed char throttle_pc);
void Driver_ESCInit(void);



#endif /* DL_DRIVER_AKTORIK_H_ */
