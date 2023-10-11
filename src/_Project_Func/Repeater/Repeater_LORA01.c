#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//=================================================
#ifdef Dev_Repeater
//=================================================
void Response_LORA01(volatile LORA_DEFINE_t *tmp_LORA)
{
	volatile LORA_Dev_List_t *LORA_Dev_List_Index = LORA_Dev_List_In_RAM;
	//UINT16u_t Target, Source;
	switch(tmp_LORA->NowStatus)
	{
		case NOP_Index:
			Search_Dev_List_index(tmp_LORA);
			if(	dev_ptr[tmp_LORA->List_Now].Now_Counter.word > dev_ptr[tmp_LORA->List_Now].Dev_Time.word )
			{
				dev_ptr[tmp_LORA->List_Now].Now_Counter.word = 0;
				//LORA進入TX Mode
				Setting_Lora_To_Tx_Mode_Func(tmp_LORA);
				//Show Mode Display
				if(tmp_LORA->Lora_Show_Flag == true)
				{
					Print_Lora_number(tmp_LORA);
					Print_Send_Dev_No_Msg(tmp_LORA);
					uTXByte(Debug_COM, 0x09);
					uart_str("Output: \0");
					Print_Write_Buffer();
					uTXByte(Debug_COM, 0x0d);
				}
			}
			/*
			else
			{
				//尋找下一個Dev List是Enable的Index，並將Dev ID存入LORA TX Buffer
				Search_Dev_List_index(tmp_LORA);
			}
			*/
			break;
	
		case Lora_Rx_Data_Ready_Index:
			tmp_LORA->f_count = 0;
			//檢查回來的Dev ID是否與要詢問的Dev ID一置，並將收到的資料存起來
			Get_Dev_Data(tmp_LORA);
			if(tmp_LORA->Lora_Show_Flag == true)	Print_Catch_Data_Form_RX_Msg(tmp_LORA, LORA_Dev_List_Index);
			tmp_LORA->NowStatus = NOP_Index;
			break;
	
		case Wait_RX_IRQ_Index:
			if(tmp_LORA->LoRa_Tx_Send_Time_Out_Count == TimeOutCount)
			{
				//RX Time Out超過1次
				tmp_LORA->LoRa_Tx_Send_Time_Out_Count = 0;
				dev_ptr[tmp_LORA->List_Now].MISS_NODE = Dev_Miss_Node;
				if(dev_ptr[tmp_LORA->List_Now].MISS_NODE_Count < MISS_NODE_Count_Max)
				{
					dev_ptr[tmp_LORA->List_Now].MISS_NODE_Count++;
				}        
				if(tmp_LORA->Lora_Show_Flag == true)	Print_MissNode_Form_RX_Msg(tmp_LORA, LORA_Dev_List_Index);
				tmp_LORA->NowStatus = NOP_Index;
			}
			break;
	}
}




//=================================================
#endif
//=================================================