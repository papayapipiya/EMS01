
#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

#define Uart_Tab		uTXByte(Debug_COM, 0x09);
#define Uart_Enter		uTXByte(Debug_COM, 0x0d);

void Print_Lora_Txt(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA == LORA01)
		uart_str("[LORA01]");
	else
		uart_str("[LORA02]");
}

void Print_LoRA_Running_Status(volatile LORA_DEFINE_t *tmp_LORA)
{
	Print_Lora_Txt(tmp_LORA);
	Uart_Enter
	uart_str(" LastStatus:");
	uart_send_word(Debug_COM, hex_to_bcd(tmp_LORA->LastStatus));
	Uart_Tab
	
	uart_str(" NowStatus:");
	uart_send_word(Debug_COM, hex_to_bcd(tmp_LORA->NowStatus));
	Uart_Tab
	
	uart_str(" NextStatus:");
	uart_send_word(Debug_COM, hex_to_bcd(tmp_LORA->NextStatus));
	
	uart_str(" f_count:");
	uart_send_word(Debug_COM, hex_to_bcd(tmp_LORA->f_count));
	Uart_Tab
	
	uart_str(" TimeOut_Count:");
	uart_send_word(Debug_COM, hex_to_bcd(tmp_LORA->LoRa_Tx_Send_Time_Out_Count));
	Uart_Tab
	
	uart_str(" List_Now:");
	uart_send_word(Debug_COM, hex_to_bcd(tmp_LORA->List_Now));
	Uart_Enter
}


