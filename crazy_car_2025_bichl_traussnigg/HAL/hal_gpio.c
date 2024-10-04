/*
 * hal_gpio.c
 *
 *  Created on: Oct 4, 2024
 *      Author: leo
 */

#include <msp430.h>
#include "hal_gpio.h"

#define setOut(adr,bit) (adr |= 1<<(bit))
#define setIn(adr, bit) (adr &= 1<<~(bit))

void HAL_GPIO_Init() {
    // setIn(P1D), RPM_SE);

}
