#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "myDefine.h"
#include "Wifi_Func_Define.h"

//=================================================
#ifdef Dev_Wifi
//=================================================

unsigned char CMD_MAC_Buffer[15];
unsigned char CMD_MAC[15];


bool getWifiKeyWord(void)
{
	if(getBusy() == true)		{return true;}
	if(chkTimeout() == true)	{return true;}
	if(getERROR() == true)		{return true;}
	if(getTimeout() == true)	{return true;}
	if(getNoChange() == true)	{return true;}
	if(getFAIL() == true)		{return true;}
	
	
	if(getHTTP() == true)		{return true;}
	if(getWIFI_GOT_IP() == true)	{return true;}
	if(getOK() == true)			{return true;}
	return	false;
}


bool CheckWifi_meanNG_Flag(void)
{
	if(mGateway.Wifi->flag.f_timeout == true)		return true;
	if(mGateway.Wifi->flag.f_mcu_timout == true)	return true;
	if(mGateway.Wifi->flag.f_error == true)			return true;
	if(mGateway.Wifi->flag.f_busy == true)			return true;
	if(mGateway.Wifi->flag.f_FAIL == true)			return true;
	return	false;
}

bool CheckWifi_meanOK_Flag(void)
{
	if(mGateway.Wifi->flag.f_nochange == true)		return true;
	if(mGateway.Wifi->flag.f_wifi_got_ip == true)	return true;
	if(mGateway.Wifi->flag.f_ok == true)			return true;
	if(mGateway.Wifi->flag.f_http == true)			return true;
	return	false;
}

void ResetWifiFlag(void)
{
	mGateway.Wifi->flag.f_ok = false;
	mGateway.Wifi->flag.f_http = false;
	mGateway.Wifi->flag.f_wifi_got_ip = false;
	
	mGateway.Wifi->flag.f_FAIL = false;
	mGateway.Wifi->flag.f_error = false;
	mGateway.Wifi->flag.f_timeout = false;
	mGateway.Wifi->flag.f_mcu_timout = false;
	mGateway.Wifi->flag.f_busy = false;
	mGateway.Wifi->flag.f_nochange = false;
}


bool getFAIL(void)
{
	unsigned int result = strstr(mGateway.Wifi->Input_buffer, "\r\nFAIL");
	if(result != 0)
	{
		uart_str(" {get FAIL}");
		mGateway.Wifi->flag.f_FAIL = true;
		return true;
	}
	else
	{
		return false;
	}
}



bool chkTimeout(void)
{
	char str[50];
	if(mGateway.Wifi->f_count > mGateway.Wifi->f_count_Max)
	{
		mGateway.Wifi->flag.f_mcu_timout = true;
		sprintf(str, " [mcu TimeOut = (%d)]", mGateway.Wifi->f_count);
		uart_str_COM(Debug_COM, str);
		return true;
	}
	else
	{
		return false;
	}
}


bool getNoChange(void)
{
	unsigned int result = strstr(mGateway.Wifi->Input_buffer, "\r\nno change");
	if(result != 0)
	{
		uart_str(" {get no change}");
		mGateway.Wifi->flag.f_nochange = true;
		return true;
	}
	else
	{
		return false;
	}
}


bool getBusy(void)
{
	unsigned int result;
	result = strstr(mGateway.Wifi->Input_buffer, "\r\nbusy");
	if(result != 0)
	{
		uart_str(" {get busy}");
		mGateway.Wifi->flag.f_busy = true;
		return true;
	}
	else
	{
		return false;
	}
}


bool getTimeout(void)
{
	unsigned int result = strstr(mGateway.Wifi->Input_buffer, "\r\n+timeout");
	if(result != 0)	
	{
		uart_str(" {get +timeout}");
		mGateway.Wifi->flag.f_timeout = true;
		return true;	
	}
	else
	{
		return false;
	}
}



