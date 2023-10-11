#include <asf.h>
#include <stdio.h>
#include "myDefine.h"
#include <string.h> 


void Main_Startup(void)
{
	UINT16u_t tmp16;
	volatile Write_PLC_List_t *Write_PLC_List_Index = Write_PLC_List_In_RAM;
	volatile LORA_Dev_List_t *LORA_Dev_List_Index = LORA_Dev_List_In_RAM;
		uint16_t timeout = 0;
	IO_init();
	system_init();
	
	delay_init();

	
	LED_G_ON
	LED_R_ON
	LED_B_ON
	configure_usart();
	configure_usart_callbacks();
	//while(1)
	uart_str_COM(Debug_COM, "\r mcu [uart]");
	
	uart_str_COM(Debug_COM, "\r mcu [i2c]");
	configure_i2c_master();
	

	uart_str_COM(Debug_COM, "\r mcu [adc]");
	configure_adc();
	
	timer_configure();
	uart_str_COM(Debug_COM, "\r mcu [timer]");
	
	/*
	configure_extint_channel();
	configure_extint_callbacks();
	uart_str_COM(Debug_COM, "\r mcu [exint]");
	*/
	
	configure_wdt();
	configure_wdt_callbacks();
	uart_str_COM(Debug_COM, "\r mcu [wdt]");
	
	//configure_nvm();
	//uart_str_COM(Debug_COM, "\r mcu [nvm]");
	
	system_interrupt_enable_global();
	uart_str_COM(Debug_COM, "\r mcu [int on]");
	wdt_reset_count();
	
	
	#ifdef Dev_Wifi
		Wifi_inital();
	#endif
	
	return;
	
		//system_interrupt_enable_global();
	//return;

	/*
	LORA01_inital();
	uart_str_COM(Debug_COM, "[LoRA1]");
	LORA02_inital();
	uart_str_COM(Debug_COM, "[LoRA2]");
	Define_Profile_COPY_TO_RAM();
	uart_str_COM(Debug_COM, "[LoRA1 & 2 Profile]");
	
	Define_Dev_List_Func(mGateway.LORA_Dev_List_Index);
	Define_Dev_Time_Func(mGateway.LORA_Dev_List_Index);
	Load_List_Form_NVM(mGateway.LORA_Dev_List_Index);
	Load_Dev_Time_Form_NVM(mGateway.LORA_Dev_List_Index);
	*/
	//Load_Write_PLC_List_in_NVM(Write_PLC_List_Index);
	
	wdt_reset_count();
	
	//Print_Start_Msg();
	/*
	Load_LoRa_Profile_in_NVM(mGateway.LoRA01);
	Load_LoRa_Profile_in_NVM(LORA02);
	*/
	
	UserKeyInStauts = NOP;
	CMD_Index = CMD_NOP_Index;	

	Dev_Value_inital();
	
	wdt_reset_count();
	LORA01->f_count = 0;
	LORA02->f_count = 0;
	//RS485_RD_Index = 0;
	UserKeyInBufCount = 0;
/*
	LORA01->Lora_Debug_Flag = true;
	LORA02->Lora_Debug_Flag = true;

	LORA01->Lora_Show_Flag = true;
	LORA02->Lora_Show_Flag = true;
*/	
//	PLC_Show_w_Flag = true;
//	PLC_Show_r_Flag = true;

		
	LED_G_OFF
	LED_R_OFF
	LED_B_OFF
	


	
		
}