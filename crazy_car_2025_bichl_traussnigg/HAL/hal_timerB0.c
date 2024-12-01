#include "hal_timerB0.h"
#include "hal_ucs.h"
#include <msp430.h>

void HAL_TimerB0_Init() {
//    TB0CTL = TBCLR;
    TB0CTL = TBSSEL__SMCLK + MC__UP + ID__8 + TBIE;
    TB0EX0 |= TBIDEX__4;
    //SMCLK_FREQU/8/4 = 78125

    TB0CCTL0 = CCIE | OUTMOD_7; // Both edges, Set Outmode to Reset/Set
    TB0CCTL1 = CCIE | OUTMOD_7;
    TB0CCTL2 = CCIE | OUTMOD_7;

    TB0CCR0 = 7813; // 39063 for 0.5 Hz; // Should be 39062.5
    TB0CCR1 = 651; // 120Hz
    TB0CCR2 = 7813; // 7813=100ms/10Hz

    TB0CTL &= ~TBIFG; // Clear Interrupt flag
}
