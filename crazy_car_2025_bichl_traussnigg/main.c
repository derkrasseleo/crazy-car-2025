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

	// TODO: Fix Crash when car moves

	int increment = 100;
    int steer_min = 2800;
	int steer_max = 4400;
	int steer_pc = (steer_max-steer_min)/100;

	while(1) {
	    if (button.active == 1)
	    {
	        if (button.button == 1)
	        {
	            LCD_BACKLIGHT_ON;
	            if (TA1CCR2 < (steer_max-increment))
	                TA1CCR2 += 5*steer_pc;
	        }
	        else if (button.button == 2)
	        {
                LCD_BACKLIGHT_OFF;
                if (TA1CCR2 > (steer_min+increment))
                    TA1CCR2 -= 5*steer_pc;
	        }
            __delay_cycles(10000);
            button.active = 0;
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

