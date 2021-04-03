#include "main.h"

void Init(uint16_t* dataBuffer, uint16_t length){
  rccInit();
  SysTick_Init();
  AFIO->MAPR &= ~(AFIO_MAPR_SWJ_CFG_0 | AFIO_MAPR_SWJ_CFG_2);//
  AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_1; // MAPR 010: JTAG-DP Disabled and SW-DP Enabled
  ADCInit();
  DMA_ADCInit(dataBuffer, length);
  initKeyboard();
}

void rccInit(){
  FLASH->ACR |= FLASH_ACR_LATENCY_1;//
  FLASH->ACR &= ~(FLASH_ACR_LATENCY_0 | FLASH_ACR_LATENCY_2);//010 Two wait states, if 48 MHz < SYSCLK = 72 MHz
  RCC->CR |= RCC_CR_HSEON; //1: HSE ON
  while((RCC->CR & RCC_CR_HSERDY) < 1){} // Wait HSE Ready
  RCC->CFGR = 
    0b1 << RCC_CFGR_PLLSRC_Pos// 1: HSE oscillator clock selected as PLL input clock
    |0b0 << RCC_CFGR_PLLXTPRE_Pos//0: HSE clock not divided
    |0b0111 << RCC_CFGR_PLLMULL_Pos// PLLMUL 0111: PLL input clock x 9 = 72Mhz
    |0b0000 << RCC_CFGR_HPRE_Pos//HPRE 0xxx: SYSCLK not divided
    |0b100 << RCC_CFGR_PPRE1_Pos//PPRE1 100: HCLK divided by 2
    |0b000 << RCC_CFGR_PPRE2_Pos;//PPRE2 0xx: HCLK not divided
  RCC->CR |= RCC_CR_PLLON; //1: PLL ON
  while((RCC->CR & RCC_CR_PLLRDY) < 1){} //Wait PLL Ready
  RCC->CFGR &= ~RCC_CFGR_SW_0;//
  RCC->CFGR |= RCC_CFGR_SW_1;// SW 10: PLL selected as system clock
  while((RCC->CFGR & RCC_CFGR_SWS) != 8){}
  RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN; //1: Backup interface clock enabled //1: Power interface clock enable
}

