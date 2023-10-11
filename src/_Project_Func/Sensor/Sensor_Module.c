#include <asf.h>
#include <stdio.h>
#include "myDefine.h"
#include "math.h"

//=================================================
#ifdef Dev_Sensor
//=================================================

unsigned int ADC_Raw_03_Count;
#define	ADC_Raw_03_Count_Max	60
unsigned int ADC_Raw_04_Count;
#define	ADC_Raw_04_Count_Max	60
unsigned int ADC_Raw_05_Count;
#define	ADC_Raw_05_Count_Max	60



unsigned int bcd_to_hex(unsigned int tmp_data)
{
	UINT16u_t	tmp16;
	UINT8u_t	tmp8;
	unsigned int tmp;
	tmp16.word = tmp_data;
	tmp8.byte = tmp16.byte[1];
	tmp = (tmp8.HL.H * 1000) + (tmp8.HL.L) * 100;
	tmp8.byte = tmp16.byte[0];
	tmp += (tmp8.HL.H * 10) + (tmp8.HL.L);
	return tmp;
}

unsigned int str_to_char(unsigned char *ptr)
{
	unsigned int tmp, tmp2;
	tmp = 0;
	tmp2 =	ascii_to_hex(*(ptr));
	tmp2 = tmp2 << 4;
	tmp = tmp2;
	tmp = tmp | ascii_to_hex(*(ptr+1));
	return tmp;
}

unsigned int str_to_int(unsigned char *ptr)
{
	unsigned int tmp, tmp2;
	tmp2 =	ascii_to_hex(*ptr);
	tmp2 = tmp2 << 12;
	tmp = tmp2;
	tmp2 =	ascii_to_hex(*(ptr+1));
	tmp2 = tmp2 << 8;
	tmp = tmp | tmp2;
	tmp2 =	ascii_to_hex(*(ptr+2));
	tmp2 = tmp2 << 4;
	tmp = tmp | tmp2;
	tmp = tmp | ascii_to_hex(*(ptr+3));
	if(tmp == 0xffff)
	{
		return 0;
	}
	else
	{
		return tmp;	
	}
	
}


void GetOnBoardHT(void)
{
	ADC_Raw_01 = adc_get_val(ADC_USER_01);
	ADC_Raw_02 = adc_get_val(ADC_USER_02);
	if(ADC_Temp_01 == 0)
		ADC_Temp_01 = ADC_Raw_01;
	else
		ADC_Temp_01 = (ADC_Temp_01 + ADC_Raw_01)/2;

	if(ADC_Temp_02 == 0)
		ADC_Temp_02 = ADC_Raw_02;
	else
		ADC_Temp_02 = (ADC_Temp_02 + ADC_Raw_02)/2;
}

void GetSensorModule_WaitUART(void)
{
	uTXByte(ETC_COM, 0x31);
	uTXByte(ETC_COM, 0x0d);
	Sensor_Module_Time_Count = Sensor_Module_TimeOut_Max;
	Sensor_Module_RX_Count = 0;
	//uart_send_word(Debug_COM, Sensor_Module_Time_Count);
	while(Sensor_Module_RX_Count != Sensor_Module_RX_Count_Max)
	{
		usart_read_buffer_job(ETC_COM, (uint8_t *)Sensor_Module_buffer, MAX_RX_BUFFER_LENGTH);
		if(Sensor_Module_Time_Count == 0)
		{
			if(LORA01->Lora_Show_Flag == true)
			{
				uart_str("Sensor Module Time Out\r\0");
				//uart_send_word(Debug_COM, Sensor_Module_Time_Count);
			}
			break;
		}
	}
	
	if(LORA01->Lora_Show_Flag == true)	
	{
		if(Sensor_Module_Time_Count > 0)
		{
			for(unsigned char i = 0; i< Sensor_Module_RX_Count_Max; i++)
			{
				uTXByte(Debug_COM, Sensor_Module_Data[i]);
			}
			uTXByte(Debug_COM, ' ');
		}
	}
}


