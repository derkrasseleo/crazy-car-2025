#include <msp430.h>
#include "HAL/hal_adc12.h"
#include "hal_gpio.h"
#include "DL/driver_lcd.h"

ADC12Com adc;

HAL_ADC12_Init(void)
{
    ADC12CTL0 &= ~ADC12ENC;
    REFCTL0 |= REFVSEL_3 + REFON + REFOUT;
    ADC12CTL0 |= ADC12SHT0_15 + ADC12MSC + ADC12REFON + ADC12REF2_5V;
    ADC12CTL2 |= ADC12RES_2;

    ADC12CTL1 |= ADC12SHS_2 + ADC12SSEL_3 + ADC12DIV_0 + ADC12CONSEQ_3 + ADC12SHP; // Timer B0 CCR1

    ADC12MCTL0 |= ADC12SREF_1 + ADC12INCH_0;
    ADC12MCTL1 |= ADC12SREF_1 + ADC12INCH_1;
    ADC12MCTL2 |= ADC12SREF_1 + ADC12INCH_2;
    ADC12MCTL3 |= ADC12SREF_1 + ADC12INCH_3;

    ADC12IE |= ADC12IE3; // Enable interrupt for EOS

    ADC12CTL0 |= ADC12ENC + ADC12ON;

}

#pragma vector = ADC12_VECTOR

__interrupt void ADC12_ISR (void) {

//    Driver_LCD_WriteText("ADC", 3, 0, 0);
//    LCD_BACKLIGHT_OFF;
    adc.ADCBuffer[0] = ADC12MEM0;
    adc.ADCBuffer[1] = ADC12MEM1;
    adc.ADCBuffer[2] = ADC12MEM2;
    adc.ADCBuffer[3] = ADC12MEM3;
    adc.Status.B.ADCrdy = 1;
    ADC12IFG &= ~ADC12IFG;
}
