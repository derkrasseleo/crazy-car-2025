#include "algo.h"

enum {STOP, FORWARD, BACKWARDS, LEFT, RIGHT};
int state = FORWARD;
int lr_diff;
int front_old = 0;
int front_sen_diff;
extern int speed;
extern int vbat;

void primitive_driving(unsigned char *perc_steer, signed char *perc_throttle, unsigned int front_sensor, unsigned int left_sensor, unsigned int right_sensor)
{
    lr_diff = left_sensor - right_sensor;
    front_sen_diff = front_sensor-front_old;
    front_old = front_sensor;

    if(front_sen_diff<0)
    {
        *perc_throttle = 40;
    }
    else if(vbat<2000)
    {
        *perc_throttle = (300+(8*(front_sensor/15)))/10;
    }
    else
    {
        *perc_throttle = (300+(5*(front_sensor/15)))/10;
    }

    if(((front_sensor <= 40) && (left_sensor <= 40)) || ((front_sensor <= 40) && (right_sensor <= 45)))
    {
        state = BACKWARDS;
    }

//    if(&perc_throttle >= 70 && front_sensor <= 200 && speed>2000)
//    {
//        *perc_throttle = 0;
//    }

    switch(state)
    {
        case FORWARD:
               *perc_steer = 50-(lr_diff>>4);
               if(left_sensor > front_sensor)
               {
                  state = LEFT;
               }
               else if(right_sensor > front_sensor)
               {
                  state = RIGHT;
               }
            break;
        case BACKWARDS:
            *perc_throttle = -50;
            if(left_sensor < right_sensor)
            {
                *perc_steer = 0;
            }
            else if(right_sensor < left_sensor)
            {
                *perc_steer = 100;
            }
            if(front_sensor >= 150)
            {
                *perc_throttle = 50;
                state = FORWARD;
            }
            break;

        case LEFT:
               *perc_steer = 0;
               if(front_sensor > left_sensor)
               {
                  state = FORWARD;
               }
            break;
        case RIGHT:
               *perc_steer = 100;
               if(front_sensor > right_sensor)
               {
                  state = FORWARD;
               }
            break;
        default:

            break;
    }
}