void Judg_KEY_DL(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	UINT16u_t tmp16;
	
	if(	ascii_to_hex(UserKeyInBuf[0]) == KEYD && ascii_to_hex(UserKeyInBuf[1]) == KEYL )
	{
		if(UserKeyInBufCount == 2 && UserKeyInBuf[2] == 0x0d)
		{
			uart_str("\t          ID    TP  R0   R1   R2   R3   R4   R5   R6   R7  VR RSSI RSSI        Timer\r\0");
			for(unsigned char i= 0; i < Dev_Item_MAX; i++)
			{
				if(tmp_LORA_Dev_List_Index[i].Dev_En_Flag == Dev_EN)
				{
					if(LORA01->List_Now == i)
					{
						uTXByte(Debug_COM, '>');
					}
					else
					{
						uTXByte(Debug_COM, ' ');
					}
					uTXByte(Debug_COM, 'N');
					uTXByte(Debug_COM, 'o');
					uTXByte(Debug_COM, '.');
					uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(tmp_LORA_Dev_List_Index[i].Dev_Number));
					uTXByte(Debug_COM, ' ');
					
					//uTXByte(Debug_COM, 0x09);
					
					// SX1272 FORMAT
					uart_send_ascii_byte(Debug_COM, dev_value[i][0]);
					uart_send_ascii_byte(Debug_COM, dev_value[i][1]);
					uart_send_ascii_byte(Debug_COM, dev_value[i][2]);
					uTXByte(Debug_COM, ' ');
					
					// ID-11
					uart_send_ascii_byte(Debug_COM, dev_value[i][3]);
					uart_send_ascii_byte(Debug_COM, dev_value[i][4]);
					uart_send_ascii_byte(Debug_COM, dev_value[i][5]);
					uart_send_ascii_byte(Debug_COM, dev_value[i][6]);
					uTXByte(Debug_COM, ' ');
					
					uart_send_ascii_byte(Debug_COM, dev_value[i][7]);
					uTXByte(Debug_COM, ' ');
					
					// WORD-15
					uart_send_ascii_byte(Debug_COM, dev_value[i][8]);
					uart_send_ascii_byte(Debug_COM, dev_value[i][9]);
					uTXByte(Debug_COM, ' ');
					uart_send_ascii_byte(Debug_COM, dev_value[i][10]);
					uart_send_ascii_byte(Debug_COM, dev_value[i][11]);
					uTXByte(Debug_COM, ' ');
					uart_send_ascii_byte(Debug_COM, dev_value[i][12]);
					uart_send_ascii_byte(Debug_COM, dev_value[i][13]);
					uTXByte(Debug_COM, ' ');
					uart_send_ascii_byte(Debug_COM, dev_value[i][14]);
					uart_send_ascii_byte(Debug_COM, dev_value[i][15]);
					uTXByte(Debug_COM, ' ');
					uart_send_ascii_byte(Debug_COM, dev_value[i][16]);
					uart_send_ascii_byte(Debug_COM, dev_value[i][17]);
					uTXByte(Debug_COM, ' ');
					uart_send_ascii_byte(Debug_COM, dev_value[i][18]);
					uart_send_ascii_byte(Debug_COM, dev_value[i][19]);
					uTXByte(Debug_COM, ' ');
					uart_send_ascii_byte(Debug_COM, dev_value[i][20]);
					uart_send_ascii_byte(Debug_COM, dev_value[i][21]);
					uTXByte(Debug_COM, ' ');
					uart_send_ascii_byte(Debug_COM, dev_value[i][22]);
					uart_send_ascii_byte(Debug_COM, dev_value[i][23]);
					uTXByte(Debug_COM, ' ');
					uart_send_ascii_byte(Debug_COM, dev_value[i][24]);	//版本
					uTXByte(Debug_COM, ' ');
					uart_send_ascii_byte(Debug_COM, dev_value[i][25]);	//RSSI
					uart_send_ascii_byte(Debug_COM, dev_value[i][26]);
					uTXByte(Debug_COM, ' ');
					uart_send_ascii_byte(Debug_COM, dev_value[i][27]);	//RSSI
					uart_send_ascii_byte(Debug_COM, dev_value[i][28]);
					uTXByte(Debug_COM, ' ');
					uart_send_ascii_byte(Debug_COM, dev_value[i][29]);	//CRC
					uTXByte(Debug_COM, ' ');	
					
					uTXByte(Debug_COM, '{');	
					uart_send_ascii_byte(Debug_COM, tmp_LORA_Dev_List_Index[i].Dev_Time.byte[1]);
					uart_send_ascii_byte(Debug_COM, tmp_LORA_Dev_List_Index[i].Dev_Time.byte[0]);
					uTXByte(Debug_COM, ' ');
					uTXByte(Debug_COM, '<');
					uTXByte(Debug_COM, ' ');
					uart_send_ascii_byte(Debug_COM, tmp_LORA_Dev_List_Index[i].Now_Counter.byte[1]);
					uart_send_ascii_byte(Debug_COM, tmp_LORA_Dev_List_Index[i].Now_Counter.byte[0]);
					uTXByte(Debug_COM, '}');	
					
					uTXByte(Debug_COM, ' ');
					uart_send_ascii_byte(Debug_COM, dev_value[i][36]);
					uTXByte(Debug_COM, ' ');
					
					tmp16.byte[1] = dev_value[i][27];
					tmp16.byte[0] = dev_value[i][28];
					tmp16.word = 0xffff - tmp16.word;
					Print_One_RSSI_Msg(tmp16.word);
					uTXByte(Debug_COM, ' ');
					uart_send_ascii_byte(Debug_COM, dev_ptr[i].RSSI_Status);
					
					
          
					
					uTXByte(Debug_COM, ' ');
					uTXByte(Debug_COM, '[');
					if(dev_ptr[i].MISS_NODE == 0x00)	//MISSNODE
					{
						uTXByte(Debug_COM, 'X');
					}
					else
					{
						uTXByte(Debug_COM, ' ');
					}
					uTXByte(Debug_COM, ']');
					uTXByte(Debug_COM, '-');
					uart_send_ascii_byte(Debug_COM, dev_ptr[i].MISS_NODE_Count);
          
					uTXByte(Debug_COM, ' ');
					uTXByte(Debug_COM, '(');
					if(dev_ptr[i].Input_New_Data == 0x00)	//New Data
					{
						uTXByte(Debug_COM, ' ');
					}
					else
					{
						uTXByte(Debug_COM, 'N');
					}
					uTXByte(Debug_COM, ')');
					if(LORA02->List_Now == i)	uTXByte(Debug_COM, '<');
					uTXByte(Debug_COM, 0x0d);
				}
			}
			Print_LoRA_Running_Status(LORA01);
			Print_LoRA_Running_Status(LORA02);
			#ifdef Dev_Repeater
				uart_str("[PLC]");
				Uart_Enter
				uart_str(" Read:");
				Print_PLC_D_Register_Number(RS485_RD_Index);
				Uart_Tab
				uart_str(" Write:");
				Print_PLC_D_Register_Number(RS485_WR_Index);
			#endif
			
		}
	}
}