bool getOK(void)
{
	unsigned int result = strstr(mGateway.Wifi->Input_buffer, "\r\n\r\nOK");
	if(result != 0)
	{
		uart_str(" {get OK}");
		mGateway.Wifi->flag.f_ok = true;
		return true;
	}
	else
	{
		return false;
	}
}

bool getERROR(void)
{
	unsigned int result = strstr(mGateway.Wifi->Input_buffer, "\r\nERROR");
	if(result != 0)
	{
		mGateway.Wifi->flag.f_error = true;
		uart_str(" {get ERROR}");
		return true;
	}
	else
	{
		return false;
	}
}

bool getHTTP(void)
{
	unsigned int result = strstr(mGateway.Wifi->Input_buffer, "HTTP");
	if(result != 0)
	{
		mGateway.Wifi->flag.f_http = true;
		uart_str(" {get HTTP}");
		return true;
	}
	else
	{
		return false;
	}
}

bool getWIFI_GOT_IP(void)
{
	unsigned int result = strstr(mGateway.Wifi->Input_buffer, "WIFI GOT IP");
	if(result != 0)
	{
		mGateway.Wifi->flag.f_wifi_got_ip = true;
		uart_str(" {get WIFI GOT IP}");
		return true;
	}
	else
	{
		return false;
	}
}





void Reset_wifi_get_data(unsigned long tmpCounter)
{
	mGateway.Wifi->Rx_buffer_Count = 0;
	mGateway.Wifi->Rx_Timeout_Count = 0;
	for(unsigned int i = 0; i < WIFI_Input_buffer_Max; i++)	mGateway.Wifi->Input_buffer[i] = '\0';
	
	mGateway.Wifi->f_count = 0;
	mGateway.Wifi->f_count_Max = tmpCounter;
	mGateway.Wifi->f_count_Max_div = My_WiFi.f_count_Max / 2;
}

