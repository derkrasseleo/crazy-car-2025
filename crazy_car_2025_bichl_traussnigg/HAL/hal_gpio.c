#include <msp430.h>
#include "hal_gpio.h"

void HAL_GPIO_Init() {
    // Port 1
    // Initialize all pins to be inputs with Pullups by default
    P1SEL = 0x00;
    P1DIR = 0x00;
    P1REN = 0xFF;
    P1OUT = 0x00;
    // Inputs
    setZero(P1DIR, RPM_SENSOR);
    setZero(P1DIR, RPM_SENSOR_DIR);
    // Buttons
    setZero(P1DIR, START_BUTTON); // Set to input
    setOne(P1REN, START_BUTTON); // Enable Pull Up/Down Resistor
    setOne(P1OUT, START_BUTTON); // PullUp selected
    setZero(P1DIR, STOP_BUTTON);
    setOne(P1REN, STOP_BUTTON);
    setOne(P1OUT, STOP_BUTTON);
    // Outputs
    setOne(P1DIR, I2C_INT_MOTION);
    // Interrupts
    setOne(P1IE, START_BUTTON); // Enable interrupt on pin
    setZero(P1IES, START_BUTTON); // Rising Edge
    setOne(P1IE, STOP_BUTTON);
    setZero(P1IES, STOP_BUTTON);
    setOne(P1IE, RPM_SENSOR); // Enable interrupt on pin
    setZero(P1IES, RPM_SENSOR); // Rising Edge
    setOne(P1IE, RPM_SENSOR_DIR); // Enable interrupt on pin
    setZero(P1IES, RPM_SENSOR_DIR); // Rising Edge
    setZero(P1IFG, 0xFF); // Clear
    // Port 2
    P2SEL = 0x00;
    P2DIR = 0x00;
    P2REN = 0xFF;
    P2OUT = 0x00;
    // Inputs
    setZero(P2DIR, DEBUG_TXD);
    setZero(P2DIR, DEBUG_TXD);
    setZero(P2DIR, IC2_SDA_MOTION);
    // Outputs
    setOne(P2DIR, DEBUG_RXD);
    setOne(P2DIR, IC2_SCL_MOTION);
    // Port 3
    P3SEL = 0x00;
    P3DIR = 0x00;
    P3REN = 0xFF;
    P3OUT = 0x00;
    // Inputs

    // Outputs
    setOne(P3DIR, THROTTLE);
    setOne(P3SEL, THROTTLE);
    setOne(P3DIR, STEERING);
    setOne(P3SEL, STEERING);
    setOne(P3DIR, DISTANCE_FRONT_EN);
    setOne(P3DIR, SMCLK);
    setOne(P3SEL, SMCLK);
    // Port 4
    P4SEL = 0x00;
    P4DIR = 0x00;
    P4REN = 0xFF;
    P4OUT = 0x00;
    // Inputs
    setZero(P4DIR, LINE_FOLLOW_2);
    setZero(P4DIR, LINE_FOLLOW_1);
    setZero(P4DIR, LINE_FOLLOW_3);
    setZero(P4DIR, LINE_FOLLOW_4);
    setZero(P4DIR, LINE_FOLLOW_5);
    // Outputs
    // Port 5
    P5SEL = 0x00;
    P5DIR = 0x00;
    P5REN = 0xFF;
    P5OUT = 0x00;
    // Inputs
    // Outputs
    // Port 6
    P6SEL = 0x00;
    P6DIR = 0x00;
    P6REN = 0xFF;
    P6OUT = 0x00;
    // Inputs
    setZero(P6DIR, DISTANCE_RIGHT);
    setOne(P6SEL, DISTANCE_RIGHT);
    setZero(P6DIR, DISTANCE_LEFT);
    setOne(P6SEL, DISTANCE_LEFT);
    setZero(P6DIR, DISTANCE_FRONT);
    setOne(P6SEL, DISTANCE_FRONT);
    setZero(P6DIR, VBAT_MEASURE);
    setOne(P6SEL, VBAT_MEASURE);
    // Outputs
    setOne(P6DIR, DISTANCE_LEFT_EN);
    // Port 7
    P7SEL = 0x00;
    P7DIR = 0x00;
    P7REN = 0xFF;
    P7OUT = 0x00;
    // Quartz IO
    P7SEL |= XT2IN; // Crystal Oscilator
    P7SEL |= XT2OUT; //
    UCSCTL6 &= ~XT2BYPASS; // SMCLK pin to output the SMCLK
    // Inputs
    setZero(P7DIR, XT2IN);
    // Outputs
    setOne(P7DIR, XT2OUT);
    // Port 8
    P8SEL = 0x00;
    P8DIR = 0x00;
    P8REN = 0xFF;
    P8OUT = 0x00;
    // Inputs
    setZero(P8DIR, UART_RXD_AUX);
    setZero(P8DIR, LCD_SPI_MISO);
    setOne(P8SEL, LCD_SPI_MISO);
    // Outputs
    setOne(P8DIR, LCD_BL);
    setOne(P8DIR, LCD_SPI_CS);
    setOne(P8SEL, LCD_SPI_CS);
    setOne(P8DIR, UART_TXD_AUX);
    setOne(P8DIR, LCD_SPI_CLK);
    setOne(P8SEL, LCD_SPI_CLK);
    setOne(P8DIR, LCD_SPI_MOSI);
    setOne(P8SEL, LCD_SPI_MOSI);
    setOne(P8DIR, LCD_SPI_DATACMD);
    // Port 9
    P9SEL = 0x00;
    P9DIR = 0x00;
    P9REN = 0xFF;
    P9OUT = 0x00;
    // Inputs
    setOne(P9DIR, LCD_RESET);
    setOne(P9DIR, DISTANCE_RIGHT_EN);
    // Outputs

    // Set General Interrupt Enable Bit
    __enable_interrupt();
}
