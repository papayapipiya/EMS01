/*
 * GetuserSSID.c
 *
 * Created: 2023/10/7 上午 09:24:30
 *  Author: papay
 */ 

#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "myDefine.h"

bool CheckSSIDisNeedSave(void)
{
	uTXByte(Debug_COM, 0x0d);
	uart_str_COM(Debug_COM, &mGateway.Wifi->SSID_Buffer);
	uart_str_COM(Debug_COM, ", ");
	uart_str_COM(Debug_COM, &mGateway.Wifi->userKeyin_SSID_Buffer);
	if(strcmp(mGateway.Wifi->SSID_Buffer, mGateway.Wifi->userKeyin_SSID_Buffer) == 0)
	{
		uart_str_COM(Debug_COM, "\rSSID = Input SSID");
		return false;
	}
	else
	{
		uart_str_COM(Debug_COM, "\rSSID != Input SSID, [save]");
		Save_eSSID_Flag(mGateway.Wifi->eSSID);
		Load_eSSID_Flag(mGateway.Wifi->eSSID);
		return true;
	}
}


void GetuserSSID(void)
{
	unsigned char str[2048];
	//+IPD,1,586:GET /?_SSID=rorohome&_PASS=0936818176&_OT=1234&_UT=ABCD&_wOT=5678&_wUT=EFGH&_Tol=%21%40%23%24
	//範例：_SSID,rorohome_PASS,0936818176*
	unsigned char *ret;
	unsigned char *ret_end;
	unsigned char SSID_Count, Password_Count ;
	unsigned char k;
	
	uart_str_COM(Debug_COM, "\r # # # # # # # # # # # # # #");
	ret = 0;
	ret		= strstr(mGateway.Wifi->Input_buffer, "_SSID");
	ret_end = strstr(mGateway.Wifi->Input_buffer, "&_PASS");

	
	//=============================================
	// 檢查_SSID與_PASS的關鍵字位置、User Connect_Number的編號
	uart_str_COM(Debug_COM, "\r_SSID addr:");
	uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(ret));
	uart_str_COM(Debug_COM, ", _PASS addr:");
	uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(ret_end));
	uart_str_COM(Debug_COM, ", User Connect_Number:");
	uTXByte(Debug_COM, mGateway.Wifi->target_Number);
	uTXByte(Debug_COM, 0x0d);
	
	//=============================================
	// 清除SSID的BUF資料
	for(unsigned char j = 0; j < SSID_PASSWORD_Max; j++)	mGateway.Wifi->userKeyin_SSID_Buffer[j] = '\0';
	
	// 開始擷取SSID的字串
	k = 0;
	ret = ret + 6;
	uart_str_COM(Debug_COM, " --> ");
	for(unsigned int i = ret; i < ret_end; i++)
	{
		mGateway.Wifi->userKeyin_SSID_Buffer[k] = *(ret + k);
		uTXByte(Debug_COM, mGateway.Wifi->userKeyin_SSID_Buffer[k]);
		k++;
	}
	//	2023/10/05	最後一個位置補'\0'，列印的時候會用到
	mGateway.Wifi->userKeyin_SSID_Buffer[k] = '\0';
	
	// 列印字串長度
	sprintf(str, " k = %d, Lenght = %d\r", k, strlen(&mGateway.Wifi->userKeyin_SSID_Buffer));
	uart_str_COM(Debug_COM, str);
	
	// 列印SSID字串
	uart_str_COM(Debug_COM, " --> SSID Text:");
	for(unsigned int i = 0; i < k; i++)	uTXByte(Debug_COM, mGateway.Wifi->userKeyin_SSID_Buffer[i]);
	
	// 列印SSID HEX格式
	uart_str_COM(Debug_COM, " (");
	for(unsigned int i = 0; i < k; i++)
	{
		sprintf(str, "%X ", mGateway.Wifi->userKeyin_SSID_Buffer[i]);
		uart_str_COM(Debug_COM, str);
	}
	uart_str_COM(Debug_COM, ")");
	
}