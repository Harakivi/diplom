#ifndef _INIT_
#define _INIT_
//------------------------------------------------
#define SYSCLOCK 72000000UL
//------------------------------------------------
void Init(uint16_t* dataBuffer, uint16_t length);
void rccInit(void);
void SysTick_Init(void);
void TIM1init(void);
void TIM4init(void);
void ADCInit(void);
void DMA_ADCInit(uint16_t* dataBuffer, uint16_t length);
void USART_DS18b20Reset(void);
void OW_SendCommand(const uint8_t *command, uint16_t len);
void OW_ReadData(const uint8_t *command, uint8_t *buf, uint16_t len);
//------------------------------------------------
#endif /* _INIT_ */