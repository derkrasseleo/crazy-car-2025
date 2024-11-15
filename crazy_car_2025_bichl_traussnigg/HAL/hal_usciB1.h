#ifndef HAL_HAL_USCIB1_H_
#define HAL_HAL_USCIB1_H_

typedef struct {

    struct{
        unsigned char TxSuc; // Bit=1W data was transmitted
    }Status;

    struct {
        unsigned char len; // length of data to transmit in byte
        unsigned char cnt; // index of currently transmitted data
        unsigned char Data[256]; // Tx data array
    }TxData;

    struct {
        unsigned char len; // length of recieved data
        unsigned char Data[256]; // RX data array
    }RxData;

}USCIB1_SPICom;

void HAL_USCIB1_Init(void);
void HAL_USCIB1_Transmit(void);
void HAL_USCIB1_SPI_TEST(void);

#endif /* HAL_HAL_USCIB1_H_ */
