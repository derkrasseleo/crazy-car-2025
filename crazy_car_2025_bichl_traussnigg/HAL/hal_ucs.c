/*
 * hal_ucs.c
 *
 *  Created on: Oct 29, 2024
 *      Author: leo
 */

#include <msp430.h>

void HAL_UCS_Init() {
    UCSCTL6 &= ~XT2OFF; // Enable XT2
    UCSCTL3 |= SELREF_2; // Set Frequency of locked loop to REFCLK
    UCSCTL4 |= SELA_2; // Set ACLK to REFCLOCK
    while(SFRIFG1 & OFIFG) // Wait until all error flags are cleared and not set again
    {
       UCSCTL7 &= ~(XT2OFFG + DCOFFG + XT1HFOFFG + XT1LFOFFG);
       SFRIFG1 &= ~OFIFG;
    }

    UCSCTL6 |= XT2DRIVE_3; // Set Drive-Strength in UCSCTL6 (16 to 24 MHz)
    UCSCTL4 |= (SELM_5 + SELS_5 + SELA_5); // Set Submaster-Clock and Master-Clock (UCSCTL4)
    UCSCTL5 |= DIVS__8; // Set new Submaster-Clocks (UCSCTL5)
}
