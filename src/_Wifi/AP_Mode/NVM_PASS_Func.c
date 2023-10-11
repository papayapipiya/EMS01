/*
 * NVM_PASS_Func.c
 *
 * Created: 2021/4/17 下午 11:26:33
 *  Author: willchen
 */ 

#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "myDefine.h"



void Load_Save_PASSWORD_txt_in_NVM(void)
{
	UINT32u_t	tmp32;
	UINT16u_t	tmp16;
	uint8_t		tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	uart_str_COM(Debug_COM, "\r Load PASSWORD NVM : ");
	do
	{
		error_code = nvm_read_buffer(
		PASSWORD_txt_NVM  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	for(unsigned char j = 0; j < SSID_PASSWORD_Max; j++)
	mGateway.Wifi->PASSWORD_Buffer[j] = 0;
	
	unsigned char k = 0;
	for(unsigned char i = 0; i < NVMCTRL_PAGE_SIZE; i++)
	{
		if(tmp_page_buffer[i] == 0x00) break;
		mGateway.Wifi->PASSWORD_Buffer[i] = tmp_page_buffer[i];
		uTXByte(Debug_COM, mGateway.Wifi->PASSWORD_Buffer[i]);
	}
	uart_str_COM(Debug_COM, " [done]");
}

void Save_PASSWORD_txt_in_NVM(void)
{
	UINT32u_t	tmp32;
	UINT16u_t	tmp16;
	uint8_t		tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	uart_str_COM(Debug_COM, "Save PASSWORD NVM : ");
	do
	{
		error_code = nvm_read_buffer(
		PASSWORD_txt_NVM  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	for(unsigned char i = 0; i < NVMCTRL_PAGE_SIZE; i++)
	tmp_page_buffer[i] = mGateway.Wifi->userKeyin_PASSWORD_Buffer[i];
	
	
	do
	{
		error_code = nvm_erase_row(
		PASSWORD_txt_NVM * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	wdt_reset_count();
	delay_ms(30);
	wdt_reset_count();
	delay_ms(30);
	wdt_reset_count();
	delay_ms(30);
	wdt_reset_count();
	delay_ms(30);
	wdt_reset_count();
	delay_ms(30);
	
	do
	{
		error_code = nvm_write_buffer(
		PASSWORD_txt_NVM  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	uart_str_COM(Debug_COM, "Save PASSWORD NVM Done..\r");
}