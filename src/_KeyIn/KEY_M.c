#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

//=================================================
#ifdef Dev_Sensor
//=================================================


void Print_mem_list(void)
{
	unsigned int i, j, k;
	k = 0;
	for(i = 0 ; i < PLC_D_Buf_Max; i = i + PLC_D_Buf_Print_Columns_Max)
	{
		uart_str("No.\0");
		uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(k));
		uart_str("   \0");
		for(j = 0 ; j < PLC_D_Buf_Print_Columns_Max; j++)
		{
			uTXByte(Debug_COM, '[');
			uart_send_word(Debug_COM, hex_to_bcd( i + j ));
			uTXByte(Debug_COM, ']');
			uTXByte(Debug_COM, ' ');
			uTXByte(Debug_COM, 'N');
			uTXByte(Debug_COM, ':');
			uart_send_ascii_byte(Debug_COM, PLC_D_Buf[i + j].DataBuf.byte[1]);
			uart_send_ascii_byte(Debug_COM, PLC_D_Buf[i + j].DataBuf.byte[0]);
			uTXByte(Debug_COM, ' ');
			uTXByte(Debug_COM, 'L');
			uTXByte(Debug_COM, ':');
			uart_send_ascii_byte(Debug_COM, PLC_D_Buf[i + j].LastDataBuf.byte[1]);
			uart_send_ascii_byte(Debug_COM, PLC_D_Buf[i + j].LastDataBuf.byte[0]);
			uTXByte(Debug_COM, ',');
			uTXByte(Debug_COM, ' ');
		}
		uTXByte(Debug_COM, 0x0d);
		k++;
	}
}

void Judg_KEY_M(void)
{
	if(ascii_to_hex(UserKeyInBuf[0]) != KEYM) return;
	
	if(ascii_to_hex(UserKeyInBuf[1]) == KEYL)
	{
		if(UserKeyInBufCount != 3 && UserKeyInBuf[2] != 0x0d) return;
		Print_mem_list();
	}
	else
	if(ascii_to_hex(UserKeyInBuf[1]) == KEYS)
	{
		if(UserKeyInBufCount != 12 && UserKeyInBuf[11] != 0x0d) return;
		unsigned int tmp_index;
		unsigned int tmp_value;
		tmp_index =	Cal_list_table_index(UserKeyInBuf[3], UserKeyInBuf[4], UserKeyInBuf[5]);
		uart_str("No.\0");
		uart_send_word(Debug_COM, hex_to_bcd(tmp_index));
		uart_str("   \0");
		uart_send_word(Debug_COM, hex_to_bcd(PLC_D_Buf[tmp_index].DataBuf.word));
		Print_Arrow_Right();
		
		tmp_value = 0;
		tmp_value  = ascii_to_hex(UserKeyInBuf[7]) * 1000;
		tmp_value += ascii_to_hex(UserKeyInBuf[8]) * 100;
		tmp_value += ascii_to_hex(UserKeyInBuf[9]) * 10;
		tmp_value += ascii_to_hex(UserKeyInBuf[10]);
		PLC_D_Buf[tmp_index].DataBuf.word = tmp_value;
		uart_send_word(Debug_COM, hex_to_bcd(PLC_D_Buf[tmp_index].DataBuf.word));
		uTXByte(Debug_COM, 0x0d);
		Sensor_mem.NowStatus = mem_CHECK;
	}
}

//=================================================
#endif
//=================================================