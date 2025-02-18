#include <msp430.h>
#include "HAL/hal_adc12.h"
#include "hal_gpio.h"
#include "DL/driver_lcd.h"

extern ADC12Com adc;

HAL_ADC12_Init(void)
{
    ADC12CTL0 &= ~ADC12ENC;
    REFCTL0 |= REFVSEL_3 + REFON + REFOUT;

    // 1024 Sample Hold cycles + Multiple SampleConversion + Reference on + 2.5V
    ADC12CTL0 |= ADC12SHT0_15 + ADC12MSC + ADC12REFON + ADC12REF2_5V;
    ADC12CTL2 |= ADC12RES_2; // ADC12+ Resolution : 12 Bit

    // SHS = TimerB0 CCR0 | SS = SMCLK | NO DIVISION | Repeat Sequence of Channels
    ADC12CTL1 |= ADC12SHS_2 + ADC12SSEL_3 + ADC12DIV_0 + ADC12CONSEQ_3 + ADC12SHP; // Timer B0 CCR1

    ADC12MCTL0 |= ADC12SREF_1 + ADC12INCH_0;
    ADC12MCTL1 |= ADC12SREF_1 + ADC12INCH_1;
    ADC12MCTL2 |= ADC12SREF_1 + ADC12INCH_2;
    ADC12MCTL3 |= ADC12SREF_1 + ADC12INCH_3 + ADC12EOS; // End of sequence

    ADC12IE &= ~ADC12IE3; // Reset interrupt for DMA

    ADC12CTL0 |= ADC12ENC + ADC12ON; // Enable ADC conversion
}
