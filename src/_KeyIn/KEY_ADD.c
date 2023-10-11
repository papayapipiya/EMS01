#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



void Judg_KEY_ADD(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	unsigned char tmp_list_table_index;
	
	if(	ascii_to_hex(UserKeyInBuf[0]) == KEYA &&
		ascii_to_hex(UserKeyInBuf[1]) == KEYD &&
		ascii_to_hex(UserKeyInBuf[2]) == KEYD )	//	add device
	{
		if(UserKeyInBufCount == 16 && UserKeyInBuf[16] == 0x0d)
		{
			tmp_list_table_index =	Cal_list_table_index(UserKeyInBuf[4], UserKeyInBuf[5], UserKeyInBuf[6]);
			if(tmp_list_table_index > Dev_Item_MAX) return;
			tmp_list_table_index =  Search_list_table_index(tmp_list_table_index, tmp_LORA_Dev_List_Index);
			if(tmp_list_table_index != Not_found_Index)
			{
				Print_Find_Msg();
				Print_One_Form_List_Table(tmp_list_table_index, tmp_LORA_Dev_List_Index);
				Print_Arrow_Right();
				
				tmp_LORA_Dev_List_Index[tmp_list_table_index].Dev_En_Flag = Dev_Dis;
				tmp_LORA_Dev_List_Index[tmp_list_table_index].MAC_ID.byte[0] = (ascii_to_hex(UserKeyInBuf[8])  << 4) | (ascii_to_hex(UserKeyInBuf[9] ));
				tmp_LORA_Dev_List_Index[tmp_list_table_index].MAC_ID.byte[1] = (ascii_to_hex(UserKeyInBuf[10]) << 4) | (ascii_to_hex(UserKeyInBuf[11]));
				tmp_LORA_Dev_List_Index[tmp_list_table_index].MAC_ID.byte[2] = (ascii_to_hex(UserKeyInBuf[12]) << 4) | (ascii_to_hex(UserKeyInBuf[13]));
				tmp_LORA_Dev_List_Index[tmp_list_table_index].MAC_ID.byte[3] = (ascii_to_hex(UserKeyInBuf[14]) << 4) | (ascii_to_hex(UserKeyInBuf[15]));

				Save_List_in_NVM(tmp_LORA_Dev_List_Index);
				Load_List_Form_NVM(tmp_LORA_Dev_List_Index);
				Print_One_Form_List_Table(tmp_list_table_index, tmp_LORA_Dev_List_Index);
				uTXByte(Debug_COM, 0x0d);
				uTXByte(Debug_COM, 0x0d);
			}
		}
	}
}