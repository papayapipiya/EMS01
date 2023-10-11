#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

//=================================================
#ifdef Dev_Master_RS232
//=================================================

void test(void)
{
	UINT16u_t tmp16;
	switch(Dev_Buf_Write_Index)
	{
		case 0:
		tmp16.byte[1] = dev_value[4][8];
		tmp16.byte[0] = dev_value[4][9];
		Insert_Data(0x183E, tmp16.word);
		Dev_Buf_Write_Index = 1;
		break;
		
		case 1:
		tmp16.byte[1] = dev_value[4][10];
		tmp16.byte[0] = dev_value[4][11];
		Insert_Data(0x1842, tmp16.word);
		Dev_Buf_Write_Index = 0;
		break;
		
		case 2:
		for(unsigned char i = 0; i < 18; i++)
		WriteBufferHex[i] = PC_WriteBufferHex[i];
		//Insert_Data(0x1842, tmp16.word);
		//Dev_Buf_Write_Index = 0;
		break;
	}
}

void Response_LORA01(volatile LORA_DEFINE_t *tmp_LORA)
{
	volatile LORA_Dev_List_t *LORA_Dev_List_Index = LORA_Dev_List_In_RAM;
	UINT16u_t Target, Source, tmp16;

	switch(tmp_LORA->NowStatus)
	{
		case NOP_Index:
			if(	LORA_Dev_List_Index[tmp_LORA->List_Now].Now_Counter.word >
			LORA_Dev_List_Index[tmp_LORA->List_Now].Dev_Time.word)
			{
				LORA_Dev_List_Index[tmp_LORA->List_Now].Now_Counter.word = 0;
				Setting_Lora_To_Tx_Mode_Func(tmp_LORA);
				if(tmp_LORA->Lora_Show_Flag == true)
				{
					Print_Lora_number(tmp_LORA);
					Print_Send_Dev_No_Msg(tmp_LORA);
					uTXByte(Debug_COM, 0x09);
					uart_str("Output: \0");
					Print_Write_Buffer();
					uTXByte(Debug_COM, Dev_Buf_Write_Index+ 0x30);
					
					uTXByte(Debug_COM, 0x0d);
				}
				test();
			}
			else
			{
				
			
				
				//尋找下一個Dev List是Enable的Index，並將Dev ID存入LORA TX Buffer
				Search_Dev_List_index(tmp_LORA);
			
			
			}
			break;
		
		case Lora_Rx_Data_Ready_Index:
			//檢查回來的Dev ID是否與要詢問的Dev ID一置，並將收到的資料存起來
			Get_Dev_Data(tmp_LORA);
			//Show Mode Display
			if(tmp_LORA->Lora_Show_Flag == true)
			{
				Print_Catch_Data_Form_RX_Msg(tmp_LORA, LORA_Dev_List_Index);
			}
			tmp_LORA->NowStatus = NOP_Index;
			break;
		
		case Wait_RX_IRQ_Index:
			if(tmp_LORA->LoRa_Tx_Send_Time_Out_Count > 2)
			{
				//RX Time Out超過3次
				tmp_LORA->LoRa_Tx_Send_Time_Out_Count = 0;
				//記錄Dev ID為Missnode
				dev_value[tmp_LORA->List_Now][dev_value_missnode] = Dev_Miss_Node;
				//Show Mode Display
				if(tmp_LORA->Lora_Show_Flag == true)
				{
					Print_MissNode_Form_RX_Msg(tmp_LORA, LORA_Dev_List_Index);
				}
				//尋找下一個Dev List是Enable的Index，並將Dev ID存入LORA TX Buffer
				Search_Dev_List_index(tmp_LORA);
				tmp_LORA->NowStatus = NOP_Index;
			}
			break;
	}
}

//=================================================
#endif
//=================================================