#include <msp430.h>
#include "hal_usciB1.h"
#include "hal_gpio.h"
#include <stdio.h>

extern USCIB1_SPICom spi;
extern ADC12Com adc;

void HAL_USCIB1_Init(void) {
    UCB1CTL1 |= UCSWRST; // Reset
    UCB1CTL0 |= UCCKPL+UCMSB+UCMST+UCSYNC;// TI 1-1, MSB first, 8-Bit
    UCB1CTL0 &= ~UCCKPH; // Data is changed on first UCLK edge
    UCB1CTL0 |= UCMODE_0;
    UCB1CTL1 |= UCSSEL__SMCLK;
    UCB1BR0 = 25; // Clock prescaler 2,5MHz / 25 = 100kHz
    UCB1BR1 = 0;

    UCB1CTL1 &= ~UCSWRST; // Reset off
    UCB1IE |= UCRXIE; // Enable RX interrupt
    P8OUT |= LCD_SPI_CS; // CS default high

    spi.Status.TxSuc = 1;
    CS_HIGH;
}

void HAL_USCIB1_Transmit(void) {
    spi.Status.TxSuc = 0;
    spi.TxData.cnt = 0;
    CS_LOW;
    UCB1TXBUF = spi.TxData.Data[spi.TxData.cnt];
    spi.TxData.cnt++;
}

void HAL_USCIB1_SPI_TEST(void) {
    spi.TxData.Data[0] = 0x69;
    spi.TxData.Data[1] = 0x42;
    spi.TxData.Data[2] = 0x18;
    spi.TxData.len = 3;
    spi.TxData.cnt = 0;

    HAL_USCIB1_Transmit();
}

#pragma vector = USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void)
{
    spi.RxData.Data[0] = UCB1RXBUF; // Read RX Buffer to reset interrupt

    if ((UCB1IE & UCRXIE) && (spi.Status.TxSuc == 0))
    {
        if(spi.TxData.cnt <= spi.TxData.len)
        {
            UCB1TXBUF = spi.TxData.Data[spi.TxData.cnt];
            spi.TxData.cnt++;
        }
        else
        {
            CS_HIGH;
            spi.Status.TxSuc = 1;
        }
    }
}