void Run_Wifi_Command_Func(void)
{
	char str[100];	
	unsigned int k, i;
	switch(My_WiFi.NowStatus)
	{
		case Wait_Send_AT:
			WiFi_txt_Separation_line();
			uart_str_COM(Debug_COM, "# Wait Send AT..\r");
			//===============================================
			Reset_wifi_get_data(wifi_delay_01);
			uart_str_COM(Wifi_COM, "AT\r\n");
			//===============================================
			mGateway.Wifi->NowCMD		= Wait_Send_AT;
			mGateway.Wifi->NowStatus	= Reset_flag_CMD;
			break;
		
		case Wait_Send_RESTORE:
			WiFi_txt_Separation_line();
			WiFi_txt_Function(&My_WiFi);
			My_WiFi.NowCMD		= Wait_Send_RESTORE;
			My_WiFi.NowStatus	= Send_CMD;
			break;
		
		case Wait_Send_RST:
			WiFi_txt_Separation_line();
			uart_str_COM(Debug_COM, "# Wait Send AT+RST..\r");
			//===============================================
			Reset_wifi_get_data(wifi_delay_12800ms);
			uart_str_COM(Wifi_COM, "AT+RST\r\n");
			//===============================================
			mGateway.Wifi->NowCMD		= Wait_Send_RST;
			mGateway.Wifi->NowStatus	= Reset_flag_CMD;
			break;
		
		case Wait_AT_CIFSR:
			WiFi_txt_Separation_line();
			WiFi_txt_Function(&My_WiFi);
			My_WiFi.NowCMD		= Wait_AT_CIFSR;
			My_WiFi.NowStatus	= Send_CMD;
			break;
		
		case Wait_AT_CWMODE_Set_1:
			WiFi_txt_Separation_line();
			WiFi_txt_Function(&My_WiFi);
			My_WiFi.NowCMD		= Wait_AT_CWMODE_Set_1;
			My_WiFi.NowStatus	= Send_CMD;
			break;
		
		case Wait_AT_CWMODE_Set_2:
			WiFi_txt_Separation_line();
			WiFi_txt_Function(&My_WiFi);
			My_WiFi.NowCMD		= Wait_AT_CWMODE_Set_2;
			My_WiFi.NowStatus	= Send_CMD;
			break;
		
		
		
		case Wait_AT_CWMODE_Set_3:
			WiFi_txt_Separation_line();
			uart_str_COM(Debug_COM, "# Wait Send AT+CWMODE_CUR=3..\r");
			//===============================================
			Reset_wifi_get_data(wifi_delay_01);
			uart_str_COM(Wifi_COM, "AT+CWMODE_CUR=3\r\n");
			//===============================================
			mGateway.Wifi->NowCMD		= Wait_AT_CWMODE_Set_3;
			mGateway.Wifi->NowStatus	= Reset_flag_CMD;
			break;
			
		case Wait_AT_CWSAP_CUR:
			WiFi_txt_Separation_line();
			uart_str_COM(Debug_COM, "# Wait Send AT CWSAP..\r");
			//===============================================
			Reset_wifi_get_data(wifi_delay_3200ms);
			uart_str_COM(Wifi_COM, "AT+CWSAP=\"Tree_Code_Wifi_Module\",\"12345678\",5,3\r\n");
			//===============================================
			mGateway.Wifi->NowCMD		= Wait_AT_CWSAP_CUR;
			mGateway.Wifi->NowStatus	= Reset_flag_CMD;
			break;
			
		case Wait_AT_CIPMUX:
			WiFi_txt_Separation_line();
			uart_str_COM(Debug_COM, "# Wait Send AT+CIPMUX=1..\r");
			//===============================================
			Reset_wifi_get_data(wifi_delay_01);
			uart_str_COM(Wifi_COM, "AT+CIPMUX=1\r\n");
			//===============================================
			mGateway.Wifi->NowCMD		= Wait_AT_CIPMUX;
			mGateway.Wifi->NowStatus	= Reset_flag_CMD;
			break;
			
			
		case Wait_AT_CIPSERVER:
			WiFi_txt_Separation_line();
			uart_str_COM(Debug_COM, "# Wait Send AT+CIPSERVER=1,80..\r");
			//===============================================
			Reset_wifi_get_data(wifi_delay_01);
			uart_str_COM(Wifi_COM, "AT+CIPSERVER=1,80\r\n");
			//===============================================
			mGateway.Wifi->NowCMD		= Wait_AT_CIPSERVER;
			mGateway.Wifi->NowStatus	= Reset_flag_CMD;
			break;
			
			
		case Wait_AT_CWLAPOPT:
			WiFi_txt_Separation_line();
			uart_str_COM(Debug_COM, "# Wait Send AT+CWLAPOPT=1,2047..\r");
			//===============================================
			Reset_wifi_get_data(wifi_delay_01);
			uart_str_COM(Wifi_COM, "AT+CWLAPOPT=1,2047\r\n");
			//Send_AT_CWLAPOPT();
			//===============================================
			mGateway.Wifi->NowCMD		= Wait_AT_CWLAPOPT;
			mGateway.Wifi->NowStatus	= Reset_flag_CMD;
			break;
			
		case Wait_AT_CWLAP:
			WiFi_txt_Separation_line();
			uart_str_COM(Debug_COM, "# Wait Send AT+CWLAP..\r");
			//===============================================
			Reset_wifi_get_data(wifi_delay_6400ms);
			uart_str_COM(Wifi_COM, "AT+CWLAP\r\n");
			//Send_AT_CWLAPOPT();
			//===============================================
			mGateway.Wifi->NowCMD		= Wait_AT_CWLAP;
			mGateway.Wifi->NowStatus	= Reset_flag_CMD;
			break;
			
			
		case Wait_AT_PING_8_8_8_8:
			WiFi_txt_Separation_line();
			uart_str_COM(Debug_COM, "# Wait Send AT+PING=8.8.8.8..\r");
			//===============================================
			Reset_wifi_get_data(wifi_delay_6400ms);
			uart_str_COM(Wifi_COM, "AT+PING=\"8.8.8.8\"\r\n");
			//===============================================
			mGateway.Wifi->NowCMD		= Wait_AT_PING_8_8_8_8;
			mGateway.Wifi->NowStatus	= Reset_flag_CMD;
			break;

		
		case Wait_AT_CWJAP_Join_SSID:
			WiFi_txt_Separation_line();
			uart_str_COM(Debug_COM, "# Wait Send AT CWJAP Join SSID..\r");
			//===============================================
			Reset_wifi_get_data(wifi_delay_12800ms);
			sprintf(GET_Update_tmp, "AT+CWJAP=\"%s\",\"%s\"\r\n", mGateway.Wifi->SSID_Buffer, mGateway.Wifi->PASSWORD_Buffer);
			uart_str_COM(Wifi_COM, GET_Update_tmp);
			//===============================================
			mGateway.Wifi->NowCMD		= Wait_AT_CWJAP_Join_SSID;
			mGateway.Wifi->NowStatus	= Reset_flag_CMD;
			break;
					
		case Wait_AT_CIPSTART_1:
			WiFi_txt_Separation_line();
			WiFi_txt_Function(&My_WiFi);
			My_WiFi.NowCMD		= Wait_AT_CIPSTART_1;
			My_WiFi.NowStatus	= Send_CMD;
			break;
		
		case Wait_AT_CIPSEND_1:
			WiFi_txt_Separation_line();
			WiFi_txt_Function(&My_WiFi);
			My_WiFi.NowCMD		= Wait_AT_CIPSEND_1;
			My_WiFi.NowStatus	= Send_CMD;
			break;
		
		case Wait_GET_1:
			WiFi_txt_Separation_line();
			WiFi_txt_Function(&My_WiFi);
			My_WiFi.NowCMD		= Wait_GET_1;
			My_WiFi.NowStatus	= Send_CMD;
			break;
		
		
		case Wait_AT_CIPCLOSE:
			WiFi_txt_Separation_line();
			WiFi_txt_Function(&My_WiFi);
			My_WiFi.NowCMD		= Wait_AT_CIPCLOSE;
			My_WiFi.NowStatus	= Send_CMD;
			break;
			
			
		case Wait_AT_CIPCLOSE_Wait_Unit:
			WiFi_txt_Separation_line();
			uart_str_COM(Debug_COM, "# Wait Send AT+CIPCLOSE=");
			uTXByte(Debug_COM, mGateway.Wifi->target_Number);
			uart_str_COM(Debug_COM, "..\r");
			//===============================================
			Reset_wifi_get_data(wifi_delay_01);
			uart_str_COM(Wifi_COM, "AT+CIPCLOSE=");
			uTXByte(Wifi_COM, mGateway.Wifi->target_Number);
			uart_str_COM(Wifi_COM, "\r\n");
			//===============================================
			mGateway.Wifi->NowCMD		= Wait_AT_CIPCLOSE_Wait_Unit;
			mGateway.Wifi->NowStatus	= Reset_flag_CMD;
			break;
		
		case Send_CMD:
			uart_str_COM(Debug_COM, " [Send CMD]");
			mGateway.Wifi->Rx_buffer_Count = 0;
			mGateway.Wifi->Rx_Timeout_Count = 0;
			for(unsigned int i = 0; i < WIFI_Input_buffer_Max; i++)	mGateway.Wifi->Input_buffer[i] = '\0';
			Setting_Send_CMD_Check_NowCMD();
			ResetWifiFlag();
			My_WiFi.NowStatus	= Wait_Reciver;
			break;
			
		case Reset_flag_CMD:
			uart_str_COM(Debug_COM, " [Reset Wifi All Flag]");
			ResetWifiFlag();
			My_WiFi.NowStatus	= Wait_Reciver;
			break;
			
		case Wait_Reciver:	
			Uart_Form_WIFI_callback(Wifi_COM);
			if(getWifiKeyWord() == true) 
			{
				My_WiFi.NowStatus = Check_Reciver; 
			}
			break;
			
		
		case Check_Reciver:
			WiFi_txt_Function(&My_WiFi);
			if(My_WiFi.Rx_buffer_Count == 0)
			{
				//沒有收到任何數值，重新測試
				if(My_WiFi.debug_flag)	uart_str_COM(Debug_COM, " [No Data]");
				My_WiFi.NowStatus = Send_CMD;
			}
			else
			{
				//有收到數值，但不確定是不是要的數值
				if(My_WiFi.debug_flag)	uart_str_COM(Debug_COM, " [Data OK]");
				My_WiFi.NowStatus = Print_Reciver_ASCII;
			}
			break;
		
		
		case Print_Reciver_ASCII:
			WiFi_txt_Function(&My_WiFi);
			if(My_WiFi.debug_flag)
			{
				Print_Line_Msg();
				for(unsigned int i = 0; i < My_WiFi.Rx_buffer_Count; i++)
				{
					uTXByte(Debug_COM, My_WiFi.Input_buffer[i]);
				}
				uTXByte(Debug_COM, 0x0d);
				Print_Line_Msg();
			}
			My_WiFi.NowStatus = Print_Reciver_MAC_CMD;
			break;
		
		case Print_Reciver_MAC_CMD:
			WiFi_txt_Function(&My_WiFi);
			Get_CMD_Form_PHP();
			My_WiFi.NowStatus = Check_Reciver_String;
			break;
		
		case Check_Reciver_String:
			WiFi_txt_Function(&My_WiFi);
			Check_Got_String();
			My_WiFi.NowStatus = WiFi_STOP;
			break;
		
		
		case Print_Reciver_Byte:
			WiFi_txt_Function(&My_WiFi);
			Print_Arrow_Right();
			if(My_WiFi.debug_flag)
			{
				for(unsigned char i = 0; i < My_WiFi.Rx_buffer_Count; i++)
				{
					uart_send_ascii_byte(Debug_COM, My_WiFi.Input_buffer[i]);
					uTXByte(Debug_COM, ' ');
				}
				uTXByte(Debug_COM, 0x0d);
			}
			
			My_WiFi.NowStatus = WiFi_STOP;
			break;
		
		case WiFi_STOP:
			WiFi_txt_Function(&My_WiFi);
			My_WiFi.NowStatus = WiFi_NOP;
			My_WiFi.f_count = 0;
			My_WiFi.f_count_Timer_Max = 200;
			//My_WiFi.f_count_Timer_Max = 5000;
			My_WiFi.NowStatus = WiFi_CMD_Delay;
			WiFi_txt_Function(&My_WiFi);
			break;
		
		case WiFi_CMD_Delay:
			if(My_WiFi.f_count > My_WiFi.f_count_Timer_Max)
			{
				My_WiFi.f_count = 0;
				My_WiFi.NowStatus = WiFi_NOP;
				WiFi_txt_Function(&My_WiFi);
			}
			break;
		
		case WiFi_NOP:

			break;
		
		case WiFi_Wait_Next_Loop:
			WiFi_txt_Function(&My_WiFi);
			My_WiFi.NowStatus = WiFi_NOP;
			break;
			
		default:
			break;
	}
}

