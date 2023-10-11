#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


//=================================================
#ifdef Dev_Feed_Weigher
//=================================================



void Dev_Value_inital(void)
{
	delay_ms(50);
	Last_Weight_Temp = 35;
	SYSTEM_RESET_FLAG = 1;
	Load_Weight_Count_in_NVM();
	Load_Total_Weight_Form_NVM();
	ON_Display();
	WeightStatus = Wegight_Catch_Index;
	FW_Status = PowerUP;
	//Total_Weight = 0xfffffffe;
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
	LORA01->Lora_Show_Flag = false;
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
	
	//LoRA LDO Power Switch
	port_pin_set_config(PIN_PA07, &pin_conf);
	port_pin_set_output_level(PIN_PA07, true);
	
	//HX710 CLK 
	port_pin_set_config(PIN_PA16, &pin_conf);
	port_pin_set_output_level(PIN_PA16, true);
	
	/* feed weigher */
	port_pin_set_config(PIN_PA24, &pin_conf);
	port_pin_set_config(PIN_PA25, &pin_conf);
	port_pin_set_config(PIN_PA00, &pin_conf);
	port_pin_set_config(PIN_PA01, &pin_conf);
	//RELAY-1
	port_pin_set_output_level(PIN_PA25, false);
	//RELAY-2
	port_pin_set_output_level(PIN_PA24, false);
	//DO1
	port_pin_set_output_level(PIN_PA00, false);
	//DO2
	port_pin_set_output_level(PIN_PA01, false);
	
	
	//	RGB LED PIN DEFINE
	port_pin_set_config(PIN_PA23, &pin_conf);
	port_pin_set_output_level(PIN_PA23, true);
	port_pin_set_config(PIN_PA27, &pin_conf);
	port_pin_set_output_level(PIN_PA27, true);
	

	
	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	port_pin_set_config(PIN_PA11, &pin_conf);
	port_pin_set_output_level(PIN_PA11, false);
	port_pin_set_config(PIN_PA17, &pin_conf);
	port_pin_set_output_level(PIN_PA17, false);
	port_pin_set_config(PIN_PA28, &pin_conf);
	port_pin_set_output_level(PIN_PA28, false);

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
	uart_str("Feed Weigher: \0");
	uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(Version));
	
}


//=================================================
#endif
//=================================================