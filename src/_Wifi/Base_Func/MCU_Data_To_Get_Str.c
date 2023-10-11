/*
 * MCUData_To_GetStr.c
 *
 * Created: 2021/4/19 上午 10:49:50
 *  Author: willchen
 */ 

#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "myDefine.h"


void MCU_Data_To_GET_Str_Func(void)
{
	//volatile LORA_Dev_List_t *LORA_Dev_List_Index = LORA_Dev_List_In_RAM;
	UINT16u_t	tmp16;
	unsigned char *tmpMAC;
	UINT32u_t Source;
	unsigned int tmpNTCA = mGateway.uEMS.uModule.uADC.NTC_No_01 / 10;
	unsigned int tmpNTCB = mGateway.uEMS.uModule.uADC.NTC_No_01 % 10;
	sprintf(GET_Update_tmp, "");
	sprintf(GET_Update_tmp, "GET /demo?text=%%e7%%8f%%be%%e5%%9C%%A8%%E6%%BA%%AB%%E5%%BA%%A6%%EF%%BC%%9A%%20%02d.%01d%%20%%E5%%BA%%A6C&token=testkey", tmpNTCA, tmpNTCB);
	//strcpy(GET_Update_tmp, "GET /send.php?umail=papayapipiya@gmail.com");
	return;
	
	//取得LoRA訊號帶來的上的MAC ID
	strcpy(GET_Update_tmp, Target_PHP);
	replace_txt(GET_Update_tmp, "@0000000", WifiMACtoASCII());
	strcat(GET_Update_tmp, "'&DATA=");
	My_WiFi.Update_Count = 0;
	for(unsigned char i = My_WiFi.Update_ptr; i < Dev_Item_MAX; i++)
	{
		Source.byte[0] = dev_value[i][3];
		Source.byte[1] = dev_value[i][4];
		Source.byte[2] = dev_value[i][5];
		Source.byte[3] = dev_value[i][6];
		My_WiFi.Update_ptr = i;
		if(dev_ptr[i].Dev_En_Flag == Dev_EN && Source.dword != 0x00000000)
		{
			if(My_WiFi.debug_flag)
			{
				uTXByte(Debug_COM, '[');
				uart_send_ascii_byte(Debug_COM, My_WiFi.Update_ptr);
				uTXByte(Debug_COM, ']');
				uTXByte(Debug_COM, 0x0d);
			}
			strcpy(DeviceData_tmp, DeviceData1);
			replace_txt(DeviceData_tmp, php_MAC_00, MACtoASCII(i));
			tmp16.byte[1] = 0;
			tmp16.byte[0] = dev_value[i][7];
			replace_txt(DeviceData_tmp, php_key_00, hex_to_val_ascii(tmp16.word));	//	D1
			tmp16.byte[1] = dev_value[i][8];
			tmp16.byte[0] = dev_value[i][9];
			replace_txt(DeviceData_tmp, php_key_01, hex_to_val_ascii(tmp16.word));	//	D2	TGS2600
			tmp16.byte[1] = dev_value[i][10];
			tmp16.byte[0] = dev_value[i][11];
			replace_txt(DeviceData_tmp, php_key_02, hex_to_val_ascii(tmp16.word));	//	D3	溫度
			tmp16.byte[1] = dev_value[i][12];
			tmp16.byte[0] = dev_value[i][13];
			replace_txt(DeviceData_tmp, php_key_03, hex_to_val_ascii(tmp16.word));	//	D4	濕度
			tmp16.byte[1] = dev_value[i][14];
			tmp16.byte[0] = dev_value[i][15];
			replace_txt(DeviceData_tmp, php_key_04, hex_to_val_ascii(tmp16.word));	//	D5	TGS2620
			tmp16.byte[1] = dev_value[i][16];
			tmp16.byte[0] = dev_value[i][17];
			replace_txt(DeviceData_tmp, php_key_05, hex_to_val_ascii(tmp16.word));	//	D6	Relay
			tmp16.byte[1] = dev_value[i][18];
			tmp16.byte[0] = dev_value[i][19];
			replace_txt(DeviceData_tmp, php_key_06, hex_to_val_ascii(tmp16.word));	//	D7	保護機制時間
			tmp16.byte[1] = dev_value[i][20];
			tmp16.byte[0] = dev_value[i][21];
			replace_txt(DeviceData_tmp, php_key_07, hex_to_val_ascii(tmp16.word));	//	D8	循環時間
			tmp16.byte[1] = dev_value[i][22];
			tmp16.byte[0] = dev_value[i][23];
			replace_txt(DeviceData_tmp, php_key_08, hex_to_val_ascii(tmp16.word));	//	D9	NA
			tmp16.byte[1] = 0;
			tmp16.byte[0] = dev_value[i][24];
			replace_txt(DeviceData_tmp, php_key_09, hex_to_val_ascii(tmp16.word));	//	D10	版本
			tmp16.byte[1] = dev_value[i][25];
			tmp16.byte[0] = dev_value[i][26];
			tmp16.word = tmp16.word;
			replace_txt(DeviceData_tmp, php_key_10, hex_to_val_ascii(tmp16.word));	//	D11	剩餘時間
			tmp16.byte[1] = dev_value[i][27];
			tmp16.byte[0] = dev_value[i][28];
			tmp16.word = 65535 - tmp16.word;
			replace_txt(DeviceData_tmp, php_key_11, hex_to_val_ascii(tmp16.word));	//	D12	RSSI
			tmp16.byte[1] = dev_ptr[i].Dev_Time.byte[1];
			tmp16.byte[0] = dev_ptr[i].Dev_Time.byte[0];
			replace_txt(DeviceData_tmp, php_key_12, hex_to_val_ascii(tmp16.word));	//	D13	Dev輪詢時間
			tmp16.byte[1] = dev_ptr[i].Now_Counter.byte[1];
			tmp16.byte[0] = dev_ptr[i].Now_Counter.byte[0];
			replace_txt(DeviceData_tmp, php_key_13, hex_to_val_ascii(tmp16.word));	//	D14	Dev輪詢時間 counter
			tmp16.byte[1] = 0;
			tmp16.byte[0] = dev_ptr[i].MISS_NODE;
			replace_txt(DeviceData_tmp, php_key_14, hex_to_val_ascii(tmp16.word));	//	D15	Missnode
			tmp16.byte[1] = 0;
			tmp16.byte[0] = dev_ptr[i].Input_New_Data;
			replace_txt(DeviceData_tmp, php_key_15, hex_to_val_ascii(tmp16.word));	//	D16	是否有新資料
			if(dev_ptr[i].cmd_buf_to_clear_mysql_Flag == true)
			{
				replace_txt(DeviceData_tmp, php_key_16, "0002");					//	D17 清除CMD指令，將不再抓下來
				dev_ptr[i].cmd_buf_to_clear_mysql_Flag = false;			//	下此不再處理此需求
			}
			else
			{
				replace_txt(DeviceData_tmp, php_key_16, "9999");					//	D17 寫入9999，會存入MySQL Data，但不會影響CMD資料
			}
			replace_txt(DeviceData_tmp, php_key_17, hex_to_val_ascii(0));			//	D18
			replace_txt(DeviceData_tmp, php_key_18, hex_to_val_ascii(0));			//	D19
			replace_txt(DeviceData_tmp, php_key_19, hex_to_val_ascii(0));			//	D20
			
			dev_ptr[i].Input_New_Data = 0x00;
			
			strcat(GET_Update_tmp, DeviceData_tmp);
			strcat(GET_Update_tmp, "_");
			My_WiFi.Update_Count++;
			dev_value[i][3] = 0x00;
			dev_value[i][4] = 0x00;
			dev_value[i][5] = 0x00;
			dev_value[i][6] = 0x00;
			
		}
		if(My_WiFi.Update_Count == 3)
		{
			break;
		}
	}
	
	//=====================================================
	//	檢查是否還有沒傳完的資料
	My_WiFi.Update_Next_ptr_Flag = false;
	My_WiFi.Update_ptr = My_WiFi.Update_ptr + 1;
	for(unsigned char i = My_WiFi.Update_ptr; i < Dev_Item_MAX; i++)
	{
		Source.byte[0] = dev_value[i][3];
		Source.byte[1] = dev_value[i][4];
		Source.byte[2] = dev_value[i][5];
		Source.byte[3] = dev_value[i][6];
		if(dev_ptr[i].Dev_En_Flag == Dev_EN && Source.dword != 0x00000000)
		{
			My_WiFi.Update_Next_ptr_Flag = true;	// 還有沒傳完的資料
		}
	}
}