/*
		dev_ptr[tmp_index].cmd_buf[0] = PC_WriteBufferHex[0];	//MAC
		dev_ptr[tmp_index].cmd_buf[1] = PC_WriteBufferHex[1];
		dev_ptr[tmp_index].cmd_buf[2] = PC_WriteBufferHex[2];
		dev_ptr[tmp_index].cmd_buf[3] = PC_WriteBufferHex[3];
		dev_ptr[tmp_index].cmd_buf[4] = PC_WriteBufferHex[4];	//功能
		dev_ptr[tmp_index].cmd_buf[5] = PC_WriteBufferHex[5];	//Counter
		dev_ptr[tmp_index].cmd_buf[6] = PC_WriteBufferHex[6];	//資料1
		dev_ptr[tmp_index].cmd_buf[7] = PC_WriteBufferHex[7];
		dev_ptr[tmp_index].cmd_buf[8] = PC_WriteBufferHex[8];	//資料2
		dev_ptr[tmp_index].cmd_buf[9] = PC_WriteBufferHex[9];
		dev_ptr[tmp_index].cmd_buf[10] = PC_WriteBufferHex[10];	//資料3
		dev_ptr[tmp_index].cmd_buf[11] = PC_WriteBufferHex[11];
		dev_ptr[tmp_index].cmd_buf[12] = PC_WriteBufferHex[12];	//參數1
		dev_ptr[tmp_index].cmd_buf[13] = PC_WriteBufferHex[13];	//參數2
		dev_ptr[tmp_index].cmd_buf[14] = PC_WriteBufferHex[14];	//參數3
		dev_ptr[tmp_index].cmd_buf[15] = PC_WriteBufferHex[15];	//參數4
*/











