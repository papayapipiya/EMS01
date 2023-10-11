#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//=================================================
#ifdef Dev_Repeater
//=================================================

void Search_Modbus_Data_index(volatile LORA_DEFINE_t *tmp_LORA)
{
	unsigned int tmp_index;
	unsigned int i,k;
	volatile PLC_Dev_List_t *PLC_Dev_List_Index = PLC_D_Buf;
	bool found_flag;
	k = 0;
	found_flag = false;
	for(i = (RS485_LORA_OUT_Index + 1); k < PLC_D_Buf_Page; i = (i + 1) % (PLC_D_Buf_Page))
	{
		if(PLC_D_Check_Equel_Flag[i] != Dev_Not_New_Data)
		{
			if(PLC_D_Check_Equel_Flag[i] > Dev_Not_New_Data) PLC_D_Check_Equel_Flag[i]--;
			RS485_LORA_OUT_Index = i;
			found_flag = true;
			break;
		}	
		k++;
	}
	
	if(found_flag == false)return;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3] = tmp_LORA->MyLoRaID[0];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4] = tmp_LORA->MyLoRaID[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5] = tmp_LORA->MyLoRaID[2];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6] = tmp_LORA->MyLoRaID[3];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][7] = 0xF0; // PLC
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7] = tmp_LORA->MyBuffer[7];	//LoRA Power
	tmp_LORA->MyLoRaPower = tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7];
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][8] = RS485_LORA_OUT_Index;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][9] = 0x00;	//格式
	
	tmp_index = RS485_LORA_OUT_Index * PLC_D_Buf_Print_Columns_Max;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][10] = PLC_Dev_List_Index[tmp_index].DataBuf.byte[0];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][11] = PLC_Dev_List_Index[tmp_index].DataBuf.byte[1];
	tmp_index++;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][12] = PLC_Dev_List_Index[tmp_index].DataBuf.byte[0];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][13] = PLC_Dev_List_Index[tmp_index].DataBuf.byte[1];
	tmp_index++;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][14] = PLC_Dev_List_Index[tmp_index].DataBuf.byte[0];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][15] = PLC_Dev_List_Index[tmp_index].DataBuf.byte[1];
	tmp_index++;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][16] = PLC_Dev_List_Index[tmp_index].DataBuf.byte[0];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][17] = PLC_Dev_List_Index[tmp_index].DataBuf.byte[1];
	tmp_index++;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][18] = PLC_Dev_List_Index[tmp_index].DataBuf.byte[0];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][19] = PLC_Dev_List_Index[tmp_index].DataBuf.byte[1];
	tmp_index++;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][20] = PLC_Dev_List_Index[tmp_index].DataBuf.byte[0];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][21] = PLC_Dev_List_Index[tmp_index].DataBuf.byte[1];
	tmp_index++;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][22] = PLC_Dev_List_Index[tmp_index].DataBuf.byte[0];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][23] = PLC_Dev_List_Index[tmp_index].DataBuf.byte[1];
	tmp_index++;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][24] = Version;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][25] = 0x00;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][26] = 0x00;

}

void Search_UpLoad_Data_index(volatile LORA_DEFINE_t *tmp_LORA)
{
	unsigned char i;
	unsigned char k = 0;
	for(i = tmp_LORA->List_Now + 1 ; k < Dev_Item_MAX; i = (i + 1) % Dev_Item_MAX)
	{
		if(dev_ptr[i].Input_New_Data == Dev_Have_New_Data)
		{
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3] = dev_value[i][3];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4] = dev_value[i][4];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5] = dev_value[i][5];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6] = dev_value[i][6];
			// dev index
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][7] = dev_value[i][7];
			// R1
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][8] = dev_value[i][8];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][9] = dev_value[i][9];
			// R2
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][10] = dev_value[i][10];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][11] = dev_value[i][11];
			// R3
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][12] = dev_value[i][12];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][13] = dev_value[i][13];
			// R4
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][14] = dev_value[i][14];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][15] = dev_value[i][15];
			// R5
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][16] = dev_value[i][16];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][17] = dev_value[i][17];
			// R6
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][18] = dev_value[i][18];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][19] = dev_value[i][19];
			// R7
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][20] = dev_value[i][20];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][21] = dev_value[i][21];
			// R8
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][22] = dev_value[i][22];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][23] = dev_value[i][23];
			
			//Version
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][24] = dev_value[i][24];
			
			//Dev RSSI
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][25] = dev_value[i][27];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][26] = dev_value[i][28];
			tmp_LORA->List_Now = i;
			break;
		}
		k++;
	}

}

//=================================================
#endif
//=================================================