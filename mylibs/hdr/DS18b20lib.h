#ifndef _DS18b20_
#define _DS18b20_
//------------------------------------------------
#define OW_0    0x00
#define OW_1    0xff
#define OW_R    0xff
//------------------------------------------------
//bool getOWFlag(void);
//void setOWFlag(bool owFlag);
uint8_t USART_DS18b20(void);
void DS18b20_Convert(void);
void DS18b20_ReadTemp(uint8_t *buf);
//------------------------------------------------
#endif /* _DS18b20_ */
