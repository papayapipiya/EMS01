#include <asf.h>
#include <stdio.h>
#include "myDefine.h"
#include "math.h"


//=================================================
#ifdef Dev_Sensor
//=================================================

void Save_Sensor_Ref_mem_in_NVM(void);
void Load_Sensor_Ref_mem_in_NVM(void);
#define Sensor_Ref_mem_NVM	547

void mem_inital(void)
{
	volatile PLC_Dev_List_t *Sensor_Mem = PLC_D_Buf;
	for(unsigned char i = 0; i < PLC_D_Buf_Max; i++)
	{
		PLC_D_Buf[i].DataBuf.word = i;
	}
	Sensor_mem.NowStatus = mem_LOAD;
	Sensor_mem.Debug_Flag = true;
	Print_mem_list();
}

void mem_main(void)
{
	switch(Sensor_mem.NowStatus)
	{
		case mem_LOAD:
		Load_Sensor_Ref_mem_in_NVM();
		for(unsigned char i = 0; i < PLC_D_Buf_Max; i++)
		{
			if(PLC_D_Buf[i].DataBuf.word != PLC_D_Buf[i].LastDataBuf.word )
			{
				PLC_D_Buf[i].LastDataBuf.word = PLC_D_Buf[i].DataBuf.word;
			}
		}
		Print_mem_list();
		Sensor_mem.Change_Flag = false;
		Sensor_mem.NowStatus = mem_NOP;
		break;
		
		case mem_SAVE:
		Save_Sensor_Ref_mem_in_NVM();
		Sensor_mem.NowStatus = mem_LOAD;
		break;
		
		case mem_CHECK:
		for(unsigned char i = 0; i < PLC_D_Buf_Max; i++)
		{
			if(PLC_D_Buf[i].DataBuf.word != PLC_D_Buf[i].LastDataBuf.word)
			{
				Sensor_mem.Change_Flag = true;
				Sensor_mem.Wait_Save_Count = Wait_Save_Count_Max;
				Sensor_mem.NowStatus = mem_Wait_SAVE_MSG;
				break;
			}
		}
		break;
		
		case mem_Wait_SAVE_MSG:
		if(Sensor_mem.Debug_Flag) uart_str_COM(Debug_COM, "mem_Wait_SAVE_MSG..\r");
		Sensor_mem.NowStatus = mem_Wait_SAVE;
		break;
		
		case mem_Wait_SAVE:
		if(Sensor_mem.Debug_Flag) uart_str_COM(Debug_COM, "mem_Wait_SAVE..\r");
		if(Sensor_mem.Wait_Save_Count > 0)
			Sensor_mem.Wait_Save_Count--;
		else
			Sensor_mem.NowStatus = mem_SAVE;
		break;
		
		case mem_NOP:
		Sensor_mem.NowStatus = mem_CHECK;
		break;
	}
}


void Save_Sensor_Ref_mem_in_NVM(void)
{
	UINT32u_t	tmp32;
	UINT16u_t	tmp16;
	uint8_t		tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	if(Sensor_mem.Debug_Flag) uart_str_COM(Debug_COM, "Save Sensor NVM..\r");
	do
	{
		error_code = nvm_read_buffer(
		Sensor_Ref_mem_NVM  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	unsigned char k = 0;
	for(unsigned char i = 0; i < (PLC_D_Buf_Max * 2); i = i + 2)
	{
		uart_str("No.\0");
		uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(k));
		uart_str("   \0");
		uart_send_ascii_byte(Debug_COM, PLC_D_Buf[k].DataBuf.byte[1]);
		uart_send_ascii_byte(Debug_COM, PLC_D_Buf[k].DataBuf.byte[0]);
		tmp_page_buffer[i]		= PLC_D_Buf[k].DataBuf.byte[1];
		tmp_page_buffer[i + 1]	= PLC_D_Buf[k].DataBuf.byte[0];
		uart_str_COM(Debug_COM, " --> ");
		uart_send_ascii_byte(Debug_COM, tmp_page_buffer[i]);
		uart_send_ascii_byte(Debug_COM, tmp_page_buffer[i + 1]);
		uart_str("\r");
		k++;
	}
	
	
	do
	{
		error_code = nvm_erase_row(
		Sensor_Ref_mem_NVM * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	do
	{
		error_code = nvm_write_buffer(
		Sensor_Ref_mem_NVM  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	if(Sensor_mem.Debug_Flag) uart_str_COM(Debug_COM, "done..\r");
}



void Load_Sensor_Ref_mem_in_NVM(void)
{
	UINT32u_t	tmp32;
	UINT16u_t	tmp16;
	uint8_t		tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	if(Sensor_mem.Debug_Flag) uart_str_COM(Debug_COM, "Load Sensor NVM..");
	do
	{
		error_code = nvm_read_buffer(
		Sensor_Ref_mem_NVM  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	unsigned char k = 0;
	for(unsigned char i = 0; i < PLC_D_Buf_Max * 2; i= i + 2)
	{
		PLC_D_Buf[k].DataBuf.byte[1] = tmp_page_buffer[i];
		PLC_D_Buf[k].DataBuf.byte[0] = tmp_page_buffer[i + 1];
		k++;
	}
	if(Sensor_mem.Debug_Flag) uart_str_COM(Debug_COM, "done..\r");
}


//=================================================
#endif
//=================================================
