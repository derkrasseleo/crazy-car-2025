#include "algo.h"

extern int speed;
extern int vbat;

enum {STOP, FORWARD, BACKWARDS, LEFT, RIGHT, STUCK, DOUBLETURN , TODESKREISEL };
enum {CW, CCW};
int state = FORWARD;
int lr_diff = 0;
int front_old = 0;
int right_sensor_old = 0;
int left_sensor_old = 0;
int right_sensor_diff = 0;
int left_sensor_diff = 0;
int front_sen_diff = 0;
int cnt_driving = 0;
unsigned char cnt_state_left = 0;
unsigned char cnt_state_right = 0;
unsigned char cnt_state_forward = 0;
unsigned char cnt_state_doubleturn = 0;
unsigned char cnt_state_todeskreisel = 0;
int max_block;
int cnt_curve;
const unsigned char direction = CCW;
unsigned char last_state = FORWARD;

int start_cnt_driving = 0;

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
    *perc_throttle = (3000+(30*(front_sensor/15)))/100;

    last_state = state;

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
               cnt_state_doubleturn = 0;
               cnt_state_todeskreisel = 0;
               cnt_state_forward++;
               max_block = 1;

               if(cnt_state_forward >= 360)
               {
                   cnt_curve = 0;
               }

               *perc_steer = 50-(lr_diff>>5);
               if (front_sensor > 1000 && left_sensor > 700 && left_sensor_diff > 300) {
                   state = DOUBLETURN;  // Double 180
               }
               else if (front_sensor > 1000 && right_sensor > 700 && right_sensor_diff > 300) {
                   state = TODESKREISEL;
               }
               else
               {
               if (left_sensor_diff >= 300 || right_sensor_diff >= 300) {
                   if (front_sensor < 1000) {
                       if (left_sensor > front_sensor) {
                           if (left_sensor >= 1200) {
                               max_block = 30;  // Normal 90-degree curve
                           }
                           if (direction == CCW && cnt_curve >= 3) {
                               max_block = 55;  // 180 degrees
                           }
                           state = LEFT;
                       }
                       else if (right_sensor > front_sensor) {
                           if (right_sensor >= 1200) {
                               max_block = 37;  // Normal 90-degree curve
                           }
                           if ((direction == CW && cnt_curve >= 3) || (direction == CCW && cnt_curve >= 5)) {
                               max_block = 55;  // Todeskreisel
                           }
                           state = RIGHT;
                       }
                   }
               }
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
                   cnt_state_forward = 0;
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
                   cnt_state_forward = 0;
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
        case DOUBLETURN:

            cnt_state_doubleturn ++;
            if (cnt_state_doubleturn <= 50) {
                *perc_throttle = 30;
                *perc_steer = 0;
            }
            else if (50 < cnt_state_doubleturn && cnt_state_doubleturn <= 170) {
                *perc_throttle = 45;
                *perc_steer = 50 - (lr_diff >> 5);
            }
            else if (170 < cnt_state_doubleturn && cnt_state_doubleturn <= 200) {
                *perc_throttle = 35;
                *perc_steer = 100;
            }
            if (cnt_state_doubleturn >= 250)
                state = FORWARD;
            break;
        case TODESKREISEL:
            cnt_state_todeskreisel ++;
            *perc_throttle = 40;
            if (cnt_state_todeskreisel <= 45) {
                *perc_steer = 100;
            }

            if (45< cnt_state_todeskreisel &&cnt_state_todeskreisel <= 75) {
                *perc_steer = 0;
            }
            if (cnt_state_todeskreisel >= 76)
                state = FORWARD;
            break;
        default:
            break;
    }

}
