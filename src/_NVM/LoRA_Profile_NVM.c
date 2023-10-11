#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

void Load_LoRa_Profile_in_NVM(volatile LORA_DEFINE_t *tmp_LORA)
{
	UINT16u_t tmp16;
	unsigned char first_time;
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	do
	{
		error_code = nvm_read_buffer(
		525  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	if(tmp_page_buffer[tmp_LORA->NVM_offset_Value] != 0x55)
	{
		first_time = 1;
	}
	else
	{
		first_time = 0;
	}
	
	tmp_LORA->MyLoRaPower = tmp_page_buffer[tmp_LORA->NVM_offset_Value + 1];
	if(first_time == 0)
	{
		tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7] = tmp_LORA->MyLoRaPower;
	}
	else
	{
		tmp_LORA->MyLoRaPower = tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7];
	}
	
	tmp_LORA->MyLoRaSF = tmp_page_buffer[tmp_LORA->NVM_offset_Value + 2];
	if(first_time == 0)
	{
		tmp_LORA->MyLoRaTmp[Write_LoRa_Setting_Index][5] = tmp_LORA->MyLoRaSF;
	}
	else
	{
		tmp_LORA->MyLoRaSF = tmp_LORA->MyLoRaTmp[Write_LoRa_Setting_Index][5];
	}
	
	tmp_LORA->MyLoRaFQ.byte[2] = tmp_page_buffer[tmp_LORA->NVM_offset_Value + 3];
	tmp_LORA->MyLoRaFQ.byte[1] = tmp_page_buffer[tmp_LORA->NVM_offset_Value + 4];
	tmp_LORA->MyLoRaFQ.byte[0] = tmp_page_buffer[tmp_LORA->NVM_offset_Value + 5];
	if(first_time == 0)
	{
		tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][4] = tmp_LORA->MyLoRaFQ.byte[2];
	}
	else
	{
		tmp_LORA->MyLoRaFQ.byte[2] = tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][4];
	}
	if(first_time == 0)
	{
		tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][5] = tmp_LORA->MyLoRaFQ.byte[1];
	}
	else
	{
		tmp_LORA->MyLoRaFQ.byte[1] = tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][5];
	}
	if(first_time == 0)
	{
		tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][6] = tmp_LORA->MyLoRaFQ.byte[0];
	}
	else
	{
		tmp_LORA->MyLoRaFQ.byte[0] = tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][6];
	}
	
	tmp_LORA->Lora_Debug_Flag = tmp_page_buffer[tmp_LORA->NVM_offset_Value + 6];
	if(first_time == 1)
	{
		tmp_LORA->Lora_Debug_Flag = NeedDebug;
	}
	
	tmp_LORA->Lora_Debug_Flag = tmp_page_buffer[tmp_LORA->NVM_offset_Value + 6];
	if(first_time == 1)
	{
		tmp_LORA->Lora_Debug_Flag = NeedDebug;
	}
	
	RESET_COUNT = tmp_page_buffer[tmp_LORA->NVM_offset_Value + 7];
	
	tmp_LORA->UserLoRaID[0] = tmp_page_buffer[tmp_LORA->NVM_offset_Value + 8];
	tmp_LORA->UserLoRaID[1] = tmp_page_buffer[tmp_LORA->NVM_offset_Value + 9];
	tmp_LORA->UserLoRaID[2] = tmp_page_buffer[tmp_LORA->NVM_offset_Value + 10];
	tmp_LORA->UserLoRaID[3] = tmp_page_buffer[tmp_LORA->NVM_offset_Value + 11];
	
	tmp_LORA->PLC_Flag = tmp_page_buffer[tmp_LORA->NVM_offset_Value + 12];
	if(first_time == 1)
	{
		tmp_LORA->PLC_Flag = NoPLC;
	}
	//tmp_LORA->PLC_Flag = NoPLC;
	
	tmp_LORA->PLC_Addr = tmp_page_buffer[tmp_LORA->NVM_offset_Value + 13];
	if(first_time == 1)
	{
		tmp_LORA->PLC_Addr = 0x00;
	}
	




	

	if(tmp_LORA == LORA01)
		uart_str_COM(Debug_COM, "LORA 01 NVM: ");
	else
		uart_str_COM(Debug_COM, "LORA 02 NVM: ");
	uart_send_ascii_byte(Debug_COM, tmp_page_buffer[tmp_LORA->NVM_offset_Value]);
	uTXByte(Debug_COM, ' ');
	uart_send_ascii_byte(Debug_COM, tmp_page_buffer[tmp_LORA->NVM_offset_Value + 1]);
	uTXByte(Debug_COM, ' ');
	uart_send_ascii_byte(Debug_COM, tmp_page_buffer[tmp_LORA->NVM_offset_Value + 2]);
	uTXByte(Debug_COM, ' ');
	uart_send_ascii_byte(Debug_COM, tmp_page_buffer[tmp_LORA->NVM_offset_Value + 3]);
	uart_send_ascii_byte(Debug_COM, tmp_page_buffer[tmp_LORA->NVM_offset_Value + 4]);
	uart_send_ascii_byte(Debug_COM, tmp_page_buffer[tmp_LORA->NVM_offset_Value + 5]);
	uTXByte(Debug_COM, ' ');
	uart_send_ascii_byte(Debug_COM, tmp_page_buffer[tmp_LORA->NVM_offset_Value + 6]);
	uTXByte(Debug_COM, ' ');
	uart_send_ascii_byte(Debug_COM, tmp_page_buffer[tmp_LORA->NVM_offset_Value + 7]);
	uTXByte(Debug_COM, ' ');
	uart_send_ascii_byte(Debug_COM, tmp_LORA->UserLoRaID[0]);
	uart_send_ascii_byte(Debug_COM, tmp_LORA->UserLoRaID[1]);
	uart_send_ascii_byte(Debug_COM, tmp_LORA->UserLoRaID[2]);
	uart_send_ascii_byte(Debug_COM, tmp_LORA->UserLoRaID[3]);


	uTXByte(Debug_COM, 0x0d);
	
}

