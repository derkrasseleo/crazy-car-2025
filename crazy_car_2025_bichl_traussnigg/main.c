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

	int x = 1234;
	int y = 1753;
	float a = 512.1;
	float b = 123.7;

	while(1) {
	    int z = x*y;
	    float c = a*b;
	    if (button.active == 1)
	    {
	        if (button.button == 1)
	            LCD_BACKLIGHT_ON;
	        else if (button.button == 2)
                LCD_BACKLIGHT_OFF;
	    }
	}
	return 0;
}

#pragma vector = TIMER0_B0_VECTOR // (fuer CCR0)

__interrupt void T0_ISR (void) {
    LCD_BACKLIGHT_ON;
    TB0CTL &= ~TBIFG;
}

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
