#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//=================================================
#ifdef Dev_Repeater
//=================================================


void Print_LoRA02_Upload_Msg(volatile LORA_DEFINE_t *tmp_LORA)
{
	volatile LORA_Dev_List_t *LORA_Dev_List_Index = LORA_Dev_List_In_RAM;
	//Show Mode Display
	if(tmp_LORA->Lora_Show_Flag == true)
	{
		if(PLC_SENSOR_SELECT_FLAG == 0)
		{
			Print_Lora_number(tmp_LORA);
			uart_str("  ^^^   No.\0");
			if(tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][7] == 0xff)
				uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(255));
			else
				uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(LORA_Dev_List_Index[tmp_LORA->List_Now].Dev_Number));
			uTXByte(Debug_COM, ' ');
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6]);
			uTXByte(Debug_COM, 0x0d);	
		}
		else
		{
			Print_Lora_number(tmp_LORA);
			uart_str("  ^^^   PLC.\0");
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6]);
			uTXByte(Debug_COM, ' ');
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][7]);
			uTXByte(Debug_COM, ' ');
			uTXByte(Debug_COM, '[');
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][8]);
			uTXByte(Debug_COM, ']');
			uTXByte(Debug_COM, ' ');
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][9]);
			uTXByte(Debug_COM, 0x0d);
		}
		Print_Line_Msg();
		Print_Lora_number(tmp_LORA);
		for(unsigned char i = 0; i < 28; i++)
		{
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][i]);
			uTXByte(Debug_COM, ' ');
		}
		uTXByte(Debug_COM, 0x0d);
		Print_Line_Msg();
	}
}


void LoRA2_Send_PLC_Data_Function(volatile LORA_DEFINE_t *tmp_LORA)
{
	// PLC回傳程式
	Setting_Lora_To_Tx_Mode_Func(tmp_LORA);
	Print_LoRA02_Upload_Msg(tmp_LORA);
}


void Get_LoRA_CMD(volatile LORA_DEFINE_t *tmp_LORA)
{
	//倒數兩個byte要為0x33才是需要的數值，因為RSSI不可能出現0x3333
	if(tmp_LORA->MyBuffer[25] == 0x33 && tmp_LORA->MyBuffer[26] == 0x33)
	{
		//擷取封包後段，延伸廣播訊號，18 Byte
		for(unsigned char i = 0; i < 18; i++)
		{
			WriteBufferHex[i] = tmp_LORA->MyBuffer[9+i];
		}
		Form_CMD_RS485_Flag = 1;
	}
}

bool MAC_Check(volatile LORA_DEFINE_t *tmp_LORA)
{
	volatile UINT32u_t Target, Source;
	Target.byte[0] = tmp_LORA->MyLoRaID[0];
	Target.byte[1] = tmp_LORA->MyLoRaID[1];
	Target.byte[2] = tmp_LORA->MyLoRaID[2];
	Target.byte[3] = tmp_LORA->MyLoRaID[3];
	
	Source.byte[0] = tmp_LORA->MyBuffer[3];
	Source.byte[1] = tmp_LORA->MyBuffer[4];
	Source.byte[2] = tmp_LORA->MyBuffer[5];
	Source.byte[3] = tmp_LORA->MyBuffer[6];
	
	if(Target.dword == Source.dword)
	{
		if(tmp_LORA->Lora_Show_Flag == true)
		{
			Print_Lora_number(tmp_LORA);
			uart_str("Yes \0");
			uTXByte(Debug_COM, 0x09);
			uart_str("Input: \0");
			Print_Write_Buffer();
			uTXByte(Debug_COM, 0x0d);
		}
		return true;
	}
	else
	{
		if(tmp_LORA->Lora_Show_Flag == true)
		{
			Print_Lora_number(tmp_LORA);
			uart_str("Not \0");
			uTXByte(Debug_COM, 0x0d);
		}
		tmp_LORA->NowStatus = Wait_RX_IRQ_Index;	//如果不是一樣的MAC ID，將跳回去並且進入RX Mode，等待下次命令
		return false;
	}
}

void Response_LORA02(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	
	if(tmp_LORA->NowStatus != Lora_Rx_Data_Ready_Index) return;
	
	tmp_LORA->f_count = 0;
	Get_LoRA_CMD(tmp_LORA);
	
	tmp_LORA->f_count = 0;
	if(!MAC_Check(tmp_LORA)) return;
	
	tmp_LORA->f_count = 0;
	
	

	//	一次傳SENSOR、一次傳PLC給Master
	switch(PLC_SENSOR_SELECT_FLAG)
	{
		case 0:	//Sensor
			Search_UpLoad_Data_index(tmp_LORA);
			if(dev_ptr[tmp_LORA->List_Now].Input_New_Data == Dev_Have_New_Data)	// Sensor有新資料
			{
				Setting_Lora_To_Tx_Mode_Func(tmp_LORA);
				dev_ptr[tmp_LORA->List_Now].Input_New_Data = Dev_Not_New_Data;
				Print_LoRA02_Upload_Msg(tmp_LORA);
			}
			else	// Sensor沒有新資料，進入傳輸PLC
			{ 
				if(tmp_LORA->Lora_Show_Flag == true)
				{
					Print_Lora_number(tmp_LORA);
					if(LORA02->PLC_Flag == NeedPLC)
					{
						uart_str("No New Data..., So Send the PLC!!\r\0");
						PLC_SENSOR_SELECT_FLAG = 1;
						LoRA2_Send_PLC_Data_Function(tmp_LORA);		// 如果都沒有新資料需要上傳，將送出PLC的資料
					}
					else
					{
						uart_str("No New Data...\r\0");
						tmp_LORA->NowStatus = Wait_RX_IRQ_Index;	// 如果都沒有新資料需要上傳，強迫進入Rx Mode等下一次資料...
					}
				}
			}
			if(LORA02->PLC_Flag == NeedPLC)						//	如果PLC Flag有啟動，才會進行切換到PLC回傳程式
				PLC_SENSOR_SELECT_FLAG = 1;						//	離開傳送Sensor狀態
			break;
			
		default: //PLC
			Search_Modbus_Data_index(tmp_LORA);
			LoRA2_Send_PLC_Data_Function(tmp_LORA);
			if(PLC_SENSOR_SELECT_FLAG < 2)
				PLC_SENSOR_SELECT_FLAG++;
			else
				PLC_SENSOR_SELECT_FLAG = 0;
			break;
	}
	
}


//=================================================
#endif
//=================================================