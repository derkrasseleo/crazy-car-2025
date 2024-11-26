#include "hal_timerB0.h"
#include "hal_ucs.h"
#include <msp430.h>

void HAL_TimerB0_Init() {
    TB0CTL |= TACLR;
    TB0CTL |= TBSSEL__SMCLK;
    TB0CTL |= ID__8;
    TB0EX0 |= TBIDEX__4;

    //SMCLK_FREQU/8/4 = 78125
    TB0CCTL0 |= CCIE;
    TB0CCTL0 |= CM_3; // Both edges
    TB0CCTL0 &= ~CAP;
    TB0CCTL0 |= OUTMOD_7; // Set Outmode to Reset/Set
//    TB0CCTL1 |= CM1; // Rising Edge
//    TB0CCTL1 &= ~CAP;
//    TB0CCTL1 |= OUTMOD_7; // Set Outmode to Reset/Set
//    TB0CCTL1 |= CCIE;

    TB0CCR0 |= 39063; // 7813=100ms/10Hz // 39063 for 0.5 Hz; // Should be 39062.5
    TB0CCR1 |= 651; // 120Hz

    TB0CTL |= MC__UP;

    TB0CTL &= ~TBIFG; // Clear Interrupt flag

}
