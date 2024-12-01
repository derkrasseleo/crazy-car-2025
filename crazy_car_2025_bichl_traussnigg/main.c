#include <msp430.h>
#include "hal_general.h"
#include "hal_gpio.h"
#include "DL/driver_aktorik.h"
#include "hal_usciB1.h"
#include "DL/driver_general.h"
#include "DL/driver_lcd.h"
#include "HAL/hal_adc12.h"

extern ButtonCom button;
extern USCIB1_SPICom spi;
extern ADC12Com adc;

unsigned char ticks = 0;
int speed = 0;
int speed_old = 0;
unsigned char speed_dd = 0;
unsigned char percent = 50;
char test_text[5] = {48, 49, 50, 51, 52};
int number = -100;
double pc_double = 0;
int var;

int main(void)
{
	HAL_Init();
    Driver_Init();

    Driver_LCD_WriteText("RIGHT:", 6, 1, 0);
    Driver_LCD_WriteText("LEFT :", 6, 2, 0);
    Driver_LCD_WriteText("FRONT:", 6, 3, 0);
    Driver_LCD_WriteText("VBAT :", 6, 4, 0);
    Driver_LCD_WriteText("SPEED:", 6, 5, 0);
    Driver_LCD_WriteText("PERCT:", 6, 6, 0);

    while (1) {
        for (var = 0; var < 4; ++var) {
            Driver_LCD_WriteNumber(adc.ADCBuffer[var], 6, var+1, 6*6);
        }
        Driver_LCD_WriteNumber(adc.ADCBuffer[0], 6, 1, 6*6);
        Driver_LCD_WriteNumber(adc.ADCBuffer[1], 6, 2, 6*6);
        Driver_LCD_WriteNumber(adc.ADCBuffer[2], 6, 3, 6*6);
        Driver_LCD_WriteNumber(adc.ADCBuffer[3], 6, 4, 6*6);
        Driver_LCD_WriteNumber(speed, 6, 5, 6*6);
        Driver_LCD_WriteNumber(percent, 6, 6, 6*6);

        percent = (adc.ADCBuffer[2]/38); // map front sensor to motor speed
        Driver_SetThrottle(percent);
        Driver_SetSteering(percent);

        if (button.active) {
            switch (button.button) {
                case 1:
//                      Driver_LCD_WriteText("EWO", 3, 0, 0);
//                    Driver_LCD_WriteText(test_text, 5, 0, 0);
                    Driver_LCD_WriteNumber(adc.ADCBuffer[0], 6, 1, 0);
                    Driver_LCD_WriteNumber(adc.ADCBuffer[1], 6, 2, 0);
                    Driver_LCD_WriteNumber(adc.ADCBuffer[2], 6, 3, 0);
                    Driver_LCD_WriteNumber(adc.ADCBuffer[3], 6, 4, 0);

                    LCD_BACKLIGHT_ON;

                    if (percent < 100)
                        percent += 5;
                    break;

                case 2:
                    LCD_BACKLIGHT_OFF;
//                    Driver_LCD_Clear();
                    Driver_LCD_Init();
//                    HAL_USCIB1_SPI_TEST();
                    if (percent > 0)
                        percent -= 5;
                    break;

                default:
                    break;
            }
        }
//        __delay_cycles(1000);
        button.active = 0; // Reset button state
    }
	return 0;
}

#pragma vector = TIMER0_B0_VECTOR // (fuer CCR0)

__interrupt void T0_ISR (void) {
//    LCD_BACKLIGHT_TOGGLE;
    // TODO: check if forwards or backwards
    speed = (ticks*10)*10; // *10 mm per tick * 10 Hz
    speed = (speed + speed_old) >> 1;
    speed_old = speed;
    ticks = 0;

    TB0CTL &= ~TBIFG;
}

//#pragma vector = TIMER0_B1_VECTOR // (fuer CCR1)
//
//__interrupt void TB0_CCR1_ISR (void) {
//
//    //Driver_LCD_WriteNumber(speed, 6, 4, 0); //  +-32766
//
//    TB0CTL &= ~TBIFG;
//}
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
            // TODO: make speed value negative?
            speed_dd = 1-speed_dd;
            P1IFG &= ~RPM_SENSOR_DIR;
            LCD_BACKLIGHT_TOGGLE;
        break;

        default:
            button.active = 0;
            button.button = 0;
            P1IFG = 0x00;
    }
}
