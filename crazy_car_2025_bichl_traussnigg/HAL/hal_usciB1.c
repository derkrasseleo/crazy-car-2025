#include <msp430.h>
#include "hal_usciB1.h"

void HAL_USCIB1_Init(void) {
    UCB1CTL1 |= UCSSEL__SMCLK;
    UCB1BR0 |= 25; // Clock prescaler /2
    //    UCB1CTL0
    UCB1CTL0 |= UCCKPH+UCCKPL+UCMSB+UCMST+UCSYNC;// TI 1-1, MSB zuerst, 8-Bit
    //    UCB1IE
    UCB1IE = UCRXIE; // Enable USCI_A0 RX,TX interrupt
//    __delay_cycles(100);
}

void HAL_USCIB1_Transmit(char byte) {
    // TODO
//    UCA0TXBUF = 0x01;
    UCB1TXBUF = byte;
}
