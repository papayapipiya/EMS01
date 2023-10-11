#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "myDefine.h"



//=================================================
#ifdef Dev_Wifi
//=================================================



	

unsigned char * WifiMACtoASCII(void)
{
	UINT8u_t	tmp8;
	tmp8.byte = LORA01->MyLoRaID[0];
	tmpMAC[0] = hex_to_ascii(tmp8.HL.H);
	tmpMAC[1] = hex_to_ascii(tmp8.HL.L);
	tmp8.byte = LORA01->MyLoRaID[1];
	tmpMAC[2] = hex_to_ascii(tmp8.HL.H);
	tmpMAC[3] = hex_to_ascii(tmp8.HL.L);
	tmp8.byte = LORA01->MyLoRaID[2];
	tmpMAC[4] = hex_to_ascii(tmp8.HL.H);
	tmpMAC[5] = hex_to_ascii(tmp8.HL.L);
	tmp8.byte = LORA01->MyLoRaID[3];
	tmpMAC[6] = hex_to_ascii(tmp8.HL.H);
	tmpMAC[7] = hex_to_ascii(tmp8.HL.L);
	return &tmpMAC[0];
}



void Setting_Send_CMD_Check_NowCMD(void)
{
	UINT16u_t	tmp16;
	char str[100];
	unsigned int tmp_count;
	unsigned int URL_len, Head_len;
	switch(My_WiFi.NowCMD)
	{
		//case Wait_Send_AT:
			//uart_str_COM(Wifi_COM, "AT\r\n");
			//My_WiFi.f_count = 0;
			//My_WiFi.f_count_Max = wifi_delay_01;
			//My_WiFi.f_count_Max_div = My_WiFi.f_count_Max / 2;
			//break;
		
		case Wait_Send_RESTORE:
			uart_str_COM(Wifi_COM, "AT+RESTORE\r\n");
			My_WiFi.f_count = 0;
			My_WiFi.f_count_Max = wifi_delay_01;
			My_WiFi.f_count_Max_div = My_WiFi.f_count_Max / 2;
			break;
		
		//case Wait_Send_RST:
			//uart_str_COM(Wifi_COM, "AT+RST\r\n");
			//My_WiFi.f_count = 0;
			//My_WiFi.f_count_Max = wifi_delay_12800ms;
			//My_WiFi.f_count_Max_div = My_WiFi.f_count_Max / 2;
			//break;
		
		case Wait_AT_CIFSR:
			uart_str_COM(Wifi_COM, "AT+CIFSR\r\n");
			My_WiFi.f_count = 0;
			My_WiFi.f_count_Max = wifi_delay_01;
			My_WiFi.f_count_Max_div = My_WiFi.f_count_Max / 2;
			break;
		
		case Wait_AT_CWMODE_Set_1:
			uart_str_COM(Wifi_COM, "AT+CWMODE_CUR=1\r\n");
			My_WiFi.f_count = 0;
			My_WiFi.f_count_Max = wifi_delay_01;
			My_WiFi.f_count_Max_div = My_WiFi.f_count_Max / 2;
			break;
			
		case Wait_AT_CWMODE_Set_2:
			uart_str_COM(Wifi_COM, "AT+CWMODE_CUR=2\r\n");
			My_WiFi.f_count = 0;
			My_WiFi.f_count_Max = wifi_delay_01;
			My_WiFi.f_count_Max_div = My_WiFi.f_count_Max / 2;
			break;
		
		//case Wait_AT_CWMODE_Set_3:
			//uart_str_COM(Wifi_COM, "AT+CWMODE_CUR=3\r\n");
			//My_WiFi.f_count = 0;
			//My_WiFi.f_count_Max = wifi_delay_01;
			//My_WiFi.f_count_Max_div = My_WiFi.f_count_Max / 2;
			//break;
			
		//case Wait_AT_CWSAP_CUR:
			//uart_str_COM(Wifi_COM, "AT+CWSAP=\"Tree_Code_Wifi_Module\",\"12345678\",5,3\r\n");
			//My_WiFi.f_count = 0;
			//My_WiFi.f_count_Max = wifi_delay_3200ms;
			//My_WiFi.f_count_Max_div = My_WiFi.f_count_Max / 2;
			//break;
		
		//case Wait_AT_CIPMUX:
			//uart_str_COM(Wifi_COM, "AT+CIPMUX=1\r\n");
			//My_WiFi.f_count = 0;
			//My_WiFi.f_count_Max = wifi_delay_01;
			//My_WiFi.f_count_Max_div = My_WiFi.f_count_Max / 2;
			//break;
		
		//case Wait_AT_CIPSERVER:
			//uart_str_COM(Wifi_COM, "AT+CIPSERVER=1,80\r\n");
			//My_WiFi.f_count = 0;
			//My_WiFi.f_count_Max = wifi_delay_01;
			//My_WiFi.f_count_Max_div = My_WiFi.f_count_Max / 2;
			//break;
		
		
		
		//case Wait_AT_CWJAP_Join_SSID:
			//sprintf(GET_Update_tmp, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID_Buffer, PASSWORD_Buffer);
			//uart_str_COM(Debug_COM, GET_Update_tmp);
			//uart_str_COM(Wifi_COM, GET_Update_tmp);
//
			//My_WiFi.f_count = 0;
			//My_WiFi.f_count_Max = wifi_delay_12800ms;
			//My_WiFi.f_count_Max_div = My_WiFi.f_count_Max / 2;
			//break;
			
		

			
			
		
		case Wait_AT_CIPSTART_1:
			uart_str_COM(Wifi_COM, &Target_IP);
			My_WiFi.f_count = 0;
			My_WiFi.f_count_Max = wifi_delay_02;
			My_WiFi.f_count_Max_div = My_WiFi.f_count_Max / 2;
			break;

		
		case Wait_AT_CIPSEND_1:
			return;
			MCU_Data_To_GET_Str_Func();
			URL_len		= strlen(&GET_Update_tmp);
			Head_len	= strlen(&GET_Update_HTTP_Head);
			tmp_count = URL_len + Head_len;
			
			sprintf(str, "\rAT+CIPSEND=%d", tmp_count);
			uart_str_COM(Debug_COM, str);
			sprintf(str, "\r[URL Len = (%03d), Head Len = (%03d)]\r", URL_len, Head_len);
			uart_str_COM(Debug_COM, str);
			
			sprintf(str, "AT+CIPSEND=%d\r\n", tmp_count);
			uart_str_COM(Wifi_COM, str);
			
			My_WiFi.f_count = 0;
			My_WiFi.f_count_Max = wifi_delay_01;
			My_WiFi.f_count_Max_div = My_WiFi.f_count_Max / 2;
			break;
		
		case Wait_GET_1:
			
			if(My_WiFi.debug_flag)
			{
				uTXByte(Debug_COM, 0x0d);
				uart_str_COM(Debug_COM, &GET_Update_tmp);
				uart_str_COM(Debug_COM, &GET_Update_HTTP_Head);
			}
			uart_str_COM(Wifi_COM,	&GET_Update_tmp);
			uart_str_COM(Wifi_COM,	&GET_Update_HTTP_Head);
			
			My_WiFi.f_count = 0;
			My_WiFi.f_count_Max = wifi_delay_6400ms;
			My_WiFi.f_count_Max_div = My_WiFi.f_count_Max;
			break;
	
		
		case Wait_AT_CIPCLOSE:
			uart_str_COM(Wifi_COM, "AT+CIPCLOSE\r\n");
			My_WiFi.f_count = 0;
			My_WiFi.f_count_Max = wifi_delay_01;
			My_WiFi.f_count_Max_div = My_WiFi.f_count_Max / 2;
			break;
	}
}


//=================================================
#endif
//=================================================