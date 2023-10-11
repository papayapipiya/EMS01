#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


//=================================================
#ifdef Dev_Weight
//=================================================

void Dev_Value_inital(void)
{
	delay_ms(50);
	Last_Weight_Temp = 35;
	SYSTEM_RESET_FLAG = 1;
	Load_Weight_Count_in_NVM();
	//RawDataBase = 200000000 / (RawDataUser - RawDataZero);	
	ON_Display();
	WeightStatus = Wegight_Catch_Index;
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
	
	port_pin_set_config(PIN_PA07, &pin_conf);
	port_pin_set_output_level(PIN_PA07, true);
	port_pin_set_config(PIN_PA22, &pin_conf);
	port_pin_set_output_level(PIN_PA22, true);
	port_pin_set_config(PIN_PA16, &pin_conf);
	port_pin_set_output_level(PIN_PA16, true);
	
	
	//	RGB LED PIN DEFINE
	port_pin_set_config(PIN_PA23, &pin_conf);
	port_pin_set_output_level(PIN_PA23, true);
	port_pin_set_config(PIN_PA27, &pin_conf);
	port_pin_set_output_level(PIN_PA27, true);
	port_pin_set_config(PIN_PA28, &pin_conf);
	port_pin_set_output_level(PIN_PA28, true);

	
	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	port_pin_set_config(PIN_PA11, &pin_conf);
	port_pin_set_output_level(PIN_PA11, false);
	port_pin_set_config(PIN_PA17, &pin_conf);
	port_pin_set_output_level(PIN_PA17, false);

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
	uTXByte(Debug_COM, 'W');
	uTXByte(Debug_COM, 'e');
	uTXByte(Debug_COM, 'i');
	uTXByte(Debug_COM, 'g');
	uTXByte(Debug_COM, 'h');
	uTXByte(Debug_COM, 't');
	uTXByte(Debug_COM, ':');
	uTXByte(Debug_COM, ' ');
	uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(Version));
	
}

void Weight_main_Func(void)
{
	switch(WeightStatus)
	{
		case Wegight_Catch_Index:
			change_and_check(&WeightStatus, Wegight_Cal_Index_01);
			Rawdata = ReadCount();
			Weight_Correction(Rawdata);	
			break;
		case Wegight_Catch_Index_01:
			change_and_check(&WeightStatus, Wegight_Catch_Index_02);
			Rawdata = (Rawdata + ReadCount())/2;
			Weight_Correction(Rawdata);
			break;
		
		case Wegight_Catch_Index_02:
			change_and_check(&WeightStatus, Wegight_Cal_Index_01);
			Rawdata = (Rawdata + ReadCount())/2;
			Weight_Correction(Rawdata);
			break;
		
		case Wegight_Cal_Index_01:
			change_and_check(&WeightStatus, Wegight_Cal_Index_02);
			if(Rawdata > RawDataZero)
			{
				RawDataTemp = ((Rawdata - RawDataZero) * RawDataBase);
			}
			else
			{
				RawDataTemp = 0;
			}
			break;
		
		case Wegight_Cal_Index_02:
			change_and_check(&WeightStatus, Wegight_Judg_Index);
			RawDataTemp = RawDataTemp / 1000;
			RawDataTemp = RawDataTemp / 100;
			break;
		
		case Wegight_Judg_Index:
			change_and_check(&WeightStatus, Wegight_Display_Index_01);
			SET_Display_Value(long_to_bcd_4byte(RawDataTemp));
			Weight_Judg();
			break;
		
		case Wegight_Display_Index_01:
			change_and_check(&WeightStatus, Wegight_Display_Index_02);
			//Weight_Display_Buffer = long_to_bcd_4byte(Weight_UpLoad_Buffer);
			break;
		
		case Wegight_Display_Index_02:
			change_and_check(&WeightStatus, Wegight_Catch_Index);
			ON_Display();
			//SET_Display_Value(Weight_Display_Buffer);
			break;
	}
}



