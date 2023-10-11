#include <asf.h>
#include <stdio.h>
#include "myDefine.h"
#include "math.h"

//=================================================
#ifdef Dev_Sensor
//=================================================


void Relay_Command_Same_Func(void)
{
	if(Sensor_Relay.Last_NowStatus != WriteBufferHex[12])
	{
		if(LORA01->Lora_Show_Flag == true) uart_str(" >[Diff Last CMD, Set..]\r");
		if(WriteBufferHex[12] == 0xFF)
		{
			if(LORA01->Lora_Show_Flag == true) uart_str(" >[Data = 0xFF, Pass..]\r");
		}
		else
		{
			if(LORA01->Lora_Show_Flag == true) uart_str(" >[Save Relay CMD]\r");
			Sensor_Relay.NowStatus = WriteBufferHex[12];
			Sensor_Relay.Last_NowStatus = Sensor_Relay.NowStatus;
		}
	
	}
	else
	{
		if(LORA01->Lora_Show_Flag == true) uart_str(" >[Same CMD, Pass..]\r");
	}
}


void realtime_CMD(void)
{
	
	
	switch(Sensor_Relay.NowStatus)
	{
		case Relay_ALL_OFF:
			Relay_Command_Same_Func();
		break;
		
		case Relay_FAN:
			Relay_Command_Same_Func();
		break;
		
		case Relay_LOAD:
			Relay_Command_Same_Func();
		break;
		
		case Relay_ALL_ON:
			Relay_Command_Same_Func();
		break;
		
		case Relay_LOAD_Wait_OFF:
			Relay_Command_Same_Func();
		break;
		
		case Relay_Time_Out:
			if(WriteBufferHex[12] != 0xFF)
			{
				Sensor_Relay.NowStatus = WriteBufferHex[12];
				Sensor_Relay.Last_NowStatus = 5;
				Sensor_Relay.min_Count = 0;
			}
		
		break;
		
		case 6:
		break;
		
		case 7:
		break;
	}
	
	
	
}

//=================================================
#endif
//=================================================