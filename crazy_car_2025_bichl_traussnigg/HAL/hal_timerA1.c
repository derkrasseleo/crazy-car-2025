#include "hal_timerA1.h"
#include <msp430.h>

void HAL_TimerA1_Init() {
    TA1CTL |= TACLR;
    TA1CTL |= TASSEL__SMCLK; // Set Source to SubMasterClock
    TA1CTL |= ID__1;
    TA1CCTL0 |= CM1; // Rising Edge
    TA1CCTL0 &= ~CAP; // Compare Mode
//    TA1CCTL0 |= OUTMOD_7; // Set Outmode to Reset/Set
    TA1CCTL0 |= CCIE;

    TA1CCTL1 |= CM1; // Rising Edge
    TA1CCTL1 &= ~CAP; // Compare Mode
    TA1CCTL1 |= OUTMOD_7; // Set Outmode to Reset/Set
//    TA1CCTL1 |= CCIE;

    TA1CCTL2 |= CM1; // Rising Edge
    TA1CCTL2 &= ~CAP; // Compare Mode
    TA1CCTL2 |= OUTMOD_7; // Set Outmode to Reset/Set
//    TA1CCTL2 |= CCIE;

    TA1CCR0 = 41667; // Periodendauer: 60Hz
    TA1CCR1 = 0; // Periodendauer: 60Hz
    TA1CCR2 = 3550; // Calibration: Center | Max_Right:  4400 | Max_left: 2700 | radius: 30cm

    TA1CTL |= MC__UP; // Up mode

    //TA1CTL &= ~TBIFG; // Clear Interrupt flag
}

//#pragma vector = TIMER1_A1_VECTOR // (fuer CCR1 und CCR2)
//
//__interrupt void T1_ISR (void) {
//
//}
