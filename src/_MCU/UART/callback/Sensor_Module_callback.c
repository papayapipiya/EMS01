/*
 * Sensor_Module_callback.c
 *
 * Created: 2021/4/19 下午 03:22:49
 *  Author: willchen
 */ 

#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



void Uart_Form_Sensor_Module_callback(struct usart_module *const usart_module)
{
	if(Sensor_Module_RX_Count > Sensor_Module_RX_Count_Max)
	{
		return;
	}
	//Sensor_Module_Time_Count = Sensor_Module_TimeOut_Max;
	if(Sensor_Module_buffer[0] == 'S')
	{
		Sensor_Module_RX_Count = 0;
		Sensor_Module_Data[Sensor_Module_RX_Count] = Sensor_Module_buffer[0];
	}
	else
	{
		Sensor_Module_RX_Count++;
		Sensor_Module_Data[Sensor_Module_RX_Count] = Sensor_Module_buffer[0];
		if(Sensor_Module_RX_Count == Sensor_Module_RX_Count_Max)
		{
			if(Sensor_Module_buffer[0] != 0x0d)
			{
				Sensor_Module_RX_Count = 0;
			}
		}
	}
}