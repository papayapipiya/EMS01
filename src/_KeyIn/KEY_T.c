#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

void Judg_KEY_T(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	UINT16u_t tmp16;
	unsigned int tmp_sec_value;
	unsigned char tmp_list_table_index;
	
	if(	ascii_to_hex(UserKeyInBuf[0]) == KEYT && UserKeyInBuf[1] == ' ')
	{
		if(UserKeyInBufCount == 10 && UserKeyInBuf[10] == 0x0d)
		{
			tmp_sec_value  = ascii_to_hex(UserKeyInBuf[6]) * 1000;
			tmp_sec_value += ascii_to_hex(UserKeyInBuf[7]) * 100;
			tmp_sec_value += ascii_to_hex(UserKeyInBuf[8]) * 10;
			tmp_sec_value += ascii_to_hex(UserKeyInBuf[9]);
			tmp16.word = tmp_sec_value;
			
			tmp_list_table_index =	Cal_list_table_index(UserKeyInBuf[2], UserKeyInBuf[3], UserKeyInBuf[4]);
			
			Print_Find_Msg();
			Print_One_Form_List_Table(tmp_list_table_index, tmp_LORA_Dev_List_Index);
			Print_Arrow_Right();
			
			tmp_LORA_Dev_List_Index[tmp_list_table_index].Dev_Time.byte[1] = tmp16.byte[1];
			tmp_LORA_Dev_List_Index[tmp_list_table_index].Dev_Time.byte[0] = tmp16.byte[0];
			/*
			Dev_Time[tmp_list_table_index][0] = tmp16.byte[1];
			Dev_Time[tmp_list_table_index][1] = tmp16.byte[0];
			*/
			dev_value[tmp_list_table_index][dev_value_Time_High] = tmp16.byte[1];
			dev_value[tmp_list_table_index][dev_value_Time_Low]  = tmp16.byte[0];
			dev_value[tmp_list_table_index][dev_value_Count_High] = 0;
			dev_value[tmp_list_table_index][dev_value_Count_Low]  = 0;
			Save_Dev_Time_into_NVM(tmp_LORA_Dev_List_Index);
			//Load_List_Form_NVM();
			Load_Dev_Time_Form_NVM(tmp_LORA_Dev_List_Index);
			Print_One_Form_List_Table(tmp_list_table_index, tmp_LORA_Dev_List_Index);
			uTXByte(Debug_COM, 0x0d);
			uTXByte(Debug_COM, 0x0d);
		}
	}
}

//=================================================
#ifdef Dev_Sensor
//=================================================
//#define base_value 1843
#define base_value 3081

void Judg_KEY_TS(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	UINT16u_t tmp16;
	unsigned int tmp_sec_value;
	unsigned char tmp_list_table_index;
	unsigned int stmp16;
	
	if(	ascii_to_hex(UserKeyInBuf[0]) == KEYT && ascii_to_hex(UserKeyInBuf[1]) == KEYS)
	{
		if(UserKeyInBufCount == 3 && UserKeyInBuf[2] == 0x0d)
		{
			uart_str("[TS] -- > Target :");
			uart_send_word(Debug_COM, hex_to_bcd(base_value));
			uTXByte(Debug_COM, 0x0d);
			//ADC1
			stmp16 = CH1->MyDataAVG;
			if(stmp16 > base_value)
			{
				sensor_offet = stmp16 - base_value;
				sensor_plus_flag = 0;
			}
			else
			{
				sensor_offet =  base_value - stmp16;
				sensor_plus_flag = 1;
			}
			
			//ADC2
			stmp16 = CH2->MyDataAVG;
			if(stmp16 > base_value)
			{
				sensor_offet2 = stmp16 - base_value;
				sensor_plus_flag2 = 0;
			}
			else
			{
				sensor_offet2 =  base_value - stmp16;
				sensor_plus_flag2 = 1;
			}
			
			
			
			uart_str_COM(Debug_COM, "ADC1 : ");
			tmp16.word = sensor_offet;
			if(sensor_plus_flag == 0)	uTXByte(Debug_COM, '-');	else uTXByte(Debug_COM, '+');
			uTXByte(Debug_COM, ' ');
			tmp16.word = sensor_offet;
			uart_send_word(Debug_COM, hex_to_bcd(tmp16.word));
			uTXByte(Debug_COM, ' ');
			uTXByte(Debug_COM, '|');
			uTXByte(Debug_COM, ' ');
			uart_str_COM(Debug_COM, "ADC2 : ");
			if(sensor_plus_flag2 == 0)	uTXByte(Debug_COM, '-');	else uTXByte(Debug_COM, '+');
			uTXByte(Debug_COM, ' ');
			tmp16.word = sensor_offet2;
			uart_send_word(Debug_COM, hex_to_bcd(tmp16.word));
			
			uTXByte(Debug_COM, 0x0d);	
			Save_LoRa_Profile_in_NVM(LORA01);
			Load_LoRa_Profile_in_NVM(LORA01);
		}
	}
}

//=================================================
#endif
//=================================================