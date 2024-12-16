#include "algo.h"

enum {STOP, FORWARD, LEFT, RIGHT};
static int state = FORWARD;

int sen_diff;

void primitive_driving(int *perc_steer, int *perc_throttle, unsigned int front_sensor, unsigned int left_sensor, unsigned int right_sensor)
{
    sen_diff = left_sensor - right_sensor;
    *perc_throttle = 50;

    switch(state)
    {
        case FORWARD:
               *perc_steer = sen_diff/10;
               if(left_sensor > front_sensor)
               {
                  state = LEFT;
               }
               else if(right_sensor > front_sensor)
               {
                  state = RIGHT;
               }
            break;
        case LEFT:
               *perc_steer = 100;
               if(front_sensor > left_sensor)
               {
                  state = FORWARD;
               }
            break;
        case RIGHT:
               *perc_steer = -100;
               if(front_sensor > right_sensor)
               {
                  state = FORWARD;
               }
            break;
        default:

            break;
    }
}
