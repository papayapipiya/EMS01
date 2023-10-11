#include <asf.h>
#include <stdio.h>
#include "myDefine.h"
#include <string.h>

void Main_Startup(void);


void Project_MainFunc(void)
{

	//My_WiFi.AP_Mode_Switch = Is_AP_Mode;
	Wifi_Rundown_AP_Func();
	return;	
	if(My_WiFi.AP_Mode_Switch == Is_AP_Mode)
	{
		Wifi_Rundown_AP_Func();
	}
	else
	{
		Wifi_Rundown_Func();
	}	
	
}


void usart_read_Mainfunc(void)
{
	usart_read_buffer_job(Debug_COM, &tmp_uart_buf, 1);
}


void SetEEPROM_packet(void)
{
	char str[100];
	uart_str_COM(Debug_COM, "\r [initial EEPROM] mGateway.uEMS.uModule.eID");
	mGateway.uEMS.uModule.eID = &define_eID;
	eEEPROM_initial(mGateway.uEMS.uModule.eID);
	
	uart_str_COM(Debug_COM, "\r [initial EEPROM] mGateway.Wifi->eSSID");
	mGateway.Wifi->eSSID = &define_eSSID;
	eEEPROM_initial(mGateway.Wifi->eSSID);
	
	uart_str_COM(Debug_COM, "\r [initial EEPROM] mGateway.Wifi->ePW");
	mGateway.Wifi->ePW = &define_ePW;
	eEEPROM_initial(mGateway.Wifi->ePW);
	
	uart_str_COM(Debug_COM, "\r [initial EEPROM] mGateway.Wifi->eOT");
	mGateway.uEMS.uSetting.eOT = &define_eOT;
	eEEPROM_initial(mGateway.uEMS.uSetting.eOT);
	
	uart_str_COM(Debug_COM, "\r [initial EEPROM] mGateway.Wifi->eUT");
	mGateway.uEMS.uSetting.eUT = &define_eUT;
	eEEPROM_initial(mGateway.uEMS.uSetting.eUT);
	
	uart_str_COM(Debug_COM, "\r [initial EEPROM] mGateway.Wifi->ewOT");
	mGateway.uEMS.uSetting.ewOT = &define_ewOT;
	eEEPROM_initial(mGateway.uEMS.uSetting.ewOT);
	
	uart_str_COM(Debug_COM, "\r [initial EEPROM] mGateway.Wifi->ewUT");
	mGateway.uEMS.uSetting.ewUT = &define_ewUT;
	eEEPROM_initial(mGateway.uEMS.uSetting.ewUT);
	
	uart_str_COM(Debug_COM, "\r [initial EEPROM] mGateway.Wifi->eTolerance");
	mGateway.uEMS.uSetting.eTolerance = &define_eTolerance;
	eEEPROM_initial(mGateway.uEMS.uSetting.eTolerance);
}

