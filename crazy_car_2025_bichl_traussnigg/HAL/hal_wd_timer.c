/*
 * hal_wd_timer.c
 *
 *  Created on: Oct 4, 2024
 *      Author: leo
 */
#include <msp430.h>
#include "hal_wd_timer.h"

void HAL_Wdt_Init() {
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
}
