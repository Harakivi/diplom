#include "main.h"
#include <string>

//------------------------------------------------
void LCD_WriteData(int dt)
{
	if(((dt>>3)&0x01)==1) {d7_set();} else {d7_reset();}
	if(((dt>>2)&0x01)==1) {d6_set();} else {d6_reset();}
	if(((dt>>1)&0x01)==1) {d5_set();} else {d5_reset();}
	if((dt&0x01)==1) {d4_set();} else {d4_reset();}
}
//------------------------------------------------
void LCD_Command(int dt)
{
	rs0;
	LCD_WriteData(dt>>4);
	e1;
	delay_ms(1);
	e0;
	LCD_WriteData(dt);
	e1;
	delay_ms(1);
	e0;
}
//------------------------------------------------
void LCD_Data(int dt)
{
	rs1;
	LCD_WriteData(dt>>4);
	e1;
	delay_ms(1);
	e0;
	LCD_WriteData(dt);
	e1;
	delay_ms(1);
	e0;
}
//------------------------------------------------
void LCD_Clear(void)
{
	LCD_Command(0x01);
	delay_ms(2);
}
//------------------------------------------------
void LCD_SendChar(char ch)
{
	LCD_Data((uint8_t)ch);
	delay_ms(2);
}
//------------------------------------------------
void LCD_String(char* st)
{
	uint8_t i=0;
	while(st[i]!=0)
	{
		LCD_Data(st[i]);
		delay_ms(1);
		i++;
	}
}
//------------------------------------------------
void LCD_SetPos(int x, int y)
{
	switch(y)
	{
		case 0:
			LCD_Command(x|0x80);
			delay_ms(1);
			break;
		case 1:
			LCD_Command((0x40+x)|0x80);
			delay_ms(1);
			break;
	}
}
//------------------------------------------------
void LCD_init(void)
{
        GPIO_Init();
	delay_ms(40);
	rs0;
	e0;
	LCD_WriteData(0x03);
	e1;
	delay_ms(5);
	e0;
	LCD_WriteData(0x03);
	e1;
	delay_ms(5);
	e0;
	LCD_WriteData(0x03);
	e1;
	delay_ms(1);
	e0;
	LCD_WriteData(0x02);
	e1;
	delay_ms(1);
	e0;
	LCD_Command(0x28);
	delay_ms(1);
	LCD_Command(0x0C);
	delay_ms(1);
	LCD_Command(0x01);
	delay_ms(1);
	LCD_Command(0x06);
	delay_ms(1);
	LCD_Command(0x02);
	delay_ms(5);
        LCD_Clear();
}

void GPIO_Init(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  GPIOA->CRH |= GPIO_CRH_MODE8_0 | GPIO_CRH_MODE8_1 | GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1 |
			  GPIO_CRH_MODE10_0 | GPIO_CRH_MODE10_1 | GPIO_CRH_MODE11_0 | GPIO_CRH_MODE11_1;
  GPIOA->CRH &= ~(GPIO_CRH_CNF8_0 | GPIO_CRH_CNF8_1 | GPIO_CRH_CNF9_0 | GPIO_CRH_CNF9_1 |
	  		  GPIO_CRH_CNF10_0 | GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_0 | GPIO_CRH_CNF11_1);
  GPIOA->ODR &= ~(GPIO_ODR_ODR8 | GPIO_ODR_ODR9 | GPIO_ODR_ODR10 | GPIO_ODR_ODR11);
  GPIOB->CRL &= ~(GPIO_CRL_MODE3_0 | GPIO_CRL_MODE3_1 | GPIO_CRL_MODE4_0 | GPIO_CRL_MODE4_1 |
			  GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1);
  GPIOB->CRL &= ~(GPIO_CRL_CNF3_0 | GPIO_CRL_CNF4_0 | GPIO_CRL_CNF5_0);
  GPIOB->CRL |=	GPIO_CRL_CNF3_1 | GPIO_CRL_CNF4_1 | GPIO_CRL_CNF5_1;
  GPIOB->ODR &= ~(GPIO_ODR_ODR3 | GPIO_ODR_ODR4 | GPIO_ODR_ODR5);
  GPIOB->CRH |= GPIO_CRH_MODE10_0 | GPIO_CRH_MODE10_1 | GPIO_CRH_MODE11_0 | GPIO_CRH_MODE11_1 |
	  			  GPIO_CRH_MODE12_0 | GPIO_CRH_MODE12_1 | GPIO_CRH_MODE13_0 | GPIO_CRH_MODE13_1 |
				  GPIO_CRH_MODE14_0 | GPIO_CRH_MODE14_1 | GPIO_CRH_MODE15_0 | GPIO_CRH_MODE15_1;
  GPIOB->CRH |= GPIO_CRH_CNF10_0 | GPIO_CRH_CNF11_0 | GPIO_CRH_CNF12_0 | GPIO_CRH_CNF13_0 |
	 	  		  GPIO_CRH_CNF14_0 | GPIO_CRH_CNF15_0;
  GPIOB->CRH &= ~(GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_1 | GPIO_CRH_CNF12_1 | GPIO_CRH_CNF13_1 |
	  	 	  		GPIO_CRH_CNF14_1 | GPIO_CRH_CNF15_1);
  GPIOB->ODR &= ~(GPIO_ODR_ODR10 | GPIO_ODR_ODR11 | GPIO_ODR_ODR12 | GPIO_ODR_ODR13 | GPIO_ODR_ODR14 |
			  	  GPIO_ODR_ODR15);
}

