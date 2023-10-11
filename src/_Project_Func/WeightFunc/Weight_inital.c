#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

//=================================================
#ifdef Dev_Weight
//=================================================

void Dev_Value_inital(void)
{
	delay_ms(50);
	sw_0_timer = 0;
	sw_2000_timer = 0;
	Last_Weight_Temp = 35;
	SYSTEM_RESET_FLAG = 1;
	Load_Weight_Count_in_NVM();
	//RawDataBase = 200000000 / (RawDataUser - RawDataZero);
	ON_Display();
	WeightStatus = Wegight_Catch_Index;
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
	LORA01->NowStatus = Read_LoRa_ID_Index;
	LORA01->LoRa_TxRx_Mode_Flag = SetRx;
	LORA01->LoRa_Tx_Send_Time_Out_Count = 0;
	LORA01->LoRa_IRQ_Flag = 0;
	LORA01->LoRa_Send_Tx_Flag = LoraTxNoSend;
	LORA01->f_count = 0;
	LORA01->Lora_Show_Flag = false;
}

void LORA02_inital(void)
{
	LORA02 = &NewLORA02;
	LORA02->DevUART = &MyUart04;
	LORA02->NVM_offset_Value = 20;
	LORA02->NowStatus = Read_LoRa_ID_Index;
	LORA02->LoRa_TxRx_Mode_Flag = SetRx;
	LORA02->LoRa_Tx_Send_Time_Out_Count = 0;
	LORA02->LoRa_IRQ_Flag = 0;
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
	
	port_pin_set_config(PIN_PA00, &pin_conf);
	port_pin_set_output_level(PIN_PA00, true);
	port_pin_set_config(PIN_PA01, &pin_conf);
	port_pin_set_output_level(PIN_PA01, true);
	port_pin_set_config(PIN_PA04, &pin_conf);
	port_pin_set_output_level(PIN_PA04, true);
	port_pin_set_config(PIN_PA05, &pin_conf);
	port_pin_set_output_level(PIN_PA05, true);
	
	/*
	port_pin_set_config(PIN_PA22, &pin_conf);
	port_pin_set_output_level(PIN_PA22, true);
	*/
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
	port_pin_set_config(PIN_PA11, &pin_conf);	//LoRA interrupt
	port_pin_set_output_level(PIN_PA11, false);
	
	port_pin_set_config(PIN_PA17, &pin_conf);	//load cell module
	port_pin_set_output_level(PIN_PA17, false);


	port_pin_set_config(PIN_PA02, &pin_conf);
	port_pin_set_output_level(PIN_PA02, true);
	port_pin_set_config(PIN_PA03, &pin_conf);
	port_pin_set_output_level(PIN_PA03, true);
	
	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	port_pin_set_config(PIN_PA10, &pin_conf);
	port_pin_set_output_level(PIN_PA10, false);	// ADC因為是用數位IO所以需要設定為輸入
}


void Print_FW_ID(void)
{
	uart_str("Weight: \0");
	uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(Version));
}




void Flash_Display(void)
{
	uTXByte(Display_COM, 'F');
	uTXByte(Display_COM, '2');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, 'E');
}

void OFF_Display(void)
{
	uTXByte(Display_COM, 'F');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, 'E');
}

void ON_Display(void)
{
	uTXByte(Display_COM, 'F');
	uTXByte(Display_COM, '1');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, 'E');
}


void SET_Display_Value(unsigned long tmp_value)
{
	UINT32u_t	tmp32;
	UINT8u_t	tmp8;

	tmp32.dword = tmp_value;

	uTXByte(Display_COM, 'S');
	tmp8.byte = tmp32.byte[2];
	uTXByte(Display_COM, tmp8.HL.H+0x30);
	uTXByte(Display_COM, tmp8.HL.L+0x30);
	tmp8.byte = tmp32.byte[1];
	uTXByte(Display_COM, tmp8.HL.H+0x30);
	uTXByte(Display_COM, tmp8.HL.L+0x30);
	tmp8.byte = tmp32.byte[0];
	uTXByte(Display_COM, tmp8.HL.H+0x30);
	uTXByte(Display_COM, tmp8.HL.L+0x30);
	uTXByte(Display_COM, 'E');
}




unsigned long long_to_bcd_4byte(unsigned long tmp_data)
{
	UINT32u_t	tmp32;
	UINT16u_t	tmp16;
	UINT8u_t	tmp8;
	uint32_t	value;

	value = tmp_data;
	
	tmp8.HL.H = value / 10000000;
	value = value % 10000000;
	tmp8.HL.L = value / 1000000;
	value = value % 1000000;
	tmp32.byte[3] = tmp8.byte;

	tmp8.HL.H = value / 100000;
	value = value % 100000;
	tmp8.HL.L = value / 10000;
	value = value % 10000;
	tmp32.byte[2] = tmp8.byte;
	
	tmp8.HL.H = value / 1000;
	value = value % 1000;
	tmp8.HL.L = value / 100;
	value = value % 100;
	tmp32.byte[1] = tmp8.byte;

	tmp8.HL.H = value / 10;
	value = value % 10;
	tmp8.HL.L = value;
	tmp32.byte[0] = tmp8.byte;

	return tmp32.dword;
}



void tx_byte(unsigned char tmp_data)
{
	UINT8u_t tmp8;
	tmp8.byte = tmp_data;
	uTXByte(Debug_COM, hex_to_ascii(tmp8.HL.H));
	uTXByte(Debug_COM, hex_to_ascii(tmp8.HL.L));
}

void tx_dword(unsigned long tmp_data)
{
	UINT32u_t tmp32;
	tmp32.dword = tmp_data;
	tx_byte(tmp32.byte[3]);
	tx_byte(tmp32.byte[2]);
	tx_byte(tmp32.byte[1]);
	tx_byte(tmp32.byte[0]);
}

//=================================================
#endif
//=================================================