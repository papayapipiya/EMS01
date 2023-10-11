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
		if(LORA_Dev_List_Index[i].Dev_En_Flag == Dev_EN)
		{
			tmp_LORA->List_Now =  i;
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3] = LORA_Dev_List_Index[i].MAC_ID.byte[0];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4] = LORA_Dev_List_Index[i].MAC_ID.byte[1];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5] = LORA_Dev_List_Index[i].MAC_ID.byte[2];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6] = LORA_Dev_List_Index[i].MAC_ID.byte[3];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][7] = tmp_LORA->MyLoRaPower;
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][8] = 0xFE;	//Write Mode
			
			//把數據寫入封包後段，當作廣播使用
			for(j = 0; j < 18; j++)
			{
				tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][9+j] = WriteBufferHex[j];
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

	
	for(unsigned char i = 0; i < 30; i++)	dev_value[tmp_LORA->List_Now][i] = tmp_LORA->MyBuffer[i];
	dev_value[tmp_LORA->List_Now][dev_value_missnode]		= Dev_Found_Node;
	dev_value[tmp_LORA->List_Now][dev_value_New_Data_Input] = Dev_Have_New_Data;

	
	Master_RS232_To_UART(tmp_LORA);
}


//=================================================
#endif
//=================================================