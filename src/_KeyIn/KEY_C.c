#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


void Judg_KEY_C(void)
{
	volatile LORA_Dev_List_t *LORA_Dev_List_Index = LORA_Dev_List_In_RAM;
	
	if(ascii_to_hex(UserKeyInBuf[0]) != KEYC) return;
	
	if(ascii_to_hex(UserKeyInBuf[1]) == KEYL)
	{
		if(UserKeyInBufCount != 3 && UserKeyInBuf[2] != 0x0d) return;
		unsigned int i, j, k;
		k = 0;
		uart_str("          MAC   Fn CT  D1   D2   D3  V1 V2 V3 V4        St OK Cl\r\0");
		for(unsigned char i= 0; i < Dev_Item_MAX; i++)
		{
			if(LORA_Dev_List_Index[i].Dev_En_Flag == Dev_EN)
			{
				uTXByte(Debug_COM, 'N');
				uTXByte(Debug_COM, 'o');
				uTXByte(Debug_COM, '.');
				uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(LORA_Dev_List_Index[i].Dev_Number));
				uTXByte(Debug_COM, ' ');	
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf[0]);		//MAC
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf[1]);
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf[2]);
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf[3]);
				uTXByte(Debug_COM, ' ');
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf[4]);		//功能
				uTXByte(Debug_COM, ' ');
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf[5]);		//Counter
				uTXByte(Debug_COM, ' ');
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf[6]);		//資料1
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf[7]);
				uTXByte(Debug_COM, ' ');
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf[8]);		//資料2
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf[9]);
				uTXByte(Debug_COM, ' ');
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf[10]);	//資料3
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf[11]);
				uTXByte(Debug_COM, ' ');
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf[12]);
				uTXByte(Debug_COM, ' ');
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf[13]);
				uTXByte(Debug_COM, ' ');
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf[14]);
				uTXByte(Debug_COM, ' ');
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf[15]);
				uTXByte(Debug_COM, ' ');
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf[16]);
				uTXByte(Debug_COM, ' ');
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf[17]);
				uTXByte(Debug_COM, ' ');
				uTXByte(Debug_COM, ' ');
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf_Sent_Flag);
				uTXByte(Debug_COM, ' ');
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf_Return_OK_Flag);
				uTXByte(Debug_COM, ' ');
				uart_send_ascii_byte(Debug_COM, LORA_Dev_List_Index[i].cmd_buf_to_clear_mysql_Flag);
				
				uTXByte(Debug_COM, 0x0d);
			}
		}
	}
}

