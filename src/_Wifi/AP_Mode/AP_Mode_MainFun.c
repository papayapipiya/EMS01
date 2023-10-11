#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "myDefine.h"

//=================================================
#ifdef Dev_Wifi
//=================================================



void AP_MainFunc_Rundown(void)
{
	char str[100];
	//===========================================
	// 2023-10-04 AT Command執行完畢後才會進入AP_MainFunc_Rundown();
	if(mGateway.Wifi->NowStatus != WiFi_NOP) return;

	switch(mGateway.Wifi->RunStep)
	{
		case Wait_Send_AT:
			mGateway.Wifi->NowStatus = Wait_Send_AT;
			mGateway.Wifi->RunStep = Wait_Send_RST;
			break;
					
		case Wait_Send_RST:
			mGateway.Wifi->NowStatus = Wait_Send_RST;
			mGateway.Wifi->RunStep = Wait_AT_CWMODE_Set_3;
			break;
			
		case Wait_AT_CWMODE_Set_3:
			mGateway.Wifi->NowStatus = Wait_AT_CWMODE_Set_3;
			mGateway.Wifi->RunStep = Step_Join_SSID;
			break;
		
		case Step_Join_SSID:
			mGateway.Wifi->NowStatus = Wait_AT_CWJAP_Join_SSID;
			mGateway.Wifi->RunStep = WiFi_Sleep;
			//2023-10-05	如果ssid或password有改變，因為已經重新加入一次ssid與pw
			//				所以清除mGateway.Wifi->flag.f_ssid_or_pw_is_change旗標
			if(mGateway.Wifi->flag.f_ssid_or_pw_is_change == true)	mGateway.Wifi->flag.f_ssid_or_pw_is_change = false;
			break;
			
		case WiFi_Sleep:
			mGateway.Wifi->NowStatus = WiFi_STOP;
			mGateway.Wifi->RunStep = Wait_AT_CWSAP_CUR;
			break;
		
		case Wait_AT_CWSAP_CUR:
			mGateway.Wifi->NowStatus = Wait_AT_CWSAP_CUR;
			mGateway.Wifi->RunStep = Wait_AT_CIPMUX;
			break;
		
		case Wait_AT_CIPMUX:
			mGateway.Wifi->NowStatus = Wait_AT_CIPMUX;
			mGateway.Wifi->RunStep = Wait_AT_CIPSERVER;
			break;
			
		case Wait_AT_CIPSERVER:
			mGateway.Wifi->NowStatus = Wait_AT_CIPSERVER;
			mGateway.Wifi->RunStep = Wait_AT_CWLAPOPT;
			break;
			
		case Wait_AT_CWLAPOPT:
			mGateway.Wifi->NowStatus = Wait_AT_CWLAPOPT;
			mGateway.Wifi->RunStep = Wait_AT_CWLAP;
			break;
			
		case Wait_AT_CWLAP:
			mGateway.Wifi->NowStatus = Wait_AT_CWLAP;
			mGateway.Wifi->RunStep = Wait_AT_PING_8_8_8_8;
			break;
			
		case Wait_AT_PING_8_8_8_8:
			mGateway.Wifi->NowStatus = Wait_AT_PING_8_8_8_8;
			My_WiFi.RunStep = WiFi_Goto_AP_Mode;
			break;
			
		case WiFi_Goto_AP_Mode:
			uart_str_COM(Debug_COM, "\r --> AP Working..");
			mGateway.Wifi->wifi_work_led_flag =  false;
			mGateway.Wifi->inAP_Mode = true;
			By_Browser = false;
			LED_R_ON
			LED_B_ON
			LED_G_ON
			Reset_wifi_get_data(wifi_delay_6400ms);
			ResetWifiFlag();
			mGateway.Wifi->RunStep = Wifi_AP_Mode_Wait_User_keyin;
			break;
			
		case Wifi_AP_Mode_Wait_User_keyin:
			Uart_Form_WIFI_callback(Wifi_COM);
			
			//===========================================
			// 2023-10-04 沒有收到資料
			if(mGateway.Wifi->Rx_buffer_Count == 0)
			{
				uart_str_COM(Debug_COM, ".");
			}
			else
			{
				sprintf(str, "\r --> mGateway.Wifi->Rx_buffer_Count = %d", mGateway.Wifi->Rx_buffer_Count);
				uart_str_COM(Debug_COM, str);
			}
			
			//===========================================
			// 2023-10-04 有收到資料，確認是否有認識的關鍵字
			if(getWifiKeyWord() == true) 
			{
				if(CheckWifi_meanNG_Flag() == true)
				{
					mGateway.Wifi->RunStep = WiFi_Goto_AP_Mode;
					break;
				}
				else
				{
					uart_str_COM(Debug_COM, "\r Print : [\r");
					for(unsigned int i = 0; i < mGateway.Wifi->Rx_buffer_Count; i++)	uTXByte(Debug_COM, mGateway.Wifi->Input_buffer[i]);
					uart_str_COM(Debug_COM, "\r]\r- - - - - - - - - - -\r");
					mGateway.Wifi->RunStep = Wifi_AP_Mode_Get_User_keyin_SSID_PW;
					break;
				}
			}
			break;
			
			
		case Wifi_AP_Mode_Get_User_keyin_SSID_PW:
			Get_User_KeyinWord_SSID_PW();
			if(mGateway.Wifi->flag.f_ssid_or_pw_is_change == true)
			{
				//========================================
				//	2023-10-05 如果ssid與password 有 改變
				mGateway.Wifi->RunStep = Step_Join_SSID;
			}
			else
			{
				//========================================
				//	2023-10-05 如果ssid與password 沒有 改變
				mGateway.Wifi->RunStep = WiFi_AP_Mode;
			}
			
			break;
			
		case WiFi_AP_Mode:
			//CMD_ERROR = true;
			uart_str_COM(Debug_COM, "\r --> response user get function");
			print_html_main_string();
			mGateway.Wifi->RunStep = Wait_AT_CIPCLOSE_Wait_Unit;
			//if(By_Browser)
			//{
				//
			//}
			//else
			//{
				//if(CMD_ERROR == true)
				//{
					//uart_str_COM(Wifi_COM, "AT+CIPSEND=");
					//uTXByte(Wifi_COM, target_Number);
					//uart_str_COM(Wifi_COM, ",9\r\n");
					//delay_cycles_ms(AT_Command_Delay_For_AP_MODE);
					//uart_str_COM(Wifi_COM, "CMD_ERROR");
				//}
			//}
			break;
			
			
		case Wait_AT_CIPCLOSE_Wait_Unit:
			mGateway.Wifi->RunStep = Wait_AT_CWLAPOPT;
			break;
			

			
			
			
	}
}


void Wifi_Rundown_AP_Func(void)
{
	//===========================================
	// 2023-10-04 讓AP Mode一段時間進來一次
	if(mGateway.Wifi->WIFI_Main_Counter != WIFI_Main_Counter_Max) return;
	mGateway.Wifi->WIFI_Main_Counter = 0;
	
	//===========================================
	// 2023-10-04 AP 控制AP Mode的模式，待完成
	if(mGateway.Wifi->AP_Mode_Switch == Is_Station_Mode);
	
	//===========================================
	// 2023-10-04 AP Mode的主程式控制
	AP_MainFunc_Rundown();
	wdt_reset_count();
	
	//===========================================
	// 2023-10-04 如果有AT Command的需求才會執行
	Run_Wifi_Command_Func();
	wdt_reset_count();

}





//=================================================
#endif
//=================================================