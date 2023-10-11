#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

//=================================================
#ifdef Dev_Water
//=================================================

void Dev_Value_inital(void)
{
	Load_Water_Count_in_NVM();
}

void LORA01_Reset_Func(bool tmp_bool)
{
	port_pin_set_output_level(PIN_PA07, tmp_bool);
}

void LORA02_Reset_Func(bool tmp_bool)
{
	port_pin_set_output_level(PIN_PA16, tmp_bool);
}


void LORA01_inital(void)
{
	LORA01 = &NewLORA01;
	LORA01->DevUART = &MyUart03;
	LORA01->NVM_offset_Value = 0;
	LORA01->List_Now = 0;
	LORA01->List_Next = 0;
	change_and_check(&(LORA01->NowStatus), Read_LoRa_ID_Index);
	change_and_check(&(LORA01->LoRa_TxRx_Mode_Flag), SetRx);
	change_and_check(&(LORA01->LoRa_Tx_Send_Time_Out_Count), 0);
	change_and_check(&(LORA01->LoRa_IRQ_Flag), 0);
	change_and_check(&(LORA01->LoRa_Send_Tx_Flag), LoraTxNoSend);
	LORA01->f_count = 0;
	LORA01->Lora_Show_Flag = true;
}

void LORA02_inital(void)
{
	LORA02 = &NewLORA02;
	LORA02->DevUART = &MyUart04;
	LORA02->NVM_offset_Value = 20;
	change_and_check(&(LORA02->NowStatus), Read_LoRa_ID_Index);
	change_and_check(&(LORA02->LoRa_TxRx_Mode_Flag), SetRx);
	change_and_check(&(LORA02->LoRa_Tx_Send_Time_Out_Count), 0);
	change_and_check(&(LORA02->LoRa_IRQ_Flag), 0);
	LORA02->Lora_Show_Flag = true;
}


void IO_init(void)
{
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);
	
	/* Configure LEDs as outputs, turn them off */
	
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	
	port_pin_set_config(PIN_PA07, &pin_conf);
	port_pin_set_output_level(PIN_PA07, true);
	port_pin_set_config(PIN_PA22, &pin_conf);
	port_pin_set_output_level(PIN_PA22, true);
	port_pin_set_config(PIN_PA16, &pin_conf);
	port_pin_set_output_level(PIN_PA16, true);
	
	//	RGB LED PIN DEFINE
	port_pin_set_config(PIN_PA23, &pin_conf);
	port_pin_set_output_level(PIN_PA23, true);
	port_pin_set_config(PIN_PA27, &pin_conf);
	port_pin_set_output_level(PIN_PA27, true);
	port_pin_set_config(PIN_PA28, &pin_conf);
	port_pin_set_output_level(PIN_PA28, true);

	
	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	port_pin_set_config(PIN_PA11, &pin_conf);
	port_pin_set_output_level(PIN_PA11, false);
	port_pin_set_config(PIN_PA17, &pin_conf);
	port_pin_set_output_level(PIN_PA17, false);

	port_pin_set_config(PIN_PA02, &pin_conf);
	port_pin_set_output_level(PIN_PA02, true);
	port_pin_set_config(PIN_PA03, &pin_conf);
	port_pin_set_output_level(PIN_PA03, true);
	
	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	port_pin_set_config(PIN_PA10, &pin_conf);
	port_pin_set_output_level(PIN_PA10, false);	// ADC因為是用數位IO所以需要設定為輸入
}

