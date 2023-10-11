/*
 * Key_Wifi_APMode.c
 *
 * Created: 2021/4/19 下午 01:47:41
 *  Author: willchen
 */ 

#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

void Judg_Key_Wifi_APMode(void)
{
	if(ascii_to_hex(UserKeyInBuf[0]) != KEYW) return;
	if(ascii_to_hex(UserKeyInBuf[1]) != KEYA) return;
	if(ascii_to_hex(UserKeyInBuf[2]) != KEYP) return;
	
	My_WiFi.WIFI_Main_Counter	= 0;
	My_WiFi.NowStatus			= WiFi_NOP;
	My_WiFi.RunStep				= Step_Initial_AT;

	
	switch(ascii_to_hex(UserKeyInBuf[3]))
	{
		case KEY0:
			My_WiFi.AP_Mode_Switch = Is_AP_Mode;
			uart_str_COM(Debug_COM, " --> Wifi To AP Mode");
			break;
		
		case KEY1:
			My_WiFi.AP_Mode_Switch = Is_Station_Mode;
			uart_str_COM(Debug_COM, " --> Wifi To STA Mode");
			break;
		
	}
}