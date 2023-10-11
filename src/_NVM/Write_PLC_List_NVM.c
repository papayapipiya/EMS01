#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


void Save_Write_PLC_List_in_NVM(volatile Write_PLC_List_t *tmp_Write_PLC_List_Index)
{
	unsigned int i, j, k;
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	uint8_t page_buffer[NVM_Write_PLC_List_MAX];
	//Print_Write_NVM_Msg();
	//======================================
	// Dev_List (二維) 轉換成 page_buffer (一維)
	//======================================
	for( i = 0; i < Write_PLC_List_Max; i++)
	{
		k = i * Write_PLC_List_Item_Max;
		page_buffer[k]		= tmp_Write_PLC_List_Index[i].MAC_ID_Index.byte[1];
		page_buffer[k + 1]	= tmp_Write_PLC_List_Index[i].MAC_ID_Index.byte[0];
		page_buffer[k + 2]	= tmp_Write_PLC_List_Index[i].Channel_Index;
		page_buffer[k + 3]	= tmp_Write_PLC_List_Index[i].Channel_En;
	}
	
	//======================================
	// NVM一次只能寫一個PAGE = 64 BYTE
	//======================================
	k = 0;
	for(j = 0; j < (NVMCTRL_PAGE_SIZE * NVM_Write_PLC_List_Page_MAX); j = j + NVMCTRL_PAGE_SIZE)
	{
		//======================================
		// page_buffer 分次存到 tmp_page_buffer，準備存到NVM使用
		//======================================
		for(i = 0; i < NVMCTRL_PAGE_SIZE; i++)
		{
			tmp_page_buffer[i] = page_buffer[i + j];
		}
		//======================================
		// 儲存前要先erase
		//======================================
		do
		{
			error_code = nvm_erase_row(
			(535 + k) * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE);
		} while (error_code == STATUS_BUSY);
		
		//======================================
		// 開始儲存寫入
		//======================================
		do
		{
			error_code = nvm_write_buffer(
			(535 + k)  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
			tmp_page_buffer, NVMCTRL_PAGE_SIZE);
		} while (error_code == STATUS_BUSY);
		//======================================
		// K = page count，每+1 = 256 位置
		// 因為位置計算公式為 NVMCTRL_ROW_PAGES(4) * NVMCTRL_PAGE_SIZE (64) = 256個位置
		//======================================
		k++;
	}
}


void Load_Write_PLC_List_in_NVM(volatile Write_PLC_List_t *tmp_Write_PLC_List_Index)
{
	unsigned int i, j, k;
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	uint8_t page_buffer[NVM_Write_PLC_List_MAX];
	
	//Print_Read_NVM_Msg();
	k = 0;
	//	64 BYTE				24個自訂義PAGE
	for(j = 0; j < (NVMCTRL_PAGE_SIZE * NVM_Write_PLC_List_Page_MAX); j = j + NVMCTRL_PAGE_SIZE)
	{
		do
		{
			error_code = nvm_read_buffer(
			(535 + k)  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
			tmp_page_buffer, NVMCTRL_PAGE_SIZE);
		} while (error_code == STATUS_BUSY);
		
		for(i = 0; i < NVMCTRL_PAGE_SIZE; i++)
		{
			page_buffer[i + j] = tmp_page_buffer[i];
		}
		k++;
	}
	
	for( i = 0; i < Write_PLC_List_Max; i++)
	{
		tmp_Write_PLC_List_Index[i].MAC_ID_Index.byte [1]	= page_buffer[(i * Write_PLC_List_Item_Max)];
		tmp_Write_PLC_List_Index[i].MAC_ID_Index.byte [0]	= page_buffer[(i * Write_PLC_List_Item_Max) + 1];
		tmp_Write_PLC_List_Index[i].Channel_Index			= page_buffer[(i * Write_PLC_List_Item_Max) + 2];
		tmp_Write_PLC_List_Index[i].Channel_En				= page_buffer[(i * Write_PLC_List_Item_Max) + 3];
	}
}