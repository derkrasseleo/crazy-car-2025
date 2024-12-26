#include <msp430.h>

const int MaxRPW = 2500;
const int MinRPW = 5000;
const int MinFPW = 7500;
const int MaxFPW = 10000;
const int MaxBreak = (MinFPW+MinRPW)/2;

volatile int speed_controller_impuls = 0;

const int steer_min = 2700;
const int steer_max = 4300;
int steer_pc = (steer_max-steer_min)/100;

int mc_val;

void Driver_SetSteering(unsigned char steer_angle_pc)
{
    TA1CCR2 = steer_min+(steer_pc*steer_angle_pc);
}

void Driver_SteeringInit(void)
{
    Driver_SetSteering(50);
}

void Driver_SetThrottle(signed char throttle_pc)
{
    if (throttle_pc > 0)
    {
        // Vorwärtsbereich
        mc_val = MinFPW + (throttle_pc * ((MaxFPW - MinFPW) / 100));
    }
    else if (throttle_pc == 0)
    {
        // Bremsen
        mc_val = MaxBreak;
    }
    else if (throttle_pc < 0)
    {
        // Rückwärtsbereich
        mc_val = MinRPW + (throttle_pc * ((MinRPW - MaxRPW) / 100));
    }

    TA1CCR1 = mc_val;
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
    createPulses(MinRPW,128);
    createPulses(MinFPW,128);
    createPulses(MaxFPW,128);

    createPulses(MaxBreak, 30);
    Driver_SetThrottle(50);
}

#pragma vector = TIMER1_A0_VECTOR // (fuer CCR0)
__interrupt void TA1_CCR0_ISR (void)
{
    speed_controller_impuls ++;
//    TA1CTL &= ~TAIFG;
}
