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

void Driver_LCD_WriteCommand (unsigned char *data, unsigned char len);
void Driver_LCD_Init(void);
void Driver_LCD_Clear(void);
void Driver_LCD_SetCursor(void);

#endif /* DL_DRIVER_LCD_H_ */
