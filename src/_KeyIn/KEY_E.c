#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


void Judg_KEY_E(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	unsigned char tmp_list_table_index;
	
	if(ascii_to_hex(UserKeyInBuf[0]) && UserKeyInBuf[1] == ' ' )	//enable & disable
	{
		if(UserKeyInBufCount == 5 && UserKeyInBuf[5] == 0x0d)
		{
			tmp_list_table_index =	Cal_list_table_index(UserKeyInBuf[2], UserKeyInBuf[3], UserKeyInBuf[4]);
			tmp_list_table_index =  Search_list_table_index(tmp_list_table_index, tmp_LORA_Dev_List_Index);
			if(tmp_list_table_index != Not_found_Index)
			{
				Print_Find_Msg();
				Print_One_Form_List_Table(tmp_list_table_index, tmp_LORA_Dev_List_Index);
				Print_Arrow_Right();
				if(tmp_LORA_Dev_List_Index[tmp_list_table_index].Dev_En_Flag == Dev_Dis)
				{
					//Dev_List[tmp_list_table_index][1] = Dev_EN;
					tmp_LORA_Dev_List_Index[tmp_list_table_index].Dev_En_Flag = Dev_EN;
				}
				else
				{
					//Dev_List[tmp_list_table_index][1] = Dev_Dis;
					tmp_LORA_Dev_List_Index[tmp_list_table_index].Dev_En_Flag = Dev_Dis;
				}
				Save_List_in_NVM(tmp_LORA_Dev_List_Index);
				Load_List_Form_NVM(tmp_LORA_Dev_List_Index);
				Print_One_Form_List_Table(tmp_list_table_index, tmp_LORA_Dev_List_Index);
				uTXByte(Debug_COM, 0x0d);
			}
		}
	}
}