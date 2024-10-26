#include "hal_timerB0.h"
#include <msp430.h>

void HAL_TimerB0_Init() {
    TB0CTL |= TBSSEL__SMCLK;
    TB0CTL |= ID__8;
    TB0EX0 |= TBIDEX_3;
    TB0CCR0 |= 39063; // Should be 39062.5
    TB0CCTL0 |= CAP;
    TB0CTL |= MC_1;

}