void SysTick_Init(){
  SysTick->LOAD =  SYSCLOCK / 1000 - 1;
  SysTick->VAL = SYSCLOCK / 1000 - 1;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void TIM4init(){
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; //1: Timer 4 clock enabled
  TIM4->CR1 &= ~TIM_CR1_CKD_0;//
  TIM4->CR1 &= ~TIM_CR1_CKD_1;//00: tDTS=tCK_INT
  TIM4->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS_1 | TIM_CR1_CMS_0); //0: Counter used as upcounter
  TIM4->CR1 &= ~TIM_CR1_OPM; //0: Counter is not stopped at update event
  TIM4->ARR = 255;
  TIM4->PSC = 5;
  TIM4->EGR |= TIM_EGR_UG; //1: Re-initialize the counter and generates an update of the registers. Note that the prescaler
                           //counter is cleared too (anyway the prescaler ratio is not affected). The counter is cleared if
                           //the center-aligned mode is selected or if DIR=0 (upcounting), else it takes the auto-reload
                           //value (TIMx_ARR) if DIR=1 (downcounting).
  TIM4->CR1 &= ~TIM_CR1_ARPE; //0: TIMx_ARR register is notDIR: buffered
  TIM4->SMCR &= ~TIM_SMCR_SMS_0; // 
  TIM4->SMCR &= ~TIM_SMCR_SMS_1; // 
  TIM4->SMCR &= ~TIM_SMCR_SMS_2; // 000: Slave mode disabled - if CEN = ‘1’ then the prescaler is clocked directly by the internal clock
  TIM4->SMCR &= ~TIM_SMCR_ECE;//
  TIM4->CCMR1 &= ~(TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE); //0: Preload register on TIMx_CCR1 disabled. TIMx_CCR1 can be written at anytime, the new value is taken in account immediately
  TIM4->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E; //0: Off - OC1 is not active. 
  TIM4->CCMR1 &= ~(TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC2M_0);//
  TIM4->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC2M_1;//
  TIM4->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC2M_2;//110: PWM mode 1 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1 else inactive. In downcounting, channel 1 is inactive (OC1REF=‘0’) as long as TIMx_CNT>TIMx_CCR1 else active (OC1REF=’1’).
  TIM4->CCMR1 &= ~(TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0); //
  TIM4->CCMR1 &= ~(TIM_CCMR1_CC1S_1 | TIM_CCMR1_CC2S_1); //00: CC1 channel is configured as output
  TIM4->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P); //0: OC1 active high
  TIM4->CCR1 = 90;
  TIM4->CCR2 = 0;
  TIM4->CCMR1 &= ~(TIM_CCMR1_OC1FE | TIM_CCMR1_OC2FE); //Output compare 1 fast enable
                                   //This bit is used to accelerate the effect of an event on the trigger in input on the CC output.
                                   //0: CC1 behaves normally depending on counter and CCR1 values even when the trigger is
                                   //ON. The minimum delay to activate CC1 output when an edge occurs on the trigger input is
                                   //5 clock cycles.
  TIM4->CR2 &= ~TIM_CR2_MMS_0; //000: Reset 
  TIM4->CR2 &= ~TIM_CR2_MMS_1; //000: Reset 
  TIM4->CR2 &= ~TIM_CR2_MMS_2; //000: Reset 
  TIM4->SMCR &= ~TIM_SMCR_MSM; //Master/Slave mode 0: No action
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  GPIOB->CRL &= ~GPIO_CRL_CNF6_0;//
  GPIOB->CRL |= GPIO_CRL_CNF6_1;//10: AF Push-pull
  GPIOB->CRL |= GPIO_CRL_MODE6_0 | GPIO_CRL_MODE6_1 ;//11: Maximum output speed 50 MHz
  GPIOB->CRL &= ~GPIO_CRL_CNF7_0;//
  GPIOB->CRL |= GPIO_CRL_CNF7_1;//CONFIG 10: AF Push-pull
  GPIOB->CRL |= GPIO_CRL_MODE7_0 | GPIO_CRL_MODE7_1 ;//11: Maximum output speed 50 MHz
  TIM4->CR1 |= TIM_CR1_CEN;//1: Counter enabled
}

