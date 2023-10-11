#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


extern void Master_RS232_To_UART(volatile LORA_DEFINE_t *tmp_LORA);
uint8_t Define_MyLoRaTmpCount[16] = {3, 3, 3, 8, 6, 27, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t Define_MyLoRaTmp[16][27] =
{
	{0x80, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
	{0xC1, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
	{0xC1, 0x02, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
	{0xC1, 0x03, 0x05, 0x03, 0x01, 0x65, 0x6C, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
	{0xC1, 0x04, 0x03, 0x03, 0x01, 0x04, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
	{0xC1, 0x05, 0x18, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17},
	{0xC1, 0x06, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
	{0xC1, 0x07, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};

void Define_Profile_COPY_TO_RAM(void)
{

	unsigned int i, j;
	
	for(i = 0; i < 15; i++)
	{
		LORA01->MyLoRaTmpCount[i] = Define_MyLoRaTmpCount[i];
		LORA02->MyLoRaTmpCount[i] = Define_MyLoRaTmpCount[i];
	}
	for(i = 0; i < 15; i++)
	{
		for(j = 0; j < 28; j++)
		{
			LORA01->MyLoRaTmp[i][j] = Define_MyLoRaTmp[i][j];
			LORA02->MyLoRaTmp[i][j] = Define_MyLoRaTmp[i][j];
		}
	}
}

/*

*/
/*
void Find_Now_And_Next_Enable_Func(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	unsigned char i,j;
	unsigned char k;
	
	k = 0;
	for(i = (tmp_LORA->List_Now + 1); k < Dev_Item_MAX; i = (i + 1) % Dev_Item_MAX)
	{
		
		#ifdef Dev_Repeater					// Repeater詢問有Enable and (Dev_Not_New_Data or Sensor)
			if(	tmp_LORA_Dev_List_Index[i].Dev_En_Flag == Dev_EN 
				&& (dev_value[i][dev_value_New_Data_Input] == Dev_Not_New_Data
				|| dev_value[i][dev_type] == 0x00))
		#elif defined(Dev_Master_RS232)		// Master只詢問有Enable
			if(tmp_LORA_Dev_List_Index[i].Dev_En_Flag == Dev_EN)
		#endif
		{
			tmp_LORA->List_Now =  i;
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3] = tmp_LORA_Dev_List_Index[i].MAC_ID.byte[0];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4] = tmp_LORA_Dev_List_Index[i].MAC_ID.byte[1];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5] = tmp_LORA_Dev_List_Index[i].MAC_ID.byte[2];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6] = tmp_LORA_Dev_List_Index[i].MAC_ID.byte[3];
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
*/
/*
void Catch_Data_Form_RX_Func(volatile LORA_DEFINE_t *tmp_LORA)
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
	
	// Repeater需要檢查詢問的設備資料是否等於回傳的資料
	#ifdef Dev_Repeater
	if(tmp_LORA->MyBuffer[3] != tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3])	return;
	if(tmp_LORA->MyBuffer[4] != tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4])	return;
	if(tmp_LORA->MyBuffer[5] != tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5])	return;
	if(tmp_LORA->MyBuffer[6] != tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6])	return;
	#endif
	
	for(unsigned char i = 0; i < 30; i++)	dev_value[tmp_LORA->List_Now][i] = tmp_LORA->MyBuffer[i];
	dev_value[tmp_LORA->List_Now][dev_value_missnode]		= Dev_Found_Node;
	dev_value[tmp_LORA->List_Now][dev_value_New_Data_Input] = Dev_Have_New_Data;

	#ifdef Dev_Master_RS232
	// Master接收到的資料馬上回傳給電腦
	Master_RS232_To_UART(tmp_LORA);
	#endif
}
*/

void Define_Dev_Time_Func(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	Load_Dev_Time_Form_NVM(tmp_LORA_Dev_List_Index);
	if(tmp_LORA_Dev_List_Index[0].Dev_Time.word == 0xffff)
	{
		for ( unsigned char i = 0; i < Dev_Item_MAX; i++)
		{
			tmp_LORA_Dev_List_Index[i].Dev_Time.word;
		}
		Save_Dev_Time_into_NVM(tmp_LORA_Dev_List_Index);
	}
}


void Define_Dev_List_Func(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	Load_List_Form_NVM(tmp_LORA_Dev_List_Index);
	if(tmp_LORA_Dev_List_Index[0].Dev_Number == 0xff)
	{
		for (unsigned char i = 0; i < Dev_Item_MAX; i++)
		{
			tmp_LORA_Dev_List_Index[i].Dev_Number = i;
			tmp_LORA_Dev_List_Index[i].Dev_En_Flag = Dev_Dis;
			tmp_LORA_Dev_List_Index[i].MAC_ID.dword = 0;
		}
		Save_List_in_NVM(tmp_LORA_Dev_List_Index);
	}
}




void Fill_Dev_Time_into_dev_value(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(LORA01->Lora_Dev_Count_IRQ_Flag == 1)
	{
		Dev_Time_Counter_By_Timer_Func(0, 100);
		tmp_LORA->Lora_Dev_Count_IRQ_Flag = 0;
		tmp_LORA->t2_tmp_count = 0;
	}
}



void Dev_Time_Counter_By_Timer_Func(unsigned char tmp_start, unsigned char tmp_end)
{
	for(unsigned char i = tmp_start; i < tmp_end; i++)
	{
		if(dev_ptr[i].Now_Counter.word < 5000)
		{
			dev_ptr[i].Now_Counter.word++;
		}
	}
}


unsigned char Search_list_table_index(unsigned char tmp_list_table_index, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	unsigned int i;
	for(i = 0; i < Dev_Item_MAX; i++)
	{
		if(dev_ptr[i].Dev_Number == tmp_list_table_index)
		{
			return i;
		}
	}
	return Not_found_Index;
}