void CMD_to_HEX(char tmp_index)
{
	unsigned char *tmp_ret;
	unsigned char *ret;

	if(tmp_index == 0)			ret = strstr(My_WiFi.Input_buffer, "@MAC1");
	else if(tmp_index == 1)		ret = strstr(My_WiFi.Input_buffer, "@MAC2");
	else if(tmp_index == 2)		ret = strstr(My_WiFi.Input_buffer, "@MAC3");
	tmp_ret = ret;
	for(unsigned int i = 0; i < 46; i++)
	uTXByte(Debug_COM, *(ret + i));
	uTXByte(Debug_COM, 0x0d);
	ret = ret+5;
	
	CMD_MAC_Buffer[0] = ascii_to_hex(*ret);
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[0]);
	CMD_MAC_Buffer[1] = ascii_to_hex(*(ret + 1));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[1]);
	CMD_MAC_Buffer[2] = ascii_to_hex(*(ret + 2));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[2]);
	CMD_MAC_Buffer[3] = ascii_to_hex(*(ret + 3));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[3]);
	CMD_MAC_Buffer[4] = ascii_to_hex(*(ret + 4));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[4]);
	CMD_MAC_Buffer[5] = ascii_to_hex(*(ret + 5));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[5]);
	CMD_MAC_Buffer[6] = ascii_to_hex(*(ret + 6));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[6]);
	CMD_MAC_Buffer[7] = ascii_to_hex(*(ret + 7));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[7]);
	
	CMD_MAC[0] = CMD_MAC_Buffer[0] << 4 | CMD_MAC_Buffer[1];
	CMD_MAC[1] = CMD_MAC_Buffer[2] << 4 | CMD_MAC_Buffer[3];
	CMD_MAC[2] = CMD_MAC_Buffer[4] << 4 | CMD_MAC_Buffer[5];
	CMD_MAC[3] = CMD_MAC_Buffer[6] << 4 | CMD_MAC_Buffer[7];
	
	
	ret = tmp_ret + 14;
	CMD_MAC_Buffer[0] = ascii_to_hex(*ret);
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[0]);
	CMD_MAC_Buffer[1] = ascii_to_hex(*(ret + 1));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[1]);
	CMD_MAC[4] = CMD_MAC_Buffer[0] << 4 | CMD_MAC_Buffer[1];
	
	ret = tmp_ret + 17;
	CMD_MAC_Buffer[0] = ascii_to_hex(*ret);
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[0]);
	CMD_MAC_Buffer[1] = ascii_to_hex(*(ret + 1));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[1]);
	CMD_MAC[5] = CMD_MAC_Buffer[0] << 4 | CMD_MAC_Buffer[1];
	
	ret = tmp_ret + 20;
	CMD_MAC_Buffer[0] = ascii_to_hex(*ret);
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[0]);
	CMD_MAC_Buffer[1] = ascii_to_hex(*(ret + 1));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[1]);
	CMD_MAC_Buffer[2] = ascii_to_hex(*(ret + 2));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[2]);
	CMD_MAC_Buffer[3] = ascii_to_hex(*(ret + 3));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[3]);
	CMD_MAC[6] = CMD_MAC_Buffer[0] << 4 | CMD_MAC_Buffer[1];
	CMD_MAC[7] = CMD_MAC_Buffer[2] << 4 | CMD_MAC_Buffer[3];
	
	ret = tmp_ret + 25;
	CMD_MAC_Buffer[0] = ascii_to_hex(*ret);
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[0]);
	CMD_MAC_Buffer[1] = ascii_to_hex(*(ret + 1));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[1]);
	CMD_MAC_Buffer[2] = ascii_to_hex(*(ret + 2));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[2]);
	CMD_MAC_Buffer[3] = ascii_to_hex(*(ret + 3));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[3]);
	CMD_MAC[8] = CMD_MAC_Buffer[0] << 4 | CMD_MAC_Buffer[1];
	CMD_MAC[9] = CMD_MAC_Buffer[2] << 4 | CMD_MAC_Buffer[3];
	
	ret = tmp_ret + 30;
	CMD_MAC_Buffer[0] = ascii_to_hex(*ret);
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[0]);
	CMD_MAC_Buffer[1] = ascii_to_hex(*(ret + 1));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[1]);
	CMD_MAC_Buffer[2] = ascii_to_hex(*(ret + 2));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[2]);
	CMD_MAC_Buffer[3] = ascii_to_hex(*(ret + 3));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[3]);
	CMD_MAC[10] = CMD_MAC_Buffer[0] << 4 | CMD_MAC_Buffer[1];
	CMD_MAC[11] = CMD_MAC_Buffer[2] << 4 | CMD_MAC_Buffer[3];
	
	ret = tmp_ret + 35;
	CMD_MAC_Buffer[0] = ascii_to_hex(*ret);
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[0]);
	CMD_MAC_Buffer[1] = ascii_to_hex(*(ret + 1));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[1]);
	CMD_MAC[12] = CMD_MAC_Buffer[0] << 4 | CMD_MAC_Buffer[1];
	
	ret = tmp_ret + 38;
	CMD_MAC_Buffer[0] = ascii_to_hex(*ret);
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[0]);
	CMD_MAC_Buffer[1] = ascii_to_hex(*(ret + 1));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[1]);
	CMD_MAC[13] = CMD_MAC_Buffer[0] << 4 | CMD_MAC_Buffer[1];
	
	ret = tmp_ret + 41;
	CMD_MAC_Buffer[0] = ascii_to_hex(*ret);
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[0]);
	CMD_MAC_Buffer[1] = ascii_to_hex(*(ret + 1));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[1]);
	CMD_MAC[14] = CMD_MAC_Buffer[0] << 4 | CMD_MAC_Buffer[1];
	
	ret = tmp_ret + 44;
	CMD_MAC_Buffer[0] = ascii_to_hex(*ret);
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[0]);
	CMD_MAC_Buffer[1] = ascii_to_hex(*(ret + 1));
	//uart_send_ascii_byte(Debug_COM, CMD_MAC_Buffer[1]);
	CMD_MAC[15] = CMD_MAC_Buffer[0] << 4 | CMD_MAC_Buffer[1];
	
	//uTXByte(Debug_COM, 0x0d);
	
	for(unsigned char i = 0; i < 16; i++)
	{
		uart_send_ascii_byte(Debug_COM,CMD_MAC[i]);
		uTXByte(Debug_COM, ' ');
	}
	uTXByte(Debug_COM, 0x0d);
	
	for(unsigned char i = 0; i < Dev_Item_MAX; i++)
	{
		if(dev_ptr[i].MAC_ID.byte[0] == CMD_MAC[0] && dev_ptr[i].MAC_ID.byte[1] == CMD_MAC[1] && dev_ptr[i].MAC_ID.byte[2] == CMD_MAC[2] && dev_ptr[i].MAC_ID.byte[3] == CMD_MAC[3])
		{
			uart_str("No.\0");
			uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(dev_ptr[i].Dev_Number));
			uTXByte(Debug_COM, ' ');
			uart_send_ascii_byte(Debug_COM, dev_ptr[i].MAC_ID.byte[0]);
			uart_send_ascii_byte(Debug_COM, dev_ptr[i].MAC_ID.byte[1]);
			uart_send_ascii_byte(Debug_COM, dev_ptr[i].MAC_ID.byte[2]);
			uart_send_ascii_byte(Debug_COM, dev_ptr[i].MAC_ID.byte[3]);
			uTXByte(Debug_COM, ' ');
			uTXByte(Debug_COM, '=');
			uTXByte(Debug_COM, ' ');
			uart_send_ascii_byte(Debug_COM, CMD_MAC[0]);
			uart_send_ascii_byte(Debug_COM, CMD_MAC[1]);
			uart_send_ascii_byte(Debug_COM, CMD_MAC[2]);
			uart_send_ascii_byte(Debug_COM, CMD_MAC[3]);
			
			dev_ptr[i].cmd_buf[0] = CMD_MAC[0];	//MAC
			dev_ptr[i].cmd_buf[1] = CMD_MAC[1];
			dev_ptr[i].cmd_buf[2] = CMD_MAC[2];
			dev_ptr[i].cmd_buf[3] = CMD_MAC[3];
			dev_ptr[i].cmd_buf[4] = CMD_MAC[4];	//功能
			dev_ptr[i].cmd_buf[5] = CMD_MAC[5];	//Counter
			dev_ptr[i].cmd_buf[6] = CMD_MAC[6];	//資料1
			dev_ptr[i].cmd_buf[7] = CMD_MAC[7];
			dev_ptr[i].cmd_buf[8] = CMD_MAC[8];	//資料2
			dev_ptr[i].cmd_buf[9] = CMD_MAC[9];
			dev_ptr[i].cmd_buf[10] = CMD_MAC[10];	//資料3
			dev_ptr[i].cmd_buf[11] = CMD_MAC[11];
			dev_ptr[i].cmd_buf[12] = CMD_MAC[12];	//參數1
			dev_ptr[i].cmd_buf[13] = CMD_MAC[13];	//參數2
			dev_ptr[i].cmd_buf[14] = CMD_MAC[14];	//參數3
			dev_ptr[i].cmd_buf[15] = CMD_MAC[15];	//參數4
			dev_ptr[i].cmd_buf[16] = 0x33;	//0X33
			dev_ptr[i].cmd_buf[17] = 0x33;	//0X33
			dev_ptr[i].cmd_buf_Sent_Flag = cmd_buf_Sent_New;
			dev_ptr[i].cmd_buf_Return_OK_Flag = false;
			dev_ptr[i].cmd_buf_to_clear_mysql_Flag = false;

			uTXByte(Debug_COM, 0x0d);
			break;
		}
		
	}
	
}

void Get_CMD_Form_PHP(void)
{
	
	unsigned char *ret;
	ret = 0;
	ret = strstr(My_WiFi.Input_buffer, "@MAC1");
	if(ret == 0)	return; 
	
	
	uart_str_COM(Debug_COM, "=======================\r");
	CMD_to_HEX(0);
	CMD_to_HEX(1);
	CMD_to_HEX(2);
	uart_str_COM(Debug_COM, "=======================\r");
}



//=================================================
#endif
//=================================================