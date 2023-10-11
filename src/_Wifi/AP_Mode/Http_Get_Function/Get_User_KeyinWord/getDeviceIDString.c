/*
 * getDeviceIDString.c
 *
 * Created: 2023/10/8 下午 05:46:45
 *  Author: papay
 */ 
#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "myDefine.h"

void getDeviceIDString(uModule_Dev_t *obj)
{
	unsigned char str[200];
	
	uart_str_COM(Debug_COM, "\r\r # # # # # # Device ID (password) # # # # # #");
	obj->uPassword.ret = 0;
	obj->uPassword.ret =  findKeyword(mGateway.Wifi->Input_buffer, "&_IDpass=");
	obj->uPassword.ret_end =  findKeyword(mGateway.Wifi->Input_buffer, "HTTP/");
	obj->uPassword.ret = obj->uPassword.ret + 9;
	sprintf(str, "\r --> mGateway.uEMS.uModule.uPassword.ret = %d, ret_end = %d", obj->uPassword.ret, obj->uPassword.ret_end);
	uart_str_COM(Debug_COM, str);
	obj->uPassword.userKeyin_Buffer[0] = mGateway.Wifi->Input_buffer[obj->uPassword.ret];
	obj->uPassword.userKeyin_Buffer[1] = mGateway.Wifi->Input_buffer[obj->uPassword.ret + 1];
	obj->uPassword.userKeyin_Buffer[2] = mGateway.Wifi->Input_buffer[obj->uPassword.ret + 2];
	obj->uPassword.userKeyin_Buffer[3] = mGateway.Wifi->Input_buffer[obj->uPassword.ret + 3];
	obj->uPassword.userKeyin_Buffer[4] = '\0';
	sprintf(str, "\r obj->uPassword.userKeyin_Buffer = [%s]", obj->uPassword.userKeyin_Buffer);
	uart_str_COM(Debug_COM, str);
	
	if(strcmp("1234", obj->uPassword.userKeyin_Buffer) == 0)
	{
		uart_str_COM(Debug_COM, "\r Pass word OK");
	}
	else
	{
		uart_str_COM(Debug_COM, "\r Pass word NG");
		return false;
	}
	
	//=============================================================================================================
	uart_str_COM(Debug_COM, "\r\r # # # # # # Device ID # # # # # #");
	obj->uID.ret = 0;
	obj->uID.ret =  findKeyword(mGateway.Wifi->Input_buffer, "&_ID=");
	obj->uID.ret_end =  findKeyword(mGateway.Wifi->Input_buffer, "&_IDpass=");
	obj->uID.ret = obj->uID.ret + 5;
	sprintf(str, "\r --> mGateway.uEMS.uModule.uID.ret = %d, ret_end = %d", obj->uID.ret, obj->uID.ret_end);
	uart_str_COM(Debug_COM, str);
		
	obj->uID.userKeyin_Buffer[0] = mGateway.Wifi->Input_buffer[obj->uID.ret];
	obj->uID.userKeyin_Buffer[1] = mGateway.Wifi->Input_buffer[obj->uID.ret + 1];
	obj->uID.userKeyin_Buffer[2] = mGateway.Wifi->Input_buffer[obj->uID.ret + 2];
	obj->uID.userKeyin_Buffer[3] = mGateway.Wifi->Input_buffer[obj->uID.ret + 3];
	obj->uID.userKeyin_Buffer[4] = mGateway.Wifi->Input_buffer[obj->uID.ret + 4];
	obj->uID.userKeyin_Buffer[5] = mGateway.Wifi->Input_buffer[obj->uID.ret + 5];
	obj->uID.userKeyin_Buffer[6] = mGateway.Wifi->Input_buffer[obj->uID.ret + 6];
	obj->uID.userKeyin_Buffer[7] = mGateway.Wifi->Input_buffer[obj->uID.ret + 7];
	obj->uID.userKeyin_Buffer[8] = '\0';
	
	sprintf(str, "\r obj->uID.userKeyin_Buffer and Buffer = [%s], [%s]", obj->uID.userKeyin_Buffer, obj->uID.Buffer);
	uart_str_COM(Debug_COM, str);
	
	if(strcmp(obj->uID.Buffer, obj->uID.userKeyin_Buffer) == 0)
	{
		uart_str_COM(Debug_COM, "\r Device ID = Input Device ID");
		return false;
	}
	else
	{
		uart_str_COM(Debug_COM, "\r Device ID != Input Device ID, [save]");
		Save_eID_Flag(mGateway.uEMS.uModule.eID);
		Load_eID_Flag(mGateway.uEMS.uModule.eID);
		return true;
	}
	
	

	

}