#include "algo.h"

extern int speed;
extern int vbat;

enum {STOP, FORWARD, BACKWARDS, LEFT, RIGHT, STUCK};
int state = FORWARD;
int lr_diff = 0;
int front_old = 0;
int right_sensor_old = 0;
int left_sensor_old = 0;
int right_sensor_diff = 0;
int left_sensor_diff = 0;
int front_sen_diff = 0;
int cnt_driving = 0;
int cnt_state_left = 0;
int cnt_state_right = 0;
int max_block;
int cnt_curve;

void primitive_driving(unsigned char *perc_steer, signed char *perc_throttle, unsigned int front_sensor, unsigned int left_sensor, unsigned int right_sensor)
{
    lr_diff = left_sensor - right_sensor;
    left_sensor_diff = left_sensor - left_sensor_old;
    right_sensor_diff = right_sensor - right_sensor_old;
    front_sen_diff = front_sensor-front_old;
    front_old = front_sensor;
    left_sensor_old = left_sensor;
    right_sensor_old = right_sensor;

    cnt_driving++;

//    else if(vbat<2200)
//    {
//        // when battery low: min: 30%, max. 100%
//        *perc_throttle =  40 + front_sensor >> 5;//(300+(5*(front_sensor/15)))/10
//    }
//    else if(vbat<=2000)
//    {
//        *perc_throttle = 0;
//    }

    //*perc_throttle = 50 + (front_sensor >> 5); // faster speed calculation
    //*perc_throttle = 40; // for constant speed
    *perc_throttle = (300+(5*(front_sensor/15)))/10;

    if(((front_sensor <= 40) && (left_sensor <= 40)) || ((front_sensor <= 40) && (right_sensor <= 45)))
    {
        state = BACKWARDS;
    }

    if(speed <= 1 && cnt_driving >= 60)
    {
        cnt_driving = 0;
        state = STUCK;
    }
    if((state == FORWARD) && (front_sensor<1000) && (speed>1500))
    {
        state = STOP;
    }
    if(front_sen_diff<0 && speed > 1000)
    {
        *perc_throttle = -10;
    }

    switch(state)
    {
        case STOP:
                *perc_throttle = -10;
                state = FORWARD;
            break;

        case FORWARD:
               cnt_state_left = 0;
               cnt_state_right = 0;
               max_block = 1;

               *perc_steer = 50-(lr_diff>>5);

               if((left_sensor > front_sensor) || (left_sensor_diff >= 500))
               {
                  if((left_sensor >= 1000) && (right_sensor_diff <= 200))
                  {
                      max_block = 35;
                  }
                  state = LEFT;
               }
               else if(right_sensor > front_sensor)
               {
                  if((right_sensor >= 1000) && (right_sensor_diff >= 500))
                  {
                      max_block = 30;
                  }
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
            else if(lr_diff<=10)
            {
                *perc_steer = 50;
            }
            if(front_sensor >= 200)
            {
                *perc_throttle = 50;
                state = FORWARD;
            }
            break;

        case LEFT:
                cnt_state_left++;
               *perc_throttle = 40;
               *perc_steer = 0;
               if((front_sensor > left_sensor) && (cnt_state_left >= max_block))
               {
                   cnt_curve++;
                   state = FORWARD;
               }
            break;

        case RIGHT:
               cnt_state_right++;
               *perc_throttle = 40;
               *perc_steer = 100;
               if((front_sensor > right_sensor) && (cnt_state_right >= max_block))
               {
                   cnt_curve++;
                   state = FORWARD;
               }
           break;

        case STUCK:
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
               *perc_steer = 40;
           }
           if(front_sensor >= 200 && (left_sensor >= 40) && (right_sensor >= 40))
           {
               state = FORWARD;
           }
           break;

        default:
            break;
    }
}
