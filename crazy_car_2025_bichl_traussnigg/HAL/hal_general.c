#include "hal_general.h"
#include "hal_gpio.h"
#include "hal_wd_timer.h"
#include "hal_pmm.h"
#include "hal_ucs.h"
#include "hal_timerB0.h"
#include "hal_timerA1.h"
#include "DL/driver_aktorik.h"
#include "hal_usciB1.h"

ButtonCom button;
USCIB1_SPICom spi;

void HAL_Init() {
    HAL_Wdt_Init();
    HAL_PMM_Init();
    HAL_GPIO_Init();
    HAL_UCS_Init();
    HAL_TimerB0_Init();
    HAL_TimerA1_Init();
//    Driver_SteeringInit();
//    Driver_ESCInit();
    HAL_USCIB1_Init();
}


