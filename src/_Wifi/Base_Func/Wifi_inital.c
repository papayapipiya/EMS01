#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "myDefine.h"
#include "memory.h"
//=================================================
#ifdef Dev_Wifi
//=================================================
//	51.79.222.210/demo?text=wifi module testing&token=testkey
//#define Target_IP_d				"AT+CIPSTART=\"TCP\",\"papayapipiya.synology.me\",80\r\n"
#define Target_IP_d				"AT+CIPSTART=\"TCP\",\"51.79.222.210\",80\r\n"
#define GET_Update_HTTP_Head_d	" HTTP/1.1\r\nContent-Type: text/html\r\nHost: papayapipiya.synology.me\r\nConnection: Keep-Alive\r\n\r\n"
void Define_GET_String(void)
{
	
	
	//asprintf()
	memcpy(Target_IP, Target_IP_d, strlen(Target_IP_d)+1);
	memcpy(GET_Update_HTTP_Head, GET_Update_HTTP_Head_d, strlen(GET_Update_HTTP_Head_d)+1);
	
	Target_PHP[0]		= "GET /MYSQL/updatesensor.php?wifi='@0000000";
	GET_Update_tmp[0]	= "GET /MYSQL/updatesensor.php?MAC='020000AC',0001,0002,0003,0004,0005,0006,0007,0008,0009,0010,0011,0012,0013,0014,0015,0016,0017,1800,1900,2000&MAC2='02EEEEEE',0001,0002,0003,0004,0005,0006,0007,0008,0009,0010,0011,0012,0013,0014,0015,0016,0017,1800,1900,2000";
	DeviceData_tmp[0]	= "'020000EA',0001,0002,0003,0004,0005,0006,0007,0008,0009,0010,0011,0012,0013,0014,0015,0016,0017,1800,1900,2000";
	DeviceData1[0]		= "'^0000000',#000,#001,#002,#003,#004,#005,#006,#007,#008,#009,#010,#011,#012,#013,#014,#015,#016,#017,#018,#019";


	php_MAC_00[0]			= "^0000000";

	php_key_00[0]			= "#000";
	php_key_01[0]			= "#001";
	php_key_02[0]			= "#002";
	php_key_03[0]			= "#003";
	php_key_04[0]			= "#004";
	php_key_05[0]			= "#005";
	php_key_06[0]			= "#006";
	php_key_07[0]			= "#007";
	php_key_08[0]			= "#008";
	php_key_09[0]			= "#009";
	php_key_10[0]			= "#010";
	php_key_11[0]			= "#011";
	php_key_12[0]			= "#012";
	php_key_13[0]			= "#013";
	php_key_14[0]			= "#014";
	php_key_15[0]			= "#015";
	php_key_16[0]			= "#016";
	php_key_17[0]			= "#017";
	php_key_18[0]			= "#018";
	php_key_19[0]			= "#019";
	php_key_20[0]			= "#020";
}

bool get_GPIO(unsigned char tmp)
{
	return port_pin_get_input_level(tmp);
}

void Wifi_inital(void)
{
	Define_GET_String();
	mGateway.Wifi						= &My_WiFi;
	mGateway.Wifi->NowStatus			= WiFi_NOP;
	mGateway.Wifi->Send_Success			= true;
	mGateway.Wifi->Cycletime			= 50;
	mGateway.Wifi->Send_UnSuccess_count	= 0;
	mGateway.Wifi->WIFI_Main_Counter	= 0;
	mGateway.Wifi->wifi_work_led_flag	= false;
	mGateway.Wifi->wifi_network_flag	= false;
	mGateway.Wifi->debug_flag			= false;
	mGateway.Wifi->Update_Count			= 0;
	mGateway.Wifi->Update_ptr			= 0;
	mGateway.Wifi->get_AP_Mode_Switch	= &get_GPIO;
	mGateway.Wifi->AP_Mode_Switch		= mGateway.Wifi->get_AP_Mode_Switch(PIN_PA07);
	
	//mGateway.Wifi->AP_Mode_Switch = false;
	if(mGateway.Wifi->AP_Mode_Switch == true)
		uart_str_COM(Debug_COM, "mGateway.Wifi->AP_Mode_Switch == True..\r");
	else
		uart_str_COM(Debug_COM, "mGateway.Wifi->AP_Mode_Switch == False..\r");

	//Load_Save_SSID_txt_in_NVM();
	//Load_Save_PASSWORD_txt_in_NVM();
	//Load_Save_EMAIL_txt_in_NVM();
	sprintf(mGateway.Wifi->SSID_Buffer, "rorohome");
	sprintf(mGateway.Wifi->PASSWORD_Buffer, "0936818176");
	sprintf(EMAIL_Buffer, "papayapipiya@gmail.com");
	
}



//=================================================
#endif
//=================================================