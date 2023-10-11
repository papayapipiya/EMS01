#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//=================================================
#ifdef Dev_Repeater
//=================================================


//uint8_t MyRS485RxCount[2] ={7, 8};
uint8_t MyRS485RxCount[2] ={19, 8};
	
volatile uint8_t Define_MyRS485Tmp[2][8] =
{
	//{0x01, 0x03, 0x17, 0xD4, 0x00, 0x01, 0x00, 0x00},	//讀取D暫存器
	{0x01, 0x03, 0x17, 0xD4, 0x00, 0x07, 0x00, 0x00},	//讀取D暫存器
	{0x01, 0x06, 0x17, 0xD4, 0x00, 0xff, 0x00, 0x00}	//寫入D暫存器
};

/*
	WriteBufferHex[0] ~ [3] : MAC ID
	WriteBufferHex[4]: CMD
	WriteBufferHex[5]: Length
	WriteBufferHex[16] ~ [17]: 0x3333
*/


unsigned int Index_To_Addr(unsigned int tmpIndex)
{
	return (PLC_D_Register_Offset + tmpIndex);
}

void Print_PLC_D_Register_Number(unsigned int tmpIndex)
{
	uTXByte(Debug_COM, 'D');
	uart_send_word(Debug_COM, hex_to_bcd(tmpIndex + PLC_D_Register_Number_Offset));
	uTXByte(Debug_COM, ' ');
}

void Uart_Form_RS485_callback(struct usart_module *const usart_module)
{
	RS485_RX_In_Buf[RS485_RX_Buf_Count] = RS485_RX_Buf[0];
	RS485_RX_Buf_Count ++;
	RS485_Time_Count = 0;
}

unsigned int crc_chk(unsigned char* data, unsigned char length)
{
	int j;
	unsigned int reg_crc = 0xFFFF;
	while(length--)
	{
		reg_crc ^= *data++;
		for(j = 0; j < 8 ; j++)
		{
			if(reg_crc & 0x01) /* LSB(b0)=1 */
			reg_crc = (reg_crc >> 1) ^ 0xA001;
			else
			reg_crc = reg_crc >> 1;
		}
	}
	return reg_crc;
}


void Print_Data_Buf_Function(unsigned char *tmp_ptr, unsigned char tmp_count)
{
	char *ptr;
	ptr = tmp_ptr;
	for(unsigned char j = 0; j < tmp_count; j++)
	{
		uart_send_ascii_byte(Debug_COM, *(ptr+j));
		uTXByte(Debug_COM, ' ');
	}
	uTXByte(Debug_COM, ' ');
	uTXByte(Debug_COM, ' ');
}



void Print_Will_Send_Data_To_PLC_Function(unsigned char tmp_CMD_Index)
{
	for(unsigned char j = 0; j < 8; j++)
	{
		uart_send_ascii_byte(Debug_COM, Define_MyRS485Tmp[tmp_CMD_Index][j]);
		uTXByte(Debug_COM, ' ');
	}
	uTXByte(Debug_COM, ' ');uTXByte(Debug_COM, ' ');uTXByte(Debug_COM, ' ');
}


//=================================================
#endif
//=================================================