
#include "myDefine.h"

//=================================================
#ifdef Dev_Wifi
//=================================================












void Get_User_KeyinWord_SSID_PW(void)
{
	unsigned char str[2048];
	
	//範例：+IPD,0,621:GET /?_SSID=rorohome&_PASS=0936818176&_OT=1234&_UT=1&_wOT=3000&_wUT=2&_Tol=0&_ID=AD54F78C&_IDpass=1234 HTTP/1.1
	unsigned char *ret;
	unsigned char *ret_end;
	unsigned char *target;
	unsigned char SSID_Count, Password_Count ;
	unsigned char k, z;


	//2023-10-07
	//找出要返回傳的編號
	target	= strstr(My_WiFi.Input_buffer, "+IPD,");
	mGateway.Wifi->target_Number = *(target+5);
	
	//=======================================================
	//	確認關鍵字
	//=======================================================
	if(strstr(mGateway.Wifi->Input_buffer, "_SSID") == 0)		{uart_str_COM(Debug_COM, "\r => No SSID, return..");			return;}
	if(strstr(mGateway.Wifi->Input_buffer, "&_PASS") == 0)		{uart_str_COM(Debug_COM, "\r => No Password, return..");		return;}
	if(strstr(mGateway.Wifi->Input_buffer, "&_OT") == 0)		{uart_str_COM(Debug_COM, "\r => No OT, return..");				return;}
	if(strstr(mGateway.Wifi->Input_buffer, "&_UT") == 0)		{uart_str_COM(Debug_COM, "\r => No UT, return..");				return;}
	if(strstr(mGateway.Wifi->Input_buffer, "&_wOT") == 0)		{uart_str_COM(Debug_COM, "\r => No wOT, return..");				return;}
	if(strstr(mGateway.Wifi->Input_buffer, "&_wUT") == 0)		{uart_str_COM(Debug_COM, "\r => No wUT, return..");				return;}
	if(strstr(mGateway.Wifi->Input_buffer, "&_Tol") == 0)		{uart_str_COM(Debug_COM, "\r => No Tol, return..");				return;}
	if(strstr(mGateway.Wifi->Input_buffer, "&_ID=") == 0)		{uart_str_COM(Debug_COM, "\r => No ID, return..");				return;}
	if(strstr(mGateway.Wifi->Input_buffer, "&_IDpass=") == 0)	{uart_str_COM(Debug_COM, "\r => No IDpass, return..");			return;}

	//=================================================
	//	判斷SSID與Password
	GetuserSSID();
	mGateway.Wifi->flag.f_ssid_is_change = CheckSSIDisNeedSave();
	
	GetuserPW();
	mGateway.Wifi->flag.f_pw_is_change	= CheckPASSWORDisNeedSave();
	if(mGateway.Wifi->flag.f_ssid_is_change == false && mGateway.Wifi->flag.f_pw_is_change == false)
	{
		mGateway.Wifi->flag.f_ssid_or_pw_is_change = false;
		uart_str_COM(Debug_COM, "\r [no charge ssid or password]");
	}
	else
	{
		mGateway.Wifi->flag.f_ssid_or_pw_is_change = true;
		uart_str_COM(Debug_COM, "\r [   charge ssid or password]");
	}
	
	//=================================================
	//	判斷OT, UT, wOT, wUT, Tol參數
	Get_user_Setting_Value();
	

	getDeviceIDString(&mGateway.uEMS.uModule);

	
	//getSettingString(&mGateway.uEMS.uSetting.Tolerance);
	//Save_user_Setting_Value(&mGateway.uEMS.uSetting.Tolerance, &mGateway.uEMS.uSetting.eTolerance);
	
	
	//findKeyword(mGateway.Wifi->Input_buffer, " HTTP/");

	

}

/*
	//擷取e-mail的@ = %40
	for(unsigned int i = ret; i < ret_end; i++)
	{
		Input_EMAIL_Buffer[k] = *(ret + z);
		if(Input_EMAIL_Buffer[k] == '%')
		{
			uart_str_COM(Debug_COM, "[ find % ]");
			Input_EMAIL_Buffer[k] = '@';
			z++;
			z++;
			i++;
			i++;
		}
		
		uTXByte(Debug_COM, Input_EMAIL_Buffer[k]);
		k++;
		z++;
	}
	Input_EMAIL_Buffer[k] = '\0';
*/

//=================================================
#endif
//=================================================