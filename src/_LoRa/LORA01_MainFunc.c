/*
 * LORA01_MAIN_FUNCTION.c
 *
 * Created: 2021/4/19 下午 04:13:01
 *  Author: willchen
 */ 
#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



void LORA01_MAIN_FUNCTION(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	
	if(tmp_LORA->NowStatus != Lora_Rx_Data_Ready_Index) return;
	
	UINT32u_t Target, Source;
	//取得自己MAC ID
	Target.byte[0] = tmp_LORA->MyLoRaID[0];
	Target.byte[1] = tmp_LORA->MyLoRaID[1];
	Target.byte[2] = tmp_LORA->MyLoRaID[2];
	Target.byte[3] = tmp_LORA->MyLoRaID[3];
	
	//取得LoRA訊號帶來的上的MAC ID
	Source.byte[0] = tmp_LORA->MyBuffer[3];
	Source.byte[1] = tmp_LORA->MyBuffer[4];
	Source.byte[2] = tmp_LORA->MyBuffer[5];
	Source.byte[3] = tmp_LORA->MyBuffer[6];
	
	//擷取LoRA訊號帶來的資料，可帶18個BYTE，扣掉MAC 4 BYTE、尾巴 2 BYTE，剩下12個BYTE
	for(unsigned char i = 0; i < 18; i++)
		WriteBufferHex[i] = tmp_LORA->MyBuffer[9+i];

	if(Target.dword == Source.dword)
	{
		Response_LORA(tmp_LORA, tmp_LORA_Dev_List_Index);
	}
	else
	{
		if(LORA01->Lora_Show_Flag)
		{
			uTXByte(Debug_COM, 0x0d);
			uTXByte(Debug_COM, 'N');
			uTXByte(Debug_COM, 'o');
			uTXByte(Debug_COM, ' ');
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaID[0]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaID[1]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaID[2]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaID[3]);
			uTXByte(Debug_COM, ' ');
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyBuffer[3]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyBuffer[4]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyBuffer[5]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyBuffer[6]);
			uTXByte(Debug_COM, 0x09);
			Print_Write_Buffer();		
		}
		#ifdef  Dev_Sensor
			Judg_Is_My_CMD(tmp_LORA);
		#endif // _DEBUG
		
		if(LORA01->Lora_Show_Flag)	uTXByte(Debug_COM, 0x0d);
		/*
		if(sleep_count == sleep_count_max)	
		{
			//若超過五秒都沒有收到Yes，進入Sleep
			tmp_LORA->LoRa_TxRx_Mode_Flag = SetSleep;
		}
		else
		{
		*/
			//若五秒內都沒有收到Yes，進入Rx
			tmp_LORA->LoRa_TxRx_Mode_Flag = SetRx;
		/*
		}
		*/
		tmp_LORA->NowStatus = Write_LoRa_Setting_Index;
	}
	
}




unsigned char LoRA_Index;
void Run_LoRA_Function(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index, volatile Write_PLC_List_t *tmp_Write_PLC_List_Index)
{
	#if defined(Dev_Repeater)
	
		Lora_CMD_Func(LORA01);
		Response_LORA01(LORA01);

		Lora_CMD_Func(LORA02);
		Response_LORA02(LORA02, tmp_LORA_Dev_List_Index);
	
	#elif defined(Dev_Master_RS232)
		Lora_CMD_Func(LORA01);
		Response_LORA01(LORA01);
	#else
		Lora_CMD_Func(LORA01);
		LORA01_MAIN_FUNCTION(LORA01, tmp_LORA_Dev_List_Index);
	#endif
}


void Running_Function(void)
{
	volatile Write_PLC_List_t *Write_PLC_List_Index = Write_PLC_List_In_RAM;
	volatile LORA_Dev_List_t *LORA_Dev_List_Index = LORA_Dev_List_In_RAM;
	Run_LoRA_Function(LORA_Dev_List_Index, Write_PLC_List_Index);
	Run_LoRA_Function(LORA_Dev_List_Index, Write_PLC_List_Index);
	Run_LoRA_Function(LORA_Dev_List_Index, Write_PLC_List_Index);
	Run_LoRA_Function(LORA_Dev_List_Index, Write_PLC_List_Index);
	Run_LoRA_Function(LORA_Dev_List_Index, Write_PLC_List_Index);
	#if	defined(Dev_Repeater_RAM)
	Polling_RS485();
	#elif defined(Dev_Repeater_DBuf)
	Polling_RS485();
	#endif
}

void LoRA_Timeout_Func(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->f_count == Timer_Timeout)
	{
		wdt_reset_count();
		Print_LoRA_Time_Out_Msg(tmp_LORA);
		uTXByte(Debug_COM, '(');
		uart_send_ascii_byte(Debug_COM, tmp_LORA->NowStatus);
		uTXByte(Debug_COM, ')');
		uTXByte(Debug_COM, ' ');
		uart_send_word(Debug_COM, tmp_LORA->f_count);
		Reset_LoRa_Func(tmp_LORA);
		uTXByte(Debug_COM, ' ');
	}
}