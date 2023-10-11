#include <asf.h>
#include <stdio.h>
#include "myDefine.h"
#include "math.h"

//=================================================
#ifdef Dev_Sensor
//=================================================

void sensor_main_func(void)
{
	//Dust_function();
	#if defined(Dev_Sensor_Module)
		GetSensorModuleVal();
	#elif defined(Dev_Sensor_HT_OnBoard)
		GetSensorOnBoardVal();
	#endif
	
	#if defined(IIC_Dev)
		IIC_Main_Func();		//Sensor SHT30主程式
	#endif
	
	mem_main();
	switch(Sensor_Relay.NowStatus)
	{
		case Relay_ALL_OFF:
			RELAY_2_OFF
			RELAY_1_OFF
			PWM_OFF
			break;
		
		case Relay_FAN:
			RELAY_2_OFF
			RELAY_1_ON
			break;
		
		case Relay_LOAD:
			RELAY_2_ON
			RELAY_1_OFF
			PWM_OFF
			Sensor_Relay.to_OFF_Count = 1200;
			Sensor_Relay.min_Count = 0;
			break;
		
		case Relay_ALL_ON:
			RELAY_2_ON
			RELAY_1_ON
			PWM_ON
			Sensor_Relay.NowStatus = Relay_LOAD_Wait_OFF;
			Sensor_Relay.to_OFF_Count = 1200;
			Sensor_Relay.min_Count = 0;
			break;
		
		case Relay_LOAD_Wait_OFF:
		
			if(Sensor_Relay.min_Count >= mem_OFF_Count_Max.DataBuf.word) //萬一突然被使用者修改時間，才能提早反應。
			{
				Sensor_Relay.NowStatus = Relay_Time_Out;
				Sensor_Relay.to_OFF_Count = 0;
			}
		
			if(Sensor_Relay.to_OFF_Count == 0)
			{
				Sensor_Relay.min_Count++;
				if(Sensor_Relay.min_Count < mem_OFF_Count_Max.DataBuf.word)
				{
					Sensor_Relay.to_OFF_Count = 1200;
				}
				else
				{
					Sensor_Relay.NowStatus = Relay_Time_Out;
				}
			}
			break;
		
		case Relay_Time_Out:
			RELAY_2_OFF
			RELAY_1_ON
			break;
		
		case 6:
			break;
		
		case 7:
			break;
	}
	
}


void Display_Sensor_Status(void)
{
	if(LORA01->Lora_Show_Flag == true)
	{
		uart_str("-------\rRelay: ");
		uart_str("Now: ");
		uart_send_ascii_byte(Debug_COM, Sensor_Relay.NowStatus);
		uTXByte(Debug_COM, ' ');
		uart_str("CMD log: (L)");
		uart_send_ascii_byte(Debug_COM, Sensor_Relay.Last_NowStatus);
		uart_str(" (C)");
		uart_send_ascii_byte(Debug_COM, WriteBufferHex[12]);
		uart_str("\rOFF Counter: ");
		uart_send_word(Debug_COM, hex_to_bcd(mem_OFF_Count_Max.DataBuf.word));
		uTXByte(Debug_COM, ' ');
		uTXByte(Debug_COM, '>');
		uTXByte(Debug_COM, ' ');
		uart_send_word(Debug_COM, hex_to_bcd(Sensor_Relay.min_Count));
		uTXByte(Debug_COM, ':');
		uart_send_word(Debug_COM, hex_to_bcd(Sensor_Relay.to_OFF_Count));
		
	}
}


void Judg_Is_My_CMD(volatile LORA_DEFINE_t *tmp_LORA)
{
	UINT32u_t Target, Source;
	
	//取得自己MAC ID
	Target.byte[0] = tmp_LORA->MyLoRaID[0];
	Target.byte[1] = tmp_LORA->MyLoRaID[1];
	Target.byte[2] = tmp_LORA->MyLoRaID[2];
	Target.byte[3] = tmp_LORA->MyLoRaID[3];
	
	//取得LoRA訊號帶來的上的MAC ID
	Source.byte[0] = WriteBufferHex[0];
	Source.byte[1] = WriteBufferHex[1];
	Source.byte[2] = WriteBufferHex[2];
	Source.byte[3] = WriteBufferHex[3];

	
	if(Target.dword != Source.dword)
	{
		if(LORA01->Lora_Show_Flag == true)
		{
			uart_str("\rCMD is Not Mine..\r");
			Display_Sensor_Status();
		}
		return;
	}
	
	if(WriteBufferHex[16] != 0x33 || WriteBufferHex[17] != 0x33)
	{
		if(LORA01->Lora_Show_Flag == true)
		{
			uart_str("\rData is Not CMD..\r");
			Display_Sensor_Status();
		}
		return;
	}
	

	if(LORA01->Lora_Show_Flag == true) uart_str("\r[CMD is Mine]\r");

	
	switch (WriteBufferHex[4])
	{
		
		case 2:
			if(LORA01->Lora_Show_Flag == true) uart_str(" >[Mem Set CMD]\r");
			mem_Set_CMD();
			if(LORA01->Lora_Show_Flag == true) uart_str(" >[Realtime CMD]\r");
			realtime_CMD();
		break;
	}
	Display_Sensor_Status();


}

//=================================================
#endif
//=================================================