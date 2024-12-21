#include <msp430.h>
#include "hal_timerA0.h"

int speed = 0;
int speed_old = 0;
unsigned char ticks = 0;

void HAL_TimerA0_Init() {
    TA0CTL |= TACLR;
    TA0CTL |= TASSEL__SMCLK; // Set Source to SubMasterClock
    TA0CTL |= ID__8;
    TA0CCTL0 |= CM1; // Rising Edge
    TA0CCTL0 &= ~CAP; // Compare Mode
//    TA1CCTL0 |= OUTMOD_7; // Set Outmode to Reset/Set
    TA0CCTL0 |= CCIE;

    TA0CCR0 = 31252; // 31252=100ms/10Hz

    TA0CTL |= MC__UP; // Up mode

    //TA1CTL &= ~TBIFG; // Clear Interrupt flag
}

#pragma vector = TIMER0_A0_VECTOR // for CCR0

__interrupt void TA0_ISR (void) {
    speed = (ticks*10)*10; // *10 mm per tick * 10 Hz
    speed = (speed + speed_old) >> 1;
    speed_old = speed;
    ticks = 0;
    TA0CCTL0 &= ~CCIFG;
}
