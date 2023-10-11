#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//=================================================
#ifdef Dev_Master_RS232
//=================================================



void Insert_Data(unsigned int tmp_index, unsigned int tmp_Data)
{
	UINT16u_t tmp16;
	
	WriteBufferHex[0] = 0x55;
	WriteBufferHex[1] = 0x55;
	WriteBufferHex[2] = 0x55;
	WriteBufferHex[3] = 0x55;
	WriteBufferHex[4] = 0x00;
	WriteBufferHex[5] = 0x00;
	WriteBufferHex[6] = 0x01;
	WriteBufferHex[7] = 0x06;
	tmp16.word = tmp_index;
	WriteBufferHex[8] = tmp16.byte[1];
	WriteBufferHex[9] = tmp16.byte[0];
	
	tmp16.word = tmp_Data;
	WriteBufferHex[10] = tmp16.byte[1];
	WriteBufferHex[11] = tmp16.byte[0];
	WriteBufferHex[12] = 0x00;
	WriteBufferHex[13] = 0x00;
	WriteBufferHex[14] = 0x00;
	WriteBufferHex[15] = 0x00;
	WriteBufferHex[16] = 0x33;
	WriteBufferHex[17] = 0x33;

}




void Search_Dev_List_index(volatile LORA_DEFINE_t *tmp_LORA)
{
	volatile LORA_Dev_List_t *LORA_Dev_List_Index = LORA_Dev_List_In_RAM;
	unsigned char i,j,k;
	
	
	k = 0;
	for(i = (tmp_LORA->List_Now + 1); k < Dev_Item_MAX; i = (i + 1) % Dev_Item_MAX)
	{
		// Master只詢問有Enable
		if(dev_ptr[i].Dev_En_Flag == Dev_EN) //&& dev_ptr[i].Input_New_Data == 0x00)
		{
			tmp_LORA->List_Now =  i;
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3] = LORA_Dev_List_Index[i].MAC_ID.byte[0];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4] = LORA_Dev_List_Index[i].MAC_ID.byte[1];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5] = LORA_Dev_List_Index[i].MAC_ID.byte[2];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6] = LORA_Dev_List_Index[i].MAC_ID.byte[3];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][7] = tmp_LORA->MyLoRaPower;
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][8] = 0xFE;	//Write Mode
			
			if(dev_ptr[i].cmd_buf_Sent_Flag == cmd_buf_Sent_New)
			{
				for(j = 0; j < 18; j++)
				{
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][ 9 + j ] = LORA_Dev_List_Index[i].cmd_buf[j];
				}
				dev_ptr[i].cmd_buf_Sent_Flag = cmd_buf_Sent_Wait_OK;
				uart_str_COM(Debug_COM, "Want to Send CMD No.");
				uart_send_ascii_byte(Debug_COM, i);
				uTXByte(Debug_COM, 0x0d);
			}
			break;
		}
		k++;
	}
}







void Master_RS232_To_UART(volatile LORA_DEFINE_t *tmp_LORA)
{
	uTXByte(Debug_COM, 'S');
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][3]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][4]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][5]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][6]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][7]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][8]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][9]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][10]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][11]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][12]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][13]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][14]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][15]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][16]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][17]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][18]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][19]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][20]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][21]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][22]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][23]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][24]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][25]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][26]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][27]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][28]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][31]);
	Encoder_uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][32]);
	uTXByte(Debug_COM, 'Z');
	uTXByte(Debug_COM, 0x0d);
}

