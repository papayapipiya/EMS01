#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


void Print_Debug_Msg(void)
{
	uTXByte(Debug_COM, 'D');
	uTXByte(Debug_COM, 'e');
	uTXByte(Debug_COM, 'B');
	uTXByte(Debug_COM, 'u');
	uTXByte(Debug_COM, 'g');
	uTXByte(Debug_COM, ' ');	
}

void Print_Debug_Mode_ON_Msg(void)
{
	Print_Debug_Msg();
	uTXByte(Debug_COM, 'O');
	uTXByte(Debug_COM, 'N');
}

void Print_Debug_Mode_OFF_Msg(void)
{
	Print_Debug_Msg();
	uTXByte(Debug_COM, 'O');
	uTXByte(Debug_COM, 'F');
	uTXByte(Debug_COM, 'F');
}

void Debug_Mode_Func(volatile LORA_DEFINE_t *tmp_LORA)
{
	Print_Lora_number(tmp_LORA);
	if(tmp_LORA->Lora_Debug_Flag == NeedDebug)
	{
		tmp_LORA->Lora_Debug_Flag = NoDebug;
		Print_Debug_Mode_OFF_Msg();
	}
	else
	{
		tmp_LORA->Lora_Debug_Flag = NeedDebug;
		Print_Debug_Mode_ON_Msg();
	}
	uTXByte(Debug_COM, 0x0d);
}

void Judg_KEY_D(void)
{
	// Example: D1←
	//  D  1  ←
	// [0][1][2]
	// UserKeyInBufCount = 3
	// ← = 0x0d
	if(ascii_to_hex(UserKeyInBuf[0]) == KEYD)
	{
		if(UserKeyInBufCount == 2 && UserKeyInBuf[2] == 0x0d)
		{
			if(ascii_to_hex(UserKeyInBuf[1]) == KEY1)		{	Debug_Mode_Func(LORA01);	CMD_Save_Func(LORA01);}
			else if(ascii_to_hex(UserKeyInBuf[1]) == KEY2)	{	Debug_Mode_Func(LORA02);	CMD_Save_Func(LORA02);}
		}
	}
}
