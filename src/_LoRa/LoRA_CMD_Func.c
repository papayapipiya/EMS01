#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



unsigned int Cal_list_table_index(unsigned char tmpA, unsigned char tmpB, unsigned char tmpC)
{
	return	ascii_to_hex(tmpA) * 100 + ascii_to_hex(tmpB) * 10 + ascii_to_hex(tmpC);
}

void CMD_Save_Func(volatile LORA_DEFINE_t *tmp_LORA)
{
	//if(tmp_LORA->Lora_initial_Flag == 1)
	//{
		Print_Lora_number(tmp_LORA);
		Save_LoRa_Profile_in_NVM(tmp_LORA);
		Load_LoRa_Profile_in_NVM(tmp_LORA);
		CMD_Index = CMD_NOP_Index;
	//}
}


void CMD_Func(void)
{

	switch(CMD_Index)
	{
		
		case CMD_NOP_Index:
			break;
	}
	

}