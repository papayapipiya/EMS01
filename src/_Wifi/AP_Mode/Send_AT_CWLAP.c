/*
 * Send_AT_CWLAP.c
 *
 * Created: 2023/10/4 上午 09:41:37
 *  Author: papay
 */ 

#include "myDefine.h"




void Send_AT_CWLAP(void)
{
	mGateway.Wifi->Rx_buffer_Count = 0;
	mGateway.Wifi->Rx_Timeout_Count = 0;
	mGateway.Wifi->Rx_buffer_Count = 0;
	mGateway.Wifi->f_count_Max = 10000;
	mGateway.Wifi->f_count_Max_div = My_WiFi.f_count_Max / 2;
	mGateway.Wifi->new_data = false;
	
	for(unsigned int i = 0; i < WIFI_Input_buffer_Max; i++)	mGateway.Wifi->Input_buffer[i] = "";
	uart_str_COM(Debug_COM, "\rAT+CWLAP");
	uart_str_COM(Wifi_COM, "AT+CWLAP\r\n");

	//=======================================
	ResetWifiFlag();
	while(1)
	{
		Uart_Form_WIFI_callback(Wifi_COM);
		if(getWifiKeyWord() == true) break;
	}
	if(CheckWifi_meanNG_Flag() == true) return;
	//=======================================
	
	uart_str_COM(Debug_COM, "\r[Get]  ");
	for(unsigned int i = 0; i < mGateway.Wifi->Rx_buffer_Count; i++)	uTXByte(Debug_COM, mGateway.Wifi->Input_buffer[i]);
}