void search_CMD_MAC(volatile LORA_DEFINE_t *tmp_LORA)
{
	UINT16u_t Target, Source, tmp16;
	bool OK_Flag = true;
	for(unsigned char i= 0; i < Dev_Item_MAX; i++)
	{
		if(	dev_ptr[i].Dev_En_Flag == Dev_EN && dev_ptr[i].cmd_buf_Return_OK_Flag == false && dev_ptr[i].cmd_buf_Sent_Flag == cmd_buf_Sent_Wait_OK)
		{
			if(	dev_ptr[i].cmd_buf[0] == dev_value[tmp_LORA->List_Now][3]	&&
				dev_ptr[i].cmd_buf[1] == dev_value[tmp_LORA->List_Now][4]	&&
				dev_ptr[i].cmd_buf[2] == dev_value[tmp_LORA->List_Now][5]	&&
				dev_ptr[i].cmd_buf[3] == dev_value[tmp_LORA->List_Now][6])
			{
				uart_str_COM(Debug_COM, "Check CMD No.");
				uart_send_ascii_byte(Debug_COM, i);
				uTXByte(Debug_COM, 0x0d);
				
				//============================================================
				//	位置1確認寫入
				Target.byte[1] = dev_ptr[i].cmd_buf[6];
				Target.byte[0] = dev_ptr[i].cmd_buf[7];
				Source.byte[1] = dev_value[tmp_LORA->List_Now][18];
				Source.byte[0] = dev_value[tmp_LORA->List_Now][19];
				uart_send_word(Debug_COM, Source.word);uTXByte(Debug_COM, ' ');uart_send_word(Debug_COM, Target.word);uTXByte(Debug_COM, ' ');
				if(Target.word == Source.word) uart_str_COM(Debug_COM, "mem[1] is set ok\r");
				else 
				{
					uart_str_COM(Debug_COM, "mem[1] is set Fail\r");
					OK_Flag = false;
				}
				
				//============================================================
				//	位置2確認寫入				
				Target.byte[1] = dev_ptr[i].cmd_buf[8];
				Target.byte[0] = dev_ptr[i].cmd_buf[9];
				Source.byte[1] = dev_value[tmp_LORA->List_Now][20];
				Source.byte[0] = dev_value[tmp_LORA->List_Now][21];
				uart_send_word(Debug_COM, Source.word);uTXByte(Debug_COM, ' ');uart_send_word(Debug_COM, Target.word);uTXByte(Debug_COM, ' ');
				if(Target.word == Source.word) uart_str_COM(Debug_COM, "mem[2] is set ok\r");
				else
				{
					uart_str_COM(Debug_COM, "mem[2] is set Fail\r");
					OK_Flag = false;
				}
				
				//============================================================
				//	位置3確認寫入
				Target.byte[1] = dev_ptr[i].cmd_buf[10];
				Target.byte[0] = dev_ptr[i].cmd_buf[11];
				Source.byte[1] = dev_value[tmp_LORA->List_Now][22];
				Source.byte[0] = dev_value[tmp_LORA->List_Now][23];
				uart_send_word(Debug_COM, Source.word);uTXByte(Debug_COM, ' ');uart_send_word(Debug_COM, Target.word);uTXByte(Debug_COM, ' ');
				if(Target.word == Source.word) uart_str_COM(Debug_COM, "mem[3] is set ok\r");
				else
				{
					uart_str_COM(Debug_COM, "mem[3] is set Fail\r");
					OK_Flag = false;
				}
				
				//============================================================
				//	確認參數1的回饋訊號，如果設定Relay = 3，收到3或4代表寫入成功，並關閉該指令，修正為0xFF
				Target.byte[1] = 0x00;
				Target.byte[0] = dev_ptr[i].cmd_buf[12];
				Source.byte[1] = dev_value[tmp_LORA->List_Now][16];
				Source.byte[0] = dev_value[tmp_LORA->List_Now][17];
				uart_send_word(Debug_COM, Source.word);uTXByte(Debug_COM, ' ');uart_send_word(Debug_COM, Target.word);uTXByte(Debug_COM, ' ');
				if(Target.word == Source.word)
				{
					uart_str_COM(Debug_COM, "Relay is set OK\r");
				}
				else
				{
					if(Target.word == 0x0003 && Source.word == 0x0004)
					{
						uart_str_COM(Debug_COM, "Relay is 4 and CMD is 3, but set OK\r");
					}
					else
					{
						uart_str_COM(Debug_COM, "Relay is set Fail\r");
						OK_Flag = false;
					}
				}
				
				
				//============================================================
				//	判斷是否成功寫入
				if(OK_Flag == false)
				{	//失敗，重新送指令出去
					dev_ptr[i].cmd_buf_Return_OK_Flag = false;
					dev_ptr[i].cmd_buf_Sent_Flag = cmd_buf_Sent_Wait_OK;
				}
				else
				{	//如果成功寫入將不再送出該指令
					dev_ptr[i].cmd_buf_Sent_Flag  = cmd_buf_Sent_ok;
					dev_ptr[i].cmd_buf_Return_OK_Flag = true;
					dev_ptr[i].cmd_buf_to_clear_mysql_Flag = true;
					for(unsigned char j = 0; j < 18; j++)
					{
						tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][ 9 + j ] = 0x00;
					}
					uart_str_COM(Debug_COM, "CMD is done.. Do not send again..\r");
				}
				
				return;
			}
		}
	}
}

void Get_Dev_Data(volatile LORA_DEFINE_t *tmp_LORA)
{
	
	if(tmp_LORA->Lora_Show_Flag == true)
	{
		Print_Lora_number(tmp_LORA);
		uart_str(" ---> \0");
		uart_send_ascii_byte(Debug_COM, tmp_LORA->MyBuffer[3]);
		uart_send_ascii_byte(Debug_COM, tmp_LORA->MyBuffer[4]);
		uart_send_ascii_byte(Debug_COM, tmp_LORA->MyBuffer[5]);
		uart_send_ascii_byte(Debug_COM, tmp_LORA->MyBuffer[6]);
		uTXByte(Debug_COM, 0x0d);
	}
	
	// 檢查詢問的設備資料是否等於回傳的資料
	if(tmp_LORA->MyBuffer[3] != tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3])	return;
	if(tmp_LORA->MyBuffer[4] != tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4])	return;
	if(tmp_LORA->MyBuffer[5] != tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5])	return;
	if(tmp_LORA->MyBuffer[6] != tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6])	return;

	
	
	for(unsigned char i = 0; i < 30; i++)		
	{
		dev_value[tmp_LORA->List_Now][i] = tmp_LORA->MyBuffer[i];
		//uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][i]);uTXByte(Debug_COM, ' ');
	}
	
	search_CMD_MAC(tmp_LORA);
	
		
	dev_ptr[tmp_LORA->List_Now].MISS_NODE		= Dev_Found_Node;
	dev_ptr[tmp_LORA->List_Now].Input_New_Data	= Dev_Have_New_Data;

	
	Master_RS232_To_UART(tmp_LORA);
}


//=================================================
#endif
//=================================================