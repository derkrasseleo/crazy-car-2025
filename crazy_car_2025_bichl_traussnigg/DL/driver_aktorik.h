#ifndef DL_DRIVER_AKTORIK_H_
#define DL_DRIVER_AKTORIK_H_

void Driver_ESCInit(void);
void Driver_SteeringInit(void);
void Driver_SetSteering(signed char steer_angle_pc);
void Driver_SetThrottle(signed char throttle_pc);

#endif /* DL_DRIVER_AKTORIK_H_ */
