/*
 * DeBug_callback.c
 *
 * Created: 2021/4/19 下午 03:24:04
 *  Author: willchen
 */ 
#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


/*
void usart_read_callback(struct usart_module *const usart_module)
{
	if(UserKeyInStauts == NOP)
	{
		//uTXByte(Debug_COM, user_rx_buffer[0]);
		UserKeyInBuf[UserKeyInBufCount] = user_rx_buffer[0];
		if(UserKeyInBufCount < UserKeyInBufMax)
		{
			UserKeyInBufCount++;
		}
		
		//if(user_rx_buffer[0] == 0x0d || user_rx_buffer[0] == 'z'  || user_rx_buffer[0] == 'Z' )
		if(user_rx_buffer[0] == 0x0d)
		{
			if(UserKeyInBufCount > 1)
			{
				UserKeyInStauts = UserKeyInEnterIndex;
			}
			else
			{
				UserKeyInBufCount = 0;
			}
		}
		else
		if(user_rx_buffer[0] == 0x08)
		{
			if(UserKeyInBufCount > 2)
			{
				UserKeyInBufCount--;
				UserKeyInBufCount--;
			}
			else
			{
				UserKeyInBufCount = 0;
			}
		}
		else
		if(user_rx_buffer[0] == 0x0A)
		{
			UserKeyInBufCount--;
		}
	}
}

*/
void usart_read_func(void)
{
	unsigned char i, k;
	if(UserKeyInStauts == UserKeyInEnterIndex) return;
	
	for(k = 1; k < uart_buffer_max; k++)
	{
		if(STATUS_OK == usart_read_buffer_job(Debug_COM, &tmp_uart_buf, k))
		{
			for(i = 0; i < k; i++)
			{
				switch(tmp_uart_buf[i])
				{
					case 0x0d:	//換行
					if(UserKeyInBufCount > 0)
					{
						UserKeyInStauts = UserKeyInEnterIndex;
						UserKeyInBuf[UserKeyInBufCount] = 0x0d;
					}
					else
					{
						UserKeyInBufCount = 0;
					}
					break;
					
					case 0x08:	//backspace
					if(UserKeyInBufCount > 2)
					{
						UserKeyInBufCount--;
						UserKeyInBufCount--;
					}
					else
					{
						UserKeyInBufCount = 0;
					}
					break;
					case 0x00:
					break;
					
					default:
					if(UserKeyInBufCount < UserKeyInBufMax)
					{
						UserKeyInBuf[UserKeyInBufCount] = tmp_uart_buf[i];
						UserKeyInBufCount++;
					}
					break;
				}
				
			}
			return;
		}
	}
}