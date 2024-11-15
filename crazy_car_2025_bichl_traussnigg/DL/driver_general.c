#include "driver_general.h"
#include "driver_aktorik.h"
#include "driver_lcd.h"

void Driver_Init(void)
{
    Driver_SteeringInit();
    Driver_ESCInit();
    Driver_LCD_Init();
}
