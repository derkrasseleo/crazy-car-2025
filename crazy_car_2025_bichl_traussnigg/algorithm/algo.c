#include "algo.h"

enum {STOP, FORWARD, BACKWARDS, LEFT, RIGHT, STUCK};
int state = FORWARD;
int lr_diff;
int front_old = 0;
int front_sen_diff;
extern int speed;
extern int vbat;
int cnt_driving = 0;

void primitive_driving(unsigned char *perc_steer, signed char *perc_throttle, unsigned int front_sensor, unsigned int left_sensor, unsigned int right_sensor)
{
    lr_diff = left_sensor - right_sensor;
    front_sen_diff = front_sensor-front_old;
    front_old = front_sensor;

    cnt_driving++;

    if(front_sen_diff<0)
    {
        *perc_throttle = 35;
    }
    else if(vbat<2000)
    {
        // when battery low: min: 30%, max. 100%
        *perc_throttle = (300+(7*(front_sensor/15)))/10;
    }
    else
    {
        // when battery high: min: 30%, max. 70%
        *perc_throttle = (300+(4*(front_sensor/15)))/10;
    }

    if(((front_sensor <= 40) && (left_sensor <= 40)) || ((front_sensor <= 40) && (right_sensor <= 45)))
    {
        state = BACKWARDS;
    }

    if(speed <= 1 && cnt_driving >= 1000)
    {
        cnt_driving = 0;
        state = STUCK;
    }

    switch(state)
    {
        case STOP:
                *perc_throttle = 0;
                state = FORWARD;
            break;
        case FORWARD:
               *perc_steer = 50-(lr_diff>>4); // divide lr_diff by 8
               if(left_sensor > front_sensor)
               {
                  state = LEFT;
               }
               else if(right_sensor > front_sensor)
               {
                  state = RIGHT;
               }
               if(front_sensor<500 && speed>1800)
               {
                   state = STOP;
               }
               if(speed <= 1 && cnt_driving >= 1000)
               {
                   cnt_driving = 0;
                   state = STUCK;
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
            else if(lr_diff<=10)
            {
                *perc_steer = 50;
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
        case STUCK:
            state = BACKWARDS;
            break;
        default:

            break;
    }
}
