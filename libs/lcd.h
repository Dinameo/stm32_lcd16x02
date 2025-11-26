#ifndef __LCD_H
#define __LCD_H

#include "main.h"


#define LCD_RT_HOME				0x02
#define LCD_CLR_DISP			0x01

#define LCD_FC_SET_BASE			0x20	// 4bit, 1 line, 5x8
#define LCD_FC_SET_DL8BIT		0x10
#define LCD_FC_SET_1x5x10		0x04
#define LCD_FC_SET_2x5x8		0x08



#define LCD_ENTR_MD_SET_BASE	0x04	// Decrements, No shift
#define LCD_ENTR_MD_SET_INCR	0x02
#define LCD_ENTR_MD_SET_SHIFT	0x01


#define LCD_DISP_CTRL_BASE		0x08	// Display off, not cursor, not blink
#define LCD_DISP_CTRL_ON		0x04
#define LCD_DISP_CTRL_CURSOR	0x02
#define LCD_DISP_CTRL_BLINK		0x01

#define LCD_CGRAM_ADDR_BASE		0x40
#define LCD_DDRAM_ADDR_BASE		0x80
#define LCD_DDRAM_ROW2_OFFS		0x40


typedef struct
{
	GPIO_TypeDef*	port;
	uint16_t		pin;
} LCD_Pin_t;


typedef enum
{
	LCD_D0_IDX	= 0,
	LCD_D1_IDX	= 1,
	LCD_D2_IDX	= 2,
	LCD_D3_IDX	= 3,
	LCD_D4_IDX	= 4,
	LCD_D5_IDX	= 5,
	LCD_D6_IDX	= 6,
	LCD_D7_IDX	= 7,
	LCD_EN_IDX	= 8,
	LCD_RS_IDX	= 9,
	LCD_RW_IDX	= 10,
	LCD_SIZE 	= 11
} LCD_IDX_t;

typedef enum
{
	LCD_MODE_4BIT = 0,
	LCD_MODE_8BIT = 1
} LCD_Mode_t;
typedef struct
{
	LCD_Pin_t pins[LCD_SIZE];
	LCD_Mode_t mode;
} LCD_t;

void DWT_Init(void);
void DWT_Delay_us(uint32_t us);


void LCD_Clear_Display(LCD_t* lcd);
void LCD_Return_Home(LCD_t* lcd);
void LCD_Enable(LCD_t* lcd);
void LCD_Send_4bit(LCD_t* lcd, uint8_t nibb);
void LCD_Send_Byte(LCD_t* lcd, uint8_t byte);
void LCD_Put(LCD_t* lcd, uint8_t byte);
void LCD_Put_Char(LCD_t* lcd, uint8_t ch);
void LCD_Put_String(LCD_t* lcd, uint8_t* str);
void LCD_Put_Command(LCD_t* lcd, uint8_t cmd);
void LCD_Init_4bit(LCD_t* lcd);
void LCD_Init_8bit(LCD_t* lcd);
void LCD_Init(LCD_t* lcd);
void LCD_Goto_XY(LCD_t* lcd, uint8_t x, uint8_t y);
void LCD_Put_Int(LCD_t* lcd, int32_t num);
void LCD_Put_Float(LCD_t* lcd, float num, uint8_t prec);

#endif
