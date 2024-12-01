#include <msp430.h>
#include "hal_dma.h"
#include "hal_adc12.h"

extern ADC12Com adc;

HAL_DMA_Init(void)
{
    // Repeated Block transfer, increment SRC and DST
    DMA0CTL |= DMADT_5 + DMADSTINCR_3 + DMASRCINCR_3 + DMAEN + DMAIE;
    DMACTL0 |= DMA0TSEL__ADC12IFG;
    DMA0SZ = 4; // Increment four times
    DMA0SA = &ADC12MEM0;
    DMA0DA = &adc.ADCBuffer[0];
}

#pragma vector = DMA_VECTOR

__interrupt void DMA_ISR (void) {
    switch(DMAIV) // Interrupt is reset on DMAIV read
    {
    case 0:
        break;
    case 2:
        break;
    default:
        break;
    }
}
