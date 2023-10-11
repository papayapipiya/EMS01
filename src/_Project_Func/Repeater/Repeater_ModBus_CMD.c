#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//=================================================
#ifdef Dev_Repeater
//=================================================


void PC_CMD(void)
{
	UINT16u_t tmp16;
	UINT16u_t tmpAddr, tmpVal;
	volatile UINT32u_t Target, Source;
	uint8_t CMD_MyRS485Tmp[8] = {0x01, 0x06, 0x17, 0xD4, 0x00, 0xff, 0x00, 0x00};
	CMD_MyRS485Tmp[0] = LORA02->PLC_Addr;
	CMD_MyRS485Tmp[1] = WriteBufferHex[7];
	CMD_MyRS485Tmp[2] = WriteBufferHex[8];
	CMD_MyRS485Tmp[3] = WriteBufferHex[9];
	CMD_MyRS485Tmp[4] = WriteBufferHex[10];
	CMD_MyRS485Tmp[5] = WriteBufferHex[11];
	tmp16.word = crc_chk(&CMD_MyRS485Tmp[0], 6);
	CMD_MyRS485Tmp[6] = tmp16.byte[0];
	CMD_MyRS485Tmp[7] = tmp16.byte[1];
	
	tmpAddr.byte[1] = CMD_MyRS485Tmp[2];
	tmpAddr.byte[0] = CMD_MyRS485Tmp[3];
	
	tmpVal.byte[1] = CMD_MyRS485Tmp[4];
	tmpVal.byte[0] = CMD_MyRS485Tmp[5];
	
	if(Form_CMD_Addr != tmpAddr.word || Form_CMD_Val != tmpVal.word)
	{
		Form_CMD_Addr_And_Val_Is_3rd_Flag = 0;
		Form_CMD_Addr = tmpAddr.word;
		Form_CMD_Val =  tmpVal.word;
	}
	else
	{
		if(Form_CMD_Addr_And_Val_Is_3rd_Flag < 10)
		Form_CMD_Addr_And_Val_Is_3rd_Flag++;
	}
	
	if(Form_CMD_Addr_And_Val_Is_3rd_Flag == 10)
	{
		if(PLC_Show_w_Flag == true)
		{
			uart_str("Be Again 10 count\r\0");
		}
		Form_CMD_Addr_And_Val_Is_3rd_Flag++;
		return;
	}
	else if(Form_CMD_Addr_And_Val_Is_3rd_Flag > 10)
	{
		//不要再顯示uart_str("Be Again 3rd\r\0");
		return;
	}
	
	//RS485_LORA_OUT_Index = (Form_CMD_Addr - 0x17d4) / RS485_RX_D_Buf_Count;
	
	if(PLC_Show_w_Flag == true)
	{
		uTXByte(Debug_COM, 0x0d);
		uart_str("[DoCMD] \0");
		uTXByte(Debug_COM, 'D');
		uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(Form_CMD_Addr - 0x1770));
		uTXByte(Debug_COM, ' ');
		uart_send_word(Debug_COM, Form_CMD_Addr);
		uTXByte(Debug_COM, ' ');
		uart_send_ascii_byte(Debug_COM, Form_CMD_Addr_And_Val_Is_3rd_Flag);
		uTXByte(Debug_COM, ' ');
		uTXByte(Debug_COM, '[');
		uTXByte(Debug_COM, ' ');
		for(unsigned char j = 0; j < 8; j++)
		{
			uart_send_ascii_byte(Debug_COM, CMD_MyRS485Tmp[j]);
			uTXByte(Debug_COM, ' ');
		}
		uTXByte(Debug_COM, ']');
	}
	
	if (PLC_is_Alive == false) return;
	
	//Print_Data_Buf_Function(&CMD_MyRS485Tmp[0], 8);
	
	RS485_For_Debug_Dn
	for(unsigned char j = 0; j < 8; j++)
	{
		uTXByte(RS485_COM, CMD_MyRS485Tmp[j]);
	}
	RS485_For_Debug_En
	//delay_ms(20);
	
	Form_CMD_RS485_Flag = 0;
	RS485_RX_Buf_Count = 0;
	RS485_Time_Count = 0;
	while(1)
	{
		usart_read_buffer_job(RS485_COM, (uint8_t *)RS485_RX_Buf, MAX_RX_BUFFER_LENGTH);
		if(RS485_RX_Buf_Count == 8)
		{
			if(PLC_Show_w_Flag == true)
			{
				uart_str("Write OK\r\0");
				Print_Data_Buf_Function(&RS485_RX_In_Buf[0], RS485_RX_Buf_Count);
			}
			break;
		}
		
		if(RS485_Time_Count >= RS485_Timeout)
		{
			if(PLC_Show_w_Flag == true)
			{
				uart_str("Write Time Out\r\0");
			}
			return;
		}
	}
}


/*
	WriteBufferHex[0] ~ [3] : MAC ID
	WriteBufferHex[4]: CMD
	WriteBufferHex[5]: Length
	WriteBufferHex[16] ~ [17]: 0x3333
*/
void Form_CMD_RS485_Function(volatile LORA_DEFINE_t *tmp_LORA)
{
	UINT16u_t tmp16;
	UINT16u_t tmpAddr, tmpVal;
	volatile UINT32u_t Target, Source;
	uint8_t CMD_MyRS485Tmp[8] = {0x01, 0x06, 0x17, 0xD4, 0x00, 0xff, 0x00, 0x00};
	if(Form_CMD_RS485_Flag == 0) return;
	
//	uart_str("New CMD\r\0");
	
	Target.byte[0] = tmp_LORA->MyLoRaID[0];
	Target.byte[1] = tmp_LORA->MyLoRaID[1];
	Target.byte[2] = tmp_LORA->MyLoRaID[2];
	Target.byte[3] = tmp_LORA->MyLoRaID[3];
	
	Source.byte[0] = WriteBufferHex[0];
	Source.byte[1] = WriteBufferHex[1];
	Source.byte[2] = WriteBufferHex[2];
	Source.byte[3] = WriteBufferHex[3];
	
	if(Source.dword == 0x55555555)	
	{
		PC_CMD();
	}
	else
	{
		if(Target.dword != Source.dword) return;
		PC_CMD();
	}
}




//=================================================
#endif
//=================================================