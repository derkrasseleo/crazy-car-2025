#include <msp430.h>
#include "hal_general.h"
#include "hal_gpio.h"
#include "DL/driver_aktorik.h"
#include "hal_usciB1.h"

extern ButtonCom button;
extern USCIB1_SPICom spi;

unsigned char percent = 50;

int main(void)
{
	HAL_Init();

	spi.TxData.Data[0] = 0x69;
	spi.TxData.len = 1;
	spi.TxData.cnt = 0;

	// TODO: Fix Crash when signal from hall sensor

	while(1) {
	    Driver_SetThrottle(percent);
        Driver_SetSteering(percent);
	    if (button.active == 1)
	    {
	        if (button.button == 1)
	        {
	            LCD_BACKLIGHT_ON;
	            if(percent<100)
	                percent+=5;

	        }
	        else if (button.button == 2)
	        {
                LCD_BACKLIGHT_OFF;
                if(percent>0)
                    percent-=5;
                HAL_USCIB1_Transmit();
	        }
            __delay_cycles(10000);
            button.active = 0;
	    }
	}
	return 0;
}

#pragma vector = TIMER0_B0_VECTOR // (fuer CCR0)

__interrupt void T0_ISR (void) {
    //LCD_BACKLIGHT_TOGGLE;
    TB0CTL &= ~TBIFG;
}

//#pragma vector = TIMER1_A1_VECTOR // (fuer CCR1 und CCR2)
//
//__interrupt void T1_ISR (void) {
//
//}

#pragma vector = PORT1_VECTOR

__interrupt void P1_ISR (void) {
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
            P1IFG = 0x00;
    }
}

