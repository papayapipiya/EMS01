#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "myDefine.h"

//=================================================
#ifdef Dev_Wifi
//=================================================


void Wifi_Rundown_StepFunc(void)
{
	char str[100];
	
	//	必須要再wifi模組於NOP的時候才可以動作
	if(My_WiFi.NowStatus != WiFi_NOP)						return;
	
	// 小於10秒不工作，LED也不閃爍，600秒 = 600
	if(My_WiFi.Cycletime < 600)
	{
		// LED不閃爍
		My_WiFi.wifi_work_led_flag = false;
		My_WiFi.Cycletime++;
		return;
	}
	else
	{
		// LED閃爍
		My_WiFi.wifi_work_led_flag = true;	
	}

	switch(My_WiFi.Send_Success)
	{
		//============================================
		// 如果整個上傳流程失敗，將重新上傳
		case false:
			My_WiFi.RunStep						= Step_Connect_API;
			My_WiFi.Send_Success				= true;
			My_WiFi.wifi_network_flag			= false;
			
			//============================================
			// 如果上傳失敗超過3次，RESET ESP8266
			My_WiFi.Send_UnSuccess_count++;
			if(My_WiFi.Send_UnSuccess_count == 3)
			{
				My_WiFi.RunStep					= Step_Initial_AT;
				My_WiFi.Send_UnSuccess_count	= 0;
			}
			break;
		//============================================
		// 上傳成功
		case true:
			break;
	}

	LED_G_ON
	switch(My_WiFi.RunStep)
	{
		case Step_Initial_AT:
			My_WiFi.NowStatus = Wait_Send_AT;
			My_WiFi.RunStep = Step_SetRST;
			break;
			
		case Step_SetRST:
			My_WiFi.NowStatus = Wait_Send_RST;
			My_WiFi.RunStep = Step_SetMode;
			break;
			
		case Step_SetMode:
			My_WiFi.NowStatus = Wait_AT_CWMODE_Set_1;
			My_WiFi.RunStep = Step_Join_SSID;
			break;
					
		case Step_Join_SSID:
			My_WiFi.NowStatus = Wait_AT_CWJAP_Join_SSID;
			My_WiFi.RunStep = Step_Connect_API;
			break;
			
		case Step_Connect_API:
			My_WiFi.NowStatus = Wait_AT_CIPSTART_1;
			My_WiFi.RunStep = Step_Send_Len_To_API;
			break;
			
		case Step_Send_Len_To_API:
			My_WiFi.NowStatus = Wait_AT_CIPSEND_1;
			My_WiFi.RunStep = Step_Send_Data_To_API;
			break;
		
		case Step_Send_Data_To_API:
			My_WiFi.NowStatus = Wait_GET_1;
			My_WiFi.RunStep = Step_Close_API;
			break;
		
		case Step_Close_API:
			My_WiFi.NowStatus = Wait_AT_CIPCLOSE;
			
			//My_WiFi.RunStep = 6;
			My_WiFi.Update_ptr = 0;
			My_WiFi.Cycletime = 0;
			My_WiFi.RunStep = Step_Connect_API;
			break;
	}
}


void Wifi_Rundown_Func(void)
{

	Wifi_Rundown_StepFunc();
	wdt_reset_count();
	//delay_cycles_ms(20);
	LED_G_OFF
	
	Run_Wifi_Command_Func();
}





//=================================================
#endif
//=================================================