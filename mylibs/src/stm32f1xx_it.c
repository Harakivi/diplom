#include "main.h"
#include "stm32f1xx_it.h"

void SysTick_Handler(void)
{
  tick_ms();
}

//void USART2_IRQHandler(void)
//{
//  if(USART2->DR != 0xf0)
//  {
//    setOWFlag(true);
//  }
//  else
//  {
//    setOWFlag(false);
//  }
//}