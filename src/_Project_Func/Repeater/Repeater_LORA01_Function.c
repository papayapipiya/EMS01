#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//=================================================
#ifdef Dev_Repeater
//=================================================




void Search_Dev_List_index(volatile LORA_DEFINE_t *tmp_LORA)
{
	volatile LORA_Dev_List_t *LORA_Dev_List_Index = LORA_Dev_List_In_RAM;
	unsigned char i, j, k;
	unsigned char tmp_List_Now;
	
	tmp_List_Now = tmp_LORA->List_Now;
	k = 0;
	for(i = (tmp_LORA->List_Now + 1); k < Dev_Item_MAX; i = (i + 1) % Dev_Item_MAX)
	{
		//Repeater詢問有Enable and (Dev_Not_New_Data or Sensor)
		if(	dev_ptr[i].Dev_En_Flag == Dev_EN &&
			(dev_ptr[i].Input_New_Data == Dev_Not_New_Data || dev_value[i][dev_type] == 0x00))
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
//	tmp_LORA->List_Now++;
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
	
	// Repeater需要檢查詢問的設備資料是否等於回傳的資料
	if(tmp_LORA->MyBuffer[3] != tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3])	return;
	if(tmp_LORA->MyBuffer[4] != tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4])	return;
	if(tmp_LORA->MyBuffer[5] != tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5])	return;
	if(tmp_LORA->MyBuffer[6] != tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6])	return;
	
	
	for(unsigned char i = 0; i < 30; i++)	dev_value[tmp_LORA->List_Now][i] = tmp_LORA->MyBuffer[i];
	dev_ptr[tmp_LORA->List_Now].MISS_NODE		= Dev_Found_Node;
	dev_ptr[tmp_LORA->List_Now].Input_New_Data	= Dev_Have_New_Data;
	dev_ptr[tmp_LORA->List_Now].MISS_NODE_Count = 0;
	//dev_value[tmp_LORA->List_Now][dev_value_missnode]		= Dev_Found_Node;
	//dev_value[tmp_LORA->List_Now][dev_value_New_Data_Input] = Dev_Have_New_Data;
	

}



//=================================================
#endif
//=================================================
