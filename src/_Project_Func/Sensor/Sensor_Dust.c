#include <asf.h>
#include <stdio.h>
#include "myDefine.h"
#include "math.h"


//=================================================
#ifdef Dev_Sensor
//=================================================

void Dust_function(void)
{
	UINT16u_t tmp16;
	
	Dust_Time_Count = 0;
	while(Dust_RX_Count != Dust_RX_Count_Max)
	{
		usart_read_buffer_job(&MyUart04, (uint8_t *)Dust_buffer, MAX_RX_BUFFER_LENGTH);
		//uTXByte(Debug_COM, Dust_buffer[0]);
		if(Dust_Time_Count == Dust_TimeOut_Max)
		{
			if(LORA01->Lora_Show_Flag == true)	uart_str("Dust Time Out\r\0");
			return;
		}
	}

	tmp16.byte[1] = Dust_Data[1];
	tmp16.byte[0] = Dust_Data[2];
	ADC_PM25_Raw = tmp16.word;

	Dust_RX_Count = 0;
	if(ADC_PM25_Raw > ADC_PM25_Temp) ADC_PM25_Temp = ADC_PM25_Raw;
}


//=================================================
#endif
//=================================================