//void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA)
void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	UINT32u_t Weight_u32;
	UINT16u_t tmp_adc;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3] = tmp_LORA->MyLoRaID[0];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4] = tmp_LORA->MyLoRaID[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5] = tmp_LORA->MyLoRaID[2];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6] = tmp_LORA->MyLoRaID[3];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][7] = 0x01;
	Weight_u32.dword = Weight_UpLoad_Buffer;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][8] = Weight_u32.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][9] = Weight_u32.byte[0];
	
	tmp_adc.word = Weight_Change[0];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][10] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][11] = tmp_adc.byte[0];
	
	tmp_adc.word = Weight_Change[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][12] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][13] = tmp_adc.byte[0];
	
	tmp_adc.word = Weight_Change[2];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][14] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][15] = tmp_adc.byte[0];
	
	tmp_adc.word = Weight_Change[3];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][16] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][17] = tmp_adc.byte[0];
	
	tmp_adc.word = Weight_Change[4];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][18] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][19] = tmp_adc.byte[0];
	
	tmp_adc.word = ADC_LVD_Temp;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][20] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][21] = tmp_adc.byte[0];
	
	tmp_adc.word = Weight_Change[5];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][22] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][23] = tmp_adc.byte[0];
	
	//tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][24] = 0;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][24] = Version;
	
	//tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7] = 0x02;
	tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7] = tmp_LORA->MyBuffer[7];
	tmp_LORA->MyLoRaPower = tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7];
	
	
	
	Setting_Lora_To_Tx_Mode_Func(tmp_LORA);
	if(LORA01->Lora_Show_Flag == true)
	{
		uTXByte(Debug_COM, 0x0d);
		uTXByte(Debug_COM, 'Y');
		uTXByte(Debug_COM, 'e');
		uTXByte(Debug_COM, 's');
		uTXByte(Debug_COM, 0x09);
		Print_Write_Buffer();
		uTXByte(Debug_COM, 0x0d);

		uart_send_word(Debug_COM, Weight_UpLoad_Buffer);
		uTXByte(Debug_COM, ' ');
		uart_send_word(Debug_COM, hex_to_bcd(Weight_Change[0]));
		uTXByte(Debug_COM, ' ');
		uart_send_word(Debug_COM, hex_to_bcd(Weight_Change[1]));
		uTXByte(Debug_COM, ' ');
		uart_send_word(Debug_COM, hex_to_bcd(Weight_Change[2]));
		uTXByte(Debug_COM, ' ');
		uart_send_word(Debug_COM, hex_to_bcd(Weight_Change[3]));
		uTXByte(Debug_COM, ' ');
		uart_send_word(Debug_COM, hex_to_bcd(Weight_Change[4]));
		uTXByte(Debug_COM, ' ');
		uart_send_word(Debug_COM, hex_to_bcd(Weight_Change[5]));
		uTXByte(Debug_COM, ' ');
		uTXByte(Debug_COM, '(');
		uart_send_word(Debug_COM, ADC_LVD_Temp);
		uTXByte(Debug_COM, ')');
		uTXByte(Debug_COM, 0x0d);
		uTXByte(Debug_COM, 0x0d);
	}
	ADC_LVD_Temp = 0;
	//Weight_UpLoad_Buffer = 0;
	Weight_Change[0] = 0;
	Weight_Change[1] = 0;
	Weight_Change[2] = 0;
	Weight_Change[3] = 0;
	Weight_Change[4] = 0;
	Weight_Change[5] = 0;
	/*
	LORAPollingCount++;
	if(LORAPollingCount == 20)
	{
		Last_Weight_Temp = 35;
		//Weight_Change_Value_Count = 0;
		LORAPollingCount = 0;
	}
	*/
}


void Flash_Display(void)
{
	uTXByte(Display_COM, 'F');
	uTXByte(Display_COM, '2');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, 'E');
}

void OFF_Display(void)
{
	uTXByte(Display_COM, 'F');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, 'E');
}

void ON_Display(void)
{
	uTXByte(Display_COM, 'F');
	uTXByte(Display_COM, '1');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, 'E');
}

unsigned long long_to_bcd_4byte(unsigned long tmp_data){
	UINT32u_t	tmp32;
	UINT16u_t	tmp16;
	UINT8u_t	tmp8;
	uint32_t	value;

	value = tmp_data;
	
	tmp8.HL.H = value / 10000000;
	value = value % 10000000;
	tmp8.HL.L = value / 1000000;
	value = value % 1000000;
	tmp32.byte[3] = tmp8.byte;

	tmp8.HL.H = value / 100000;
	value = value % 100000;
	tmp8.HL.L = value / 10000;
	value = value % 10000;
	tmp32.byte[2] = tmp8.byte;
	
	tmp8.HL.H = value / 1000;
	value = value % 1000;
	tmp8.HL.L = value / 100;
	value = value % 100;
	tmp32.byte[1] = tmp8.byte;

	tmp8.HL.H = value / 10;
	value = value % 10;
	tmp8.HL.L = value;
	tmp32.byte[0] = tmp8.byte;

	return tmp32.dword;
}

