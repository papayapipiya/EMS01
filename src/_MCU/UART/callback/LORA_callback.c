/*
 * LORA01_callback.c
 *
 * Created: 2021/4/19 下午 03:19:02
 *  Author: willchen
 */ 

#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

void Uart_Form_LORA01_callback(struct usart_module *const usart_module)
{
	if(LORA01->MyBufferCount <= LORA01->UserRxCountMax)
	{
		LORA01->MyBuffer[LORA01->MyBufferCount] =  LORA01->rx_buffer[0];
		LORA01->MyBufferCount = ++LORA01->MyBufferCount;
	}
	LORA01->f_count = 0;
}

void Uart_Form_LORA02_callback(struct usart_module *const usart_module)
{

	if(LORA02->MyBufferCount <= LORA02->UserRxCountMax)
	{
		LORA02->MyBuffer[LORA02->MyBufferCount] = LORA02->rx_buffer[0];
		LORA02->MyBufferCount =  ++LORA02->MyBufferCount;
	}
	LORA02->f_count = 0;
}

