#include <asf.h>
#include <stdio.h>
#include "myDefine.h"





//=================================================
#ifdef Dev_Repeater
//=================================================



void Print_FW_ID(void)
{
	uart_str("Repeater: \0");
	uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(Version));
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
	change_and_check(&(LORA01->LoRa_TxRx_Mode_Flag), SetNOP);
	change_and_check(&(LORA01->LoRa_Tx_Send_Time_Out_Count), 0);
	//change_and_check(&(LORA01->LoRa_IRQ_Flag), 0);
	LORA01->LoRa_IRQ_Flag = 0;
	change_and_check(&(LORA01->LoRa_Send_Tx_Flag), LoraTxNoSend);
	LORA01->f_count = 0;
	LORA01->Lora_Show_Flag = false;
	#ifdef HMI_001
		HMI_inital();
	#endif
	
}

void LORA02_inital(void)
{
	LORA02 = &NewLORA02;
	LORA02->DevUART = &MyUart04;
	LORA02->NVM_offset_Value = 20;
	change_and_check(&(LORA02->NowStatus), Read_LoRa_ID_Index);
	change_and_check(&(LORA02->LoRa_TxRx_Mode_Flag), SetRx);
	change_and_check(&(LORA02->LoRa_Tx_Send_Time_Out_Count), 0);
	//change_and_check(&(LORA02->LoRa_IRQ_Flag), 0);
	LORA02->LoRa_IRQ_Flag = 0;
	LORA02->Lora_Show_Flag = false;
	PLC_is_Alive = true;
	Modbus_Status = Modbus_S1;
	//PLC_Show_w_Flag = false;
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
	port_pin_set_config(PIN_PA22, &pin_conf);
	port_pin_set_output_level(PIN_PA22, true);
	
	port_pin_set_config(PIN_PA00, &pin_conf);
	port_pin_set_output_level(PIN_PA00, false);
	port_pin_set_config(PIN_PA01, &pin_conf);
	port_pin_set_output_level(PIN_PA01, false);
	port_pin_set_config(PIN_PA02, &pin_conf);
	port_pin_set_output_level(PIN_PA02, false);
	port_pin_set_config(PIN_PA03, &pin_conf);
	port_pin_set_output_level(PIN_PA03, false);
	port_pin_set_config(PIN_PA04, &pin_conf);
	port_pin_set_output_level(PIN_PA04, false);

	
	//pin_conf.input_pull = PORT_PIN_PULL_DOWN;
	pin_conf.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(PIN_PA17, &pin_conf);
	port_pin_set_output_level(PIN_PA17, false);
	port_pin_set_config(PIN_PA16, &pin_conf);
	port_pin_set_output_level(PIN_PA16, false);
	
	
	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	port_pin_set_config(PIN_PA11, &pin_conf);
	port_pin_set_output_level(PIN_PA11, false);
	port_pin_set_config(PIN_PA07, &pin_conf);
	port_pin_set_output_level(PIN_PA07, false);
	
	port_pin_set_config(PIN_PA09, &pin_conf);
	port_pin_set_output_level(PIN_PA09, true);
	port_pin_set_config(PIN_PA15, &pin_conf);
	port_pin_set_output_level(PIN_PA15, true);
	
//	port_pin_set_config(PIN_PA01, &pin_conf);
//	port_pin_set_output_level(PIN_PA01, false);
	
	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	port_pin_set_config(PIN_PA10, &pin_conf);
	port_pin_set_output_level(PIN_PA10, false);	// ADC因為是用數位IO所以需要設定為輸入

	
	RS485_For_Debug_En
}




void Dev_Value_inital(void)
{
	unsigned int i,j,k;
	volatile PLC_Dev_List_t *PLC_Dev_List_Index = PLC_D_Buf;
	volatile LORA_Dev_List_t *LORA_Dev_List_Index = LORA_Dev_List_In_RAM;
	RS485_CMD_Index = 1;
	RS485_LORA_OUT_Index = 0;
	RS485_WR_Index = 0;
	RS485_RD_Index = 0;
	PLC_SENSOR_SELECT_FLAG = 0;
	Form_CMD_Addr = 0;
	Form_CMD_Val = 0;
	Form_CMD_Addr_And_Val_Is_3rd_Flag = 0;
	PLC_Show_w_Flag = false;
	
	j = 0;
	k = 0;
	for(i = 0; i < PLC_D_Buf_Max; i++)
	{
		if(PLC_D_Buf_Print_Columns_Max == j)
		{
			k++;
			j = 0;
		}
		PLC_Dev_List_Index[i].DataBuf.byte[1] = 0x55;
		PLC_Dev_List_Index[i].DataBuf.byte[0] = 0x55;
		PLC_Dev_List_Index[i].Chage_Flag_Index = k;
		j++;
	}
	Form_CMD_RS485_Flag = 0;
	for(i = 0; i < PLC_D_Buf_Page; i++)
	{
		//PLC_D_Check_Equel_Flag[i] = Dev_Have_New_Data;
		PLC_D_Check_Equel_Flag[i] = Dev_Not_New_Data;
		//PLC_D_Check_Equel_Flag[i] = 3;
	}
}


//=================================================
#endif
//=================================================