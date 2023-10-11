#include <asf.h>
#include <stdio.h>
#include "myDefine.h"




//! [setup]
void configure_nvm(void)
{
	//! [setup_1]
	struct nvm_config config_nvm;
	//! [setup_1]

	//! [setup_2]
	nvm_get_config_defaults(&config_nvm);
	//! [setup_2]

	//! [setup_3]
	config_nvm.manual_page_write = false;
	//config_nvm.manual_page_write = true;
	//! [setup_3]

	//! [setup_4]
	nvm_set_config(&config_nvm);
	//! [setup_4]
}
//! [setup]




void Save_List_in_NVM(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	//UINT32u_t tmp_MAC_ID;
	unsigned int i, j, k;
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	uint8_t page_buffer[NVM_Dev_List_MAX];
	//Print_Write_NVM_Msg();
	//======================================
	// Dev_List (二維) 轉換成 page_buffer (一維)
	//======================================
	for( i = 0; i < Dev_Item_MAX; i++)
	{
		j = i * Dev_List_Data_Item_Max;
		page_buffer[j] = tmp_LORA_Dev_List_Index[i].Dev_Number;
		page_buffer[j+1] = tmp_LORA_Dev_List_Index[i].Dev_En_Flag;
		page_buffer[j+2] = tmp_LORA_Dev_List_Index[i].MAC_ID.byte[0];
		page_buffer[j+3] = tmp_LORA_Dev_List_Index[i].MAC_ID.byte[1];
		page_buffer[j+4] = tmp_LORA_Dev_List_Index[i].MAC_ID.byte[2];
		page_buffer[j+5] = tmp_LORA_Dev_List_Index[i].MAC_ID.byte[3];
	}
	
	//======================================
	// NVM一次只能寫一個PAGE = 64 BYTE
	//======================================
	k = 0;
	for(j = 0; j < (NVMCTRL_PAGE_SIZE * NVM_Dev_List_Use_Page_MAX); j = j + NVMCTRL_PAGE_SIZE)
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
			(500 + k) * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE);
		} while (error_code == STATUS_BUSY);
		
		//======================================
		// 開始儲存寫入
		//======================================
		do
		{
			error_code = nvm_write_buffer(
			(500 + k)  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
			tmp_page_buffer, NVMCTRL_PAGE_SIZE);
		} while (error_code == STATUS_BUSY);
		//======================================
		// K = page count，每+1 = 256 位置
		// 因為位置計算公式為 NVMCTRL_ROW_PAGES(4) * NVMCTRL_PAGE_SIZE (64) = 256個位置
		//======================================
		k++;
	}
}







void Load_List_Form_NVM(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	unsigned int i, j, k;
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	uint8_t page_buffer[NVM_Dev_List_MAX];
	
	//Print_Read_NVM_Msg();
	k = 0;
					//	64 BYTE				24個自訂義PAGE					
	for(j = 0; j < (NVMCTRL_PAGE_SIZE * NVM_Dev_List_Use_Page_MAX); j = j + NVMCTRL_PAGE_SIZE)
	{
		do
		{
			error_code = nvm_read_buffer(	(500 + k)  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
											tmp_page_buffer, NVMCTRL_PAGE_SIZE);
		} while (error_code == STATUS_BUSY);
		
		for(i = 0; i < NVMCTRL_PAGE_SIZE; i++)
		{
			page_buffer[i + j] = tmp_page_buffer[i];
		}
		k++;
	}
	
	for( i = 0; i < Dev_Item_MAX; i++)
	{
		j = i * Dev_List_Data_Item_Max;
		tmp_LORA_Dev_List_Index[i].Dev_Number = page_buffer[j];
		tmp_LORA_Dev_List_Index[i].Dev_En_Flag = page_buffer[j+1];
		tmp_LORA_Dev_List_Index[i].MAC_ID.byte[0] = page_buffer[j+2];
		tmp_LORA_Dev_List_Index[i].MAC_ID.byte[1] = page_buffer[j+3];
		tmp_LORA_Dev_List_Index[i].MAC_ID.byte[2] = page_buffer[j+4];
		tmp_LORA_Dev_List_Index[i].MAC_ID.byte[3] = page_buffer[j+5];
	}
}







