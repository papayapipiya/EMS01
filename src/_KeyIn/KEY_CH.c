#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

void Judg_KEY_CH(void)
{
	UINT16u_t tmp16;
	unsigned int tmp_value;
	unsigned char tmp_index;
	
	
	if(	ascii_to_hex(UserKeyInBuf[0]) == KEYC && ascii_to_hex(UserKeyInBuf[1]) == KEYH )
	{
		if(UserKeyInBufCount == 9 && UserKeyInBuf[8] == 0x0d  && UserKeyInBuf[3] == ' ')
		{
			tmp_index = ascii_to_hex(UserKeyInBuf[2]);
			tmp_value = 0;
			tmp_value  = ascii_to_hex(UserKeyInBuf[4]) * 1000;
			tmp_value += ascii_to_hex(UserKeyInBuf[5]) * 100;
			tmp_value += ascii_to_hex(UserKeyInBuf[6]) * 10;
			tmp_value += ascii_to_hex(UserKeyInBuf[7]);
			
			for (unsigned char i = 0 ; i < 4; i++)
			{
				uart_str("CH\0");
				uTXByte(Debug_COM, 0x30 + i);
				uTXByte(Debug_COM, ':');
				uart_send_word(Debug_COM, hex_to_bcd(CH_Formula[i]));
				uTXByte(Debug_COM, ' ');
				uTXByte(Debug_COM, '(');
				uart_send_word(Debug_COM, CH_Formula_base[i]);
				uTXByte(Debug_COM, ')');
				uart_str("\t\0");
			}
			uTXByte(Debug_COM, 0x0d);
			
			uart_str("CH\0");
			uTXByte(Debug_COM, UserKeyInBuf[2]);
			uTXByte(Debug_COM, ':');
			uart_send_word(Debug_COM, hex_to_bcd(CH_Formula[tmp_index]));
			Print_Arrow_Right();
			
			CH_Formula[tmp_index] = tmp_value;
			uart_str("CH\0");
			uTXByte(Debug_COM, UserKeyInBuf[2]);
			uTXByte(Debug_COM, ':');
			uart_send_word(Debug_COM, hex_to_bcd(CH_Formula[tmp_index]));
			uTXByte(Debug_COM, 0x0d);
			
			uTXByte(Debug_COM, 0x0d);
			
			Save_LoRa_Profile_in_NVM(LORA01);
			Load_LoRa_Profile_in_NVM(LORA01);
			
			uTXByte(Debug_COM, 0x0d);
		}
		else if(UserKeyInBufCount == 3 && UserKeyInBuf[2] == 0x0d)
		{
			{
				for (unsigned char i = 0 ; i < 4; i++)
				{
					uart_str("CH\0");
					uTXByte(Debug_COM, 0x30 + i);
					uTXByte(Debug_COM, ':');
					uart_send_word(Debug_COM, hex_to_bcd(CH_Formula[i]));
					uTXByte(Debug_COM, ' ');
					uTXByte(Debug_COM, '(');
					uart_send_word(Debug_COM, CH_Formula_base[i]);
					uTXByte(Debug_COM, ')');
					
					uart_str("\t\0");
				}
				uTXByte(Debug_COM, 0x0d);
			}
		}
	}
}