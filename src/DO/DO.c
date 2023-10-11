#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

//=================================================
#ifdef Dev_DO
//=================================================

void Dev_Value_inital(void)
{
	//nop
}

void LORA01_Reset_Func(bool tmp_bool)
{
	port_pin_set_output_level(PIN_PA17, tmp_bool);
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
	//	RGB LED PIN DEFINE
	port_pin_set_config(PIN_PA23, &pin_conf);
	port_pin_set_output_level(PIN_PA23, true);
	port_pin_set_config(PIN_PA27, &pin_conf);
	port_pin_set_output_level(PIN_PA27, true);
	port_pin_set_config(PIN_PA28, &pin_conf);
	port_pin_set_output_level(PIN_PA28, true);
	port_pin_set_config(PIN_PA17, &pin_conf);
	port_pin_set_output_level(PIN_PA17, false);
	
	port_pin_set_config(PIN_PA03, &pin_conf);
	port_pin_set_output_level(PIN_PA03, false);
	port_pin_set_config(PIN_PA04, &pin_conf);
	port_pin_set_output_level(PIN_PA04, false);
	port_pin_set_config(PIN_PA05, &pin_conf);
	port_pin_set_output_level(PIN_PA05, false);
	port_pin_set_config(PIN_PA06, &pin_conf);
	port_pin_set_output_level(PIN_PA06, false);

	
	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	port_pin_set_config(PIN_PA11, &pin_conf);
	port_pin_set_output_level(PIN_PA11, false);
	
	port_pin_set_config(PIN_PA10, &pin_conf);
	port_pin_set_output_level(PIN_PA10, false);	// ADC因為是用數位IO所以需要設定為輸入
	
	port_pin_set_config(PIN_PA00, &pin_conf);
	port_pin_set_output_level(PIN_PA00, false);
	

}


void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA)
{
	UINT16u_t tmp_adc;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3] = tmp_LORA->MyLoRaID[0];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4] = tmp_LORA->MyLoRaID[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5] = tmp_LORA->MyLoRaID[2];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6] = tmp_LORA->MyLoRaID[3];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][7] = 0x00;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7] = tmp_LORA->MyBuffer[7];
	tmp_LORA->MyLoRaPower = tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7];

	
	
	if(tmp_LORA->Lora_Show_Flag == true)
	{
		uTXByte(Debug_COM, 0x0d);
		uTXByte(Debug_COM, 'Y');
		uTXByte(Debug_COM, 'e');
		uTXByte(Debug_COM, 's');
		uTXByte(Debug_COM, 0x09);
		Print_Write_Buffer();
		uTXByte(Debug_COM, 0x0d);
	}
	//wdt_reset_count();
	tmp_adc.word = WriteBufferHex[4];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][8] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][9] = tmp_adc.byte[0];
	
	uart_send_ascii_byte(Debug_COM, tmp_adc.byte[1]);
	uart_send_ascii_byte(Debug_COM, tmp_adc.byte[0]);
	uTXByte(Debug_COM, ' ');
	
	//wdt_reset_count();
	tmp_adc.word = WriteBufferHex[5];
	//tmp_adc.word = tmp_adc.word / 2; //舊版電路使用
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][10] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][11] = tmp_adc.byte[0];
	
	uart_send_ascii_byte(Debug_COM, tmp_adc.byte[1]);
	uart_send_ascii_byte(Debug_COM, tmp_adc.byte[0]);
	uTXByte(Debug_COM, ' ');
	
	//wdt_reset_count();
	tmp_adc.word = WriteBufferHex[6];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][12] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][13] = tmp_adc.byte[0];
	
	uart_send_ascii_byte(Debug_COM, tmp_adc.byte[1]);
	uart_send_ascii_byte(Debug_COM, tmp_adc.byte[0]);
	uTXByte(Debug_COM, ' ');
	
	//wdt_reset_count();
	tmp_adc.word = WriteBufferHex[7];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][14] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][15] = tmp_adc.byte[0];
	
	uart_send_ascii_byte(Debug_COM, tmp_adc.byte[1]);
	uart_send_ascii_byte(Debug_COM, tmp_adc.byte[0]);
	uTXByte(Debug_COM, ' ');
	
	//wdt_reset_count();
	tmp_adc.word = 0x0000;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][16] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][17] = tmp_adc.byte[0];
	
	uart_send_ascii_byte(Debug_COM, tmp_adc.byte[1]);
	uart_send_ascii_byte(Debug_COM, tmp_adc.byte[0]);
	uTXByte(Debug_COM, ' ');
	
	//wdt_reset_count();
	tmp_adc.word = 0x0000;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][18] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][19] = tmp_adc.byte[0];
	
	uart_send_ascii_byte(Debug_COM, tmp_adc.byte[1]);
	uart_send_ascii_byte(Debug_COM, tmp_adc.byte[0]);
	uTXByte(Debug_COM, ' ');
	
	
	tmp_adc.word = ADC_LVD_Temp;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][20] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][21] = tmp_adc.byte[0];
	
	uTXByte(Debug_COM, '(');
	uart_send_ascii_byte(Debug_COM, tmp_adc.byte[1]);
	uart_send_ascii_byte(Debug_COM, tmp_adc.byte[0]);
	uTXByte(Debug_COM, ')');
	uTXByte(Debug_COM, 0x0d);
	uTXByte(Debug_COM, 0x0d);
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][22] = 0x00;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][23] = 0x00;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][24] = Version;
	
	//RSSI
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][25] = 0x00;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][26] = 0x00;
	
	Setting_Lora_To_Tx_Mode_Func(tmp_LORA);
	ADC_Temp_01 = 0;
	ADC_Temp_02 = 0;
	ADC_Temp_03 = 0;
	ADC_Temp_04 = 0;
	ADC_Temp_05 = 0;
	ADC_Temp_06 = 0;
	ADC_LVD_Temp = 0;
}