void Save_LoRa_Profile_in_NVM(volatile LORA_DEFINE_t *tmp_LORA)
{
	UINT16u_t tmp16;
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	do
	{
		error_code = nvm_read_buffer(
		525  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	tmp_page_buffer[tmp_LORA->NVM_offset_Value] = 0x55;
	tmp_page_buffer[tmp_LORA->NVM_offset_Value + 1] = tmp_LORA->MyLoRaPower;
	tmp_page_buffer[tmp_LORA->NVM_offset_Value + 2] = tmp_LORA->MyLoRaSF;
	tmp_page_buffer[tmp_LORA->NVM_offset_Value + 3] = tmp_LORA->MyLoRaFQ.byte[2];
	tmp_page_buffer[tmp_LORA->NVM_offset_Value + 4] = tmp_LORA->MyLoRaFQ.byte[1];
	tmp_page_buffer[tmp_LORA->NVM_offset_Value + 5] = tmp_LORA->MyLoRaFQ.byte[0];
	tmp_page_buffer[tmp_LORA->NVM_offset_Value + 6] = tmp_LORA->Lora_Debug_Flag;
	tmp_page_buffer[tmp_LORA->NVM_offset_Value + 7] = RESET_COUNT + 1;
	tmp_page_buffer[tmp_LORA->NVM_offset_Value + 8] = tmp_LORA->MyLoRaID[0];
	tmp_page_buffer[tmp_LORA->NVM_offset_Value + 9] = tmp_LORA->MyLoRaID[1];
	tmp_page_buffer[tmp_LORA->NVM_offset_Value + 10] = tmp_LORA->MyLoRaID[2];
	tmp_page_buffer[tmp_LORA->NVM_offset_Value + 11] = tmp_LORA->MyLoRaID[3];
	tmp_page_buffer[tmp_LORA->NVM_offset_Value + 12] = tmp_LORA->PLC_Flag;
	tmp_page_buffer[tmp_LORA->NVM_offset_Value + 13] = tmp_LORA->PLC_Addr;


	
	do
	{
		error_code = nvm_erase_row(
		525 * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	do
	{
		error_code = nvm_write_buffer(
		525  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
}