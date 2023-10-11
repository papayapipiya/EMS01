/*
 * getSettingString.c
 *
 * Created: 2023/10/7 下午 03:39:22
 *  Author: papay
 */ 

#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "myDefine.h"



void userKeyin_Buffer_or_Buffer_to_value(bool tmpSelect, uSetting_item_Dev_t *obj)
{
	unsigned char str[100];
	unsigned int tmpCheck;
	
	//===================================
	// 判斷 tmpSelect 的數值
	//  -> uSelect_userKeyin_Buffer = Html過來的資料
	//  -> uSelect_Buffer			= 開機的時候，EEPROM過來的資料
	//===================================
	// 文字轉成數字
	if(tmpSelect == uSelect_userKeyin_Buffer)
		tmpCheck = ascii_check_0_and_9_and_return_hex(obj->userKeyin_Buffer[0]);
	else
		tmpCheck = ascii_check_0_and_9_and_return_hex(obj->Buffer[0]);
	if(tmpCheck != 0xff)	{obj->tmpValue += tmpCheck * 1000;} else {obj->f_error = true;}

	if(tmpSelect == uSelect_userKeyin_Buffer)
		tmpCheck = ascii_check_0_and_9_and_return_hex(obj->userKeyin_Buffer[1]);
	else
		tmpCheck = ascii_check_0_and_9_and_return_hex(obj->Buffer[1]);
	if(tmpCheck != 0xff)	{obj->tmpValue += tmpCheck * 100;} else {obj->f_error = true;}

	if(tmpSelect == uSelect_userKeyin_Buffer)
		tmpCheck = ascii_check_0_and_9_and_return_hex(obj->userKeyin_Buffer[2]);
	else
		tmpCheck = ascii_check_0_and_9_and_return_hex(obj->Buffer[2]);
	if(tmpCheck != 0xff)	{obj->tmpValue += tmpCheck * 10;} else {obj->f_error = true;}

	if(tmpSelect == uSelect_userKeyin_Buffer)
		tmpCheck = ascii_check_0_and_9_and_return_hex(obj->userKeyin_Buffer[3]);
	else
		tmpCheck = ascii_check_0_and_9_and_return_hex(obj->Buffer[3]);
	if(tmpCheck != 0xff)	{obj->tmpValue += tmpCheck;} else {obj->f_error = true;}
	
	if(tmpSelect == uSelect_userKeyin_Buffer) return;

	//============================
	// 如果是eeprom過來的資料 tmpSelect == uSelect_Buffer
	// 且eeprom的資料是錯誤的，不儲存資料到value
	if(tmpCheck == 0xff) 
	{
		obj->Value = 0;
		sprintf(str, "\r --> data error, than obj->Value = %d", obj->Value);
		uart_str_COM(Debug_COM, str);
		return;
	}
		
	//============================
	//如果eeprom的資料是正確的，儲存資料到value
	obj->Value = obj->tmpValue;
	sprintf(str, "\r --> obj->Value = %d", obj->Value);
	uart_str_COM(Debug_COM, str);
	
}




void getSettingString(uSetting_item_Dev_t *obj)
{
	unsigned char str[2048];
	unsigned int tmpCheck;
	unsigned char k;
	
	sprintf(str, "\r --> ret = %d, ret_end = %d, payload = ", obj->ret, obj->ret_end);
	uart_str_COM(Debug_COM, str);
	
	// 清除OT的BUF資料
	for(unsigned char j = 0; j < Setting_Max; j++)	obj->userKeyin_Buffer[j] = 0;
	
	if(obj->ret == -1) return;
	if(obj->ret_end == -1) return;
	
	// 開始擷取PASSWORD的字串
	k = 0;
	for(unsigned int i = obj->ret; i < obj->ret_end; i++)
	{
		obj->userKeyin_Buffer[k] = mGateway.Wifi->Input_buffer[i];
		uTXByte(Debug_COM, obj->userKeyin_Buffer[k]);
		k++;
	}
	
	//	2023/10/05	最後一個位置補'\0'，列印的時候會用到
	obj->userKeyin_Buffer[k] = '\0';
	
	//=============================================
	//	列印字串長度, 列印OT字串
	sprintf(str, " k = %d, Lenght = %d", k, strlen(&obj->userKeyin_Buffer));
	uart_str_COM(Debug_COM, str);
	
	uart_str_COM(Debug_COM, "\r get Text:");
	for(unsigned int i = 0; i < k; i++)	uTXByte(Debug_COM, obj->userKeyin_Buffer[i]);
		
	// 列印OT HEX格式
	uart_str_COM(Debug_COM, " (");
	for(unsigned int i = 0; i < k; i++)
	{
		sprintf(str, "%X ", obj->userKeyin_Buffer[i]);
		uart_str_COM(Debug_COM, str);
	}
	uart_str_COM(Debug_COM, ")");
	
	
	
	//==============================
	// 文字補0 [ 12 --> 0012 ]
	string_to_4str(obj);
	
	obj->f_error = false;
	obj->tmpValue = 0;
	

	userKeyin_Buffer_or_Buffer_to_value(uSelect_userKeyin_Buffer, obj);

}