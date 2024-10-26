#include "hal_timerA1.h"
#include <msp430.h>

void HAL_TimerA1_Init() {
    TA1CTL |= TACLR;
    TA1CTL |= TASSEL__SMCLK; // Set Source to SubMasterClock
    TA1CTL |= ID__1;
    TA1CCTL0 |= CM1; // Rising Edge
    TA1CCTL0 &= ~CAP;
    TA1CCTL0 |= OUTMOD_7; // Set Outmode to Reset/Set
//    TA1CCTL0 |= CCIE;

    TA1CCTL1 |= CM1; // Rising Edge
    TA1CCTL1 &= ~CAP;
    TA1CCTL1 |= OUTMOD_7; // Set Outmode to Reset/Set
//    TA1CCTL1 |= CCIE;

    TA1CCTL2 |= CM1; // Rising Edge
    TA1CCTL2 &= ~CAP;
    TA1CCTL2 |= OUTMOD_7; // Set Outmode to Reset/Set
//    TA1CCTL2 |= CCIE;

    TA1CCR0 = 41667;
    TA1CCR1 = 0;
    TA1CCR2 = 3750;

    TA1CTL |= MC__UP; // Up mode
}
