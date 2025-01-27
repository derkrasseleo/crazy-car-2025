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

int var = 0;

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
    *perc_throttle = (3500+(30*(front_sensor/15)))/100;
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
    if((state == FORWARD) && (front_sensor<800) && (speed>1500))
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


               if (front_sensor > 1300 && left_sensor > 700 && left_sensor_diff > 300) {
                   state = DOUBLETURN;  // Double 180
               }
               else if (front_sensor > 1000 && right_sensor > 650 && right_sensor_diff > 200) {
                   state = TODESKREISEL;
               }
               else if (left_sensor_diff >= 450 && left_sensor >= 1000 && front_sensor < 600 && right_sensor <= 800) {
                           max_block = 15;  // Normal 90-degree curve
                           state = LEFT;
                       }
               else if (right_sensor_diff >= 450 && right_sensor >= 1000 && left_sensor <= 800 && front_sensor < 600) {
                              max_block = 15;  // Normal 90-degree curve
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
               *perc_throttle = 45;
               *perc_steer = 15;
               if((front_sensor > left_sensor) && (cnt_state_left >= max_block))
               {
                   cnt_curve++;
                   cnt_state_forward = 0;
                   state = FORWARD;
               }
            break;

        case RIGHT:
               cnt_state_right++;
               *perc_throttle = 45;
               *perc_steer = 85;
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
            if(direction == CCW)
            {
                if (cnt_state_todeskreisel <= 5)
                {
                    *perc_throttle = 30;
                    *perc_steer = 60;
                }
                if (5 < cnt_state_doubleturn && cnt_state_doubleturn <= 65) {
                    *perc_throttle = 45;
                    *perc_steer = 10;
                }
                if (65 < cnt_state_doubleturn && cnt_state_doubleturn <= 135) {
                    //*perc_throttle = 50; normal speed calculation better
                    *perc_throttle = 55;
                    *perc_steer = 50 - (lr_diff >> 5);//
                }
                if (135 < cnt_state_doubleturn && cnt_state_doubleturn <= 190) {
                    *perc_throttle = 33;
                    *perc_steer = 100;
                }
                if (cnt_state_doubleturn >= 190)
                    state = FORWARD;
            }
            else if(direction == CW)
            {
                if (cnt_state_todeskreisel <= 20)
                    *perc_throttle = 20;
                    *perc_steer = 0;
                if (20 < cnt_state_doubleturn <= 25) {
                    *perc_throttle = 35;
                    *perc_steer = 0;
                }
                else if (25 < cnt_state_doubleturn && cnt_state_doubleturn <= 45) {
                    *perc_throttle = 40;
                    *perc_steer = 50 ;
                }
                else if (45 < cnt_state_doubleturn && cnt_state_doubleturn <= 145) {
                    *perc_throttle = 40;
                    *perc_steer = 50 - (lr_diff >> 5);
                }
                else if (155 < cnt_state_doubleturn && cnt_state_doubleturn <= 190) {
                    *perc_throttle = 33;
                    *perc_steer = 100;
                }
                if (cnt_state_doubleturn >= 190)
                    state = FORWARD;
            }

            break;
        case TODESKREISEL:
            cnt_state_todeskreisel ++;
            *perc_throttle = 45;
            if(direction == CCW)
            {
                if (cnt_state_todeskreisel <= 10)
                    *perc_throttle = 10;
                else
                    *perc_throttle = 45;
                if (cnt_state_todeskreisel <= 50) {
                    *perc_steer = 80;
                }

                if (50< cnt_state_todeskreisel && cnt_state_todeskreisel <= 115) {
                    *perc_steer = 15;
                }
                if (cnt_state_todeskreisel >= 95)
                    state = FORWARD;
            }
            else if (direction == CW)
                ;

            break;
        default:
            state = FORWARD;
            break;
    }
}
