#include <msp430.h>
#include "hal_usciB1.h"
#include "hal_gpio.h"

extern USCIB1_SPICom spi;

void HAL_USCIB1_Init(void) {
    UCB1CTL1 |= UCSWRST; // Reset
    UCB1CTL0 |= UCCKPL+UCMSB+UCMST+UCSYNC;// TI 1-1, MSB first, 8-Bit
    UCB1CTL0 &= ~UCCKPH; // Data is changed on first UCLK edge
    UCB1CTL0 |= UCMODE_0;
    UCB1CTL1 |= UCSSEL__SMCLK;
    UCB1BR0 = 25; // Clock prescaler 2,5MHz / 25 = 100kHz
    UCB1BR1 = 0;

//    __delay_cycles(100);
    UCB1CTL1 &= ~UCSWRST; // Reset off
    UCB1IE |= UCRXIE; // Enable RX interrupt
}

void HAL_USCIB1_Transmit(void) {
    CS_LOW;
    UCB1TXBUF = spi.TxData.Data[0];
    CS_HIGH;
    spi.TxData.cnt++;
}

#pragma vector = USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void)
{
  //  while (!(UCB1IFG&UCTXIFG));
    CS_LOW;
    UCB1TXBUF = 0xAA;
   __delay_cycles(1000);
    CS_HIGH;


    if(spi.TxData.cnt != spi.TxData.len)
    {
        CS_LOW;
        UCB1TXBUF = spi.TxData.Data[spi.TxData.cnt];
        CS_HIGH;
        spi.TxData.cnt++;
    }
    else
        spi.Status.TxSuc = 1;
}
