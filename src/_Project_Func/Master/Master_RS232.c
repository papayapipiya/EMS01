﻿#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//=================================================
#ifdef Dev_Master_RS232
//=================================================

void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	
}

void Wait_RX_IRQ_Index_Func(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA == LORA01)	// Dev To Repeater
	{
		if(tmp_LORA->f_count > Lora_Wait_RX_Delay_1500ms)
		{
			Print_Who_Lora(tmp_LORA);
			Print_LoRA_Time_Out_Msg(tmp_LORA);
			Setting_Lora_To_Tx_Mode_Func(tmp_LORA);
			tmp_LORA->LoRa_Tx_Send_Time_Out_Count++;
		}
		else
		if(tmp_LORA->LoRa_IRQ_Flag == 1)
		{
			LED_R_ON
			wdt_reset_count();
			tmp_LORA->NowStatus = Read_LoRa_RxBuffer_Index;
			//change_and_check(&(tmp_LORA->NowStatus), Read_LoRa_RxBuffer_Index);
		}
	}
	else if(tmp_LORA == LORA02)	// Master To Repeater
	{
		if(tmp_LORA->LoRa_IRQ_Flag == 1)
		{
			LED_B_ON
			wdt_reset_count();
			tmp_LORA->NowStatus = Read_LoRa_RxBuffer_Index;
			//change_and_check(&(tmp_LORA->NowStatus), Read_LoRa_RxBuffer_Index);
		}
	}
}



//=================================================
#endif
//=================================================