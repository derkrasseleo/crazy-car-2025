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

    TA2CCR0 = 20834; // 31252=100ms/10Hz

    TA2CTL |= MC__UP; // Up mode
}
//    TA2CTL |= TACLR;
//    TA2CTL |= TASSEL__SMCLK; // Set Source to SubMasterClock
//    TA2CTL |= ID__1;
//    TA2CCTL0 |= CM1; // Rising Edge
//    TA2CCTL0 &= ~CAP; // Compare Mode
////    TA2CCTL0 |= OUTMOD_7; // Set Outmode to Reset/Set
//    TA2CCTL0 |= CCIE;
//
//    TA2CCTL1 |= CM1; // Rising Edge
//    TA2CCTL1 &= ~CAP; // Compare Mode
//    TA2CCTL1 |= OUTMOD_7; // Set Outmode to Reset/Set
////    TA2CCTL1 |= CCIE;
//
//    TA2CCTL2 |= CM1; // Rising Edge
//    TA2CCTL2 &= ~CAP; // Compare Mode
//    TA2CCTL2 |= OUTMOD_7; // Set Outmode to Reset/Set
////    TA2CCTL2 |= CCIE;
//
//    TA2CCR0 = 41667; // Periodendauer: 60Hz
//    TA2CCR1 = 0; // Periodendauer: 60Hz
//    TA2CCR2 = 3550; // Calibration: Center | Max_Right:  4400 | Max_left: 2700 | radius: 30cm
//
//    TA2CTL |= MC__UP; // Up mode
//
//    //TA2CTL &= ~TBIFG; // Clear Interrupt flag
//}
//
//// TIMER1_A0_VECTOR in driver_aktorik.c
//
////#pragma vector = TIMER1_A1_VECTOR // (fuer CCR1 und CCR2)
////
////__interrupt void T1_ISR (void) {
////
////}
