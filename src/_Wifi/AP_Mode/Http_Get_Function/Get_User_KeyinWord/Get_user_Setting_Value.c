/*
 * GetuserOT.c
 *
 * Created: 2023/10/7 上午 09:48:52
 *  Author: papay
 */ 
#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "myDefine.h"







bool Check_OT_is_Need_Save(void)
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


void Save_user_Setting_Value(uSetting_item_Dev_t *obj, Item_Dev_t *tmpItem)
{
	char str[100];
	//==============================
	// 判斷是否要儲存
	
	if(obj->f_error == true)
	{
		sprintf(str, "\r Value (hex) is not 0~65535, pass..");
		uart_str_COM(Debug_COM, str);
		return;
	}
	

	obj->Value = obj->tmpValue;
	if(tmpItem == &mGateway.uEMS.uSetting.eOT)			Save_eOT(mGateway.uEMS.uSetting.eOT);
	if(tmpItem == &mGateway.uEMS.uSetting.eUT)			Save_eUT(mGateway.uEMS.uSetting.eUT);
	if(tmpItem == &mGateway.uEMS.uSetting.ewOT)			Save_ewOT(mGateway.uEMS.uSetting.ewOT);
	if(tmpItem == &mGateway.uEMS.uSetting.ewUT)			Save_ewUT(mGateway.uEMS.uSetting.ewUT);
	if(tmpItem == &mGateway.uEMS.uSetting.eTolerance)	Save_eTolerance(mGateway.uEMS.uSetting.eTolerance);



}

void Get_user_Setting_Value(void)
{
	
	//+IPD,1,586:GET /?_SSID=rorohome&_PASS=0936818176&_OT=1234&_UT=ABCD&_wOT=5678&_wUT=EFGH&_Tol=%21%40%23%24
	uart_str_COM(Debug_COM, "\r\r # # # # # # OT # # # # # #");	
	mGateway.uEMS.uSetting.OT.ret = 0;
	mGateway.uEMS.uSetting.OT.ret =  findKeyword(mGateway.Wifi->Input_buffer, "&_OT=");
	mGateway.uEMS.uSetting.OT.ret_end =  findKeyword(mGateway.Wifi->Input_buffer, "&_UT");
	mGateway.uEMS.uSetting.OT.ret = mGateway.uEMS.uSetting.OT.ret + 5;
	getSettingString(&mGateway.uEMS.uSetting.OT);
	Save_user_Setting_Value(&mGateway.uEMS.uSetting.OT, &mGateway.uEMS.uSetting.eOT);
	
	
	uart_str_COM(Debug_COM, "\r\r # # # # # # UT # # # # # #");
	mGateway.uEMS.uSetting.UT.ret = 0;
	mGateway.uEMS.uSetting.UT.ret =  findKeyword(mGateway.Wifi->Input_buffer, "&_UT=");
	mGateway.uEMS.uSetting.UT.ret_end =  findKeyword(mGateway.Wifi->Input_buffer, "&_wOT");
	mGateway.uEMS.uSetting.UT.ret = mGateway.uEMS.uSetting.UT.ret + 5;
	getSettingString(&mGateway.uEMS.uSetting.UT);
	Save_user_Setting_Value(&mGateway.uEMS.uSetting.UT, &mGateway.uEMS.uSetting.eUT);
	
	uart_str_COM(Debug_COM, "\r\r # # # # # # wOT # # # # # #");
	mGateway.uEMS.uSetting.wOT.ret = 0;
	mGateway.uEMS.uSetting.wOT.ret =  findKeyword(mGateway.Wifi->Input_buffer, "&_wOT=");
	mGateway.uEMS.uSetting.wOT.ret_end =  findKeyword(mGateway.Wifi->Input_buffer, "&_wUT");
	mGateway.uEMS.uSetting.wOT.ret = mGateway.uEMS.uSetting.wOT.ret + 6;
	getSettingString(&mGateway.uEMS.uSetting.wOT);
	Save_user_Setting_Value(&mGateway.uEMS.uSetting.wOT, &mGateway.uEMS.uSetting.ewOT);
	
	uart_str_COM(Debug_COM, "\r\r # # # # # # wUT # # # # # #");
	mGateway.uEMS.uSetting.wUT.ret = 0;
	mGateway.uEMS.uSetting.wUT.ret =  findKeyword(mGateway.Wifi->Input_buffer, "&_wUT=");
	mGateway.uEMS.uSetting.wUT.ret_end =  findKeyword(mGateway.Wifi->Input_buffer, "&_Tol=");
	mGateway.uEMS.uSetting.wUT.ret = mGateway.uEMS.uSetting.wUT.ret + 6;
	getSettingString(&mGateway.uEMS.uSetting.wUT);
	Save_user_Setting_Value(&mGateway.uEMS.uSetting.wUT, &mGateway.uEMS.uSetting.ewUT);
	
	uart_str_COM(Debug_COM, "\r\r # # # # # # Tolerance # # # # # #");
	mGateway.uEMS.uSetting.Tolerance.ret = 0;
	mGateway.uEMS.uSetting.Tolerance.ret =  findKeyword(mGateway.Wifi->Input_buffer, "&_Tol=");
	mGateway.uEMS.uSetting.Tolerance.ret_end =  findKeyword(mGateway.Wifi->Input_buffer, "&_ID=");
	mGateway.uEMS.uSetting.Tolerance.ret = mGateway.uEMS.uSetting.Tolerance.ret + 6;
	getSettingString(&mGateway.uEMS.uSetting.Tolerance);
	Save_user_Setting_Value(&mGateway.uEMS.uSetting.Tolerance, &mGateway.uEMS.uSetting.eTolerance);
}

