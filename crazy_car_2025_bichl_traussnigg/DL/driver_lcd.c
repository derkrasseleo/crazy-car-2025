#include "driver_fonttable.h"
#include "driver_lcd.h"
#include "HAL/hal_usciB1.h"
#include "HAL/hal_gpio.h"
#include "driver_lcd.h"
#include <msp430.h>

extern USCIB1_SPICom spi;

unsigned char LCD_Pos_Array[8];

void Driver_LCD_Init()
{
    LCD_BACKLIGHT_ON; // P8OUT = (P8OUT | LCD_BL); //LCD Backlight ON
    P9OUT = (P9OUT & ~LCD_RESET); // Reset Line LOW

    __delay_cycles (500000); //Wait for Display
    P9OUT = (P9OUT | LCD_RESET); // Reset Line HIGH

    unsigned char LCD_Init_Array[9] = {
        LCD_RESET_FUNC,
        LCD_BIAS,
        ADC_SEL_NORMAL,
        COMMON_REVERSE,
        RES_RATIO,
        ELEC_VOL_MODE,
        ELEC_VOL_VALUE,
        POWER_CONT,
        DISPLAY_ON
    };

    Driver_LCD_WriteCommand(LCD_Init_Array , 9); // Send Command
    Driver_LCD_Clear(); // LCD Clear: Reset all Pixels on Display
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
    HAL_USCIB1_Transmit();
    while(spi.Status.TxSuc == 0); // Wait for transmission to finish
}

void Driver_LCD_WriteText(char *text , unsigned char text_length , unsigned char page , unsigned char col)
{
    unsigned char i, text_length_cnt, col_pos = col;
    Driver_LCD_SetCursor(page, col); // Dislay Cursor setzen
//    __delay_cycles(500000); // Wait for Display
    LCD_DATA; // Auf Datenmodus wechseln

    for(text_length_cnt=0;text_length_cnt < text_length; text_length_cnt++)
        // Textlaenge des Strings abarbeiten
    {
        for(i=0; i < fonts_width_max; i++) // Jedes Character des Strings
        {
            spi.TxData.Data[i] = font_table[*text][i];
            col_pos++;
        }
        spi.TxData.len = LCD_MAX_COLM;
        HAL_USCIB1_Transmit();  // Send Data to Display
        while(spi.Status.TxSuc == 0); // Wait for transmission to finish

        // Column "overflow"?
        // Einfuegen einer Abfrage ob das Ende der Page erreicht ist, ist das der Fall, soll auf der naechsten
        // weitergeschrieben werden
        text++;
    }
}

void Driver_LCD_Clear()
{
    unsigned char i, j;

    for(j=0; (SET_PAGE+j) <= LAST_PAGE; j++)
    {
        Driver_LCD_SetCursor(j, 0);
        LCD_DATA;

        for(i=0; i < 128; i++)
        {
            spi.TxData.Data[i] = 0x00;
        }
        spi.TxData.len = 128;
        HAL_USCIB1_Transmit();
        while(spi.Status.TxSuc == 0); // Wait for transmission
    }

//    for(j=0; (SET_PAGE+j) <= LAST_PAGE; j++)
//    {
//        Driver_LCD_WriteText("                  ", 18, (SET_PAGE+j), 0);
//    }
}

void Driver_LCD_SetCursor(unsigned char page_sel , unsigned char col_sel)
{
    unsigned char msb_col = (col_sel >> 4); //0001 xxxx shiften zu MSB Wert
    unsigned char lsb_col = col_sel & 0x0F; //0000 xxxx maskieren auf LSB Wert

    LCD_Pos_Array[0] = SET_PAGE+page_sel; // Page-Value must be set first
    LCD_Pos_Array[1] = MSB_COL_ADDR+msb_col; // MSB Column Address
    LCD_Pos_Array[2] = LSB_COL_ADDR+lsb_col; // LSB Column Address

    Driver_LCD_WriteCommand(LCD_Pos_Array, 3); // Send LCD_POS_Array commands to LCD
    while(spi.Status.TxSuc == 0); // wait until data is transfered
}
