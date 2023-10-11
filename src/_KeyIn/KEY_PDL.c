#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

void Judg_KEY_PDL(void)
{
	//UINT16u_t tmp16;
	volatile PLC_Dev_List_t *PLC_Dev_List_Index = PLC_D_Buf;
	
	if(	ascii_to_hex(UserKeyInBuf[0]) == KEYP &&
		ascii_to_hex(UserKeyInBuf[1]) == KEYD &&
		ascii_to_hex(UserKeyInBuf[2]) == KEYL )
	{
		if(UserKeyInBufCount == 4 && UserKeyInBuf[3] == 0x0d)
		{
			unsigned int i, j, k;
			//k = 0;
			for(i = 0 ; i < PLC_D_Buf_Max; i = i + PLC_D_Buf_Print_Columns_Max)
			{
				uart_str("No.\0");
				uart_send_word(Debug_COM, PLC_Dev_List_Index[i].Chage_Flag_Index);
				uart_str("   \0");
				for(j = 0 ; j < PLC_D_Buf_Print_Columns_Max; j++)
				{
					
					uTXByte(Debug_COM, '[');
					uart_send_word(Debug_COM, hex_to_bcd( i + j ));
					uTXByte(Debug_COM, ']');
					uTXByte(Debug_COM, ' ');
					uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[i + j].DataBuf.byte[1]);
					uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[i + j].DataBuf.byte[0]);
					uTXByte(Debug_COM, ' ');
					uTXByte(Debug_COM, '{');
					uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[i + j].LastDataBuf.byte[1]);
					uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[i + j].LastDataBuf.byte[0]);
					uTXByte(Debug_COM, '}');
					uTXByte(Debug_COM, ' ');
					uTXByte(Debug_COM, ',');
					uTXByte(Debug_COM, ' ');
				}
				uTXByte(Debug_COM, 0x0d);
				//k++;
			}
		}

	}
}