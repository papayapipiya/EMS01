
#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


void Print_List_Table(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	unsigned char i, tmp_count;
	tmp_count = 0;
	for ( i = 0; i < Dev_Item_MAX; i++)
	{
		Print_One_Form_List_Table(i, tmp_LORA_Dev_List_Index);
		if(tmp_count == 3)
		{
			uTXByte(Debug_COM, 0x0d);
			tmp_count = 0;
		}
		else
		{
			uTXByte(Debug_COM, 0x09);	
			tmp_count++;
		}
	}
}

void Judg_KEY_L(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	if(	ascii_to_hex(UserKeyInBuf[0]) == KEYL)
	{
		if(UserKeyInBufCount == 2 && UserKeyInBuf[1] == 0x0d)
		{
			//Clean_LiSt_Table();
			Load_List_Form_NVM(tmp_LORA_Dev_List_Index);
			Print_List_Table(tmp_LORA_Dev_List_Index);
			CMD_Index = CMD_NOP_Index;
		}
	}
}