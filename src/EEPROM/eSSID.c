/*
 * eSSID.c
 *
 * Created: 2023/10/4 下午 06:03:50
 *  Author: papay
 */ 



#include <asf.h>
#include <stdio.h>
#include "myDefine.h"




void Load_eSSID_Flag(Item_Dev_t *tmpItem)
{
	char str[255];
	UINT16u_t tmp16;
	char k, i;
	for(i = 0; i < 64; i++)	mGateway.Wifi->SSID_Buffer[i] = '\0';
	
	tmp16.word = eMAP_eSSID;
	tmpItem->Read->Write_Buf[0]	= tmp16.byte[0];
	tmpItem->Read->Write_length	= 1;	// 寫兩個位置，Reg_Addr
	tmpItem->Read->Read_length	= 64;	// 讀一個位置
	bool tmpFlag = IIC_Read_packet(*tmpItem->Read);
	for(i = 0; i < 64; i++)
	{
		mGateway.Wifi->SSID_Buffer[i] = *(tmpItem->Read->Read_Buf+i);
	}
	sprintf(str, "\r Load [mGateway.Wifi->SSID_Buffer] = [%s]", mGateway.Wifi->SSID_Buffer);
	uart_str_COM(Debug_COM, str);
	if(tmpFlag == true)	show_read_eeprom_ok(true);	else	show_read_eeprom_ok(false);
}



void Save_eSSID_Flag(Item_Dev_t *tmpItem)
{
	UINT16u_t tmp16;
	char str[255];
	char k = 0;
	char i;
	char TempStr[17];
	for(char j = 0; j < 4; j++)
	{
		for(i = 0; i < 17; i++)	TempStr[i] = '\0';
		k = j * 16;
		tmp16.word = eMAP_eSSID + k;
		tmpItem->Write->Write_Buf[0] = tmp16.byte[0];
		for(i = 0; i < 16; i++)
		{
			tmpItem->Write->Write_Buf[1+i] = mGateway.Wifi->userKeyin_SSID_Buffer[i + k];
			TempStr[i] = mGateway.Wifi->userKeyin_SSID_Buffer[i + k];
		}
		tmpItem->Write->Write_length = 17;	// 寫兩個位置，Reg_Addr(2) + data(byte)
		sprintf(str, "\r Save [mGateway.Wifi->SSID_Buffer][%02d] = [%s]", k, TempStr);//tmpItem->Write->Write_Buf);
		uart_str_COM(Debug_COM, str);
		
		if(IIC_Write(*tmpItem->Write) == true)
			show_eeprom_ok(true);
		else
			show_eeprom_ok(false);
		delay_ms(10);
	}
	
}

