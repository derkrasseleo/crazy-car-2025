/*
 * hal_adc12.h
 *
 *  Created on: Nov 24, 2024
 *      Author: leo
 */

#ifndef HAL_HAL_ADC12_H_
#define HAL_HAL_ADC12_H_

typedef struct {
    union{
        unsigned char R;
        struct {
            unsigned char ADCrdy:1; // Bit=1 wenn Daten übertragen wurden
            unsigned char dummy:7;
        }B;
    }Status;
    unsigned short ADCBuffer[4]; // Speicherplatz der ADC Werte
}ADC12Com;

HAL_ADC12_Init(void);

#endif /* HAL_HAL_ADC12_H_ */
