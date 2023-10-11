/*
 * Dust_callback.c
 *
 * Created: 2021/4/19 下午 03:21:03
 *  Author: willchen
 */ 

#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


void Uart_Form_Dust_callback(struct usart_module *const usart_module)
{
	if(Dust_RX_Count > Dust_RX_Count_Max)
	{
		return;
	}
	Dust_Time_Count = 0;
	if(Dust_buffer[0] == 0xaa)
	{
		Dust_RX_Count = 0;
		Dust_Data[Dust_RX_Count] = Dust_buffer[0];
	}
	else
	{
		Dust_RX_Count++;
		Dust_Data[Dust_RX_Count] = Dust_buffer[0];
		if(Dust_RX_Count == Dust_RX_Count_Max)
		{
			if(Dust_buffer[0] != 0xff)
			{
				Dust_RX_Count = 0;
			}
		}
	}
}
