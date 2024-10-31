/*
 * driver_aktorik.c
 *
 *  Created on: Oct 28, 2024
 *      Author: leo
 */
#include <msp430.h>

const int MaxRPW = 2500;
const int MinRPW = 5000;
const int MinFPW = 7500;
const int MaxFPW = 10000;
const int MaxBreak = (MinFPW+MinRPW)/2;

volatile int speed_controller_impuls = 0;

void Driver_SetSteering(unsigned char steer_angle_pc)
{

    int increment = 100;
    int steer_min = 2900;
    int steer_max = 4300;
    int steer_pc = (steer_max-steer_min)/100;

    if (TA1CCR2 < (steer_max-increment) && TA1CCR2 > (steer_min+increment))
        TA1CCR2 = steer_min+(steer_pc*steer_angle_pc);
}

void Driver_SteeringInit(void)
{
    Driver_SetSteering(50);
}

void Driver_SetThrottle(signed char throttle_pc)
{
    int val = MinFPW+(throttle_pc*((MaxFPW-MinFPW)/100));
    TA1CCR1 = val;
}

void createPulses(int pwm, int pulseDuration)
{
    speed_controller_impuls = 0;
    TA1CCR1 = pwm;
    while(speed_controller_impuls <= pulseDuration)
    {

    }
}

void Driver_ESCInit(void)
{
    // Reset/Startup
    // PWM Mode
    // Beep
    //
    // TODO: Set throttle to 0;

    createPulses(MaxRPW,131);
    createPulses(MinRPW,128); // 128
    createPulses(MinFPW,128);
    createPulses(MaxFPW,128);

    createPulses(MaxBreak, 30);
}

#pragma vector = TIMER1_A0_VECTOR // (fuer CCR0)
__interrupt void TA1_CCR0_ISR (void)
{
    speed_controller_impuls ++;
//    TA1CTL &= ~TAIFG;
}
