#ifndef DL_DRIVER_LCD_H_
#define DL_DRIVER_LCD_H_

#define LCD_RESET_FUNC 0xE2
#define LCD_BIAS 0xA3
#define ADC_SEL_NORMAL 0xA0
#define COMMON_REVERSE 0xC8
#define RES_RATIO 0x24
#define ELEC_VOL_MODE 0x81
#define ELEC_VOL_VALUE 0x0F // Kontrast Einstellung
#define POWER_CONT 0x2F
#define DISPLAY_ON 0xAF

#define LCD_COMMAND (P8OUT=(P8OUT & ~LCD_SPI_DATACMD))
#define LCD_DATA (P8OUT=(P8OUT | LCD_SPI_DATACMD))

#define SET_PAGE 0xB0
#define LAST_PAGE 0xB7
#define MSB_COL_ADDR 0x10 //0x10, 0x11,.., 0x1F für spalten über 16
#define LSB_COL_ADDR 0x00 //0x01, 0x02,.., 0x0F für spalten bis 15
#define LCD_MAX_COLM 6

void Driver_LCD_Init();
void Driver_LCD_Clear();
void Driver_LCD_SetCursor(unsigned char page_sel , unsigned char col_sel);
void Driver_LCD_WriteCommand(unsigned char *data, unsigned char len);
void Driver_LCD_WriteText(char *text , unsigned char text_length , unsigned char page , unsigned char col);

#endif /* DL_DRIVER_LCD_H_ */