unsigned long ReadCount(void)
{
	unsigned long Count;
	unsigned char i;
	port_pin_set_output_level(PIN_PA16, false);
	Count = 0;
	
	while(port_pin_get_input_level(PIN_PA17) == true)
	{
		//wdt_reset_count();
	};
	
	for (i = 0; i < 24; i++)
	{
		port_pin_set_output_level(PIN_PA16, true);
		Count = Count << 1;
		port_pin_set_output_level(PIN_PA16, false);
		if(port_pin_get_input_level(PIN_PA17) == true) Count++;
	}
	//wdt_reset_count();
	port_pin_set_output_level(PIN_PA16, true);
	Count = Count ^ 0x800000;
	port_pin_set_output_level(PIN_PA16, false);

	return Count;
}

void tx_byte(unsigned char tmp_data)
{
	UINT8u_t tmp8;
	tmp8.byte = tmp_data;
	uTXByte(Debug_COM, hex_to_ascii(tmp8.HL.H));
	uTXByte(Debug_COM, hex_to_ascii(tmp8.HL.L));
}

void tx_dword(unsigned long tmp_data)
{
	UINT32u_t tmp32;
	tmp32.dword = tmp_data;
	tx_byte(tmp32.byte[3]);
	tx_byte(tmp32.byte[2]);
	tx_byte(tmp32.byte[1]);
	tx_byte(tmp32.byte[0]);
}

void SET_Display_Value(unsigned long tmp_value)
{
	UINT32u_t	tmp32;
	UINT8u_t	tmp8;

	tmp32.dword = tmp_value;

	uTXByte(Display_COM, 'S');
	tmp8.byte = tmp32.byte[2];
	uTXByte(Display_COM, tmp8.HL.H+0x30);
	uTXByte(Display_COM, tmp8.HL.L+0x30);
	tmp8.byte = tmp32.byte[1];
	uTXByte(Display_COM, tmp8.HL.H+0x30);
	uTXByte(Display_COM, tmp8.HL.L+0x30);
	tmp8.byte = tmp32.byte[0];
	uTXByte(Display_COM, tmp8.HL.H+0x30);
	uTXByte(Display_COM, tmp8.HL.L+0x30);
	uTXByte(Display_COM, 'E');
}


void Weight_Correction(unsigned long tmp_data)
{
	

	if(port_pin_get_input_level(PIN_PA02) == false)
	{
		OFF_Display();
		RawDataZero = tmp_data;
		Save_Weight_Count_in_NVM();
		Load_Weight_Count_in_NVM();
		uTXByte(Debug_COM, 'S');
		uTXByte(Debug_COM, 'a');
		uTXByte(Debug_COM, 'v');
		uTXByte(Debug_COM, 'e');
		uTXByte(Debug_COM, '0');
		uTXByte(Debug_COM, 0x0d);
		ON_Display();
	}
	else
	if(port_pin_get_input_level(PIN_PA03) == false)
	{
		OFF_Display();
		RawDataUser = tmp_data;
		Save_Weight_Count_in_NVM();
		Load_Weight_Count_in_NVM();
		uTXByte(Debug_COM, 'S');
		uTXByte(Debug_COM, 'a');
		uTXByte(Debug_COM, 'v');
		uTXByte(Debug_COM, 'e');
		uTXByte(Debug_COM, '2');
		uTXByte(Debug_COM, '0');
		uTXByte(Debug_COM, '0');
		uTXByte(Debug_COM, 0x0d);
		ON_Display();
	}

}

void Print_Weight_Change_Value_Buf_Func(void)
{
	unsigned char i;
	for(i = 0; i < 6; i++)
	{
		if(Weight_Change_Value_Count == i)
		{
			uTXByte(Debug_COM, '_');uTXByte(Debug_COM, '_');uTXByte(Debug_COM, '_');uTXByte(Debug_COM, '_');
		}
		else
		{
			uart_send_word(Debug_COM, hex_to_bcd(Weight_Change[i]));
		}	
		uTXByte(Debug_COM, ' ');
	}
	
}

