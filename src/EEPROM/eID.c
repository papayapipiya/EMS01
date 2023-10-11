
#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


/*
	EPPROM 1M
void Load_eID_Flag(Item_Dev_t *tmpItem)
{
	char str[50];
	UINT16u_t tmp16;
	tmp16.word = eMAP_eID;
	tmpItem->Read->Write_Buf[0]	= tmp16.byte[1];
	tmpItem->Read->Write_Buf[1]	= tmp16.byte[0];
	tmpItem->Read->Write_length	= 2;	// 寫兩個位置，Reg_Addr
	tmpItem->Read->Read_length	= 4;	// 讀一個位置
	IIC_Read_packet(*tmpItem->Read);
	
	mGateway.uEMS.uModule.uID.byte[0] = *(tmpItem->Read->Read_Buf);
	mGateway.uEMS.uModule.uID.byte[1] = *(tmpItem->Read->Read_Buf+1);
	mGateway.uEMS.uModule.uID.byte[2] = *(tmpItem->Read->Read_Buf+2);
	mGateway.uEMS.uModule.uID.byte[3] = *(tmpItem->Read->Read_Buf+3);
	
	sprintf(str, "\r Load [mGateway.uEMS.uModule.uID] = 0x%08X", mGateway.uEMS.uModule.uID.dword);
	uart_str_COM(Debug_COM, str);
}
*/


void Load_eID_Flag(Item_Dev_t *tmpItem)
{
	char str[50];
	UINT16u_t tmp16;
	tmp16.word = eMAP_eID;
	tmpItem->Read->Write_Buf[0]	= tmp16.byte[0];
	//tmpItem->Read->Write_Buf[1]	= tmp16.byte[0];
	tmpItem->Read->Write_length	= 1;	// 寫兩個位置，Reg_Addr
	tmpItem->Read->Read_length	= 9;	// 讀一個位置
	bool tmpFlag = IIC_Read_packet(*tmpItem->Read);
	mGateway.uEMS.uModule.uID.Buffer[0] = *(tmpItem->Read->Read_Buf);
	mGateway.uEMS.uModule.uID.Buffer[1] = *(tmpItem->Read->Read_Buf+1);
	mGateway.uEMS.uModule.uID.Buffer[2] = *(tmpItem->Read->Read_Buf+2);
	mGateway.uEMS.uModule.uID.Buffer[3] = *(tmpItem->Read->Read_Buf+3);
	mGateway.uEMS.uModule.uID.Buffer[4] = *(tmpItem->Read->Read_Buf+4);
	mGateway.uEMS.uModule.uID.Buffer[5] = *(tmpItem->Read->Read_Buf+5);
	mGateway.uEMS.uModule.uID.Buffer[6] = *(tmpItem->Read->Read_Buf+6);
	mGateway.uEMS.uModule.uID.Buffer[7] = *(tmpItem->Read->Read_Buf+7);
	mGateway.uEMS.uModule.uID.Buffer[8] = '\0';
	sprintf(str, "\r Load [mGateway.uEMS.uModule.uID] = 0x%s", mGateway.uEMS.uModule.uID.Buffer);
	uart_str_COM(Debug_COM, str);
	
	if(tmpFlag == true)	show_read_eeprom_ok(true);	else	show_read_eeprom_ok(false);
}



void Save_eID_Flag(Item_Dev_t *tmpItem)
{
	UINT16u_t tmp16;
	char str[50];

	tmp16.word = eMAP_eID ;
	tmpItem->Write->Write_Buf[0] = tmp16.byte[0];
	//tmpItem->Write->Write_Buf[1] = tmp16.byte[0];
	
	tmpItem->Write->Write_Buf[1] = mGateway.uEMS.uModule.uID.userKeyin_Buffer[0];
	tmpItem->Write->Write_Buf[2] = mGateway.uEMS.uModule.uID.userKeyin_Buffer[1];
	tmpItem->Write->Write_Buf[3] = mGateway.uEMS.uModule.uID.userKeyin_Buffer[2];
	tmpItem->Write->Write_Buf[4] = mGateway.uEMS.uModule.uID.userKeyin_Buffer[3];
	tmpItem->Write->Write_Buf[5] = mGateway.uEMS.uModule.uID.userKeyin_Buffer[4];
	tmpItem->Write->Write_Buf[6] = mGateway.uEMS.uModule.uID.userKeyin_Buffer[5];
	tmpItem->Write->Write_Buf[7] = mGateway.uEMS.uModule.uID.userKeyin_Buffer[6];
	tmpItem->Write->Write_Buf[8] = mGateway.uEMS.uModule.uID.userKeyin_Buffer[7];
	mGateway.uEMS.uModule.uID.userKeyin_Buffer[8] = '\0';
	tmpItem->Write->Write_Buf[9] = mGateway.uEMS.uModule.uID.userKeyin_Buffer[8];
	tmpItem->Write->Write_length = 10;	// 寫兩個位置，Reg_Addr(2) + data(byte)
	
	/*
	sprintf(str, "\r Save [i2c] = 0x%02X, 0x%02X, 0x%02X, Len(0x%02X),",
	tmpItem->Write->Write_Buf[0],tmpItem->Write->Write_Buf[1],tmpItem->Write->Write_Buf[2],tmpItem->Write->Write_length);
	uart_str_COM(Debug_COM, str);
	*/
	
	sprintf(str, "\r Save [mGateway.uEMS.uModule.uID] = 0x%s", mGateway.uEMS.uModule.uID.userKeyin_Buffer);
	uart_str_COM(Debug_COM, str);
	
	if(IIC_Write(*tmpItem->Write) == true)
		show_eeprom_ok(true);
	else
		show_eeprom_ok(false);
	delay_ms(100);
}


