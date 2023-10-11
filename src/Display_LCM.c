#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

void lcd_4bit(unsigned char rs, unsigned char tmp)
{
	UINT8u_t user_data;
	user_data.byte = tmp;
	if(rs == 1)					LCM_RS_High		else	LCM_RS_Low
	LCM_RW_Low
	if(user_data.bit.b0 == 1)	LCM_04_High		else	LCM_04_Low
	if(user_data.bit.b1 == 1)	LCM_05_High		else	LCM_05_Low
	if(user_data.bit.b2 == 1)	LCM_06_High		else	LCM_06_Low
	if(user_data.bit.b3 == 1)	LCM_07_High		else	LCM_07_Low
	LCM_EN_High		
	delay_us(15);			//等待寫入
	LCM_EN_Low
}

void lcd_command(unsigned char rs, unsigned char tmp)
{
	UINT8u_t	data;
	data.byte = tmp;
	lcd_4bit(rs, data.HL.H);
	lcd_4bit(rs, data.HL.L);
}

void lcd_init(void)
{
	lcd_4bit(lcd_instr, 0x03);
	lcd_4bit(lcd_instr, 0x03);
	lcd_4bit(lcd_instr, 0x03);
	lcd_4bit(lcd_instr, 0x02);

	lcd_command(lcd_instr, 0x28);
	lcd_command(lcd_instr, 0x08);

	lcd_command(lcd_instr, 0x06);
	lcd_command(lcd_instr, 0x0c);
}