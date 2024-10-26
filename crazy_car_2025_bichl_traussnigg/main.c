#include <msp430.h>
#include "hal_general.h"
#include "hal_gpio.h"
// #include <stdbool.h>

/**
 * main.c
 */

extern ButtonCom button;

int main(void)
{
	HAL_Init();

	TA1CCR2 = 3750; // 1250 bis 4750

	while(1) {
	    if (button.active == 1)
	    {
	        if (button.button == 1)
	        {
	            LCD_BACKLIGHT_ON;
	            TA1CCR2 += 10;
	        }
	        else if (button.button == 2)
	        {
                LCD_BACKLIGHT_OFF;
                TA1CCR2 -= 10;
	        }
	    }
	}
	return 0;
}

//#pragma vector = TIMER0_B0_VECTOR // (fuer CCR0)
//
//__interrupt void T0_ISR (void) {
//    LCD_BACKLIGHT_ON;
//    TB0CTL &= ~TBIFG;
//}

//#pragma vector = TIMER1_A0_VECTOR // (fuer CCR0)
//
//__interrupt void T1_CCR0_ISR (void) {
//    LCD_BACKLIGHT_ON;
//    TB0CTL &= ~TBIFG;
//}

//#pragma vector = TIMER1_A1_VECTOR // (fuer CCR1 und CCR2)
//
//__interrupt void T1_ISR (void) {
//
//}

#pragma vector = PORT1_VECTOR

__interrupt void P1_ISR (void) {
//    if(P1IFG & STOP_BUTTON) {
//        button.active = 1;
//        button.button = 0;
//        P1IFG &= ~STOP_BUTTON;
//    }
//    else if(P1IFG & START_BUTTON) {
//        button.active = 1;
//        button.button = 1;
//        P1IFG &= ~START_BUTTON;
//    }
    switch P1IFG {
        case STOP_BUTTON:
            button.active = 1;
            button.button = 1;
            P1IFG &= ~STOP_BUTTON;
        break;
        case START_BUTTON:
            button.active = 1;
            button.button = 2;
            P1IFG &= ~START_BUTTON;
        break;
        default:
            button.active = 0;
            button.button = 0;
    }
}

