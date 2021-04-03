#include "main.h"

void initKeyboard(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;
  // GPIO INPUT//
  GPIOB->CRL &= ~(GPIO_CRL_MODE3_0 | GPIO_CRL_MODE3_1 | GPIO_CRL_MODE4_0 | GPIO_CRL_MODE4_1);
  GPIOB->CRL &= ~(GPIO_CRL_CNF3_0 | GPIO_CRL_CNF4_0);
  GPIOB->CRL |= GPIO_CRL_CNF3_1 | GPIO_CRL_CNF4_1;
  GPIOB->ODR &= ~(GPIO_ODR_ODR3 | GPIO_ODR_ODR4); 
  // GPIO OUTPUT//
  GPIOB->CRL |= GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1;
  GPIOB->CRL &= ~(GPIO_CRL_CNF5_0 | GPIO_CRL_CNF5_1);
  GPIOB->CRH |= GPIO_CRH_MODE8_0 | GPIO_CRH_MODE8_1 | GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1;
  GPIOB->CRH &= ~(GPIO_CRH_CNF8_0 | GPIO_CRH_CNF8_1 | GPIO_CRH_CNF9_0 | GPIO_CRH_CNF9_1);
  GPIOA->CRH |= GPIO_CRH_MODE15_0 | GPIO_CRH_MODE15_1;
  GPIOA->CRH &= ~(GPIO_CRH_CNF15_0 | GPIO_CRH_CNF15_1);
  OUT1_1;
  OUT2_1;
  OUT3_1;
  OUT4_1;
}

void tryKeypad(bool* buttArr)
{
  OUT1_1;
  OUT2_1;
  OUT3_1;
  OUT4_1;
  if((GPIOB->IDR & GPIO_IDR_IDR3))
  {
    checkKeys(buttArr);
  }
  else
  {
    for(int i = 0; i < 8; ++i)
    {
      buttArr[i] = false;
    }
  }
}

void checkKeys(bool* buttArr)
{
  for(int i = 0; i < 4; ++i)
  {
    OUT1_0;
    OUT2_0;
    OUT3_0;
    OUT4_0;
    switch(i)
    {
    case 0:
      OUT1_1;
      if(IN1)
      {
        buttArr[0] = true;
      }
      if(IN2)
      {
        buttArr[1] = true;
      }
      break;
    case 1:
      OUT2_1;
      if(IN1)
      {
        buttArr[2] = true;
      }
      if(IN2)
      {
        buttArr[3] = true;
      }
      break;
    case 2:
      OUT3_1;
      if(IN1)
      {
        buttArr[4] = true;
      }
      if(IN2)
      {
        buttArr[5] = true;
      }
      break;
    case 3:
      OUT4_1;
      if(IN1)
      {
        buttArr[6] = true;
      }
      if(IN2)
      {
        buttArr[7] = true;
      }
      break;
    }
  }
}