#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


void Judg_KEY_HELP(void)
{
	unsigned char tmp_list_table_index;
	
	if(	ascii_to_hex(UserKeyInBuf[0]) == KEYH &&
		ascii_to_hex(UserKeyInBuf[1]) == KEYE &&
		ascii_to_hex(UserKeyInBuf[2]) == KEYL &&
		ascii_to_hex(UserKeyInBuf[3]) == KEYP )	//	add device
	{
		if(UserKeyInBufCount == 4 && UserKeyInBuf[4] == 0x0d)
		{
			uart_str("[LoRA Command]\r\0");
			uart_str(" LoRA Debug Mode:         D1, D2\r\0");
			uart_str(" LoRA Status:             S1, S2\r\0");
			uart_str(" LoRA FQ:                 SF1 XXXXX, SF2 XXXXX\r\0");
			uart_str(" LoRA SF:                 SS1 XX, SS2 XX (07~12)\r\0");
			uart_str(" LoRA Power:              SP1 XX, SP2 XX (02~17)\r\0");
			uart_str(" LoRA ID:                 SI1 XXXXXXXX, SI2 XXXXXXXX\r\0");
			uart_str(" LoRA Rundown Debug Mode: SHOW\r\0");
			uart_str("[Device Command]\r\0");
			uart_str(" ADD Device:              ADD XXX XXXXXXXX\r\0");
			uart_str(" Enable Device:           E XXX\r\0");
			uart_str(" Device Timer:            T XXX XXXX\r\0");
			uart_str(" Device List:             L\r\0");
			uart_str(" Device Data List:        DL\r\0");
			uart_str("[PLC Command]\r\0");
			uart_str(" PLC ADDR:                PA XX\r\0");
			uart_str(" Enable PLC:              PLC\r\0");
			uart_str(" PLC Write MAC List:      PL\r\0");
			uart_str(" Set PLC Write MAC:       PL XXX XXX XX\r\0");
			uart_str(" Enable PLC Write MAC :   PLE XXX\r\0");
			uart_str(" PLC Data List:           PDL\r\0");
			uart_str(" PLC Write Debug Mode:    PSOHWW\r\0");
			uart_str(" PLC Read Debug Mode:     PSOHWR\r\0");
			
			
			uTXByte(Debug_COM, 0x0d);
		}
	}
}