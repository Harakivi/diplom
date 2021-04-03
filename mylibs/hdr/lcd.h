#ifndef _LCD_
#define _LCD_
//------------------------------------------------
#define d4_set() GPIOB->ODR |= GPIO_ODR_ODR12
#define d5_set() GPIOB->ODR |= GPIO_ODR_ODR13
#define d6_set() GPIOB->ODR |= GPIO_ODR_ODR14
#define d7_set() GPIOB->ODR |= GPIO_ODR_ODR15
#define d4_reset() GPIOB->ODR &= ~(GPIO_ODR_ODR12)
#define d5_reset() GPIOB->ODR &= ~(GPIO_ODR_ODR13)
#define d6_reset() GPIOB->ODR &= ~(GPIO_ODR_ODR14)
#define d7_reset() GPIOB->ODR &= ~(GPIO_ODR_ODR15)
#define e1 GPIOB->ODR |= GPIO_ODR_ODR10
#define e0 GPIOB->ODR &= ~(GPIO_ODR_ODR10)
#define rs1 GPIOB->ODR |= GPIO_ODR_ODR11
#define rs0 GPIOB->ODR &= ~(GPIO_ODR_ODR11)
//------------------------------------------------
void LCD_init(void);
void LCD_Clear(void);
void LCD_SendChar(char ch);
void LCD_String(char* st);
void LCD_SetPos(int x, int y);
void GPIO_Init(void);
//------------------------------------------------
#endif /* _LCD_ */