int main(void)
{
	UINT16u_t tmp16;
	volatile Write_PLC_List_t *Write_PLC_List_Index = Write_PLC_List_In_RAM;
	volatile LORA_Dev_List_t *LORA_Dev_List_Index = LORA_Dev_List_In_RAM;
	char str[100];
	
	Main_Startup();
	mGateway.Wifi->Cycletime = 600;
	SetEEPROM_packet();


	Load_eID_Flag(mGateway.uEMS.uModule.eID);
	//sprintf(mGateway.uEMS.uModule.uID, "%s", "1234ABCD");
	//Save_eID_Flag(mGateway.uEMS.uModule.eID);
	//Load_eID_Flag(mGateway.uEMS.uModule.eID);
	
	Load_eSSID_Flag(mGateway.Wifi->eSSID);
	Load_ePW_Flag(mGateway.Wifi->ePW);
	
	Load_eOT(mGateway.uEMS.uSetting.eOT);
	userKeyin_Buffer_or_Buffer_to_value(uSelect_Buffer, &mGateway.uEMS.uSetting.OT);
	
	Load_ewOT(mGateway.uEMS.uSetting.ewOT);
	userKeyin_Buffer_or_Buffer_to_value(uSelect_Buffer, &mGateway.uEMS.uSetting.wOT);
	
	Load_eUT(mGateway.uEMS.uSetting.eUT);
	userKeyin_Buffer_or_Buffer_to_value(uSelect_Buffer, &mGateway.uEMS.uSetting.UT);
	
	Load_ewUT(mGateway.uEMS.uSetting.ewUT);
	userKeyin_Buffer_or_Buffer_to_value(uSelect_Buffer, &mGateway.uEMS.uSetting.wUT);
	
	Load_eTolerance(mGateway.uEMS.uSetting.ewUT);
	userKeyin_Buffer_or_Buffer_to_value(uSelect_Buffer, &mGateway.uEMS.uSetting.eTolerance);

	
	mGateway.uEMS.uModule.uADC.timer = 0;
	user_switch = false;
	My_WiFi.wifi_work_led_flag =  true;
	My_WiFi.debug_flag = true;
	
	//LORA01->Lora_Debug_Flag = 1;
	My_WiFi.inAP_Mode = false;
	
	usart_read_Mainfunc(); //觸發用 	
	
	

	
	while (true)
	{
		wdt_reset_count();
		ems01_input_adc();
		ems01_input_gpio();
		Project_MainFunc();
		
		//
		//sprintf(str, "\r NTC_No_01 = (%06d) ", My_WiFi.NTC_No_01);
		//uart_str_COM(Debug_COM, str);
		//==================================================
		//Fill_Dev_Time_into_dev_value(LORA01);		
		//==================================================
		switch(UserKeyInStauts)
		{
			case UserKeyInEnterIndex:
				Print_Line_Msg();
				uart_str_COM(Debug_COM, "Keyin..\r");
				for(unsigned char k = 0; k < UserKeyInBufCount; k++)
				{
					if(UserKeyInBuf[k] != 0x0d && ascii_to_hex(UserKeyInBuf[k]) != KEYZ) 
						uTXByte(Debug_COM, UserKeyInBuf[k]);	
					if(ascii_to_hex(UserKeyInBuf[k]) == KEYZ)	
						uTXByte(Debug_COM, 'P');	//避免PC COM PORT誤認資料
						
					sprintf(str, " (%02X) ", UserKeyInBuf[k]);
					uart_str_COM(Debug_COM, str);	
				}
				uTXByte(Debug_COM, 0x09);
				uTXByte(Debug_COM, '-');
				uart_send_word(Debug_COM, hex_to_bcd(UserKeyInBufCount));
				uTXByte(Debug_COM, 0x0d);
				Print_Line_Msg();
				
				//Judg_Key_Wifi_APMode();
				
				Judg_KEY_D();
				Judg_KEY_S();
				Judg_KEY_L(LORA_Dev_List_Index);
				Judg_KEY_E(LORA_Dev_List_Index);
				Judg_KEY_ADD(LORA_Dev_List_Index);
				Judg_KEY_DL(LORA_Dev_List_Index);
				Judg_KEY_SHOW();
				//Judg_KEY_PSHOW();
				Judg_KEY_T(LORA_Dev_List_Index);
				Judg_KEY_W();							// PC Write DATA TO PCB
				//Judg_KEY_P(Write_PLC_List_Index);
				//Judg_KEY_P();
				Judg_KEY_PDL();
				//Judg_KEY_CH();
				Judg_KEY_HELP();
				Judg_KEY_C();
				
				
				#if	defined(Dev_Sensor)
					Judg_KEY_M();
					Judg_KEY_TS(LORA_Dev_List_Index);
				#endif		
				UserKeyInStauts = NOP;
				UserKeyInBufCount = 0;
				My_WiFi.Cycletime = 0;
				break;
				
			case NOP:
				break;
		}
		CMD_Func();
	}

}