unsigned char dynamic_weight_level;
unsigned char dynamic_count;
#define dynamic_value_max 21
#define dynamic_std_number 20
unsigned long dynamic_value[dynamic_value_max];
unsigned long dynamic_avg;
unsigned long dynamic_sum_temp;
unsigned long dynamic_sum_temp_02;
unsigned long dynamic_std;
unsigned long dynamic_std_max;
unsigned long dynamic_std_min;

void Weight_Judg(void)
{
	unsigned char i;
	unsigned long TmpValue;
	
	if(LORA01->Lora_Show_Flag == true)
	{
		//wdt_reset_count();
		
		uTXByte(Debug_COM, '[');
		tx_dword(Rawdata);
		uTXByte(Debug_COM, ' ');
		//tx_dword(RawDataZero);
		//uTXByte(Debug_COM, ' ');
		//tx_dword(RawDataUser);
		//uTXByte(Debug_COM, ' ');
		uart_send_word(Debug_COM, RawDataBase);
		//uTXByte(Debug_COM, ' ');
		//uart_send_word(Debug_COM, SEG7_TimeOut_Count);
		uTXByte(Debug_COM, ']');
		uTXByte(Debug_COM, ' ');
		//wdt_reset_count();
		/*
		uTXByte(Debug_COM, '{');
		uart_send_word(Debug_COM, RawDataBuffer);
		uTXByte(Debug_COM, '}');
		uTXByte(Debug_COM, ' ');
		*/
		/*
		for(i=0; i<dynamic_value_max; i++)
		{
			uart_send_word(Debug_COM, hex_to_bcd(dynamic_value[i]));
			uTXByte(Debug_COM, ' ');	
		}
		*/
		

		uart_send_word(Debug_COM, Weight_UpLoad_Buffer);
		uTXByte(Debug_COM, ' ');
		Print_Weight_Change_Value_Buf_Func();
		
		uTXByte(Debug_COM, ' ');
		uTXByte(Debug_COM, '[');
		uart_send_ascii_byte(Debug_COM, RawDataCount);
		uTXByte(Debug_COM, ']');
		uTXByte(Debug_COM, ' ');
		
		
		uTXByte(Debug_COM, '(');
		uart_send_word(Debug_COM, hex_to_bcd(dynamic_std));
		uTXByte(Debug_COM, ')');
		uTXByte(Debug_COM, ' ');
		
		uart_send_word(Debug_COM, hex_to_bcd(dynamic_sum_temp_02));
		uTXByte(Debug_COM, ' ');
		/*
		uart_send_word(Debug_COM, hex_to_bcd(dynamic_std_max));
		uTXByte(Debug_COM, ' ');
		
		uart_send_word(Debug_COM, hex_to_bcd(dynamic_std_min));
		uTXByte(Debug_COM, ' ');
		*/
		
		/*
		uTXByte(Debug_COM, '<');
		uart_send_word(Debug_COM, hex_to_bcd(Last_Weight_Temp));
		uTXByte(Debug_COM, '>');
		*/
		/*
		uTXByte(Debug_COM, ' ');
		uart_send_ascii_byte(Debug_COM, LORAPollingCount);
		*/
		/*
		uTXByte(Debug_COM, ' ');
		uart_send_ascii_byte(Debug_COM, dynamic_weight_level);
		*/
		
		uTXByte(Debug_COM, 0x0d);
		
		
		wdt_reset_count();
	}
	

	//	將重量差異轉成正值
	if(RawDataTemp >= RawDataBuffer)
	{
		TmpValue = RawDataTemp - RawDataBuffer;
	}
	else
	{
		TmpValue = RawDataBuffer - RawDataTemp;
	}

	//	儲存取樣樣本，輪迴寫入
	if(dynamic_count < dynamic_value_max)
	{
		dynamic_value[dynamic_count] = TmpValue;
		dynamic_count++;
	}
	else
	{
		dynamic_count = 0;
	}

	//	計算平均數
	dynamic_avg = dynamic_value[0];
	for(i=1; i<dynamic_value_max; i++)
	{
		dynamic_avg = (dynamic_value[i] + dynamic_avg) / 2;
	}

	//	計算標準差
	for(i = 0; i < dynamic_value_max; i++)
	{
		if(dynamic_value[i] > dynamic_avg)
		{
			dynamic_sum_temp += (dynamic_value[i] - dynamic_avg)^2;
		}
		else
		{
			dynamic_sum_temp += (dynamic_avg - dynamic_value[i])^2;
		}
	}
	dynamic_sum_temp = dynamic_sum_temp / dynamic_std_number;
	dynamic_std = sqrt(dynamic_sum_temp);

	//	判斷標準差低於5
	if(dynamic_std < 5)
	{
		//	維持3次標準差都在5
		if(RawDataCount > 2)	
		{
			//	取一個標準差的範圍做平均
			dynamic_sum_temp_02 = 0;
			dynamic_std_max = dynamic_avg + (dynamic_std * 1);
			dynamic_std_min = dynamic_avg - (dynamic_std * 1);
			for(i = 0; i < dynamic_value_max; i++)
			{
				if(dynamic_value[i] < dynamic_std_max && dynamic_value[i] > dynamic_std_min)
				{
					if(dynamic_sum_temp_02 == 0)
					{
						dynamic_sum_temp_02 = dynamic_value[i];
					}
					else
					{
						dynamic_sum_temp_02 = (dynamic_value[i] + dynamic_sum_temp_02) / 2;	
					}
				}
			}
		
			//	若沒有數據再範圍內，將用平均數替代
			if(dynamic_sum_temp_02 == 0)
			{
				dynamic_sum_temp_02 = dynamic_avg;
			}
			//Weight_UpLoad_Buffer = dynamic_sum_temp_02;	
		
			//Weight_UpLoad_Buffer = dynamic_avg;
	
			//	第一次開機用到
			if(SYSTEM_RESET_FLAG == 1)
			{
				Weight_UpLoad_Buffer = dynamic_sum_temp_02;
				Weight_OLD_Buffer = dynamic_sum_temp_02;
				SYSTEM_RESET_FLAG = 0;
				//return;
			}
	
			//	將差異重量轉換成正值
			if(Weight_OLD_Buffer > dynamic_sum_temp_02)
			{
				//Weight_Temp = Weight_OLD_Buffer - dynamic_avg;
				Weight_Temp = Weight_OLD_Buffer - dynamic_sum_temp_02;
			}
			else
			{
				//Weight_Temp = dynamic_avg - Weight_OLD_Buffer;
				Weight_Temp = dynamic_sum_temp_02 - Weight_OLD_Buffer;
			}
	
			/*
			if(Weight_Temp > Last_Weight_Temp)	// 兩次變化量需大於上次變化量的 20% 克，才承認有變化量產生
			{
			*/
			if(Weight_Temp > 40)
			{
				Weight_UpLoad_Buffer = dynamic_sum_temp_02;	
				OFF_Display();
				delay_ms(100);
				LORAPollingCount = 0;
				Last_Weight_Temp = Weight_Temp * 0.2;
				if(Last_Weight_Temp < 35)
				{
					Last_Weight_Temp = 35;
				}
				Weight_Change[Weight_Change_Value_Count] = Weight_Temp;
		
				if(Weight_Change_Value_Count == Weight_Change_Value_Max)
				{
					Weight_Change_Value_Count = 0;
				}
				else
				{
					Weight_Change_Value_Count++;
				}
				Weight_OLD_Buffer = dynamic_sum_temp_02;
			}
			else
			{
				Weight_OLD_Buffer = dynamic_sum_temp_02;
			}
			/*
			}
			else
			{
				Weight_OLD_Buffer = dynamic_sum_temp_02;
			}
			*/
		}
		else
		{
			RawDataCount++;
		}	
	}
	else
	{
		RawDataCount = 0;
	}

	/*
	if(RawDataBuffer > 200)			//如果秤盤上大於200g，則穩態用5g去取樣，反之用10g
	{
		dynamic_weight_level = 5;
	}
	else
	{
		dynamic_weight_level = 10;
	}
	
	
	if(TmpValue > dynamic_weight_level)						//等待穩態時，大於5g，重新取樣
	{
		if(RawDataCount == 10)				//如果count = 10，代表已經有穩態過
		{
			RawDataBuffer = RawDataTemp;	//儲存現在的重量，提供給後面穩態基準使用
			RawDataCount = 1;				//開始進行計算count
			dynamic_count = 1;
		}
		else if(RawDataCount != 0 && RawDataCount != 10)	//count不等於0與10，代表正在穩態計算中
		{
			if(RawDataCount > 0)							//如果穩態失敗，若count大於0，將進行減1
			{
				RawDataCount = RawDataCount - 1;
			}
		}
		else
		{												
			RawDataBuffer = RawDataTemp;					//count = 0代表穩態失敗，重新儲存現在重量
		}
	}
	else
	{
		
		if(RawDataCount == 10)	//	穩態中變化量值小TmpValue，並且滿足RawDataCount的次數，將承認這筆數值
		{
			
			Weight_UpLoad_Buffer = RawDataBuffer;
						
			if(SYSTEM_RESET_FLAG == 1)
			{
				Weight_OLD_Buffer = RawDataBuffer;
				SYSTEM_RESET_FLAG = 0;
			}
			
			
			if(Weight_OLD_Buffer > RawDataBuffer)
			{
				Weight_Temp = Weight_OLD_Buffer - RawDataBuffer;
			}
			else
			{
				Weight_Temp = RawDataBuffer - Weight_OLD_Buffer;
			}
			
			
			
			if(Weight_Temp > Last_Weight_Temp)	// 兩次變化量需大於上次變化量的 20% 克，才承認有變化量產生
			{
				OFF_Display();
				delay_ms(100);
				LORAPollingCount = 0;
				Last_Weight_Temp = Weight_Temp * 0.2;
				if(Last_Weight_Temp < 35)
				{
					Last_Weight_Temp = 35;
				}
				Weight_Change[Weight_Change_Value_Count] = Weight_Temp;					
				
				if(Weight_Change_Value_Count == Weight_Change_Value_Max)
				{
					Weight_Change_Value_Count = 0;
				}
				else
				{
					Weight_Change_Value_Count++;
				}
			}
			
			Weight_OLD_Buffer = RawDataBuffer;
		}
		else
		{
			RawDataCount++;		//穩態成功一次，count + 1
		}
	}
	*/
}




