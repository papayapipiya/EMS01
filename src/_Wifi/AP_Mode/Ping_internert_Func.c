/*
 * Ping_internert_Func.c
 *
 * Created: 2023/10/4 上午 09:18:19
 *  Author: papay
 */ 
#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "myDefine.h"

void Ping_internert_Func(void)
{
	int ret;
	int ret_end;
	unsigned int i, k;
	char str[100];


	
	if(mGateway.Wifi->flag.f_ok		== false)	return;
	if(mGateway.Wifi->flag.f_busy	== true)	return;

	for(i = 0; i < Ping_Buffer_Max; i++)		mGateway.Wifi->Ping_Buffer[i] = '\0';
	uart_str_COM(Debug_COM, "\r==== Ping Time ====");
	
	ret		= findKeyword(mGateway.Wifi->Input_buffer, "\r\n+");
	ret		= ret + 3;
	ret_end	= findKeyword(mGateway.Wifi->Input_buffer, "\r\n\r\nOK");
	sprintf(str, "\r --> start: %02d ,end: %02d \r", ret, ret_end);
	uart_str_COM(Debug_COM, str);
	
	if(ret_end < ret)
	{
		sprintf(mGateway.Wifi->Ping_Buffer, "%s", "---");
	}
	else
	{
		k = 0;
		for(i = ret; i < ret_end; i++)
		{
			mGateway.Wifi->Ping_Buffer[k] = mGateway.Wifi->Input_buffer[i];
			k++;
		}
	}
	sprintf(str, " [%s] ms", mGateway.Wifi->Ping_Buffer);
	uart_str_COM(Debug_COM, str);
	uart_str_COM(Debug_COM, "\r==== Ping Time End ====\r");
}