void TIM1init(){
  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN | RCC_APB2ENR_AFIOEN; //1: Timer 1 clock enabled
  TIM1->CR1 &= ~TIM_CR1_CKD_0;//
  TIM1->CR1 &= ~TIM_CR1_CKD_1;//00: tDTS=tCK_INT
  TIM1->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS_1 | TIM_CR1_CMS_0); //0: Counter used as upcounter
  TIM1->CR1 &= ~TIM_CR1_OPM; //0: Counter is not stopped at update event
  TIM1->ARR = 255;
  TIM1->PSC = 5;
  TIM1->EGR |= TIM_EGR_UG; //1: Re-initialize the counter and generates an update of the registers. Note that the prescaler
                           //counter is cleared too (anyway the prescaler ratio is not affected). The counter is cleared if
                           //the center-aligned mode is selected or if DIR=0 (upcounting), else it takes the auto-reload
                           //value (TIMx_ARR) if DIR=1 (downcounting).
  TIM1->CR1 &= ~TIM_CR1_ARPE; //0: TIMx_ARR register is notDIR: buffered
  TIM1->SMCR &= ~TIM_SMCR_SMS_0; // 
  TIM1->SMCR &= ~TIM_SMCR_SMS_1; // 
  TIM1->SMCR &= ~TIM_SMCR_SMS_2; // 000: Slave mode disabled - if CEN = ‘1’ then the prescaler is clocked directly by the internal clock
  TIM1->SMCR &= ~TIM_SMCR_ECE;//
  TIM1->CCMR1 = 
    0b0 << TIM_CCMR1_OC1PE_Pos//0: Preload register on TIMx_CCR1 disabled. TIMx_CCR1 can be written at anytime, the new value is taken in account immediately
    | 0b0 << TIM_CCMR1_OC2PE_Pos
    | 0b110 << TIM_CCMR1_OC1M_Pos//110: PWM mode 1 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1 else inactive. In downcounting, channel 1 is inactive (OC1REF=‘0’) as long as TIMx_CNT>TIMx_CCR1 else active (OC1REF=’1’).
    | 0b110 << TIM_CCMR1_OC2M_Pos
    | 0b00 <<  TIM_CCMR1_CC1S_Pos //00: CC1 channel is configured as output
    | 0b00 <<  TIM_CCMR1_CC2S_Pos
    | 0b0 << TIM_CCMR1_OC1FE_Pos//Output compare 1 fast enable
                                   //This bit is used to accelerate the effect of an event on the trigger in input on the CC output.
                                   //0: CC1 behaves normally depending on counter and CCR1 values even when the trigger is
                                   //ON. The minimum delay to activate CC1 output when an edge occurs on the trigger input is
                                   //5 clock cycles.
    | 0b0 << TIM_CCMR1_OC2FE_Pos;
  TIM1->CCMR2 = 
    0b0 << TIM_CCMR2_OC3PE_Pos
    | 0b0 << TIM_CCMR2_OC4PE_Pos
    | 0b110 << TIM_CCMR2_OC3M_Pos
    | 0b110 << TIM_CCMR2_OC4M_Pos
    | 0b00 <<  TIM_CCMR2_CC3S_Pos
    | 0b00 <<  TIM_CCMR2_CC4S_Pos
    | 0b0 << TIM_CCMR2_OC3FE_Pos
    | 0b0 << TIM_CCMR2_OC4FE_Pos;
  TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E; //0: Off - OC1 is not active. 
  TIM1->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P | TIM_CCER_CC3P | TIM_CCER_CC4P); //0: OC1 active high
  TIM1->CCR1 = 0;
  TIM1->CCR2 = 0;
  TIM1->CCR3 = 0;
  TIM1->CCR4 = 0;
  TIM1->CR2 &= ~TIM_CR2_MMS_0; //000: Reset 
  TIM1->CR2 &= ~TIM_CR2_MMS_1; //000: Reset 
  TIM1->CR2 &= ~TIM_CR2_MMS_2; //000: Reset 
  TIM1->SMCR &= ~TIM_SMCR_MSM; //Master/Slave mode 0: No action
  TIM1->BDTR |= TIM_BDTR_MOE;
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  GPIOA->CRH &= ~(GPIO_CRH_CNF8_0 | GPIO_CRH_CNF9_0 | GPIO_CRH_CNF10_0 | GPIO_CRH_CNF11_0);//
  GPIOA->CRH |= GPIO_CRH_CNF8_1 | GPIO_CRH_CNF9_1 | GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_1;//10: AF Push-pull
  GPIOA->CRH |= GPIO_CRH_MODE8_0 | GPIO_CRH_MODE8_1 | GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1 | GPIO_CRH_MODE10_0 | GPIO_CRH_MODE10_1 | GPIO_CRH_MODE11_0 | GPIO_CRH_MODE11_1;// MODE CONFIG 11: Maximum output speed 50 MHz
  TIM1->CR1 |= TIM_CR1_CEN;//1: Counter enabled
}

