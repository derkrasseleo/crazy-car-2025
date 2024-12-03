#include <msp430.h>
#include "hal_general.h"
#include "hal_gpio.h"
#include "DL/driver_aktorik.h"
#include "hal_usciB1.h"
#include "DL/driver_general.h"
#include "DL/driver_lcd.h"
#include "HAL/hal_adc12.h"
#include "HAL/hal_timerA0.h"

void Display_Init(void);

extern ButtonCom button;
extern USCIB1_SPICom spi;
extern ADC12Com adc;

unsigned char ticks = 0;
int speed = 0;
int speed_old = 0;
unsigned char speed_dd = 0;
unsigned char percent = 50;
int var;
int cnt = 0;

int main(void)
{
 	HAL_Init();
    Driver_Init();

    Display_Init();

    while (1) {
        cnt++;

        if (adc.Status.B.ADCrdy == 1 && cnt == 10000)
        {
            adc.Status.B.ADCrdy = 0;
            cnt = 0;
            for (var = 0; var < 4; ++var) {
                Driver_LCD_WriteNumber(adc.ADCBuffer[var], 6, var+1, 6*6);
            }
            Driver_LCD_WriteNumber(speed, 6, 5, 6*6);
            Driver_LCD_WriteNumber(percent, 6, 6, 6*6);
        }
        percent = (adc.ADCBuffer[2]/38); // map front sensor to motor speed
        Driver_SetThrottle(percent);
        Driver_SetSteering(percent);

        if (button.active) {
            switch (button.button) {
                case 1:
                    LCD_BACKLIGHT_ON;
                    Display_Init();

                    if (percent < 100)
                        percent += 5;
                    break;

                case 2:
                    LCD_BACKLIGHT_OFF;
                    Driver_LCD_Clear();
                    if (percent > 0)
                        percent -= 5;
                    break;

                default:
                    break;
            }
        }
        button.active = 0; // Reset button state
    }
	return 0;
}

void Display_Init(void)
{
    Driver_LCD_WriteText("RIGHT:", 6, 1, 0);
    Driver_LCD_WriteText("LEFT :", 6, 2, 0);
    Driver_LCD_WriteText("FRONT:", 6, 3, 0);
    Driver_LCD_WriteText("VBAT :", 6, 4, 0);
    Driver_LCD_WriteText("SPEED:", 6, 5, 0);
    Driver_LCD_WriteText("PERCT:", 6, 6, 0);
}

#pragma vector = TIMER0_A0_VECTOR // for CCR0

__interrupt void TA0_ISR (void) {
    speed = (ticks*10)*10; // *10 mm per tick * 10 Hz
    speed = (speed + speed_old) >> 1;
    speed_old = speed;
    ticks = 0;
    TB0CCTL2 &= ~CCIFG;
    if (TB0CCTL0 & CCIFG)
    {
        TB0CCTL0 &= ~CCIFG;
    }
}

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
