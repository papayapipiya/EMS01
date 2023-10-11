/*
 * NVM_EMAIL_Func.c
 *
 * Created: 2021/4/19 上午 09:23:51
 *  Author: willchen
 */ 

#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "myDefine.h"


void Save_EMAIL_txt_in_NVM(void)
{
	UINT32u_t	tmp32;
	UINT16u_t	tmp16;
	uint8_t		tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	uart_str_COM(Debug_COM, "\r nvm_read_buffer email..");
	do
	{
		error_code = nvm_read_buffer(
		EMAIL_txt_NVM  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	uart_str_COM(Debug_COM, "\r tmp_page_buffer <-- Input_EMAIL_Buffer");
	for(unsigned char i = 0; i < NVMCTRL_PAGE_SIZE; i++)
	tmp_page_buffer[i] = Input_EMAIL_Buffer[i];
	
	uart_str_COM(Debug_COM, "\r nvm_erase_row email");
	do
	{
		error_code = nvm_erase_row(
		EMAIL_txt_NVM * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE);
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
	
	uart_str_COM(Debug_COM, "\r nvm_write_buffer email");
	do
	{
		error_code = nvm_write_buffer(
		EMAIL_txt_NVM  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	uart_str_COM(Debug_COM, "\r Save EMAIL NVM Done..");
}



void Load_Save_EMAIL_txt_in_NVM(void)
{
	UINT32u_t	tmp32;
	UINT16u_t	tmp16;
	uint8_t		tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	uart_str_COM(Debug_COM, "\r Load EMAIL NVM : ");
	do
	{
		error_code = nvm_read_buffer(
		EMAIL_txt_NVM  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	for(unsigned char j = 0; j < SSID_PASSWORD_Max; j++)
	EMAIL_Buffer[j] = 0;
	
	unsigned char k = 0;
	for(unsigned char i = 0; i < NVMCTRL_PAGE_SIZE; i++)
	{
		if(tmp_page_buffer[i] == 0x00) break;
		EMAIL_Buffer[i] = tmp_page_buffer[i];
		uTXByte(Debug_COM, EMAIL_Buffer[i]);
	}
	uart_str_COM(Debug_COM, " [done]");
}
