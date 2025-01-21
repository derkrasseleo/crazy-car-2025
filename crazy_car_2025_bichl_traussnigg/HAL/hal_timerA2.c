#include "hal_timerA1.h"
#include <msp430.h>

void HAL_TimerA2_Init() {

    TA2CTL |= TACLR;
    TA2CTL |= TASSEL__SMCLK; // Set Source to SubMasterClock
    TA2CTL |= ID__1;
    TA2CCTL0 |= CM1; // Rising Edge
    TA2CCTL0 &= ~CAP; // Compare Mode
//    TA1CCTL0 |= OUTMOD_7; // Set Outmode to Reset/Set
    TA2CCTL0 |= CCIE;

    TA2CCR0 = 41667; // 60 HZ

    TA2CTL |= MC__UP; // Up mode
}
