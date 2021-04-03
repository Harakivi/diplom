#ifndef _LCD5110_
#define _LCD5110_
//------------------------------------------------
#define DC_HIGH GPIOA->ODR |= GPIO_ODR_ODR6 /// DATA/COMMAND SET
#define DC_LOW GPIOA->ODR &= ~GPIO_ODR_ODR6 /// DATA/COMMAND RESET
#define RST_HIGH GPIOB->ODR |= GPIO_ODR_ODR0 /// RST SET
#define RST_LOW GPIOB->ODR &= ~GPIO_ODR_ODR0 /// RST RESET
#define CS_HIGH GPIOB->ODR |= GPIO_ODR_ODR1 /// CHIP SELECT SET
#define CS_LOW GPIOB->ODR &= ~GPIO_ODR_ODR1 /// CHIP SELECT RESET
//------------------------------------------------
#define BLACK 1 ///< Black pixel
#define WHITE 0 ///< White pixel
//------------------------------------------------
#define LCDWIDTH 84   ///< LCD is 84 pixels wide
#define LCDHEIGHT 48  ///< 48 pixels high
//------------------------------------------------
#define PCD8544_POWERDOWN 0x04            ///< Function set, Power down mode
#define PCD8544_ENTRYMODE 0x02            ///< Function set, Entry mode
#define PCD8544_EXTENDEDINSTRUCTION 0x01  ///< Function set, Extended instruction set control
//------------------------------------------------
#define PCD8544_DISPLAYBLANK 0x0      ///< Display control, blank
#define PCD8544_DISPLAYNORMAL 0x4     ///< Display control, normal mode
#define PCD8544_DISPLAYALLON 0x1      ///< Display control, all segments on
#define PCD8544_DISPLAYINVERTED 0x5   ///< Display control, inverse mode
//------------------------------------------------
#define PCD8544_FUNCTIONSET 0x20      ///< Basic instruction set
#define PCD8544_DISPLAYCONTROL 0x08   ///< Basic instruction set - Set display configuration
#define PCD8544_SETYADDR 0x40         ///< Basic instruction set - Set Y address of RAM, 0 <= Y <= 5
#define PCD8544_SETXADDR 0x80         ///< Basic instruction set - Set X address of RAM, 0 <= X <= 83
//------------------------------------------------
#define PCD8544_SETTEMP 0x04          ///< Extended instruction set - Set temperature coefficient
#define PCD8544_SETBIAS 0x10          ///< Extended instruction set - Set bias system
#define PCD8544_SETVOP 0x80           ///< Extended instruction set - Write Vop to register
//------------------------------------------------
void LCDInit();
void lcd5110_command(int data);
void lcd5110_data(int data);
void lcd5110_setBias(int val);
void lcd5110_setContrast(int val);
void gpioInit();
void SPI1Init();
//------------------------------------------------
#endif /* _LCD5110_ */