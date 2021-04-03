#include "main.h"
#include <math.h>
#include <cstring>
#define SIZE 50

void regToCels(uint16_t *InputReg, double *Input, double *coldJuncTemp);

//РАБОТАЕТ НЕ СТАБИЛЬНО ПОЭТОМУ ПОКА НЕ ИСПОЛЬЗУЕТСЯ
//bool groupSetup(uint16_t *InputReg, double *Input, uint32_t **Outputs, double* Setpoint, PID* PIDs, double *coldJuncTemp)
//{
//  uint16_t startTicks = get_tickms();
//  regToCels(InputReg, Input, coldJuncTemp);
//  double startInputs[5];
//  for(int i = 0; i < 5; i++)
//  {
//    startInputs[i] = Input[i];
//  }
//  for(int i = 0; i < 5; i++)
//  {
//    *Outputs[i] = 100;
//    while(get_tickms() - startTicks <= 30000)
//    {
//      regToCels(InputReg, Input, coldJuncTemp);
//      if(Input[0] - startInputs[0] >= 100)
//      {
//        PIDs[i] = PID(Input, Outputs[i], Setpoint, 10, 10, 10, DIRECT);
//        startInputs[0] = Input[0];
//        break;
//      }
//      else if(Input[1] - startInputs[1] >= 100)
//      {
//        PIDs[i] = PID(Input + 1, Outputs[i], Setpoint + 1, 10, 10, 10, DIRECT);
//        startInputs[1] = Input[1];
//        break;
//      }
//      else if(Input[2] - startInputs[2] >= 100)
//      {
//        PIDs[i] = PID(Input + 2, Outputs[i], Setpoint + 2, 10, 10, 10, DIRECT);
//        startInputs[2] = Input[2];
//        break;
//      }
//      else if(Input[3] - startInputs[3] >= 100)
//      {
//        PIDs[i] = PID(Input + 3, Outputs[i], Setpoint + 3, 10, 10, 10, DIRECT);
//        startInputs[3] = Input[3];
//        break;
//      }
//      else if(Input[4] - startInputs[4] >= 100)
//      {
//        PIDs[i] = PID(Input + 4, Outputs[i], Setpoint + 4, 10, 10, 10, DIRECT);
//        startInputs[4] = Input[4];
//        break;
//      }
//    }
//    if(get_tickms() - startTicks > 30000)
//    {
//      for(int j = 5; j < 5; j++)
//      {
//        *Outputs[j] = 0;
//      }
//      return false;
//    }
//    *Outputs[i] = 0;
//  }
//  for(int i = 0; i < 5; i++)
//  {
//    *Outputs[i] = 0;
//  }
//  return true;
//}

