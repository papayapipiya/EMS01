#include <asf.h>
#include <stdio.h>
#include "myDefine.h"
#include "math.h"

//=================================================
#ifdef Dev_Sensor
//=================================================


void write_mem_func(unsigned char tmp_index, unsigned char tmp_addr_1, unsigned char tmp_addr_2)
{
	
	if(tmp_addr_1 == 0xFF && tmp_addr_2 == 0xFF)
	{
		if(LORA01->Lora_Show_Flag == true)
		{
			uart_send_ascii_byte(Debug_COM, tmp_addr_1);
			uart_send_ascii_byte(Debug_COM, tmp_addr_2);
			uart_str_COM(Debug_COM, " Data = 0xFFFF\r");
		}
	}
	else
	{
		if(LORA01->Lora_Show_Flag == true)
		{
			uTXByte(Debug_COM, '[');
			uart_send_word(Debug_COM, hex_to_bcd(tmp_index));
			uTXByte(Debug_COM, ']');
			uart_send_word(Debug_COM, hex_to_bcd(PLC_D_Buf[tmp_index].DataBuf.word));
			Print_Arrow_Right();
		}
		PLC_D_Buf[tmp_index].DataBuf.byte[1] = tmp_addr_1;
		PLC_D_Buf[tmp_index].DataBuf.byte[0] = tmp_addr_2;
		if(LORA01->Lora_Show_Flag == true)
		{
			uart_send_word(Debug_COM, hex_to_bcd(PLC_D_Buf[tmp_index].DataBuf.word));
			uTXByte(Debug_COM, 0x0d);
		}
	}
}






void mem_Set_CMD(void)
{
	unsigned char tmp_index;
	
	
	tmp_index = WriteBufferHex[5];
	write_mem_func(tmp_index, WriteBufferHex[6], WriteBufferHex[7]);
	tmp_index++;
	write_mem_func(tmp_index, WriteBufferHex[8], WriteBufferHex[9]);
	tmp_index++;
	write_mem_func(tmp_index, WriteBufferHex[10], WriteBufferHex[11]);
}

//=================================================
#endif
//=================================================