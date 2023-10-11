#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//=================================================
#ifdef Dev_Master_RS232
//=================================================

void Dev_Value_inital(void)
{
	// nop;
}


void LORA01_Reset_Func(bool tmp_bool)
{
	port_pin_set_output_level(PIN_PA17, tmp_bool);
}

void LORA02_Reset_Func(bool tmp_bool)
{
	//port_pin_set_output_level(PIN_PA16, tmp_bool);
}


void LORA01_inital(void)
{
	LORA01 = &NewLORA01;
	
	mGateway.LoRA01 = &NewLORA01;
	mGateway.LoRA01->DevUART = &MyUart01;
	mGateway.LoRA01->NVM_offset_Value = 0;
	mGateway.LoRA01->List_Now = 0;
	mGateway.LoRA01->List_Next = 0;
	mGateway.LoRA01->NowStatus = Read_LoRa_ID_Index;
	mGateway.LoRA01->LoRa_TxRx_Mode_Flag = SetNOP;
	mGateway.LoRA01->LoRa_Tx_Send_Time_Out_Count = 0;
	mGateway.LoRA01->LoRa_IRQ_Flag = 0;
	mGateway.LoRA01->LoRa_Send_Tx_Flag = LoraTxNoSend;
	mGateway.LoRA01->f_count = 0;
	mGateway.LoRA01->Lora_Show_Flag = false;
	
	mGateway.LORA_Dev_List_Index = &LORA_Dev_List_In_RAM;
	
	//volatile LORA_Dev_List_t *LORA_Dev_List_Index = LORA_Dev_List_In_RAM;
	
	/*
	LORA01 = &NewLORA01;
	LORA01->DevUART = &MyUart01;
	LORA01->NVM_offset_Value = 0;
	LORA01->List_Now = 0;
	LORA01->List_Next = 0;
	LORA01->NowStatus = Read_LoRa_ID_Index;
	LORA01->LoRa_TxRx_Mode_Flag = SetNOP;
	LORA01->LoRa_Tx_Send_Time_Out_Count = 0;
	LORA01->LoRa_IRQ_Flag = 0;
	LORA01->LoRa_Send_Tx_Flag = LoraTxNoSend;
	LORA01->f_count = 0;
	LORA01->Lora_Show_Flag = false;
	*/
}

void LORA02_inital(void)
{
	mGateway.LoRA02 = &NewLORA02;
	LORA02 = &NewLORA02;
	LORA02->DevUART = &MyUart04;
	LORA02->NVM_offset_Value = 20;
	change_and_check(&(LORA02->NowStatus), Read_LoRa_ID_Index);
	change_and_check(&(LORA02->LoRa_TxRx_Mode_Flag), SetRx);
	change_and_check(&(LORA02->LoRa_Tx_Send_Time_Out_Count), 0);
	change_and_check(&(LORA02->LoRa_IRQ_Flag), 0);
	LORA02->Lora_Show_Flag = true;
}



void Print_FW_ID(void)
{
	uart_str("Master RS232: ");
	uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(Version));
}


//=================================================
#endif
//=================================================