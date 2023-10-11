﻿/*
 * OV.c
 *
 * Created: 2023/10/6 下午 07:23:35
 *  Author: papay
 */ 


#include <asf.h>
#include <stdio.h>
#include "myDefine.h"




void Load_eOT(Item_Dev_t *tmpItem)
{
	char str[255];
	UINT16u_t tmp16;
	char k, i;
	for(i = 0; i < 16; i++)	mGateway.uEMS.uSetting.OT.Buffer[i] = '\0';
	tmp16.word = eMAP_eOT;
	tmpItem->Read->Write_Buf[0]	= tmp16.byte[0];
	tmpItem->Read->Write_length	= 1;	// 寫1個位置，Reg_Addr
	tmpItem->Read->Read_length	= 16;	// 讀16個位置
	
	bool tmpFlag = IIC_Read_packet(*tmpItem->Read);
	
	for(i = 0; i < 16; i++)	mGateway.uEMS.uSetting.OT.Buffer[i] = *(tmpItem->Read->Read_Buf+i);
	sprintf(str, "\r Load [mGateway.uEMS.uSetting.OT_Buffer] = [%s]", mGateway.uEMS.uSetting.OT.Buffer);
	uart_str_COM(Debug_COM, str);
	
	if(tmpFlag == true)	show_read_eeprom_ok(true);	else	show_read_eeprom_ok(false);
}



void Save_eOT(Item_Dev_t *tmpItem)
{
	UINT16u_t tmp16;
	char str[255];
	char i;

	tmp16.word = eMAP_eOT;
	tmpItem->Write->Write_Buf[0] = tmp16.byte[0];
	for(i = 0; i < 16; i++)	tmpItem->Write->Write_Buf[1+i] = mGateway.uEMS.uSetting.OT.userKeyin_Buffer[i];
	tmpItem->Write->Write_length = 17;	// 寫1個位置，Reg_Addr(1 byte) + data(16 byte) = 17 byte
	sprintf(str, "\r Save [mGateway.uEMS.uSetting.OT_Buffer] = [%s]", mGateway.uEMS.uSetting.OT.userKeyin_Buffer);
	uart_str_COM(Debug_COM, str);
		
	if(IIC_Write(*tmpItem->Write) == true)	show_eeprom_ok(true);	else	show_eeprom_ok(false);
	delay_ms(10);
}


//================================================================

void Load_ewOT(Item_Dev_t *tmpItem)
{
	char str[255];
	UINT16u_t tmp16;
	char k, i;
	for(i = 0; i < 16; i++)	mGateway.uEMS.uSetting.wOT.Buffer[i] = '\0';
	tmp16.word = eMAP_ewOT;
	tmpItem->Read->Write_Buf[0]	= tmp16.byte[0];
	tmpItem->Read->Write_length	= 1;	// 寫1個位置，Reg_Addr
	tmpItem->Read->Read_length	= 16;	// 讀16個位置
	
	bool tmpFlag = IIC_Read_packet(*tmpItem->Read);
	
	
	for(i = 0; i < 16; i++)	mGateway.uEMS.uSetting.wOT.Buffer[i] = *(tmpItem->Read->Read_Buf+i);
	sprintf(str, "\r Load [mGateway.uEMS.uSetting.wOT_Buffer] = [%s]", mGateway.uEMS.uSetting.wOT.Buffer);
	uart_str_COM(Debug_COM, str);
	
	if(tmpFlag == true)	show_read_eeprom_ok(true);	else	show_read_eeprom_ok(false);
}


void Save_ewOT(Item_Dev_t *tmpItem)
{
	UINT16u_t tmp16;
	char str[255];
	char i;

	tmp16.word = eMAP_ewOT;
	tmpItem->Write->Write_Buf[0] = tmp16.byte[0];
	for(i = 0; i < 16; i++)	tmpItem->Write->Write_Buf[1+i] = mGateway.uEMS.uSetting.wOT.userKeyin_Buffer[i];
	tmpItem->Write->Write_length = 17;	// 寫1個位置，Reg_Addr(1 byte) + data(16 byte) = 17 byte
	sprintf(str, "\r Save [mGateway.uEMS.uSetting.wOT_Buffer] = [%s]", mGateway.uEMS.uSetting.wOT.userKeyin_Buffer);
	uart_str_COM(Debug_COM, str);
	
	if(IIC_Write(*tmpItem->Write) == true)	show_eeprom_ok(true);	else	show_eeprom_ok(false);
	delay_ms(10);
}