/*
 * GetuserPW.c
 *
 * Created: 2023/10/7 上午 09:29:55
 *  Author: papay
 */ 
#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "myDefine.h"


bool CheckPASSWORDisNeedSave(void)
{
	uTXByte(Debug_COM, 0x0d);
	uart_str_COM(Debug_COM, &mGateway.Wifi->PASSWORD_Buffer);
	uart_str_COM(Debug_COM, ", ");
	uart_str_COM(Debug_COM, &mGateway.Wifi->userKeyin_PASSWORD_Buffer);
	if(strcmp(mGateway.Wifi->PASSWORD_Buffer, mGateway.Wifi->userKeyin_PASSWORD_Buffer) == 0)
	{
		uart_str_COM(Debug_COM, "\rPASSWORD = Input PASSWORD");
		return false;
	}
	else
	{
		uart_str_COM(Debug_COM, "\rPASSWORD != Input rPASSWORD, [save]");
		Save_ePW_Flag(mGateway.Wifi->ePW);
		Load_ePW_Flag(mGateway.Wifi->ePW);
		return true;
	}
}


void GetuserPW(void)
{
	unsigned char str[2048];
	//+IPD,1,586:GET /?_SSID=rorohome&_PASS=0936818176&_OT=1234&_UT=ABCD&_wOT=5678&_wUT=EFGH&_Tol=%21%40%23%24
	//範例：_SSID,rorohome_PASS,0936818176*
	unsigned char *ret;
	unsigned char *ret_end;
	unsigned char SSID_Count, Password_Count ;
	unsigned char k;
	
	uart_str_COM(Debug_COM, "\r # # # # # # # # # # # # # #");
	
	//=======================================================
	// 開始擷取PASS的字串
	ret = 0;
	ret	= strstr(My_WiFi.Input_buffer, "_PASS");
	ret_end = strstr(My_WiFi.Input_buffer, "&_OT");
	
	// 清除PASSWORD的BUF資料
	for(unsigned char j = 0; j < SSID_PASSWORD_Max; j++)	mGateway.Wifi->userKeyin_PASSWORD_Buffer[j] = 0;
	
	// 開始擷取PASSWORD的字串
	k = 0;
	ret = ret + 6;
	uart_str_COM(Debug_COM, "\r --> ");
	for(unsigned int i = ret; i < ret_end; i++)
	{
		mGateway.Wifi->userKeyin_PASSWORD_Buffer[k] = *(ret + k);
		uTXByte(Debug_COM, mGateway.Wifi->userKeyin_PASSWORD_Buffer[k]);
		k++;
	}
	
	//	2023/10/05	最後一個位置補'\0'，列印的時候會用到
	mGateway.Wifi->userKeyin_PASSWORD_Buffer[k] = '\0';
	
	//	列印字串長度
	sprintf(str, " k = %d, Lenght = %d\r", k, strlen(&mGateway.Wifi->userKeyin_PASSWORD_Buffer));
	uart_str_COM(Debug_COM, str);
	
	//=============================================
	//	列印PASSWORD字串
	uart_str_COM(Debug_COM, " --> PW Text:");
	for(unsigned int i = 0; i < k; i++)	uTXByte(Debug_COM, mGateway.Wifi->userKeyin_PASSWORD_Buffer[i]);
	
	// 列印PASSWORD HEX格式
	uart_str_COM(Debug_COM, " (");
	for(unsigned int i = 0; i < k; i++)
	{
		sprintf(str, "%X ", mGateway.Wifi->userKeyin_PASSWORD_Buffer[i]);
		uart_str_COM(Debug_COM, str);
	}
	uart_str_COM(Debug_COM, ")");
}