void Save_Weight_Count_in_NVM(void)
{
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	UINT32u_t	tmp32;
	
	do
	{
		error_code = nvm_read_buffer(
		526  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	tmp32.dword = RawDataZero;
	tmp_page_buffer[0] = tmp32.byte[3];
	tmp_page_buffer[1] = tmp32.byte[2];
	tmp_page_buffer[2] = tmp32.byte[1];
	tmp_page_buffer[3] = tmp32.byte[0];
	tmp32.dword = RawDataUser;
	tmp_page_buffer[4] = tmp32.byte[3];
	tmp_page_buffer[5] = tmp32.byte[2];
	tmp_page_buffer[6] = tmp32.byte[1];
	tmp_page_buffer[7] = tmp32.byte[0];
	tmp32.dword = RawDataBase;
	tmp_page_buffer[8] = tmp32.byte[3];
	tmp_page_buffer[9] = tmp32.byte[2];
	tmp_page_buffer[10] = tmp32.byte[1];
	tmp_page_buffer[11] = tmp32.byte[0];
	
	
	do
	{
		error_code = nvm_erase_row(
		526 * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	do
	{
		error_code = nvm_write_buffer(
		526  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
}



void Load_Weight_Count_in_NVM(void)
{
	UINT32u_t	tmp32;
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	do
	{
		error_code = nvm_read_buffer(
		526  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	tmp32.byte[3] = tmp_page_buffer[0];
	tmp32.byte[2] = tmp_page_buffer[1];
	tmp32.byte[1] = tmp_page_buffer[2];
	tmp32.byte[0] = tmp_page_buffer[3];
	RawDataZero = tmp32.dword;
	
	tmp32.byte[3] = tmp_page_buffer[4];
	tmp32.byte[2] = tmp_page_buffer[5];
	tmp32.byte[1] = tmp_page_buffer[6];
	tmp32.byte[0] = tmp_page_buffer[7];
	RawDataUser = tmp32.dword;
	
	tmp32.byte[3] = tmp_page_buffer[8];
	tmp32.byte[2] = tmp_page_buffer[9];
	tmp32.byte[1] = tmp_page_buffer[10];
	tmp32.byte[0] = tmp_page_buffer[11];
	RawDataBase = tmp32.dword;

	if(port_pin_get_input_level(PIN_PA03) == false)
	{
		RawDataBase = 200000000 / (RawDataUser - RawDataZero);	
	}
	//RawDataBase = 0x0403;
	//2KG
}

void Wait_RX_IRQ_Index_Func(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->LoRa_IRQ_Flag == 1)
	{
		LED_R_ON
		wdt_reset_count();
		change_and_check(&(tmp_LORA->NowStatus), Read_LoRa_RxBuffer_Index);
	}
}

//=================================================
#endif
//=================================================