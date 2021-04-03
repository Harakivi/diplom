#include "lcd5110.h"
#include "stm32f1xx.h"

void LCDInit(){
  gpioInit();
  SPI1Init();
  RST_LOW;
  for(int i=0; i < 1000; i++){}
  RST_HIGH;

  lcd5110_setBias(0x04);
  lcd5110_setContrast(94);

  // normal mode
  lcd5110_command(PCD8544_FUNCTIONSET);

  // Set display to Normal
  lcd5110_command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
}

void lcd5110_command(int data) {
  DC_LOW;
  //CS_LOW;
  SPI1->DR = data;
  while((SPI1->SR & SPI_SR_BSY) > 0){}
  //CS_HIGH;
}

void lcd5110_data(int data) {
  DC_HIGH;
  //CS_LOW;
  SPI1->DR = data;
  while((SPI1->SR & SPI_SR_BSY) > 0){}
  //CS_HIGH;
}

void lcd5110_setContrast(int val) {
  if (val > 0x7f) {
    val = 0x7f;
  }
  lcd5110_command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );
  lcd5110_command( PCD8544_SETVOP | val);
  lcd5110_command(PCD8544_FUNCTIONSET);

 }


void lcd5110_setBias(int val) {
  if (val > 0x07) {
    val = 0x07;
  }
  lcd5110_command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );
  lcd5110_command(PCD8544_SETBIAS | val);
  lcd5110_command(PCD8544_FUNCTIONSET);
}

void gpioInit(){
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;// GPIOA clock enable
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;// GPIOB clock enable
  GPIOA->CRL |= GPIO_CRL_MODE7_0 | GPIO_CRL_MODE7_1 ;// MOSI MODE CONFIG 11: Maximum output speed 50 MHz
  GPIOA->CRL |= GPIO_CRL_MODE6_0 | GPIO_CRL_MODE6_1 ;// D/C MODE CONFIG 11: Maximum output speed 50 MHz
  GPIOA->CRL |= GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1 ;// CLK MODE CONFIG 11: Maximum output speed 50 MHz
  GPIOB->CRL |= GPIO_CRL_MODE0_0 | GPIO_CRL_MODE0_1 ;// RST MODE CONFIG 11: Maximum output speed 50 MHz
  GPIOB->CRL |= GPIO_CRL_MODE1_0 | GPIO_CRL_MODE1_1 ;// CHIP SELECT MODE CONFIG 11: Maximum output speed 50 MHz
  GPIOA->CRL &= ~(GPIO_CRL_CNF6_0 | GPIO_CRL_CNF6_1);// D/C CNF CONFIG 00: GPIO Push-pull
  GPIOA->ODR |= GPIO_ODR_ODR6;// D/C SET TO HIGH
  GPIOB->CRL &= ~(GPIO_CRL_CNF0_0 | GPIO_CRL_CNF0_1);// RST CNF CONFIG 00: GPIO Push-pull
  GPIOB->ODR |= GPIO_ODR_ODR0;//RST SET TO HIGH
  GPIOB->CRL &= ~(GPIO_CRL_CNF1_0 | GPIO_CRL_CNF1_1);// CHIP SELECT CNF CONFIG 00: GPIO Push-pull
  GPIOB->ODR &= ~GPIO_ODR_ODR1;// CHIP SELECT SET TO LOW
  GPIOA->CRL &= ~GPIO_CRL_CNF7_0;//
  GPIOA->CRL |= GPIO_CRL_CNF7_1;// MOSI CNF CONFIG 10: AF Push-pull
  GPIOA->CRL &= ~GPIO_CRL_CNF5_0;//
  GPIOA->CRL |= GPIO_CRL_CNF5_1;// CLK CNF CONFIG 10: AF Push-pull
}

void SPI1Init(){
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;// SPI1 clock enable
  SPI1->CR1 &= ~SPI_CR1_BIDIMODE;
  SPI1->CR1 |= SPI_CR1_BIDIOE;
  SPI1->CR1 &= ~SPI_CR1_DFF;
  SPI1->CR1 &= ~SPI_CR1_RXONLY;
  SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
  SPI1->CR1 &= ~SPI_CR1_BR_0;
  SPI1->CR1 &= ~SPI_CR1_BR_1;
  SPI1->CR1 |= SPI_CR1_BR_2; // 100: fPCLK/32 72/32=2.25Mhz
  SPI1->CR1 |= SPI_CR1_SSM;
  SPI1->CR1 |= SPI_CR1_CPOL; // 1: CK to 1 when idle
  SPI1->CR1 |= SPI_CR1_CPHA;
  SPI1->CR1 |= SPI_CR1_MSTR;
  SPI1->CR1 |= SPI_CR1_SPE;
}