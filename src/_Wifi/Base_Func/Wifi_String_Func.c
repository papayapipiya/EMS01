#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "myDefine.h"

//=================================================
#ifdef Dev_Wifi
//=================================================

char const wifi_result_CIPSEND_OK[]			= ">";
char const wifi_result_OK[]					= "OK";
char const wifi_result_CLOSE[]				= "CLOSE";
char const wifi_result_200_OK[]				= "200 OK";
char const wifi_result_WIFI_GOT_IP[]		= "WIFI GOT IP";
char const wifi_result_WIFI_DISCONNECT[]	= "WIFI DISCONNECT";
char const wifi_result_WIFI_CONNECTED[]		= "WIFI CONNECTED";
char const wifi_result_ready[]				= "ready";
char const wifi_result_HTTP[]				= "HTTP";

unsigned char * MACtoASCII(unsigned char tmpIndex)
{
	UINT8u_t	tmp8;
	tmp8.byte = dev_value[tmpIndex][3];
	tmpMAC[0] = hex_to_ascii(tmp8.HL.H);
	tmpMAC[1] = hex_to_ascii(tmp8.HL.L);
	tmp8.byte = dev_value[tmpIndex][4];
	tmpMAC[2] = hex_to_ascii(tmp8.HL.H);
	tmpMAC[3] = hex_to_ascii(tmp8.HL.L);
	tmp8.byte = dev_value[tmpIndex][5];
	tmpMAC[4] = hex_to_ascii(tmp8.HL.H);
	tmpMAC[5] = hex_to_ascii(tmp8.HL.L);
	tmp8.byte = dev_value[tmpIndex][6];
	tmpMAC[6] = hex_to_ascii(tmp8.HL.H);
	tmpMAC[7] = hex_to_ascii(tmp8.HL.L);
	return &tmpMAC[0];
}


void replace_txt(char *t, char *s, char *tmp_val)
{
	char *MyTest;
	char *Input_str_ptr;
	MyTest = strstr(t, s);
	if(MyTest != 0)
	{
		Input_str_ptr = &s;
		for(unsigned char i = 0; i < strlen(s); i++)
		{
			*(MyTest + i) = *(tmp_val + i);
		}
		uart_str(&t);
	}
}






char * hex_to_val_ascii(unsigned int tmpVal)
{
	UINT16u_t	tmp16;
	UINT8u_t	tmp8;
	
	char *ptr;
	ptr = &tmp_txt[0];
	tmp16.word = hex_to_bcd(tmpVal);
	tmp8.byte = tmp16.byte[1];
	*ptr = tmp8.HL.H + 0x30;
	ptr++;
	*ptr = tmp8.HL.L + 0x30;
	ptr++;
	
	tmp8.byte = tmp16.byte[0];
	*ptr = tmp8.HL.H + 0x30;
	ptr++;
	*ptr = tmp8.HL.L + 0x30;
	ptr++;
	*ptr = '\0';
	return &tmp_txt[0];
}



bool check_keyword_in_String(char *t, char *s)
{
	char *result;
	result = strstr(t, s);
	if(result == 0)
		return false;
	else
		return true;
}


bool Check_Send_CMD_Response(void)
{
	char *wifi_result_txt_ptr;
	char *result;
	
	switch(My_WiFi.NowCMD)
	{
		case Wait_AT_CWSAP_CUR:
			wifi_result_txt_ptr = &wifi_result_OK;
			break;
		
		case Wait_Send_AT:
			wifi_result_txt_ptr = &wifi_result_OK;
			break;
		
		case Wait_Send_RESTORE:
			wifi_result_txt_ptr = &wifi_result_ready;
			break;
		
		case Wait_Send_RST:
			wifi_result_txt_ptr = &wifi_result_OK;
			break;
		
		case Wait_AT_CIFSR:
			wifi_result_txt_ptr = &wifi_result_OK;
			break;
		
		case Wait_AT_CWMODE_Set_1:
			wifi_result_txt_ptr = &wifi_result_OK;
			break;
		
		case Wait_AT_CWJAP_Join_SSID:
			wifi_result_txt_ptr = &wifi_result_WIFI_GOT_IP;
			break;
		
		case Wait_AT_CIPSTART_1:
			wifi_result_txt_ptr = &wifi_result_OK;
			break;
		
		case Wait_AT_CIPSEND_1:
			wifi_result_txt_ptr = &wifi_result_CIPSEND_OK;
			break;
		
		case Wait_GET_1:
			wifi_result_txt_ptr = &wifi_result_200_OK;
			break;
		
		case Wait_AT_CIPSTART_2:
			wifi_result_txt_ptr = &wifi_result_OK;
			break;
		
		case Wait_AT_CIPSEND_2:
			wifi_result_txt_ptr = &wifi_result_OK;
			break;
		
		case Wait_GET_2:
			wifi_result_txt_ptr = &wifi_result_CLOSE;
			break;
			
		case Wait_AT_CIPCLOSE:
			wifi_result_txt_ptr = &wifi_result_CLOSE;
			break;
			
		default:
			wifi_result_txt_ptr = &wifi_result_HTTP;
			break;
	}
	
	result = strstr(My_WiFi.Input_buffer, wifi_result_txt_ptr);
	if(result != 0)
	{
		if(My_WiFi.debug_flag)	uart_str(" [Wait Get KeyWord]");
		return true;
	}
	return false;
}


