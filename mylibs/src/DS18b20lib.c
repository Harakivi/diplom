#include "main.h"

//bool OWFlag = false;

const uint8_t convert_T[] = {
                OW_0, OW_0, OW_1, OW_1, OW_0, OW_0, OW_1, OW_1, // 0xcc SKIP ROM
                OW_0, OW_0, OW_1, OW_0, OW_0, OW_0, OW_1, OW_0  // 0x44 CONVERT
};

const uint8_t read_scratch[] = {
                OW_0, OW_0, OW_1, OW_1, OW_0, OW_0, OW_1, OW_1, // 0xcc SKIP ROM
                OW_0, OW_1, OW_1, OW_1, OW_1, OW_1, OW_0, OW_1, // 0xbe READ SCRATCH
                OW_R, OW_R, OW_R, OW_R, OW_R, OW_R, OW_R, OW_R,
                OW_R, OW_R, OW_R, OW_R, OW_R, OW_R, OW_R, OW_R,
};

void DS18b20_Convert(void)
{
  RCC->AHBENR |= RCC_AHBENR_DMA1EN;
  DMA1_Channel7->CPAR = (uint32_t)(&USART2->DR);
  DMA1_Channel7->CMAR = (uint32_t)convert_T;
  DMA1_Channel7->CCR = 
    0 << DMA_CCR_MEM2MEM_Pos //0: Memory to memory mode disabled
    | 0x00 << DMA_CCR_PL_Pos //00: Low
    | 0x00 << DMA_CCR_MSIZE_Pos //00: 8-bits
    | 0x00 << DMA_CCR_PSIZE_Pos //00: 8-bits
    | 1 << DMA_CCR_MINC_Pos //1: Memory increment mode enabled 
    | 0 << DMA_CCR_PINC_Pos //0: Peripheral increment mode disabled
    | 0 << DMA_CCR_CIRC_Pos //0: Circular mode disabled
    | 1 << DMA_CCR_DIR_Pos; //1: Write to peripheral
  DMA1_Channel7->CNDTR = sizeof(convert_T); //Number of data to transfer
  USART2->CR3 |= USART_CR3_DMAT;//1: DMA mode is enabled for transmission
  DMA1_Channel7->CCR |= DMA_CCR_EN; //1: Channel enabled
}

void DS18b20_ReadTemp(uint8_t *buf)
{
  RCC->AHBENR |= RCC_AHBENR_DMA1EN;
  DMA1_Channel6->CPAR = (uint32_t)(&USART2->DR);
  DMA1_Channel6->CMAR = (uint32_t)buf;
  DMA1_Channel6->CCR = 
    0 << DMA_CCR_MEM2MEM_Pos //0: Memory to memory mode disabled
    | 0x00 << DMA_CCR_PL_Pos //00: Low
    | 0x00 << DMA_CCR_MSIZE_Pos //00: 8-bits
    | 0x00 << DMA_CCR_PSIZE_Pos //00: 8-bits
    | 1 << DMA_CCR_MINC_Pos //1: Memory increment mode enabled 
    | 0 << DMA_CCR_PINC_Pos //0: Peripheral increment mode disabled
    | 0 << DMA_CCR_CIRC_Pos //0: Circular mode disabled
    | 0 << DMA_CCR_DIR_Pos; //0: Read from peripheral
  DMA1_Channel6->CNDTR = sizeof(read_scratch); //Number of data to transfer  
  DMA1_Channel7->CPAR = (uint32_t)(&USART2->DR);
  DMA1_Channel7->CMAR = (uint32_t)read_scratch;
  DMA1_Channel7->CCR = 
    0 << DMA_CCR_MEM2MEM_Pos //0: Memory to memory mode disabled
    | 0x00 << DMA_CCR_PL_Pos //00: Low
    | 0x00 << DMA_CCR_MSIZE_Pos //00: 8-bits
    | 0x00 << DMA_CCR_PSIZE_Pos //00: 8-bits
    | 1 << DMA_CCR_MINC_Pos //1: Memory increment mode enabled 
    | 0 << DMA_CCR_PINC_Pos //0: Peripheral increment mode disabled
    | 0 << DMA_CCR_CIRC_Pos //0: Circular mode disabled
    | 1 << DMA_CCR_DIR_Pos; //1: Write to peripheral
  DMA1_Channel7->CNDTR = sizeof(read_scratch); //Number of data to transfer
  USART2->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;//1: DMA mode is enabled for transmission and for recievier
  DMA1_Channel6->CCR |= DMA_CCR_EN; //1: Channel enabled
  DMA1_Channel7->CCR |= DMA_CCR_EN; //1: Channel enabled
  //while (DMA1->ISR & DMA_ISR_TCIF6 == 0);
}

uint8_t USART_DS18b20()
{
  uint8_t ow_presence;
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN | RCC_APB2ENR_IOPAEN;
  GPIOA->CRL |= GPIO_CRL_CNF2_0;//
  GPIOA->CRL |= GPIO_CRL_CNF2_1;//11: Alternate Function  Open-drain
  GPIOA->CRL |= GPIO_CRL_MODE2_0 | GPIO_CRL_MODE2_1 ;//11: 50 MHz
   USART2->CR3 &= ~(USART_CR3_DMAT | USART_CR3_DMAR);
  USART2->BRR = 
    468 << USART_BRR_DIV_Mantissa_Pos     
   | 12 << USART_BRR_DIV_Fraction_Pos;//Baud rate - 9600 Value programmed in the BRR = 468.75(reference 799)
  USART2->CR1 = 
    0b0 << USART_CR1_M_Pos//0: 1 Start bit, 8 Data bits, n Stop bit
    | 0b0 << USART_CR1_PCE_Pos//0: Parity control disabled
    | 0b0 << USART_CR1_RXNEIE_Pos//1: A USART interrupt is generated whenever ORE=1 or RXNE=1 in the USART_SR register
    | 0b1 << USART_CR1_TE_Pos//1: Transmitter is enabled
    | 0b1 << USART_CR1_RE_Pos;//1: Receiver is enabled and begins searching for a start bit
  USART2->CR2 = 
    0b00 << USART_CR2_STOP_Pos;//00: 1 Stop bit
  USART2->CR3 = 
    0b0 << USART_CR3_CTSE_Pos//0: CTS hardware flow control disabled
    |0b1 << USART_CR3_HDSEL_Pos;//1: Half duplex mode is selected 
  USART2->CR1 |= USART_CR1_UE;// USART2 Enabled
  USART2->SR &= ~USART_SR_TC;
  USART2->DR = 0xf0;
  while((USART2->SR & USART_SR_TC) == 0);
  ow_presence = USART2->DR;
  USART2->CR1 &= ~(USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);
  USART2->BRR = 
    39 << USART_BRR_DIV_Mantissa_Pos     
   | 1 << USART_BRR_DIV_Fraction_Pos;//Baud rate - 115200 Value programmed in the BRR = 39.0625(reference 799)
  USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;
  USART2->CR1 |= USART_CR1_UE;
  if (ow_presence != 0xf0 && ow_presence != 0x00)
  {
    return 1;
  }
  return 0;
}

//bool getOWFlag()
//{
//  return OWFlag;
//}
//
//void setOWFlag(bool owFlag)
//{
//  OWFlag = owFlag;
//}