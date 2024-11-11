#include "driver_fonttable.h"
#include "driver_lcd.h"
#include "HAL/hal_usciB1.h"
#include "HAL/hal_gpio.h"
#include "driver_lcd.h"
#include <msp430.h>

extern USCIB1_SPICom spi;

void Driver_LCD_Init(void)
{
    LCD_BACKLIGHT_ON; // P8OUT = (P8OUT | LCD_BL); //LCD Backlight ON
    P9OUT = (P9OUT & ~LCD_RESET); //Reset Line LOW
    __delay_cycles (500000); //Wait for Display
    P9OUT = (P9OUT | LCD_RESET); //Reset Line HIGH
    unsigned char LCD_Init_Array[9] = {
        LCD_RESET_FUNC,
        LCD_BIAS,
        ADC_SEL_NORMAL,
        COMMON_REVERSE,
        RES_RATIO,
        ELEC_VOL_MODE,
        ELEC_VOL_VALUE,
        POWER_CONT,
        DISPLAY_ON };
    Driver_LCD_WriteCommand(LCD_Init_Array , 9); // Send Command
    while(spi.Status.TxSuc == 0); // Warten auf Display bzw. beendete SPI-Uebertragung

    Driver_LCD_Clear(); // LCD Clear: Reset all Pixels on Display
    while(spi.Status.TxSuc == 0);
}

void Driver_LCD_WriteCommand(unsigned char *data , unsigned char data_length)
{
    unsigned char i;
    while(spi.Status.TxSuc == 0);
    LCD_COMMAND;
    for(i=0; i< data_length; i++)
    {
        spi.TxData.Data[i] = *data;
        data++;
    }
    spi.TxData.len = data_length;
    spi.TxData.cnt = 0;
    HAL_USCIB1_Transmit();
}

void Driver_LCD_WriteText(char *text , unsigned char text_length , unsigned char page , unsigned char col)
{
    unsigned char i, text_length_cnt, col_pos = col;
    Driver_LCD_SetCursor(); // Dislay Cursor setzen
    __delay_cycles(500000); // Wait for Display
    // Auf Datenmodus wechseln
    for(text_length_cnt=0;text_length_cnt < text_length; text_length_cnt++)
        // Textlaenge des Strings abarbeiten
    {
    for(i=0; i < fonts_width_max; i++) // Jedes Character des Strings
    {
        spi.TxData.Data[i] = font_table[*text][i];
        col_pos++;
    }
    // Daten an das Display senden // Warten bis Uebertragung fertig ist
    // Column "overflow"?
    // Einfuegen einer Abfrage ob das Ende der Page erreicht ist, ist das der Fall, soll auf der naechsten
    // weitergeschrieben werden
    text++;
    }
}