//void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA)
void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	UINT16u_t tmp_adc;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3] = tmp_LORA->MyLoRaID[0];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4] = tmp_LORA->MyLoRaID[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5] = tmp_LORA->MyLoRaID[2];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6] = tmp_LORA->MyLoRaID[3];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][7] = 0x04; //水錶
	
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][8] = 0x00;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][9] = 0x00;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][10] = water_count.byte[3];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][11] = water_count.byte[2];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][12] = water_count.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][13] = water_count.byte[0];
	
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][14] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][15] = tmp_adc.byte[0];
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][16] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][17] = tmp_adc.byte[0];
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][18] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][19] = tmp_adc.byte[0];
	
	tmp_adc.word = ADC_LVD_Temp;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][20] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][21] = tmp_adc.byte[0];
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][22] = 0;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][23] = 0;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][24] = Version;
	
	//預留給RSSI
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][25] = 0;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][26] = 0;
	
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7] = tmp_LORA->MyBuffer[7];
	tmp_LORA->MyLoRaPower = tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7];
	
	
	
	Setting_Lora_To_Tx_Mode_Func(tmp_LORA);
	if(LORA01->Lora_Show_Flag == true)
	{
		uTXByte(Debug_COM, 0x0d);
		uTXByte(Debug_COM, 'Y');
		uTXByte(Debug_COM, 'e');
		uTXByte(Debug_COM, 's');
		uTXByte(Debug_COM, ' ');
		
		uart_send_ascii_byte(Debug_COM, water_count.byte[3]);
		uart_send_ascii_byte(Debug_COM, water_count.byte[2]);
		uart_send_ascii_byte(Debug_COM, water_count.byte[1]);
		uart_send_ascii_byte(Debug_COM, water_count.byte[0]);
		uTXByte(Debug_COM, ' ');

		uTXByte(Debug_COM, '(');
		uart_send_word(Debug_COM, ADC_LVD_Temp);
		uTXByte(Debug_COM, ')');
		uTXByte(Debug_COM, 0x0d);
	}
	ADC_LVD_Temp = 0;
}

void Print_FW_ID(void)
{
	uTXByte(Debug_COM, 'W');
	uTXByte(Debug_COM, 'a');
	uTXByte(Debug_COM, 't');
	uTXByte(Debug_COM, 'e');
	uTXByte(Debug_COM, 'r');
	uTXByte(Debug_COM, ' ');
	uTXByte(Debug_COM, ':');
	uTXByte(Debug_COM, ' ');
	uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(Version));
}

void Print_Water_Info_Func(void)
{
	uTXByte(Debug_COM, 0x09);
	uTXByte(Debug_COM, '(');
	uart_send_word(Debug_COM, Last_water_count.word[1]);
	uart_send_word(Debug_COM, Last_water_count.word[0]);
	uTXByte(Debug_COM, ')');
	uTXByte(Debug_COM, ' ');
	uart_send_word(Debug_COM, water_count.word[1]);
	uart_send_word(Debug_COM, water_count.word[0]);
	uTXByte(Debug_COM, 0x09);
	uTXByte(Debug_COM, 'S');
	uTXByte(Debug_COM, 'a');
	uTXByte(Debug_COM, 'v');
	uTXByte(Debug_COM, 'e');
	uTXByte(Debug_COM, ' ');
	uTXByte(Debug_COM, 'T');
	uTXByte(Debug_COM, 'i');
	uTXByte(Debug_COM, 'm');
	uTXByte(Debug_COM, 'e');
	uTXByte(Debug_COM, 'r');
	uTXByte(Debug_COM, ':');
	uTXByte(Debug_COM, ' ');
	uart_send_word(Debug_COM, water_nvm_savetimer_count);
	uTXByte(Debug_COM, ' ');
	uTXByte(Debug_COM, '(');
	uart_send_word(Debug_COM, water_nvm_savetimer_count_Max);
	uTXByte(Debug_COM, ')');
	uTXByte(Debug_COM, 0x0d);
}

