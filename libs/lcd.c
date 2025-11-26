#include "main.h"
#include "lcd.h"



void DWT_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}
void DWT_Delay_us(uint32_t us)
{
    uint32_t cycles = (SystemCoreClock / 1000000L) * us;
    uint32_t start = DWT->CYCCNT;
    while ((DWT->CYCCNT - start) < cycles);
}

void LCD_Enable(LCD_t* lcd)
{
	HAL_GPIO_WritePin(lcd->pins[LCD_EN_IDX].port, lcd->pins[LCD_EN_IDX].pin, GPIO_PIN_SET);
	DWT_Delay_us(1);	// min 450ns
	HAL_GPIO_WritePin(lcd->pins[LCD_EN_IDX].port, lcd->pins[LCD_EN_IDX].pin, GPIO_PIN_RESET);
	DWT_Delay_us(40);	// min 37us
}

void LCD_Send_4bit(LCD_t* lcd, uint8_t nibb)
{
	for(uint8_t i = 0; i < 4; i++)
		HAL_GPIO_WritePin(lcd->pins[LCD_D4_IDX + i].port, lcd->pins[LCD_D4_IDX + i].pin, (GPIO_PinState) ((nibb >> i) & 0x1));
	LCD_Enable(lcd);
}
void LCD_Send_Byte(LCD_t* lcd, uint8_t byte)
{
	for(uint8_t i = 0; i < 8; i++)
		HAL_GPIO_WritePin(lcd->pins[LCD_D0_IDX + i].port, lcd->pins[LCD_D0_IDX + i].pin, (GPIO_PinState) ((byte >> i) & 0x1));
	LCD_Enable(lcd);
}

void LCD_Put(LCD_t* lcd, uint8_t byte)
{
	if(lcd->mode == LCD_MODE_4BIT)
	{
		LCD_Send_4bit(lcd, (byte >> 4) & 0xF);
		LCD_Send_4bit(lcd, byte & 0xF);
	}
	else if(lcd->mode == LCD_MODE_8BIT)
		LCD_Send_Byte(lcd, byte);

	if(byte == LCD_RT_HOME || byte == LCD_CLR_DISP)
		HAL_Delay(2);
}

void LCD_Put_Char(LCD_t* lcd, uint8_t ch)
{
	HAL_GPIO_WritePin(lcd->pins[LCD_RS_IDX].port, lcd->pins[LCD_RS_IDX].pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(lcd->pins[LCD_RW_IDX].port, lcd->pins[LCD_RW_IDX].pin, GPIO_PIN_RESET);

	LCD_Put(lcd, ch);
}

void LCD_Put_String(LCD_t* lcd, uint8_t* str)
{
	while(*str != 0)
	{
		LCD_Put_Char(lcd, *str);
		str++;
	}
}

void LCD_Put_Int(LCD_t* lcd, int32_t num)
{
	uint8_t str[10] = {0};

	int8_t i = 0;
	if(num < 0)
	{
		LCD_Put_Char(lcd, '-');
		num = -num;
	}

	do
	{
		str[i++] = num%10 + '0';
		num /= 10;
	}
	while(num != 0);


	while(i != 0)
	{
		i--;
		LCD_Put_Char(lcd, str[i]);
	}
}

void LCD_Put_Float(LCD_t* lcd, float num, uint8_t prec)
{
	if(num < 0)
	{
		LCD_Put_Char(lcd, '-');
		num = -num;
	}
	int32_t frac_multiplier = 1;
	for(int8_t i = 0; i < prec; i++)
		frac_multiplier *= 10;
	num = (num + 0.5f/frac_multiplier);
	uint32_t int_part = (uint32_t) num;
	LCD_Put_Int(lcd, int_part);
	if (prec == 0) return;

	LCD_Put_Char(lcd, '.');
	int32_t frac_part = (num - int_part)*frac_multiplier;
	for(int8_t i = 0; i < prec; i++)
	{
		frac_multiplier /= 10;
		uint8_t digit = frac_part / frac_multiplier;
		LCD_Put_Char(lcd, digit + '0');
		frac_part %= frac_multiplier;

	}
}

void LCD_Put_Command(LCD_t* lcd, uint8_t cmd)
{
	HAL_GPIO_WritePin(lcd->pins[LCD_RS_IDX].port, lcd->pins[LCD_RS_IDX].pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->pins[LCD_RW_IDX].port, lcd->pins[LCD_RW_IDX].pin, GPIO_PIN_RESET);

	LCD_Put(lcd, cmd);
}
void LCD_Clear_Display(LCD_t* lcd)
{
	LCD_Put_Command(lcd, LCD_CLR_DISP);
}
void LCD_Return_Home(LCD_t* lcd)
{
	LCD_Put_Command(lcd, LCD_RT_HOME);
}
void LCD_Init_4bit(LCD_t* lcd)
{
	HAL_Delay(40);

	LCD_Send_4bit(lcd, 0x3);
	HAL_Delay(5);
	LCD_Send_4bit(lcd, 0x3);
	DWT_Delay_us(100);
	LCD_Send_4bit(lcd, 0x3);
	LCD_Send_4bit(lcd, 0x2);

	LCD_Put_Command(lcd, LCD_FC_SET_BASE | LCD_FC_SET_2x5x8);
	LCD_Put_Command(lcd, LCD_DISP_CTRL_BASE);
	LCD_Put_Command(lcd, LCD_CLR_DISP);
	LCD_Put_Command(lcd, LCD_ENTR_MD_SET_BASE | LCD_ENTR_MD_SET_INCR);
	LCD_Put_Command(lcd, LCD_DISP_CTRL_BASE | LCD_DISP_CTRL_ON);
}
void LCD_Init_8bit(LCD_t* lcd)
{
	HAL_Delay(40);

	LCD_Put_Command(lcd, 0x30);
	HAL_Delay(5);
	LCD_Put_Command(lcd, 0x30);
	DWT_Delay_us(100);
	LCD_Put_Command(lcd, 0x30);

	LCD_Put_Command(lcd, LCD_FC_SET_BASE | LCD_FC_SET_DL8BIT);
	LCD_Put_Command(lcd, LCD_DISP_CTRL_BASE);
	LCD_Put_Command(lcd, LCD_CLR_DISP);
	LCD_Put_Command(lcd, LCD_ENTR_MD_SET_BASE | LCD_ENTR_MD_SET_INCR);
	LCD_Put_Command(lcd, LCD_DISP_CTRL_BASE | LCD_DISP_CTRL_ON);
}

void LCD_Goto_XY(LCD_t* lcd, uint8_t x, uint8_t y)
{
	if(y == 0)
		LCD_Put_Command(lcd, LCD_DDRAM_ADDR_BASE | x);
	else
		LCD_Put_Command(lcd, LCD_DDRAM_ADDR_BASE | LCD_DDRAM_ROW2_OFFS | x);
}

void LCD_Init(LCD_t* lcd)
{
	DWT_Init();
	if(lcd->mode == LCD_MODE_4BIT)
		LCD_Init_4bit(lcd);
	else if(lcd->mode == LCD_MODE_8BIT)
		LCD_Init_8bit(lcd);
}
