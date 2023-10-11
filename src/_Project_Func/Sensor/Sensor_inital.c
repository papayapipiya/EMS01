#include <asf.h>
#include <stdio.h>
#include "myDefine.h"
#include "math.h"

//=================================================
#ifdef Dev_Sensor
//=================================================

#define offset_value 125
unsigned long tmp_temp;
unsigned long tmp_humi;

void Dev_Value_inital(void)
{
	#if defined(IIC_Dev)
		SHT30_RESET_ON
		delay_ms(10);
		SHT30_RESET_OFF
		delay_ms(10);
		configure_i2c_master();
		IIC_EEPROM_Inital();
	#endif
	mem_inital();
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
	CH1 = &NewCH1;
	CH2 = &NewCH2;
	ADC_Raw_01 = 0;
	ADC_Raw_02 = 0;
	ADC_Raw_03 = 0;
	ADC_Raw_04 = 0;
	ADC_Raw_05 = 0;
	ADC_Raw_06 = 0;
	ADC_Temp_01 = 0;
	ADC_Temp_02 = 0;
	ADC_Temp_03 = 0;
	ADC_Temp_04 = 0;
	ADC_Temp_05 = 0;
	ADC_Temp_06 = 0;
	
	LORA01 = &NewLORA01;
	LORA01->DevUART = &MyUart01;
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
	LORA01->Show_IIC = false;
	LORA01->Show_Sensor_Raw = false;
	LORA01->Power_On_Delay = 1000;
	
	startup_flag = false;
	//Sensor_Relay.NowStatus = Relay_LOAD;
	Sensor_Relay.NowStatus = Relay_FAN;
	Sensor_Relay.Last_NowStatus = Sensor_Relay.NowStatus;
	
	
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
	

	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	//	RGB LED PIN DEFINE
	//	PIN_PA23 = LED GREEN
	port_pin_set_config(PIN_PA23, &pin_conf);	
	port_pin_set_output_level(PIN_PA23, true);
	//	PIN_PA27 = LED RED
	port_pin_set_config(PIN_PA27, &pin_conf);
	port_pin_set_output_level(PIN_PA27, true);
	//	PIN_PA28 = LED BLUE
	port_pin_set_config(PIN_PA28, &pin_conf);
	port_pin_set_output_level(PIN_PA28, true);
	//	PIN_PA16 = SHT30 RESET
	port_pin_set_config(PIN_PA16, &pin_conf);
	port_pin_set_output_level(PIN_PA16, true);
	
	
	//	PIN_PA05 = RELAY-1
	RELAY_1_OFF
	port_pin_set_config(PIN_PA05, &pin_conf);
	RELAY_1_OFF
	//	PIN_PA06 = RELAY-2
	RELAY_2_OFF
	port_pin_set_config(PIN_PA06, &pin_conf);
	RELAY_2_OFF
	//	PIN_PA07 = RELAY-3
	RELAY_3_OFF
	port_pin_set_config(PIN_PA07, &pin_conf);
	RELAY_3_OFF
	
	//	PIN_PA00 = PWM
	PWM_ON
	port_pin_set_config(PIN_PA00, &pin_conf);
	PWM_ON


	
	pin_conf.direction  = PORT_PIN_DIR_INPUT;

	//	PIN_PA11 = LoRA IRQ
	port_pin_set_config(PIN_PA11, &pin_conf);
	port_pin_set_output_level(PIN_PA11, false);

	//	PIN_PA02 = ADC (TGS2600) 
	pin_conf.direction = PORT_PIN_PULL_NONE;
	port_pin_set_config(PIN_PA02, &pin_conf);
	port_pin_set_output_level(PIN_PA02, false);
	
	port_pin_set_config(PIN_PA03, &pin_conf);
	port_pin_set_output_level(PIN_PA03, false);
	
}



void Print_FW_ID(void)
{
	uart_str("Sensor : \0");
	uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(Version));
}



//=================================================
#endif
//=================================================