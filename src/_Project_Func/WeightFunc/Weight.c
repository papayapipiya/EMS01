﻿#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


//=================================================
#ifdef Dev_Weight
//=================================================

unsigned char dynamic_weight_level;
unsigned char dynamic_count;
#define dynamic_value_max 11
#define dynamic_std_number 10
unsigned long dynamic_value[dynamic_value_max];
unsigned long dynamic_avg;
unsigned long dynamic_sum_temp;
unsigned long dynamic_sum_temp_02;
unsigned long dynamic_std;
unsigned long dynamic_std_max;
unsigned long dynamic_std_min;


void Weight_main_Func(void)
{
	switch(WeightStatus)
	{
		case Wegight_Catch_Index:
			WeightStatus = Wegight_Cal_Index_01;
			//change_and_check(&WeightStatus, Wegight_Cal_Index_01);
			Rawdata = ReadCount();
			Weight_Correction(Rawdata);	
			break;
		case Wegight_Catch_Index_01:
			WeightStatus = Wegight_Catch_Index_02;
			//change_and_check(&WeightStatus, Wegight_Catch_Index_02);
			Rawdata = (Rawdata + ReadCount())/2;
			Weight_Correction(Rawdata);
			break;
		
		case Wegight_Catch_Index_02:
			WeightStatus = Wegight_Cal_Index_01;
			//change_and_check(&WeightStatus, Wegight_Cal_Index_01);
			Rawdata = (Rawdata + ReadCount())/2;
			Weight_Correction(Rawdata);
			break;
		
		case Wegight_Cal_Index_01:
			WeightStatus = Wegight_Cal_Index_02;
			//change_and_check(&WeightStatus, Wegight_Cal_Index_02);
			if(Rawdata > RawDataZero)
				RawDataTemp = ((Rawdata - RawDataZero) * RawDataBase);
			else
				RawDataTemp = 0;
			break;
		
		case Wegight_Cal_Index_02:
			WeightStatus = Wegight_Judg_Index;
			//change_and_check(&WeightStatus, Wegight_Judg_Index);
			RawDataTemp = RawDataTemp / 1000;
			RawDataTemp = RawDataTemp / 100;
			break;
		
		case Wegight_Judg_Index:
			WeightStatus = Wegight_Display_Index_01;
			//change_and_check(&WeightStatus, Wegight_Display_Index_01);
			SET_Display_Value(long_to_bcd_4byte(RawDataTemp));
			Weight_Judg();
			break;
		
		case Wegight_Display_Index_01:
			WeightStatus = Wegight_Display_Index_02;
			//change_and_check(&WeightStatus, Wegight_Display_Index_02);
			break;
		
		case Wegight_Display_Index_02:
			WeightStatus = Wegight_Catch_Index;
			//change_and_check(&WeightStatus, Wegight_Catch_Index);
			ON_Display();
			break;
	}
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




void Weight_Correction(unsigned long tmp_data)
{
	// 0 g
	// PA02 = 0 g, PA03 = 2000g, False = ON, True = OFF
	if(port_pin_get_input_level(PIN_PA02) == false && port_pin_get_input_level(PIN_PA03) == true)
	{
		if(sw_0_timer == sw_0_timer_Max)
		{
			OFF_Display();
			RawDataZero = tmp_data;
			Save_Weight_Count_in_NVM();
			Load_Weight_Count_in_NVM();
			uart_str("Save 0 g\r\0");
			ON_Display();	
			sw_0_timer = 0;
		}
	}
	else
	{
		sw_0_timer = 0;
	}
	
	//================================================
	// 2000 g
	// PA02 = 0 g, PA03 = 2000g, False = ON, True = OFF
	if(port_pin_get_input_level(PIN_PA02) == true && port_pin_get_input_level(PIN_PA03) == false)
	{
		if(sw_2000_timer == sw_2000_timer_Max)
		{
			OFF_Display();
			RawDataUser = tmp_data;
			Save_Weight_Count_in_NVM();
			Load_Weight_Count_in_NVM();
			uart_str("Save 2000 g\r\0");
			ON_Display();
			sw_2000_timer = 0;
		}
	}
	else
	{
		sw_2000_timer = 0;
	}

}

void Print_Weight_Change_Value_Buf_Func(void)
{
	unsigned char i;
	for(i = 0; i < 6; i++)
	{
		if(Weight_Change_Value_Count == i)
		{
			uart_str("____");
		}
		else
		{
			uart_send_word(Debug_COM, hex_to_bcd(Weight_Change[i]));
		}	
		uTXByte(Debug_COM, ' ');
	}
	
}



void Weight_Judg(void)
{
	unsigned char i;
	unsigned long TmpValue;
	
	if(LORA01->Lora_Show_Flag == true)
	{
		//wdt_reset_count();
		
		/*
		uTXByte(Debug_COM, '{');
		uart_send_word(Debug_COM, sw_0_timer);
		uTXByte(Debug_COM, ',');
		uart_send_word(Debug_COM, sw_2000_timer);
		uTXByte(Debug_COM, '}');
		uTXByte(Debug_COM, ' ');
		*/
		
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
		TmpValue = RawDataTemp - RawDataBuffer;
	else
		TmpValue = RawDataBuffer - RawDataTemp;

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
	for(i = 1; i < dynamic_value_max; i++)
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
			if(dynamic_sum_temp_02 == 0)	dynamic_sum_temp_02 = dynamic_avg;
	
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
				Weight_Temp = Weight_OLD_Buffer - dynamic_sum_temp_02;
			else
				Weight_Temp = dynamic_sum_temp_02 - Weight_OLD_Buffer;
	
			if(Weight_Temp > 40)	//變化量需大於40克以上
			{
				Weight_UpLoad_Buffer = dynamic_sum_temp_02;	
				OFF_Display();
				delay_ms(100);
				//LORAPollingCount = 0;
				//Last_Weight_Temp = Weight_Temp * 0.2;
				//if(Last_Weight_Temp < 35)
				//{
					//Last_Weight_Temp = 35;
				//}
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