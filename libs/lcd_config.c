#include "main.h"
#include "lcd.h"
#include "lcd_config.h"


LCD_t lcd =
{
	.pins[LCD_EN_IDX] = {LCD_EN_GPIO_Port, LCD_EN_Pin},
	.pins[LCD_RS_IDX] = {LCD_RS_GPIO_Port, LCD_RS_Pin},
	.pins[LCD_RW_IDX] = {LCD_RW_GPIO_Port, LCD_RW_Pin},

	.pins[LCD_D4_IDX] = {LCD_D4_GPIO_Port, LCD_D4_Pin},
	.pins[LCD_D5_IDX] = {LCD_D5_GPIO_Port, LCD_D5_Pin},
	.pins[LCD_D6_IDX] = {LCD_D6_GPIO_Port, LCD_D6_Pin},
	.pins[LCD_D7_IDX] = {LCD_D7_GPIO_Port, LCD_D7_Pin},

	.mode = LCD_MODE_4BIT
};
