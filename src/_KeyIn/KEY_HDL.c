#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

#if defined(HMI_001)

void Judg_KEY_HDL(void)
{
	if(	ascii_to_hex(UserKeyInBuf[0]) != KEYH && ascii_to_hex(UserKeyInBuf[1]) != KEYD && ascii_to_hex(UserKeyInBuf[2]) != KEYL ) return;
	if(UserKeyInBufCount != 4 && UserKeyInBuf[3] != 0x0d) return;
		
	unsigned int i, j, k;
	k = 0;
	for(i = 0 ; i < 255; i++)
	{
		uart_str("[\0");
		uart_send_word(Debug_COM, hex_to_bcd(i));
		uart_str("] \0");
		uart_send_word(Debug_COM, PLC_D[i].D);
		uart_str("  \0");
		if(k == 5)
		{
			uTXByte(Debug_COM, 0x0d);	
			k = 0;
		}
		else
		{
			k++;
		}
	}
		
}

#endif