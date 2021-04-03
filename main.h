#ifndef _MAIN_
#define _MAIN_
//------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//------------------------------------------------
#define Output0 TIM4->CCR2
#define Output1 TIM1->CCR1
#define Output2 TIM1->CCR2
#define Output3 TIM1->CCR3
#define Output4 TIM1->CCR4
//------------------------------------------------
#include "stm32f1xx.h"
#include "Init.h"
#include "lcd.h"
#include "delay.h"
#include "PID_v1.h"
//#include "lcd5110.h"
#include "DS18b20lib.h"
#include <cstdio>
#include "keyboard.h"
//------------------------------------------------
#ifdef __cplusplus
}
#endif
//------------------------------------------------
#endif /* _MAIN_ */
