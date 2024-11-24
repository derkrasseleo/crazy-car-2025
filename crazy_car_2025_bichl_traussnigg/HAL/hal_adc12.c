#include <msp430.h>
#include "HAL/hal_adc12.h"

ADC12Com adc;

HAL_ADC12_Init(void)
{
    ADC12CTL0 &= ~ADC12ENC;

}

