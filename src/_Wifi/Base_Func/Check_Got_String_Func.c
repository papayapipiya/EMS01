#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "myDefine.h"

//=================================================
#ifdef Dev_Wifi
//=================================================



char const wifi_CIFSR_STAIP_IP_NOT_0000[]				= "+CIFSR:STAIP,\"0.0.0.0\"";

char const wifi_Wait_AT_CIPSEND_ALREADY_CONNECTED[]		= "ALREADY CONNECTED";
char const wifi_Wait_AT_CIPSEND_CONNECT[]				= "CONNECT";
char const wifi_Wait_AT_CIPSEND_ERROR[]					= "ERROR";
char const wifi_Wait_AT_CIPSEND_busy[]					= "busy";
char const wifi_Wait_AT_CIPSEND_SEND_OK[]				= "SEND OK";
char const wifi_Wait_AT_CIPSEND_SEND_FAIL[]				= "SEND FAIL";
char const wifi_Wait_AT_CIPSEND_0CLOSED[]				= "CLOSED";
char const wifi_Wait_AT_CIPSEND_200_OK[]				= "200 OK";
char const wifi_Wait_AT_WIFI_GOT_IP[]					= "WIFI GOT IP";



void check_AT_CIPSTART_func(void)
{
	bool result_bool;
	result_bool = check_keyword_in_String(&My_WiFi.Input_buffer, &wifi_Wait_AT_CIPSEND_ERROR);
	My_WiFi.Send_Success = true;
	if(result_bool)
	{
		//有找到字串
		if(My_WiFi.debug_flag)	uart_str(" [ERROR]\r");
		My_WiFi.Send_Success = false;
	}
	else
	{
		//沒找到字串
		if(My_WiFi.debug_flag)	uart_str(" [No ERROR]");
	}
	//--------------------------------------------------------
	result_bool = check_keyword_in_String(&My_WiFi.Input_buffer, &wifi_Wait_AT_CIPSEND_busy);
	if(result_bool)
	{
		//有找到字串
		if(My_WiFi.debug_flag)	uart_str(" [busy]\r");
		My_WiFi.Send_Success = false;
	}
	else
	{
		//沒找到字串
		if(My_WiFi.debug_flag)	uart_str(" [No busy]");
	}
	//--------------------------------------------------------
	result_bool = check_keyword_in_String(&My_WiFi.Input_buffer, &wifi_Wait_AT_CIPSEND_CONNECT);
	if(result_bool)
	{
		//有找到字串
		if(My_WiFi.debug_flag)	uart_str(" [CONNECT OK]");
	}
	else
	{
		//沒找到字串
		if(My_WiFi.debug_flag)	uart_str(" [No CONNECT]");
		My_WiFi.Send_Success = false;
	}
	//--------------------------------------------------------
	result_bool = check_keyword_in_String(&My_WiFi.Input_buffer, &wifi_Wait_AT_CIPSEND_ALREADY_CONNECTED);
	if(result_bool)
	{
		//有找到字串
		if(My_WiFi.debug_flag)	uart_str(" [ALREADY]");
		My_WiFi.Send_Success = true;
	}
	else
	{
		//沒找到字串
		if(My_WiFi.debug_flag)	uart_str(" [No ALREADY]");
	}
}

void check_AT_GET_func(void)
{
	bool result_bool;
	//Wait_GET_1 - Step - 001
	//- - - - - - - - - - - - - - - - - - - - - -
	result_bool = check_keyword_in_String(&My_WiFi.Input_buffer, &wifi_Wait_AT_CIPSEND_ERROR);
	My_WiFi.Send_Success = true;
	if(result_bool)
	{
		//有找到ERROR字串
		if(My_WiFi.debug_flag)	uart_str(" [ERROR]");
		My_WiFi.Send_Success = false;
	}
	else
	{
		//沒找到字串
		if(My_WiFi.debug_flag)	uart_str(" [OK]");
	}
	//Wait_GET_1 - Step - 002
	//- - - - - - - - - - - - - - - - - - - - - -
	result_bool = check_keyword_in_String(&My_WiFi.Input_buffer, &wifi_Wait_AT_CIPSEND_SEND_FAIL);
	if(result_bool)
	{
		//有找到FAIL字串
		if(My_WiFi.debug_flag)	uart_str(" [FAIL]");
		My_WiFi.Send_Success = false;
	}
	else
	{
		//沒找到字串
		if(My_WiFi.debug_flag)	uart_str(" [OK]");
	}
	
	
	if (My_WiFi.Send_Success == true)
	{
		user_switch = false;
		LED_R_OFF
	}

}

void Check_Got_String(void)
{
	bool result_bool;

	switch(My_WiFi.NowCMD)
	{
		case Wait_AT_CIFSR:
			result_bool = check_keyword_in_String(&My_WiFi.Input_buffer, &wifi_CIFSR_STAIP_IP_NOT_0000);
			if(result_bool)
			{
				//有找到字串
				if(My_WiFi.debug_flag)	uart_str(" [IP = 0.0.0.0]");
			
			}
			else
			{
				//沒找到字串
				if(My_WiFi.debug_flag)	uart_str(" [IP != 0.0.0.0]");
			}
			break;
		
		
		case Wait_AT_CIPSTART_1:
			check_AT_CIPSTART_func();
			break;
			
		case Wait_AT_CIPSTART_2:
			check_AT_CIPSTART_func();
			break;
		
		case Wait_GET_1:
			check_AT_GET_func();
			break;
			
		case Wait_GET_2:
			check_AT_GET_func();
			break;
			
			
		case Wait_AT_CWLAP:	
			Get_Discovery_Network_KeyWord();
			Print_And_Save_SSID();
			break;
		
		case Wait_AT_PING_8_8_8_8:
			Ping_internert_Func();
			break;
		
		default:
			break;
	}
}


//=================================================
#endif
//=================================================