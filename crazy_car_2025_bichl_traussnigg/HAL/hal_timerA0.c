#include <msp430.h>
#include "hal_timerA0.h"

void HAL_TimerA0_Init() {
    TA0CTL |= TACLR;
    TA0CTL |= TASSEL__SMCLK; // Set Source to SubMasterClock
    TA0CTL |= ID__1;
    TA0CCTL0 |= CM1; // Rising Edge
    TA0CCTL0 &= ~CAP; // Compare Mode
//    TA1CCTL0 |= OUTMOD_7; // Set Outmode to Reset/Set
    TA0CCTL0 |= CCIE;

//    TA0CCTL1 |= CM1; // Rising Edge
//    TA0CCTL1 &= ~CAP; // Compare Mode
//    TA0CCTL1 |= OUTMOD_7; // Set Outmode to Reset/Set
////    TA1CCTL1 |= CCIE;
//
//    TA0CCTL2 |= CM1; // Rising Edge
//    TA0CCTL2 &= ~CAP; // Compare Mode
//    TA0CCTL2 |= OUTMOD_7; // Set Outmode to Reset/Set
////    TA1CCTL2 |= CCIE;

    TA0CCR0 = 7813; // Periodendauer: 60Hz
//    TA0CCR1 = 0; // Periodendauer: 60Hz
//    TA0CCR2 = 3550; // Calibration: Center | Max_Right:  4400 | Max_left: 2700 | radius: 30cm

    TA0CTL |= MC__UP; // Up mode

    //TA1CTL &= ~TBIFG; // Clear Interrupt flag
}