void Water_Detect_Func(void)
{
	switch(water_input_clk_flag)
	{
		case 0:
			if(port_pin_get_input_level(PIN_PA02) == false)
			{
				water_input_clk_flag = 1;
			}
			break;			
			
		case 3:
			water_input_clk_flag = 4;
			if(water_count.dword == 999999999)
			{
				water_count.dword = 0;
			}
			else
			{
				water_count.dword++;	
			}
			uTXByte(Debug_COM, 'W');
			uTXByte(Debug_COM, 'a');
			uTXByte(Debug_COM, 't');
			uTXByte(Debug_COM, 'e');
			uTXByte(Debug_COM, 'r');
			uTXByte(Debug_COM, ' ');
			uTXByte(Debug_COM, 'O');
			uTXByte(Debug_COM, 'N');
			Print_Water_Info_Func();
			break;
			
		case 4:
			if(port_pin_get_input_level(PIN_PA02) == true)
			{
				water_input_clk_flag = 5;
			}
			break;
			
		case 6:
			water_input_clk_flag = 0;
			uTXByte(Debug_COM, 'W');
			uTXByte(Debug_COM, 'a');
			uTXByte(Debug_COM, 't');
			uTXByte(Debug_COM, 'e');
			uTXByte(Debug_COM, 'r');
			uTXByte(Debug_COM, ' ');
			uTXByte(Debug_COM, 'O');
			uTXByte(Debug_COM, 'F');
			uTXByte(Debug_COM, 'F');
			Print_Water_Info_Func();
			break;
		
		default:
			if(water_input_clk_flag < 3)
			{
				if(port_pin_get_input_level(PIN_PA02) == true)
				{	
					water_input_clk_flag = 0;
				}
				else
				{
					water_input_clk_flag++;	
				}
			}
			else
			if(water_input_clk_flag < 6)
			{
				if(port_pin_get_input_level(PIN_PA02) == false)
				{
					water_input_clk_flag = 4;
				}
				else
				{
					water_input_clk_flag++;
				}
			}
			
			break;
	}	
	
	if(water_nvm_savetimer_count >= water_nvm_savetimer_count_Max)
	{
		if(Last_water_count.dword != water_count.dword)
		{
			Save_Water_Count_in_NVM();
			uTXByte(Debug_COM, 's');
			uTXByte(Debug_COM, 'a');
			uTXByte(Debug_COM, 'v');
			uTXByte(Debug_COM, 'e');
			uTXByte(Debug_COM, 0x0d);
			
		}
		else
		{
			uTXByte(Debug_COM, 'N');
			uTXByte(Debug_COM, 'O');
			uTXByte(Debug_COM, ' ');
			uTXByte(Debug_COM, 's');
			uTXByte(Debug_COM, 'a');
			uTXByte(Debug_COM, 'v');
			uTXByte(Debug_COM, 'e');
			uTXByte(Debug_COM, 0x0d);
		}
		water_nvm_savetimer_count = 0;
	}

	
}



void Save_Water_Count_in_NVM(void)
{
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	UINT32u_t	tmp32;
	
	do
	{
		error_code = nvm_read_buffer(
		151  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	tmp_page_buffer[0] = water_count.byte[3];
	tmp_page_buffer[1] = water_count.byte[2];
	tmp_page_buffer[2] = water_count.byte[1];
	tmp_page_buffer[3] = water_count.byte[0];
	
	do
	{
		error_code = nvm_erase_row(
		151 * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	do
	{
		error_code = nvm_write_buffer(
		151  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	Last_water_count.dword = water_count.dword;
}



void Load_Water_Count_in_NVM(void)
{
	UINT32u_t	tmp32;
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	do
	{
		error_code = nvm_read_buffer(
		151  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	water_count.byte[3] = tmp_page_buffer[0];
	water_count.byte[2] = tmp_page_buffer[1];
	water_count.byte[1] = tmp_page_buffer[2];
	water_count.byte[0]=  tmp_page_buffer[3];
	
	Last_water_count.dword = water_count.dword;

}

void Wait_RX_IRQ_Index_Func(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->LoRa_IRQ_Flag == 1)
	{
		LED_R_ON
		wdt_reset_count();
		change_and_check(&(tmp_LORA->NowStatus), Read_LoRa_RxBuffer_Index);
	}
}


//=================================================
#endif
//=================================================