void GetSensorModuleVal(void)
{
	UINT8u_t tmp8;
	
	#if defined(Dev_Sensor_Module_NO_HT)
		GetOnBoardHT();
	#endif
	GetSensorModule_WaitUART();
	
	unsigned char tmpVal;
	UINT16u_t tmp;
	tmpVal = 0;

	tmp.word = str_to_int(&Sensor_Module_Data[13]);
	tmpVal = tmp.byte[0] + tmp.byte[1];

	tmp.word = str_to_int(&Sensor_Module_Data[17]);
	tmpVal = tmpVal + tmp.byte[0] + tmp.byte[1];

	tmp.word = str_to_int(&Sensor_Module_Data[1]);
	tmpVal = tmpVal + tmp.byte[0] + tmp.byte[1];

	tmp.word = str_to_int(&Sensor_Module_Data[5]);
	tmpVal = tmpVal + tmp.byte[0] + tmp.byte[1];

	tmp.word = str_to_int(&Sensor_Module_Data[9]);
	tmpVal = tmpVal + tmp.byte[0] + tmp.byte[1];

	if(LORA01->Lora_Show_Flag == true)
	{
		uart_send_ascii_byte(Debug_COM, str_to_char(&Sensor_Module_Data[21]));	uTXByte(Debug_COM, '_');
		uart_send_ascii_byte(Debug_COM, tmpVal);	uTXByte(Debug_COM, ',');
	}

	//if(str_to_char(&Sensor_Module_Data[20]) != tmpVal)
	if(str_to_char(&Sensor_Module_Data[21]) != tmpVal)
	{
		if(LORA01->Lora_Show_Flag == true)	uart_str("NG\r\0");
	}
	else
	{
		UINT16u_t tmp16;
		if(LORA01->Lora_Show_Flag == true)	uart_str("OK \0");
		#if defined(Dev_Sensor_Module)
			ADC_Temp_01 = str_to_int(&Sensor_Module_Data[13]);
			ADC_Temp_01 = bcd_to_hex(ADC_Temp_01);
			//uTXByte(Debug_COM, tmp16.half.h0);
			ADC_Temp_02 = str_to_int(&Sensor_Module_Data[17]);
			ADC_Temp_02 = bcd_to_hex(ADC_Temp_02);
			startup_flag = true;
		#endif
	}
	
	#if defined(Dev_Sensor_Module_NO_HT)
		ADC_Temp_01 = ADC_Temp_01;
		ADC_Temp_02 = ADC_Temp_02;
	#endif
	
	
	
	
	unsigned long tmp32;
	#if defined(Dev_Sensor_AI_OnBoard)
		ADC_Raw_03 = adc_get_val(ADC_USER_03);
		ADC_Raw_04 = adc_get_val(ADC_USER_04);
		ADC_Raw_05 = adc_get_val(ADC_USER_05);
		ADC_Raw_06 = adc_get_val(ADC_USER_06);
		tmp32 = HexTomA(ADC_Raw_03) - 400;
		ADC_Raw_03 = (unsigned int*)((tmp32 * CH_Formula_base[0]) / 10000);
		
		tmp32 = HexTomA(ADC_Raw_04) - 400;
		ADC_Raw_04 = (unsigned int*)((tmp32 * CH_Formula_base[1]) / 10000);
		
		tmp32 = HexTomA(ADC_Raw_05) - 400;
		ADC_Raw_05 = (unsigned int*)((tmp32 * CH_Formula_base[2]) / 10000);
		
		tmp32 = HexTomA(ADC_Raw_06) - 400;
		ADC_Raw_06 = (unsigned int*)((tmp32 * CH_Formula_base[3]) / 10000);
		
		
		if(ADC_Raw_03 > ADC_Temp_03) ADC_Temp_03 = ADC_Raw_03;
		if(ADC_Raw_04 > ADC_Temp_04) ADC_Temp_04 = ADC_Raw_04;
		if(ADC_Raw_05 > ADC_Temp_05) ADC_Temp_05 = ADC_Raw_05;
		if(ADC_Raw_06 > ADC_Temp_06) ADC_Temp_06 = ADC_Raw_06;
		
	#else
		ADC_Raw_03 = str_to_int(&Sensor_Module_Data[1]);
		ADC_Raw_04 = str_to_int(&Sensor_Module_Data[5]);
		ADC_Raw_05 = str_to_int(&Sensor_Module_Data[9]);
		ADC_Raw_06 = adc_get_val(ADC_USER_06);
		
		if(ADC_Raw_03 > 100)
		if(ADC_Raw_03_Count < ADC_Raw_03_Count_Max)	ADC_Raw_03_Count++;
		else
		ADC_Raw_03_Count = 0;
		if(ADC_Raw_03_Count == ADC_Raw_03_Count_Max)	ADC_Raw_03 = 100;
		if(ADC_Raw_03 > ADC_Temp_03) ADC_Temp_03 = ADC_Raw_03;
		
		if(ADC_Raw_04 > 9999)
		if(ADC_Raw_04_Count < ADC_Raw_04_Count_Max)	ADC_Raw_04_Count++;
		else
		ADC_Raw_04_Count = 0;
		if(ADC_Raw_04_Count == ADC_Raw_04_Count_Max)	ADC_Raw_04 = 100;
		if(ADC_Raw_04 > ADC_Temp_04) ADC_Temp_04 = ADC_Raw_04;
		
		
		if(ADC_Raw_05 > 100)
		if(ADC_Raw_05_Count < ADC_Raw_05_Count_Max)	ADC_Raw_05_Count++;
		else
		ADC_Raw_05_Count = 0;
		if(ADC_Raw_05_Count == ADC_Raw_05_Count_Max)	ADC_Raw_05 = 100;
		if(ADC_Raw_05 > ADC_Temp_05) ADC_Temp_05 = ADC_Raw_05;
		
		if(ADC_Raw_06 > ADC_Temp_06) ADC_Temp_06 = ADC_Raw_06;
	#endif
	

	
	
	if(LORA01->Lora_Show_Flag == true)	
	{
		uart_send_word(Debug_COM, ADC_Temp_01);	uTXByte(Debug_COM, ',');
		uart_send_word(Debug_COM, ADC_Temp_02);	uTXByte(Debug_COM, ',');
		uart_send_word(Debug_COM, ADC_Temp_03);	uTXByte(Debug_COM, ',');
		uart_send_word(Debug_COM, ADC_Temp_04);	uTXByte(Debug_COM, ',');
		uart_send_word(Debug_COM, ADC_Temp_05);	uTXByte(Debug_COM, ',');
		uart_send_word(Debug_COM, ADC_Temp_06);	uTXByte(Debug_COM, ',');
		uTXByte(Debug_COM, 0x0d);
	}
}



//=================================================
#endif
//=================================================