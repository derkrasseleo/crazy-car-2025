/*
 * hal_general.c
 *
 *  Created on: Oct 4, 2024
 *      Author: leo
 */
#include "hal_general.h"
#include "hal_wd_timer.h"

void HAL_Init() {
    HAL_PMM_Init();
    HAL_Wdt_Init();
}


