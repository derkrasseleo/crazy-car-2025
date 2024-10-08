#include <msp430.h> 
#include "hal_general.h"
#include "hal_gpio.h"
#include <stdbool.h>

#define LCD_BACKLIGHT_ON (P8OUT |= LCD_BL)
#define LCD_BACKLIGHT_OFF (P8OUT &= ~LCD_BL)

/**
 * main.c
 */

bool bt1_flag = false;
bool bt2_flag = false;

int main(void)
{
	HAL_Init();
	HAL_GPIO_Init();
	__enable_interrupt();

	int x = 1234;
	int y = 1753;
	float a = 512.1;
	float b = 123.7;

	while(1) {
	    int z = x*y;
	    float c = a*b;

	}
	return 0;
}

#pragma vector = PORT1_VECTOR

__interrupt void P1_ISR (void) {
    // Should no work
    if(P1IFG &= STOP_BUTTON) {
        LCD_BACKLIGHT_ON;
        // P1IFG = 0x00;
    }
    if(P1IFG &= START_BUTTON) {
        LCD_BACKLIGHT_OFF;
        // P1IFG = 0x00;
    }
}
