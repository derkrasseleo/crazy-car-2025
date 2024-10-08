#include <msp430.h> 
#include "hal_general.h"

#define LCD_BACKLIGHT_ON (P8OUT |= LCD_BL)

/**
 * main.c
 */
int main(void)
{
	HAL_Init();

	int x = 0;
	float y = 0.0;

	while(1) {
	    x = 1+1;
	    LCD_BACKLIGHT_ON;
	    y = 1.0+1.0;

	}

	return 0;
}
