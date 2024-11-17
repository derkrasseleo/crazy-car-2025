#include <msp430.h>
#include "hal_general.h"
#include "hal_gpio.h"
#include "DL/driver_aktorik.h"
#include "hal_usciB1.h"
#include "DL/driver_general.h"
#include "DL/driver_lcd.h"

extern ButtonCom button;
extern USCIB1_SPICom spi;

unsigned char ticks = 0;
unsigned char speed = 0;
unsigned char speed_old = 0;
unsigned char speed_dd = 0;
unsigned char percent = 50;

int main(void)
{
	HAL_Init();
	Driver_Init();

	// TODO: Fix Crash when signal from hall sensor

    while (1) {

        Driver_SetThrottle(percent);
        Driver_SetSteering(percent);

        if (button.active) {
            switch (button.button) {
                case 1:
                    Driver_LCD_WriteText("CRAZY CAR 2025", 14, 4, 0);

                    LCD_BACKLIGHT_ON;

                    if (percent < 100)
                        percent += 5;
                    break;

                case 2:
                    LCD_BACKLIGHT_OFF;
                    Driver_LCD_Clear();
//                    HAL_USCIB1_SPI_TEST();
                    if (percent > 0)
                        percent -= 5;
                    break;

                default:
                    break;
            }
        }
        __delay_cycles(1000);
        button.active = 0; // Button-Zustand zurücksetzen
    }
	return 0;
}

#pragma vector = TIMER0_B0_VECTOR // (fuer CCR0)

__interrupt void T0_ISR (void) {
//    LCD_BACKLIGHT_TOGGLE;

    // TODO: check if forwards or backwards
    speed = (ticks*5)*10;
    speed = (speed + speed_old) >> 1;
    speed_old = speed;
    ticks = 0;
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
        case RPM_SENSOR:
            ticks++;
            P1IFG &= ~RPM_SENSOR;
        break;
        case RPM_SENSOR_DIR:
            // TODO: invert value, maybe speed_dd ^= 1; ?
            speed_dd = 1;
            P1IFG &= ~RPM_SENSOR_DIR;
        break;

        default:
            button.active = 0;
            button.button = 0;
            P1IFG = 0x00;
    }
}
