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
#define LCD_BACKLIGHT_ON (P8OUT |= LCD_BL)

void HAL_GPIO_Init() {
    // Port 1
    // Inputs
    setIn(P1DIR, RPM_SENSOR);
    setIn(P1DIR, RPM_SENSOR_DIR);
    setIn(P1DIR, START_BUTTON); // TODO: Pull Up/Down
    setIn(P1DIR, STOP_BUTTON); // TODO: Pull Up/Down
    // Outputs
    setOut(P1DIR, I2C_INT_MOTION);
    // Port 2
    // Inputs
    setIn(P2DIR, DEBUG_TXD);
    setIn(P2DIR, DEBUG_TXD);
    setIn(P2DIR, IC2_SDA_MOTION)
    // Outputs
    setOut(P2DIR, DEBUG_RXD);
    setOut(P2DIR, IC2_SCL_MOTION);
    // Port 3
    // Inputs
    setIn(P3DIR, THROTTLE);
    setIn(P3DIR, STEERING);
    // Outputs
    setOut(P3DIR, DISTANCE_FRONT_EN);
    // Port 4
    // Inputs
    setIn(P4DIR, LINE_FOLLOW_2);
    setIn(P4DIR, LINE_FOLLOW_1);
    setIn(P4DIR, LINE_FOLLOW_3;
    setIn(P4DIR, LINE_FOLLOW_4);
    setIn(P4DIR, LINE_FOLLOW_5);
    // Outputs
    // Port 5
    // Inputs
    // Outputs
    // Port 6
    // Inputs
    setIn(P6DIR, DISTANCE_RIGHT);
    setIn(P6DIR, DISTANCE_LEFT);
    setIn(P6DIR, DISTANCE_FRONT);
    setIn(P6DIR, VBAT_MEASURE);
    // Outputs
    setOut(P6DIR, DISTANCE_LEFT_EN);
    // Port 7
    // Inputs
    setIn(P7DIR, XT2IN);
    // Outputs
    setOut(P7DIR, XT2OUT);
    // Port 8
    // Inputs
    setIn(P8DIR, UART_RXD_AUX);
    setIn(P8DIR, LCS_SPI_MISO);
    UART_TXD_AUX
    // Outputs
    setOut(P8DIR, LCD_BL);
    setOut(P8DIR, UART_TXD_AUX);
    setOut(P8DIR, LCS_SPI_CLK);
    setOut(P8DIR, LCS_SPI_MOSI);
    setOut(P8DIR, LCS_SPI_DATACMD);
    // Port 9
    // Inputs
    setOut(P9DIR, LCD_RESET);
    setOut(P9DIR, DISTANCE_RIGHT_EN);

    // Outputs


}