void Print_FW_ID(void)
{
	uTXByte(Debug_COM, 'D');
	uTXByte(Debug_COM, 'O');
	uTXByte(Debug_COM, ' ');
	uTXByte(Debug_COM, ':');
	uTXByte(Debug_COM, ' ');
	uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(Version));
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

void Dev_DO_Main_Func(volatile LORA_DEFINE_t *tmp_LORA)
{
	UINT32u_t Target, Source;
	Target.byte[0] = tmp_LORA->MyLoRaID[0];
	Target.byte[1] = tmp_LORA->MyLoRaID[1];
	Target.byte[2] = tmp_LORA->MyLoRaID[2];
	Target.byte[3] = tmp_LORA->MyLoRaID[3];

	Source.byte[0] = WriteBufferHex[0];
	Source.byte[1] = WriteBufferHex[1];
	Source.byte[2] = WriteBufferHex[2];
	Source.byte[3] = WriteBufferHex[3];
	/*
	port_pin_set_output_level(PIN_PA03, false);	//OUT-4
	port_pin_set_output_level(PIN_PA04, false);	//OUT-3
	port_pin_set_output_level(PIN_PA05, false);	//OUT-2
	port_pin_set_output_level(PIN_PA06, false);	//OUT-1
	*/
	
	if(Target.dword == Source.dword 
		&& WriteBufferHex[16] == 0x33
		&& WriteBufferHex[17] == 0x33)	//因為RSSI不可能出現0x3333，所以出現0x3333代表一定是廣播的資料
		
	{
		port_pin_set_output_level(PIN_PA03, WriteBufferHex[4]);
		port_pin_set_output_level(PIN_PA04, WriteBufferHex[5]);
		port_pin_set_output_level(PIN_PA05, WriteBufferHex[6]);
		port_pin_set_output_level(PIN_PA06, WriteBufferHex[7]);
	}
	
}


//=================================================
#endif
//=================================================