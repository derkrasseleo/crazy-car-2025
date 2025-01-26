#include <msp430.h>
#include "HAL/hal_general.h"
#include "HAL/hal_gpio.h"
#include "HAL/hal_usciB1.h"
#include "HAL/hal_adc12.h"
#include "HAL/hal_timerA0.h"
#include "DL/driver_aktorik.h"
#include "DL/driver_general.h"
#include "DL/driver_lcd.h"
#include "algorithm/algo.h"

void Display_Init(void);

extern ButtonCom button;
extern ADC12Com adc;

extern unsigned char ticks;
extern int speed;
extern int state;

unsigned char car_direction = 0;
int cnt = 0;
signed char perc_throt = 0;
unsigned char perc_steer = 0;
int ir_front_val = 0;
int ir_left_val = 0;
int ir_right_val = 0;
int vbat = 0;

unsigned char driving_status = 0;
unsigned char timer_a2_flag = 0;

int speed = 0;
int speed_old = 0;
unsigned char ticks = 0;

extern int right_sensor_diff;
extern int left_sensor_diff;
extern int cnt_curve;

extern unsigned short ir_front[512];
extern unsigned short ir_left[512];
extern unsigned short ir_right[512];

//unsigned char cnt_adc_rdy = 0;
//const unsigned char MAX_CNT_ADC_RDY = 4;
//unsigned short front_buffer[MAX_CNT_ADC_RDY];
//unsigned short left_buffer[MAX_CNT_ADC_RDY];
//unsigned short right_buffer[MAX_CNT_ADC_RDY];
//
//int front_sum = 0, left_sum = 0, right_sum = 0;
//int var = 0;

int main(void)
{
 	HAL_Init();
    Driver_Init();

    Display_Init();

    while (1) {

        if (adc.Status.B.ADCrdy == 1)
        {
            adc.Status.B.ADCrdy = 0;
//            if(cnt_adc_rdy < MAX_CNT_ADC_RDY)
//            {
//                front_buffer[cnt_adc_rdy] = adc.ADCBuffer[2];
//                left_buffer[cnt_adc_rdy] = adc.ADCBuffer[1];
//                right_buffer[cnt_adc_rdy] = adc.ADCBuffer[0];
//                cnt_adc_rdy++;
//            }
//            else {
//                cnt_adc_rdy = 0;
//                for (var = 0; var < MAX_CNT_ADC_RDY; var++) {
//                    front_sum += front_buffer[var];
//                    left_sum += left_buffer[var];
//                    right_sum += right_buffer[var];
//                }
//                ir_front_val = (front_sum)>>2;
//                ir_left_val = (left_sum)>>2;
//                ir_right_val = (right_sum)>>2;
//
//            }
            ir_front_val = (ir_front_val+ir_front[adc.ADCBuffer[2]>>3])>>1;
            ir_left_val = (ir_left_val+ir_left[adc.ADCBuffer[1]>>3])>>1;
            ir_right_val = (ir_right_val+ir_right[adc.ADCBuffer[0]>>3])>>1;
//            ir_front_val = adc.ADCBuffer[2];
//            ir_left_val = adc.ADCBuffer[1];
//            ir_right_val = adc.ADCBuffer[0];

            vbat = (vbat+adc.ADCBuffer[3])>>1; // max: 2500, 2400? min: 1300?

            if(timer_a2_flag == 1) //(cnt >= 10000) // use cnt to slow down display
            {
                timer_a2_flag = 0;
                cnt++;
                if (cnt == 30)
                {
                    cnt = 0;

                    Driver_LCD_WriteNumber(ir_front_val, 5, 0, 6*6);
                    Driver_LCD_WriteNumber(ir_left_val,  5, 1, 6*6);
                    Driver_LCD_WriteNumber(ir_right_val, 5, 2, 6*6);
                    Driver_LCD_WriteNumber(cnt_curve,    5, 3, 6*6);
                    //Driver_LCD_WriteNumber(speed,        5, 3, 6*6);
                    Driver_LCD_WriteNumber(vbat,         5, 4, 6*6);
                    //Driver_LCD_WriteNumber(perc_throt,   5, 5, 6*6);
                    Driver_LCD_WriteNumber(left_sensor_diff,   5, 5, 6*6);
                    Driver_LCD_WriteNumber(right_sensor_diff,   5, 6, 6*6);

                    //Driver_LCD_WriteNumber(perc_throt,   5, 5, 6*6);
                    //Driver_LCD_WriteNumber(perc_steer,   5, 6, 6*6);
                    Driver_LCD_WriteNumber(state,        5, 7, 6*6);
                }

                if(driving_status == 1)
                {
                    primitive_driving(&perc_steer, &perc_throt, ir_front_val, ir_left_val, ir_right_val);
                    Driver_SetSteering(perc_steer);
                    Driver_SetThrottle(perc_throt);
                }
                else if(driving_status == 0)
                {
                    Driver_SteeringInit();
                    Driver_SetThrottle(0);
                    cnt_curve = 0;
                }
            }
        }

        if (button.active) {
            switch (button.button) {
                case 1:
                    LCD_BACKLIGHT_ON;
                    Display_Init();
                    driving_status = 0;
                    break;

                case 2:
                    //LCD_BACKLIGHT_OFF;
                    Driver_LCD_Clear();
                    Display_Init();
                    driving_status = 1;
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
    Driver_LCD_WriteText("FRONT:", 6, 0, 0);
    Driver_LCD_WriteText("LEFT :", 6, 1, 0);
    Driver_LCD_WriteText("RIGHT:", 6, 2, 0);
//    Driver_LCD_WriteText("SPEED:", 6, 3, 0);
    Driver_LCD_WriteText("CNT_C:", 6, 3, 0);
    Driver_LCD_WriteText("VBAT :", 6, 4, 0);
    Driver_LCD_WriteText("LDIFF:", 6, 5, 0);
    Driver_LCD_WriteText("RDIFF:", 6, 6, 0);
//    Driver_LCD_WriteText("THROT:", 6, 5, 0);
//    Driver_LCD_WriteText("STEER:", 6, 6, 0);
    Driver_LCD_WriteText("STATE:", 6, 7, 0);
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
            car_direction = 1-car_direction;
            P1IFG &= ~RPM_SENSOR_DIR;
//            LCD_BACKLIGHT_TOGGLE;
        break;

        default:
            button.active = 0;
            button.button = 0;
            P1IFG = 0x00;
    }
}

#pragma vector = TIMER0_A0_VECTOR // for CCR0

__interrupt void TA0_ISR (void) {

    speed = (ticks*10)*10; // *10 mm per tick * 10 Hz
    speed = (speed + speed_old) >> 1;
    speed_old = speed;
    ticks = 0;
    TA0CCTL0 &= ~CCIFG;
}

#pragma vector = TIMER2_A0_VECTOR // for CCR0

__interrupt void TA2_ISR (void) {

    timer_a2_flag = 1;
}
