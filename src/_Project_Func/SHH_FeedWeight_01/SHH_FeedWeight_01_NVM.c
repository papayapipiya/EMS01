#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


//=================================================
#ifdef SHH_FeedWeight_01
//=================================================

//==========
//	紀錄飼料桶打到飼料線的重量
//==========
void Save_Feed_Count_in_NVM(void)
{
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	UINT32u_t	tmp32;
	UINT16u_t	tmp16;
	
	do
	{
		error_code = nvm_read_buffer(
		527  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	tmp_page_buffer[0] = SaveACCFeedWeight.byte[3];
	tmp_page_buffer[1] = SaveACCFeedWeight.byte[2];
	tmp_page_buffer[2] = SaveACCFeedWeight.byte[1];
	tmp_page_buffer[3] = SaveACCFeedWeight.byte[0];
	
	tmp16.word = WeightCount;
	tmp_page_buffer[4] = tmp16.byte[1];
	tmp_page_buffer[5] = tmp16.byte[0];
	
	do
	{
		error_code = nvm_erase_row(
		527 * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	do
	{
		error_code = nvm_write_buffer(
		527  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	Last_FeedWeight.dword = SaveACCFeedWeight.dword;
}


//==========
//	讀取飼料桶打到飼料線的重量
//==========
void Load_Feed_Count_in_NVM(void)
{
	UINT32u_t	tmp32;
	UINT16u_t	tmp16;
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	do
	{
		error_code = nvm_read_buffer(
		527  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	
	SaveACCFeedWeight.byte[3] = tmp_page_buffer[0];
	SaveACCFeedWeight.byte[2] = tmp_page_buffer[1];
	SaveACCFeedWeight.byte[1] = tmp_page_buffer[2];
	SaveACCFeedWeight.byte[0]=  tmp_page_buffer[3];
	
	if(SaveACCFeedWeight.dword > Max_Feeding_Value) SaveACCFeedWeight.dword = 0;
	Last_FeedWeight.dword = SaveACCFeedWeight.dword;
	
	tmp16.byte[1] = tmp_page_buffer[4];
	tmp16.byte[0] = tmp_page_buffer[5];
	WeightCount = tmp16.word;
}




//==========
//	紀錄校正後的重量倍數
//==========
void Save_Weight_Count_in_NVM(void)
{
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	UINT32u_t	tmp32;
	
	do
	{
		error_code = nvm_read_buffer(
		526  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	tmp32.dword = RawDataZero;
	tmp_page_buffer[0] = tmp32.byte[3];
	tmp_page_buffer[1] = tmp32.byte[2];
	tmp_page_buffer[2] = tmp32.byte[1];
	tmp_page_buffer[3] = tmp32.byte[0];
	tmp32.dword = RawDataUser;
	tmp_page_buffer[4] = tmp32.byte[3];
	tmp_page_buffer[5] = tmp32.byte[2];
	tmp_page_buffer[6] = tmp32.byte[1];
	tmp_page_buffer[7] = tmp32.byte[0];
	tmp32.dword = RawDataBase;
	tmp_page_buffer[8] = tmp32.byte[3];
	tmp_page_buffer[9] = tmp32.byte[2];
	tmp_page_buffer[10] = tmp32.byte[1];
	tmp_page_buffer[11] = tmp32.byte[0];
	
	
	do
	{
		error_code = nvm_erase_row(
		526 * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	do
	{
		error_code = nvm_write_buffer(
		526  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
}


//==========
//	讀取校正後的重量倍數
//==========
void Load_Weight_Count_in_NVM(void)
{
	UINT32u_t	tmp32;
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	do
	{
		error_code = nvm_read_buffer(
		526  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	tmp32.byte[3] = tmp_page_buffer[0];
	tmp32.byte[2] = tmp_page_buffer[1];
	tmp32.byte[1] = tmp_page_buffer[2];
	tmp32.byte[0] = tmp_page_buffer[3];
	RawDataZero = tmp32.dword;
	
	tmp32.byte[3] = tmp_page_buffer[4];
	tmp32.byte[2] = tmp_page_buffer[5];
	tmp32.byte[1] = tmp_page_buffer[6];
	tmp32.byte[0] = tmp_page_buffer[7];
	RawDataUser = tmp32.dword;
	
	tmp32.byte[3] = tmp_page_buffer[8];
	tmp32.byte[2] = tmp_page_buffer[9];
	tmp32.byte[1] = tmp_page_buffer[10];
	tmp32.byte[0] = tmp_page_buffer[11];
	RawDataBase = tmp32.dword;

	//RawDataBase = 200000000 / (RawDataUser - RawDataZero);
	
	RawDataBase = 2000000000 / (RawDataUser - RawDataZero);
	
	//RawDataBase = 0x0403;
	//2KG
}

//=================================================
#endif
//=================================================