#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


void Set_Lora_User_ID_Func(volatile LORA_DEFINE_t *tmp_LORA)
{
	UINT8u_t tmp8;
	tmp8.HL.H = ascii_to_hex(UserKeyInBuf[4]);
	tmp8.HL.L = ascii_to_hex(UserKeyInBuf[5]);
	tmp_LORA->MyLoRaID[0] = tmp8.byte;
	tmp8.HL.H = ascii_to_hex(UserKeyInBuf[6]);
	tmp8.HL.L = ascii_to_hex(UserKeyInBuf[7]);
	tmp_LORA->MyLoRaID[1] = tmp8.byte;
	tmp8.HL.H = ascii_to_hex(UserKeyInBuf[8]);
	tmp8.HL.L = ascii_to_hex(UserKeyInBuf[9]);
	tmp_LORA->MyLoRaID[2] = tmp8.byte;
	tmp8.HL.H = ascii_to_hex(UserKeyInBuf[10]);
	tmp8.HL.L = ascii_to_hex(UserKeyInBuf[11]);
	tmp_LORA->MyLoRaID[3] = tmp8.byte;
}

void Set_Lora_FQ_Func(volatile LORA_DEFINE_t *tmp_LORA)
{
	UINT32u_t tmp_FQ;
	tmp_FQ.dword =  (ascii_to_hex(UserKeyInBuf[4]) * 10000);
	tmp_FQ.dword += (ascii_to_hex(UserKeyInBuf[5]) * 1000);
	tmp_FQ.dword += (ascii_to_hex(UserKeyInBuf[6]) * 100);
	tmp_FQ.dword += (ascii_to_hex(UserKeyInBuf[7]) * 10);
	tmp_FQ.dword +=  ascii_to_hex(UserKeyInBuf[8]);
	uart_send_ascii_byte(Debug_COM, tmp_FQ.byte[2]);
	uart_send_ascii_byte(Debug_COM, tmp_FQ.byte[1]);
	uart_send_ascii_byte(Debug_COM, tmp_FQ.byte[0]);
	tmp_LORA->MyLoRaFQ.dword = tmp_FQ.dword;
	tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][4] = tmp_FQ.byte[2];
	tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][5] = tmp_FQ.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][6] = tmp_FQ.byte[0];
	tmp_LORA->NowStatus = Write_LoRa_Setting_Index;
}

void Set_Lora_Power_Func(volatile LORA_DEFINE_t *tmp_LORA)
{
	unsigned char tmp;
	tmp = ((ascii_to_hex(UserKeyInBuf[4]) * 10) + ascii_to_hex(UserKeyInBuf[5]) - 2);
	if(tmp > 15) tmp = 15;
	tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7] = tmp;
	tmp_LORA->MyLoRaPower = tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7];
	tmp_LORA->NowStatus = Write_LoRa_Setting_Index;
}

void Set_Lora_SF_Func(volatile LORA_DEFINE_t *tmp_LORA)
{
	unsigned char tmp;
	tmp = ((ascii_to_hex(UserKeyInBuf[4]) * 10) + ascii_to_hex(UserKeyInBuf[5]) - 5);
	if(tmp < 2)	tmp = 2;
	if(tmp > 7) tmp = 7;
	tmp_LORA->MyLoRaTmp[Write_LoRa_Setting_Index][5] = tmp;
	tmp_LORA->MyLoRaSF = tmp_LORA->MyLoRaTmp[Write_LoRa_Setting_Index][5];
	tmp_LORA->NowStatus = Write_LoRa_Setting_Index;
}

void Print_Lora_FW_ID_Func(volatile LORA_DEFINE_t *tmp_LORA)
{
	Print_Lora_number(tmp_LORA);
	uTXByte(Debug_COM, 0x0d);
	Print_Line_Msg();
	Print_Catch_LoRa_ID_Msg(tmp_LORA);
	uTXByte(Debug_COM, ' ');
	uTXByte(Debug_COM, ' ');
	uTXByte(Debug_COM, ' ');
	Load_LoRa_Profile_in_NVM(tmp_LORA);
	Print_Catch_LoRa_Setting_Msg(tmp_LORA);
}

void Judg_KEY_S(void)
{
	if(ascii_to_hex(UserKeyInBuf[0]) == KEYS)
	{
		switch(ascii_to_hex(UserKeyInBuf[1]))
		{
			case KEY1:	//Print LORA01 PROFILE
			if(UserKeyInBufCount == 2 && UserKeyInBuf[2] == 0x0d)
			{
				Print_Lora_FW_ID_Func(LORA01);
			}
			break;
			
			case KEY2:	//Print LORA02 PROFILE
			if(UserKeyInBufCount == 2 && UserKeyInBuf[2] == 0x0d)
			{
				Print_Lora_FW_ID_Func(LORA02);
			}
			break;
			
			case KEYS:	//SETTING LORA1, LORA2 SF
			if(UserKeyInBufCount == 6 && UserKeyInBuf[6] == 0x0d)
			{
				if(ascii_to_hex(UserKeyInBuf[2]) == KEY1)		{	Set_Lora_SF_Func(LORA01);	CMD_Save_Func(LORA01);}
				else if(ascii_to_hex(UserKeyInBuf[2]) == KEY2)	{	Set_Lora_SF_Func(LORA02);	CMD_Save_Func(LORA02);}
			}
			break;
			
			case KEYP:	//SETTING LORA1, LORA2 POWER
			if(UserKeyInBufCount == 6 && UserKeyInBuf[6] == 0x0d)
			{
				if(ascii_to_hex(UserKeyInBuf[2]) == KEY1)		{	Set_Lora_Power_Func(LORA01);	CMD_Save_Func(LORA01);}
				else if(ascii_to_hex(UserKeyInBuf[2]) == KEY2)	{	Set_Lora_Power_Func(LORA02);	CMD_Save_Func(LORA02);}
			}
			break;
			
			case KEYF:	//SETTING LORA1, LORA2 FQ
			if(UserKeyInBufCount == 9 && UserKeyInBuf[9] == 0x0d)
			{
				if(ascii_to_hex(UserKeyInBuf[2]) == KEY1)		{	Set_Lora_FQ_Func(LORA01);	CMD_Save_Func(LORA01);}
				else if(ascii_to_hex(UserKeyInBuf[2]) == KEY2)	{	Set_Lora_FQ_Func(LORA02);	CMD_Save_Func(LORA02);}
			}
			break;
			
			case KEYI:	//SETTING LORA1, LORA2 MAC ID
			if(UserKeyInBufCount == 12 && UserKeyInBuf[12] == 0x0d)
			{
				if(ascii_to_hex(UserKeyInBuf[2]) == KEY1)		{	Set_Lora_User_ID_Func(LORA01);	CMD_Save_Func(LORA01);}
				else if(ascii_to_hex(UserKeyInBuf[2]) == KEY2)	{	Set_Lora_User_ID_Func(LORA02);	CMD_Save_Func(LORA02);}
			}
			break;
		}
	}
}