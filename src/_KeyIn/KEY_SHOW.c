#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

void Judg_KEY_SHOW(void)
{
	//UINT16u_t tmp16;
	
	if(	ascii_to_hex(UserKeyInBuf[0]) == KEYS && 
		ascii_to_hex(UserKeyInBuf[1]) == KEYH &&
		ascii_to_hex(UserKeyInBuf[2]) == KEYO && 
		ascii_to_hex(UserKeyInBuf[3]) == KEYW)
	{
		if(UserKeyInBufCount == 4 && UserKeyInBuf[4] == 0x0d)
		{
			if(LORA01->Lora_Show_Flag)
			{
				LORA01->Lora_Show_Flag = false;
				uart_str_COM(Debug_COM, "LORA01 Show Dis..\r");
			}
			else
			{
				LORA01->Lora_Show_Flag = true;
				uart_str_COM(Debug_COM, "LORA01 Show En..\r");
			}
			
			if(LORA02->Lora_Show_Flag)
			{
				LORA02->Lora_Show_Flag = false;
				uart_str_COM(Debug_COM, "LORA02 Show Dis..\r");
			}
			else
			{
				LORA02->Lora_Show_Flag = true;
				uart_str_COM(Debug_COM, "LORA02 Show En..\r");
			}
			
		}
		else if(UserKeyInBufCount == 5 && ascii_to_hex(UserKeyInBuf[4]) == KEYW && 
				UserKeyInBuf[5] == 0x0d)
		{
			if(Show_weight_Flag) 
			{
				Show_weight_Flag = false; 
				uart_str_COM(Debug_COM, "Show_weight_Flag Show Dis..\r");
			}
			else 
			{
				Show_weight_Flag = true;
				uart_str_COM(Debug_COM, "Show_weight_Flag Show En..\r");
			}
		}
		else if(UserKeyInBufCount == 5 && ascii_to_hex(UserKeyInBuf[4]) == KEYI &&
				UserKeyInBuf[5] == 0x0d)
		{
			if(LORA01->Show_IIC)
			{
				LORA01->Show_IIC = false;
				uart_str_COM(Debug_COM, "LORA01->Show_IIC Show Dis..\r");
			}
			else
			{
				LORA01->Show_IIC = true;
				uart_str_COM(Debug_COM, "LORA01->Show_IIC Show En..\r");
			}
		}
		else if(UserKeyInBufCount == 5 && ascii_to_hex(UserKeyInBuf[4]) == KEYA &&
				UserKeyInBuf[5] == 0x0d)
		{
			if(LORA01->Show_Sensor_Raw)
				LORA01->Show_Sensor_Raw = false;
			else
				LORA01->Show_Sensor_Raw = true;
		}
		
	}
}