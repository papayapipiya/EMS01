/*
 * Get_Netlist_KeyWord.c
 *
 * Created: 2021/4/19 上午 10:05:43
 *  Author: willchen
 */ 

#include "myDefine.h"



#define str_maximum	1024
void Get_Discovery_Network_KeyWord(void)
{
	unsigned char str[str_maximum];
	unsigned char str2[str_maximum];
	unsigned char str_temp[str_maximum];
	
	
	//範例：_FINDSSID*
	//unsigned char *ret;
	unsigned int ret_length;
	//unsigned char *ret_end;
	
	int ret_diff;
	int ret_test;
	int ret_end_test;
	
	//unsigned char *target_ret;
	//unsigned char *target_ret_end;
	unsigned int target_ret_length;
	unsigned int total_length;
	//unsigned char *target;
	unsigned char SSID_Count, Password_Count;
	
	unsigned int i,j,k;
	
	unsigned char OK_Flag;
	uart_str_COM(Debug_COM, "GO..");
	uart_str_COM(Debug_COM, "\r Get_Discovery_Network_KeyWord");
	Key_Word_Flag = 0;
	By_Browser = false;
		
	if(mGateway.Wifi->Rx_buffer_Count == 0)
	{
		uart_str_COM(Debug_COM, "My_WiFi.Rx_buffer_Count = 0, SKIP..");
		goto Return_Get_Discovery_Network_KeyWord;
	}
	
	for(i = 0; i < 19; i++)
	{
		for(j = 0; j < 49; j++)
		{
			mGateway.Wifi->ssid.SearchList[i][j] = '\0';
		}
	}
	mGateway.Wifi->ssid.SearchListCounter = 0;
	
	mGateway.Wifi->inAP_Mode = true;
	
	uart_str_COM(Debug_COM, "\r- - - - - - - - - - -\r");
	/*
	ret				= strstr(mGateway.Wifi->Input_buffer, "+CWLAP:");
	ret_end			= strstr(mGateway.Wifi->Input_buffer, "\r\n\r\nOK");
	uTXByte(Debug_COM, 0x0d);
	*/
	
	uart_str_COM(Debug_COM, "\r ~ ~ ~ ~ ~ ~ Start ~ ~ ~ ~ ~ ");
	//===============================================================
	uart_str_COM(Debug_COM, "\r\r [step - 01] : get Wifi Module Response Stirng..");
	
	ret_test		= findKeyword(mGateway.Wifi->Input_buffer, "+CWLAP:");
	ret_end_test	= findKeyword(mGateway.Wifi->Input_buffer, "\r\n\r\nOK");
	if(ret_test == -1) {uart_str_COM(Debug_COM, "\r ret_test error.."); goto Return_Get_Discovery_Network_KeyWord;}
	if(ret_end_test == -1) {uart_str_COM(Debug_COM, "\r ret_end_test error.."); goto Return_Get_Discovery_Network_KeyWord;}
	ret_diff		= ret_end_test - ret_test;
	if(ret_end_test < ret_test) {uart_str_COM(Debug_COM, "\r lenght error.."); return;}
	if(ret_diff > 800) ret_diff = 800;
	sprintf(str, "\r --> start: %02d ,end: %02d", ret_test, ret_end_test);
	uart_str_COM(Debug_COM, str);
	
	total_length = ret_diff;
	
	sprintf(str, "\r     ret_end_test - ret_test = %d [Rx_buffer_Count = %d]" ,ret_diff ,mGateway.Wifi->Rx_buffer_Count);
	uart_str_COM(Debug_COM, str);
	
	
	//===============================================================
	uart_str_COM(Debug_COM, "\r\r [step - 02] : Prepare string..");

	for(i = 0; i < str_maximum; i++)	str2[i] = '\0';
	for(i = 0; i < ret_diff; i++)		str2[i] = mGateway.Wifi->Input_buffer[i + ret_test];
	target_ret_length = strlen(&str2);
	sprintf(str, "\r str2 = [\r%s\r] ", str2);
	uart_str_COM(Debug_COM, str);
	
	sprintf(str, "\r Total Keyword length: %d ", target_ret_length);
	uart_str_COM(Debug_COM, str);
	
	sprintf(str_temp, "%s ", str2);
	sprintf(str, "\r str_temp = [\r%s\r] ", str_temp);
	uart_str_COM(Debug_COM, str);
	
	//===============================================================
	for(j = 0; j < 8; j++)
	{
		uart_str_COM(Debug_COM, "\r\r [step - 03 : save ssid function]");
		
		ret_test			= findKeyword(str_temp, ",\"");
		if(ret_test == -1) {uart_str_COM(Debug_COM, "\r ret_test error.."); goto Return_Get_Discovery_Network_KeyWord;}
		ret_test			= ret_test + 2;
		
		ret_end_test		= findKeyword(str_temp, "\",");
		if(ret_end_test == -1) {uart_str_COM(Debug_COM, "\r ret_end_test error.."); goto Return_Get_Discovery_Network_KeyWord;}
			
		ret_diff			= ret_end_test - ret_test;
		if(ret_end_test < ret_test) {uart_str_COM(Debug_COM, "\r lenght error.."); goto Return_Get_Discovery_Network_KeyWord;}
		if(ret_diff > 800) ret_diff = 800;
		sprintf(str, "\r --> start: %02d ,end: %02d", ret_test, ret_end_test);
		uart_str_COM(Debug_COM, str);
		if(ret_end_test != ret_test)
		{
			for(i = 0; i < str_maximum; i++)	str2[i] = '\0';
			for(i = 0; i < ret_diff; i++)		str2[i] = str_temp[i + ret_test];
			sprintf(str, "\r     str2(ssid): %s", str2);
			uart_str_COM(Debug_COM, str);
			k = strlen(&str2);
			for(i = 0; i < k; i++)
			{
				mGateway.Wifi->ssid.SearchList[mGateway.Wifi->ssid.SearchListCounter][i] = str2[i];
			}
			sprintf(str, "\r     save(ssid): [[ %s ]]", mGateway.Wifi->ssid.SearchList[mGateway.Wifi->ssid.SearchListCounter]);
			uart_str_COM(Debug_COM, str);
			mGateway.Wifi->ssid.SearchListCounter++;
			target_ret_length = strlen(&str2);
		}
		else
		{
			uart_str_COM(Debug_COM, "\r ==> ssid is null, pass..");
		}
	
		//===============================================================
		uart_str_COM(Debug_COM, "\r\r [step - 04] : Prepare NEXT SSID String..");
		
		ret_test		= findKeyword(str_temp, "+CWLAP:");
		if(ret_test == -1) {uart_str_COM(Debug_COM, "\r ret_test error.."); goto Return_Get_Discovery_Network_KeyWord;}
			
		ret_end_test	= findKeyword(str_temp, ")");
		if(ret_end_test == -1) {uart_str_COM(Debug_COM, "\r ret_end_test error.."); goto Return_Get_Discovery_Network_KeyWord;}
		ret_end_test	= ret_end_test+2;
		
		ret_diff		= ret_end_test - ret_test;
		if(ret_end_test < ret_test) {uart_str_COM(Debug_COM, "\r lenght error.."); goto Return_Get_Discovery_Network_KeyWord;}
		if(ret_diff > 800) ret_diff = 800;
		sprintf(str, "\r --> start: %02d ,end: %02d, diff", ret_test, ret_end_test, ret_diff);
		uart_str_COM(Debug_COM, str);
	
		for(i = 0; i < str_maximum; i++)	str2[i] = '\0';
		for(i = 0; i < ret_diff; i++)		str2[i] = str_temp[i + ret_test];
		target_ret_length = strlen(&str2);
		sprintf(str, "\r str2 = [\r%s\r] ", str2);
		uart_str_COM(Debug_COM, str);
	
	
		for(i = 0; i < str_maximum; i++)	str2[i] = '\0';
		k = 0;
		for(i = ret_end_test; i < total_length; i++)	
		{
			str2[k] = str_temp[i];
			k++;
		}
		sprintf(str_temp, "%s ", str2);
		
		//==================================================
		//	2023-10-06 因為debug字太多，刪除部分文字
		//sprintf(str, "\r next str2 = [\r%s\r] ", str2);
		//uart_str_COM(Debug_COM, str);
		//sprintf(str, "\r Total Keyword length: %d ", target_ret_length);
		//uart_str_COM(Debug_COM, str);
	
		
	
		
	}

Return_Get_Discovery_Network_KeyWord:
	uart_str_COM(Debug_COM, "\r ~ ~ ~ ~ ~ ~ End ~ ~ ~ ~ ~ ~ ");
	return;
}


void Print_And_Save_SSID(void)
{
	char str[1024];
	unsigned int i;
	wdt_reset_count();
	for(i = 0; i < 1024; i++)	mGateway.Wifi->ssid.ApListSrting[i] = '\0';
		

	for(i = 0; i < mGateway.Wifi->ssid.SearchListCounter; i++)
	{
		sprintf(mGateway.Wifi->ssid.ApListSrting, "%s%s<br>", mGateway.Wifi->ssid.ApListSrting, mGateway.Wifi->ssid.SearchList[i]);
		sprintf(str, "\r%02d(%02d). %s",i,  mGateway.Wifi->ssid.SearchListCounter, mGateway.Wifi->ssid.SearchList[i]);
		uart_str_COM(Debug_COM, str);
	}
	
	return true;

}