void ADCInit(void){
  RCC->CFGR &= ~RCC_CFGR_ADCPRE_0;//
  RCC->CFGR |= RCC_CFGR_ADCPRE_1;//10: PCLK2 divided by 6
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  GPIOA->CRL &= ~(GPIO_CRL_MODE3_0 | GPIO_CRL_MODE3_1 | GPIO_CRL_MODE4_0 | GPIO_CRL_MODE4_1 |
			  GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1 | GPIO_CRL_MODE6_0 | GPIO_CRL_MODE6_1 | GPIO_CRL_MODE7_0 | GPIO_CRL_MODE7_1);
  GPIOA->CRL &= ~(GPIO_CRL_CNF3_0 | GPIO_CRL_CNF3_1 | GPIO_CRL_CNF4_0 | GPIO_CRL_CNF4_1 |
	  		  GPIO_CRL_CNF5_0 | GPIO_CRL_CNF5_1 | GPIO_CRL_CNF6_0 | GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_0 | GPIO_CRL_CNF7_1);
  ADC1->CR2 |= ADC_CR2_CONT;
  ADC1->CR1 |= ADC_CR1_SCAN; //1: Scan mode enabled
  ADC1->SQR3 =
    0b00011 << ADC_SQR3_SQ1_Pos// Bits 4:0 SQ1[4:0]: first conversion in regular sequence = 00011(CH3)
    |0b00100 << ADC_SQR3_SQ2_Pos// Bits 4:0 SQ2[4:0]: first conversion in regular sequence = 00100(CH4)
    |0b00101 << ADC_SQR3_SQ3_Pos// Bits 4:0 SQ3[4:0]: first conversion in regular sequence = 00101(CH5)
    |0b00110 << ADC_SQR3_SQ4_Pos// Bits 4:0 SQ4[4:0]: first conversion in regular sequence = 00110(CH6)
    |0b00111 << ADC_SQR3_SQ5_Pos;// Bits 4:0 SQ5[4:0]: first conversion in regular sequence = 00111(CH7)
  ADC1->SQR1 = 0b0100 << ADC_SQR1_L_Pos;// Regular channel sequence length = 0100: 5 conversions 
  ADC1->SMPR2 = 
    0b111 << ADC_SMPR2_SMP3_Pos// 111: 239.5 cycles CH3
    |0b111 << ADC_SMPR2_SMP4_Pos// 111: 239.5 cycles CH4
    |0b111 << ADC_SMPR2_SMP5_Pos// 111: 239.5 cycles CH5
    |0b111 << ADC_SMPR2_SMP6_Pos// 111: 239.5 cycles CH6
    |0b111 << ADC_SMPR2_SMP7_Pos;// 111: 239.5 cycles CH7
  ADC1->CR2 |= ADC_CR2_EXTSEL_0 | ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_2 | ADC_CR2_EXTTRIG; //EXTTRIG 0: Conversion on external event disabled
  ADC1->CR2 |= ADC_CR2_DMA;//1: DMA mode enabled
  ADC1->CR2 |= ADC_CR2_ADON;
  ADC1->CR2 |= ADC_CR2_RSTCAL;
  ADC1->CR2 |= ADC_CR2_SWSTART;
  while ((ADC1->CR2 & ADC_CR2_RSTCAL) == ADC_CR2_RSTCAL){}
  ADC1->CR2 |= ADC_CR2_CAL;
  while ((ADC1->CR2 & ADC_CR2_RSTCAL) == ADC_CR2_CAL){}
}

void DMA_ADCInit(uint16_t* dataBuffer, uint16_t length){
  RCC->AHBENR |= RCC_AHBENR_DMA1EN;
  DMA1_Channel1->CPAR = (uint32_t)(&ADC1->DR);
  DMA1_Channel1->CMAR = (uint32_t)dataBuffer;
  DMA1_Channel1->CCR = 
    0 << DMA_CCR_MEM2MEM_Pos //0: Memory to memory mode disabled
    | 0x00 << DMA_CCR_PL_Pos //00: Low
    | 0x01 << DMA_CCR_MSIZE_Pos //01: 16-bits
    | 0x01 << DMA_CCR_PSIZE_Pos //01: 16-bits
    | 1 << DMA_CCR_MINC_Pos //1: Memory increment mode enabled 
    | 0 << DMA_CCR_PINC_Pos //0: Peripheral increment mode disabled
    | 1 << DMA_CCR_CIRC_Pos //1: Circular mode enabled
    | 0 << DMA_CCR_DIR_Pos; //0: Read from peripheral
  DMA1_Channel1->CNDTR = length * 5; //Number of data to transfer
  DMA1_Channel1->CCR |= 1 << DMA_CCR_EN_Pos; //1: Channel enabled
}