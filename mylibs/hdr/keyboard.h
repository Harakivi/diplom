#ifndef _KEYBOARD_
#define _KEYBOARD_
//------------------------------------------------
#define OW_0    0x00
#define OW_1    0xff
#define OW_R    0xff
//------------------------------------------------
#define IN1 (GPIOB->IDR & GPIO_IDR_IDR3)
#define IN2 (GPIOB->IDR & GPIO_IDR_IDR4)
#define OUT1_0 GPIOB->ODR &= ~GPIO_ODR_ODR5
#define OUT2_0 GPIOB->ODR &= ~GPIO_ODR_ODR8
#define OUT3_0 GPIOB->ODR &= ~GPIO_ODR_ODR9
#define OUT4_0 GPIOA->ODR &= ~GPIO_ODR_ODR15
#define OUT1_1 GPIOB->ODR |= GPIO_ODR_ODR5
#define OUT2_1 GPIOB->ODR |= GPIO_ODR_ODR8
#define OUT3_1 GPIOB->ODR |= GPIO_ODR_ODR9
#define OUT4_1 GPIOA->ODR |= GPIO_ODR_ODR15
//------------------------------------------------
void initKeyboard(void);
void tryKeypad(bool* buttArr);
void checkKeys(bool* buttArr);
//------------------------------------------------
#endif /* _KEYBOARD_ */
