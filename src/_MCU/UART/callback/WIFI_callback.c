/*
 * WIFI_callback.c
 *
 * Created: 2021/4/19 下午 03:36:27
 *  Author: willchen
 */ 


#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

#define			wifi_uart_buffer_max	2048
unsigned char	wifi_tmp_uart_buf[wifi_uart_buffer_max];


void Uart_Form_WIFI_callback(struct usart_module *const usart_module)
{
	char str[50];
	unsigned int k, i;
	

	for(k = 1; k < uart_buffer_max; k++)
	{
		if(STATUS_OK == usart_read_buffer_job(Wifi_COM, &tmp_uart_buf, k))
		{
			for(i = 0; i < k; i++)
			{
				if(My_WiFi.Rx_buffer_Count < WIFI_Input_buffer_Max)
				{
					My_WiFi.Rx_Timeout_Count = 0;
					My_WiFi.Input_buffer[My_WiFi.Rx_buffer_Count] = tmp_uart_buf[i];
					My_WiFi.Rx_buffer_Count++;
					My_WiFi.new_data = true;
				}
			}
			//如果有接收到新的資料，不斷清除Time Out
			My_WiFi.f_count = 0;
			return;
		}
	}
	
	
}