void WiFi_txt_Separation_line(void)
{
	if(!My_WiFi.debug_flag) return;
	uart_str("\r\r###############################\r");
}

void WiFi_txt_Function(volatile WiFi_t *tmpWiFi)
{
	if(!My_WiFi.debug_flag) return;
	if(tmpWiFi->NowStatus == Wait_Send_AT)					uart_str("# Wait Send AT..\r");
	else if(tmpWiFi->NowStatus == Wait_Send_RESTORE)		uart_str("# Wait Send RESTORE..\r");
	else if(tmpWiFi->NowStatus == Wait_Send_RST)			uart_str("# Wait Send RST..\r");
	else if(tmpWiFi->NowStatus == Wait_Reciver)				uart_str("# Wait Reciver..\r");
	else if(tmpWiFi->NowStatus == Wait_AT_CIPSTART_1)		uart_str("# Wait Send AT CIPSTART..\r");
	else if(tmpWiFi->NowStatus == Wait_AT_CIPSEND_1)		uart_str("# Wait Send AT CIPSEND..\r");
	else if(tmpWiFi->NowStatus == Wait_GET_1)				uart_str("# Wait Send AT GET..\r");
	else if(tmpWiFi->NowStatus == Wait_AT_CWJAP_Join_SSID)	uart_str("# Wait Send AT CWJAP Join SSID..\r");
	else if(tmpWiFi->NowStatus == Wait_AT_CIFSR)			uart_str("# Wait Send AT CIFSR..\r");
	else if(tmpWiFi->NowStatus == Wait_AT_CWMODE_Set_1)		uart_str("# Wait Send AT CWMODE = 1..\r");
	else if(tmpWiFi->NowStatus == Wait_AT_CWMODE_Set_2)		uart_str("# Wait Send AT CWMODE = 2..\r");
	else if(tmpWiFi->NowStatus == Wait_AT_CWMODE_Set_3)		uart_str("# Wait Send AT CWMODE = 3..\r");
	else if(tmpWiFi->NowStatus == Wait_AT_CIPCLOSE)			uart_str("# Wait Send AT CIPCLOSE..\r");
	else if(tmpWiFi->NowStatus == Wait_AT_CWSAP_CUR)		uart_str("# Wait Send AT CWSAP..\r");
	else if(tmpWiFi->NowStatus == Wait_AT_CIPMUX)			uart_str("# Wait Send AT CIPMUX..\r");
	else if(tmpWiFi->NowStatus == Wait_AT_CIPSERVER)		uart_str("# Wait Send AT CIPSERVER..\r");

	else if(tmpWiFi->NowStatus == Check_Reciver)			uart_str(" [Check Reciver]");
	else if(tmpWiFi->NowStatus == Reciver_OK)				uart_str(" [Reciver OK]");
	else if(tmpWiFi->NowStatus == Send_CMD)					uart_str(" [Send CMD]");
	else if(tmpWiFi->NowStatus == Print_Reciver_Byte)		uart_str("\r [Print_Reciver_Byte]");
	else if(tmpWiFi->NowStatus == Print_Reciver_ASCII)		uart_str("\r [Print Data ASCII]");
	else if(tmpWiFi->NowStatus == WiFi_NOP)					uart_str(" [Wifi NOP]");
	else if(tmpWiFi->NowStatus == WiFi_STOP)				uart_str(" [WiFi STOP]");
	else if(tmpWiFi->NowStatus == WiFi_CMD_Delay)			uart_str(" [WiFi CMD Delay]");
	else if(tmpWiFi->NowStatus == Check_Reciver_String)		uart_str(" [Check Reciver String]");
	else if(tmpWiFi->NowStatus == WiFi_Wait_Next_Loop)		uart_str(" [The End] Wait Next Loop..\r");
	else if(tmpWiFi->NowStatus == Print_Reciver_MAC_CMD)	uart_str(" [Print_Reciver_MAC_CMD]");
	
	
}


//=================================================
#endif
//=================================================