#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "myDefine.h"

//=================================================
#ifdef Dev_Wifi
//=================================================


void Load_Save_SSID_txt_in_NVM(void)
{
	UINT32u_t	tmp32;
	UINT16u_t	tmp16;
	uint8_t		tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	uart_str_COM(Debug_COM, "\r Load SSID NVM : ");
	do
	{
		error_code = nvm_read_buffer(
		SSID_txt_NVM  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	for(unsigned char j = 0; j < SSID_PASSWORD_Max; j++)
	mGateway.Wifi->SSID_Buffer[j] = 0;
	
	unsigned char k = 0;
	for(unsigned char i = 0; i < NVMCTRL_PAGE_SIZE; i++)
	{
		if(tmp_page_buffer[i] == 0x00) break;
		mGateway.Wifi->SSID_Buffer[i] = tmp_page_buffer[i];
		uTXByte(Debug_COM, mGateway.Wifi->SSID_Buffer[i]);
	}
	uart_str_COM(Debug_COM, " [done]");
}


void Save_SSID_txt_in_NVM(void)
{
	UINT32u_t	tmp32;
	UINT16u_t	tmp16;
	uint8_t		tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	uart_str_COM(Debug_COM, "Save SSID NVM : [1]");
	do
	{
		error_code = nvm_read_buffer(
		SSID_txt_NVM  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	uart_str_COM(Debug_COM, "[2]");
	for(unsigned char i = 0; i < NVMCTRL_PAGE_SIZE; i++)
		tmp_page_buffer[i] = mGateway.Wifi->userKeyin_SSID_Buffer[i];
	
	uart_str_COM(Debug_COM, "[3]");
	do
	{
		error_code = nvm_erase_row(
		SSID_txt_NVM * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE);
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
	uart_str_COM(Debug_COM, "[4]");
	do
	{
		error_code = nvm_write_buffer(
		SSID_txt_NVM  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	uart_str_COM(Debug_COM, "Save SSID NVM Done..\r");
}







//=================================================
#endif
//=================================================