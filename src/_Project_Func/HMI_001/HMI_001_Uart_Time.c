#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//=================================================
#ifdef HMI_001
//=================================================

void HMI_Uart_Form_RS485_callback(struct usart_module *const usart_module)
{
	if(HMI_RS485_OK_Flag == true) return;
	switch(HMI_RS485_RX_Buf[0])
	{
		case 0x02:
		HMI_RS485_RX_Buf_Count = 0;
		HMI_RS485_RX_Buf[0] = 'S';
		break;
		
		case 0x03:
		HMI_RS485_OK_Flag = true;
		HMI_RS485_RX_Buf[0] = 'Z';
		break;
		
		default:
		break;
	}
	HMI_RS485_RX_In_Buf[HMI_RS485_RX_Buf_Count] = HMI_RS485_RX_Buf[0];
	HMI_RS485_RX_Buf_Count++;
}


void RS485_uart_str(const char *tmp_ptr)
{
	char *ptr;
	ptr = tmp_ptr;
	RS485_For_Debug_Dn
	uTXByte(RS485_COM, 0x02);
	while(*ptr != '\0')
	{
		uTXByte(RS485_COM, *ptr);
		ptr++;
	}
	uTXByte(RS485_COM, 0x03);
	RS485_For_Debug_En
	
	if(PLC_Show_r_Flag == true) uart_str("[Send]\0");
}

void debug_RS485_uart_str(const char *tmp_ptr)
{
	char *ptr;
	ptr = tmp_ptr;
	uTXByte(Debug_COM, 0x02);
	while(*ptr != '\0')
	{
		uTXByte(Debug_COM, *ptr);
		ptr++;
	}
	uTXByte(Debug_COM, 0x03);
	
	if(PLC_Show_r_Flag == true) uart_str("[Send]\0");
}

void tc_PLC_Catch(struct tc_module *const module_inst)
{
	usart_read_buffer_job(RS485_COM, (uint8_t *)HMI_RS485_RX_Buf, HMI_RS485_RX_D_Buf_Count);
}


//=================================================
#endif
//=================================================