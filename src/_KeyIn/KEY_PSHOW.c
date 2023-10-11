#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

void Judg_KEY_PSHOW(void)
{
	//UINT16u_t tmp16;
	
	if(	ascii_to_hex(UserKeyInBuf[0]) == KEYP &&
		ascii_to_hex(UserKeyInBuf[1]) == KEYS &&
		ascii_to_hex(UserKeyInBuf[2]) == KEYH &&
		ascii_to_hex(UserKeyInBuf[3]) == KEYO &&
		ascii_to_hex(UserKeyInBuf[4]) == KEYW)
	{
		if(UserKeyInBufCount == 7 && UserKeyInBuf[6] == 0x0d)
		{
			if(ascii_to_hex(UserKeyInBuf[5]) == KEYW)
			{
				if(PLC_Show_w_Flag == true)
					PLC_Show_w_Flag = false;
				else
					PLC_Show_w_Flag = true;	
			}
			else if(ascii_to_hex(UserKeyInBuf[5]) == KEYR)
			{
				if(PLC_Show_r_Flag == true)
					PLC_Show_r_Flag = false;
				else
					PLC_Show_r_Flag = true;
			}
		}
	}

}