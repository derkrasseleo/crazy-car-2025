#ifndef DL_DRIVER_GENERAL_H_
#define DL_DRIVER_GENERAL_H_

void Driver_SteeringInit(void);
void Driver_Init(void);
void Driver_ESCInit(void);
void Driver_SetSteering(signed char steer_angle_pc);

#endif /* DL_DRIVER_GENERAL_H_ */
