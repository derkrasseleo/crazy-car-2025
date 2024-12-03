#include "hal_timerB0.h"
#include "hal_ucs.h"
#include "hal_gpio.h"
#include <msp430.h>

void HAL_TimerB0_Init() {
//    TB0CTL = TBCLR;
    TB0CTL = TBSSEL__SMCLK + MC__UP + ID__8 + TBIE;
    TB0EX0 |= TBIDEX__4;
    //SMCLK_FREQU/8/4 = 78125

    TB0CCTL0 = CCIE | OUTMOD_7; // Set Outmode to Reset/Set
//    TB0CCTL1 = CCIE | OUTMOD_7;
//    TB0CCTL2 = CCIE | OUTMOD_7;

    TB0CCR0 = 651; // 39063 for 0.5 Hz; // Should be 39062.5
//    TB0CCR1 = 651; // 120Hz
//    TB0CCR2 = 651; // 7813=100ms/10Hz

    TB0CTL &= ~TBIFG; // Clear Interrupt flag
}

#pragma vector = TIMER0_B0_VECTOR // for CCR0

__interrupt void T0_ISR (void) {
    if (TB0CCTL0 & CCIFG)
    {
        TB0CCTL0 &= ~CCIFG;
    }
}

#pragma vector = TIMER0_B1_VECTOR // for CCR1-6

__interrupt void TB0_CCR1_6_ISR (void) {

    switch (TB0IV) {
        case TB0IV_TBCCR1:
            TB0CCTL1 &= ~CCIFG;
            break;
        case TB0IV_TBCCR2:
            TB0CCTL2 &= ~CCIFG;
            break;
        default:
            break;
    }
}