int main()
{
  uint16_t InputReg[SIZE * 5] = {};
  unsigned long long int opr = 0;
  Init(InputReg, SIZE);
  LCD_init();
  LCD_Clear();
  char string[32] = {};
  LCD_SetPos(0,0);
  strcpy(string, "Initializating...");
  LCD_String(string);
  double Input[5] = {};
  /* ------------------ МАССИВ В КОТОРОМ ЗАДАЕТСЯ ТЕМПЕРАТУРА ------------------*/
  double Setpoint[5] = { 200, 200, 200, 200, 200}; 
  TIM1init();
  TIM4init();
  double Outputs[5] = {};
  PID PID1(Input + 3, Outputs, Setpoint, 10, 10, 10, DIRECT);
  PID PID2(Input + 4, Outputs + 1, Setpoint + 1, 10, 10, 10, DIRECT);
  PID PID3(Input, Outputs + 2, Setpoint + 2, 10, 10, 10, DIRECT);
  PID PID4(Input + 2, Outputs + 3, Setpoint + 3, 10, 10, 10, DIRECT);
  PID PID5(Input + 1, Outputs + 4, Setpoint + 4, 10, 10, 10, DIRECT);
  //PID PIDs[5] = {};
  //РАБОТАЕТ НЕ СТАБИЛЬНО ПОЭТОМУ ПОКА НЕ ИСПОЛЬЗУЕТСЯ
//  if(!groupSetup(InputReg, Input, Outputs, Setpoint, PIDs))
//  {
//    LCD_Clear();
//    LCD_SetPos(0,0);
//    strcpy(string, "ERROR");
//    LCD_String(string);
//    while(1);
//  } 
  LCD_Clear();
  LCD_SetPos(0,0);
  strcpy(string, "Done");
  LCD_String(string);
  uint64_t dsConvTime = 0;
  uint8_t scratch[32];
  double coldJuncTemp = 0.0;
  bool buttArr[8] = {};
  while(1)
  {
    tryKeypad(buttArr);
    if(dsConvTime == 0 && (DMA1->ISR & DMA_ISR_TCIF6) > 0 && (USART2->SR & USART_SR_TC) > 0)
    {
      //delay_ms(10);
      USART_DS18b20();
        DS18b20_Convert();
        dsConvTime = get_tickms();
        uint16_t tt=0;
        for(int i=16;i<32; i++)
        {
          if (scratch[i] == 0xff)
          {
            tt = (tt>>1) | 0x8000;
          } 
          else
          {
            tt = tt>>1;
          }
        }
        coldJuncTemp = (tt >> 4) + ((tt & 0xf)/16.0);
    }
    if(get_tickms() - dsConvTime >= 1000)
    {
      USART_DS18b20();
      DS18b20_ReadTemp(scratch);
      dsConvTime = 0;
    }
    regToCels(InputReg, Input, &coldJuncTemp);
    PID1.Compute();
    PID2.Compute();
    PID3.Compute();
    PID4.Compute();
    PID5.Compute();
    Output0 = (int)Outputs[0];
    Output1 = (int)Outputs[1];
    Output2 = (int)Outputs[2];
    Output3 = (int)Outputs[3];
    Output4 = (int)Outputs[4];
    for(int i = 0; i < 5; i++)
    {
      //PIDs[i].Compute();
    }
    if(get_tickms() - opr > 100){
      LCD_Clear();
      LCD_SetPos(0,0);
      sprintf(string, "%3.0f", Input[0]);
      LCD_String(string);
      sprintf(string, " %3.0f", Input[1]);
      LCD_String(string);
      sprintf(string, " %3.0f", Input[2]);
      LCD_String(string);
      LCD_SetPos(0,1);
      sprintf(string, "%3.0f", Input[3]);
      LCD_String(string);
      sprintf(string, " %3.0f", Input[4]);
      LCD_String(string);
//      int buttCount = 0;
//      for(int i = 0; i < 8; i++)
//      {
//        if(buttArr[i] == true)
//        {
//          buttCount++;
//        }
//      }
      sprintf(string, "   %3.2f", coldJuncTemp);
      LCD_String(string);
      //PWR->CR |=  PWR_CR_DBP;              
      //BKP->DR8 = (uint16_t)32567;                      
      //PWR->CR &= ~PWR_CR_DBP;   
      opr = get_tickms();
    }
  }
}

void regToCels(uint16_t *InputReg, double *Input, double *coldJuncTemp)
{
  int chanIncr = 0;
  for(int i = 0; i < 5; i++)
  {
    Input[i] = 0;
  }
  for(int i = 0; i < SIZE * 5; i++)
  {
    if(chanIncr == 0)
    {
      Input[0] += InputReg[i];
      chanIncr++;
    }
    else if(chanIncr == 1) {
      Input[1] += InputReg[i];
      chanIncr++;
    }
    else if(chanIncr == 2){
      Input[2] += InputReg[i];
      chanIncr++;
    }
    else if(chanIncr == 3){
      Input[3] += InputReg[i];
      chanIncr++;
    }
    else if(chanIncr == 4){
      Input[4] += InputReg[i];
      chanIncr = 0;
    }
  }
  for(int i = 0; i < 5; i++)
  {
    Input[i] /= SIZE;
    Input[i] /= 1241.212121;
    Input[i] = (Input[i] / 100)/0.000039 + *coldJuncTemp;
  }
}
