#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


//=================================================
#ifdef Dev_Feed_Weigher
//=================================================

const char Msg_PowerUP[]			= "PowerOn   \0";
const char Msg_Setting[]			= "Setting   \0";
const char Msg_CHKOverSW[]			= "ChkGate   \0";
const char Msg_SettingCloseGATE[]	= "CloseGate \0";
const char Msg_SettingOPENGATE[]	= "OpenGate  \0";
const char Msg_WeightMeasure[]		= "Sensing   \0";
const char Msg_WaitSomeTime[]		= "stable    \0";
const char Msg_WeightCatch[]		= "Catch     \0";
const char Msg_WeightNext[]			= "Next      \0";
const char *ptr;



void Display_Status(void)
{
	
	uart_str(ptr);
	tx_dword(Total_Weight);
	uTXByte(Debug_COM, ' ');
	tx_dword(long_to_bcd_4byte(RawDataTemp));
	uart_str(" Set: ");
	if(Setting_SW == true)	uart_str("1");	else	uart_str("0");
	uart_str(" A1: ");
	uTXByte(Debug_COM, AI1 + 0x30);
	uart_str(" A2: ");
	uTXByte(Debug_COM, AI2 + 0x30);
	uTXByte(Debug_COM, ' ');
	uart_send_word(Debug_COM, hex_to_bcd(OverCount));
	uTXByte(Debug_COM, ' ');
	
	uTXByte(Debug_COM, '[');
	tx_dword(Rawdata);
	uTXByte(Debug_COM, ' ');
	uart_send_word(Debug_COM, RawDataBase);
	uTXByte(Debug_COM, ']');
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
	
	uTXByte(Debug_COM, 0x0d);
	
		
}


void Feed_Weigher_main_Func(void)
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
	

	
	tmpAI2 = adc_get_val(ADC_USER_03);
	tmpAI1 = adc_get_val(ADC_USER_04);
	if(tmpAI2 > 100) AI2 = true; else AI2 = false;
	if(tmpAI1 > 100) AI1 = true; else AI1 = false;
		
	switch(FW_Status)
	{
		case PowerUP:
			ptr = &Msg_PowerUP[0];
			if(Setting_SW == true)
			{
				if(AI2 == false)	FW_Status = SettingCloseGATE;
			}
			else
			{
				if(AI2 == true)		FW_Status = CHKOverSW;
			}
			break;
			
		case SettingCloseGATE:
			ptr = &Msg_SettingCloseGATE[0];
			if(AI1 == true)
			{
				MOTOR_STOP
				FW_Status = Setting;
			}
			else
			{
				MOTOR_CCW
			}
			break;
			
		case Setting:
			ptr = &Msg_Setting[0];
			if(Setting_SW == false)
			{
				MOTOR_CW
				OverCount = Set30Sec;
				FW_Status = SettingOPENGATE;
				break;
			}
			break;
			
		case SettingOPENGATE:
			ptr = &Msg_SettingOPENGATE[0];
			if(OverCount == 0)
			{
				MOTOR_STOP
				FW_Status = PowerUP;
			}
			break;

		case CHKOverSW:
			ptr = &Msg_CHKOverSW[0];
			if(AI1 == true)
			{
				MOTOR_STOP
				DO1_ON
				FW_Status = WeightMeasure;
			}
			else
			{
				MOTOR_CCW
			}
			break;
			
		case WeightMeasure:
			ptr = &Msg_WeightMeasure[0];
			if(WeightStatus == Wegight_Judg_Index)
			{
				if(RawDataTemp > 100)
				{
					DO1_OFF
					OverCount = Set03Sec;
					FW_Status = WaitSomeTime;
				}
			}
			break;
			
		case WaitSomeTime:
			ptr = &Msg_WaitSomeTime[0];
			if(Stable_flag == true)	//維持標準差5以下2次
			{
				FW_Status = WeightCatch;
			}
			break;
			
		case WeightCatch:
			ptr = &Msg_WeightCatch[0];
			if(WeightStatus == Wegight_Judg_Index)
			{
					//Total_Weight = Total_Weight + (RawDataTemp / 10);
					Total_Weight = Total_Weight + (dynamic_sum_temp_02 / 10);
					
					Total_Weight = Total_Weight; //相容力固的秤重0.00Kg
					if(Total_Weight > 0xffffffff)
					{
						Total_Weight = Total_Weight - 0xffffffff;
					}
					Save_Total_Weight_into_NVM();
					MOTOR_CW
					OverCount = Set30Sec;
					FW_Status = WeightNext;
					Stable_flag = false;
			}
			break;
			
		case WeightNext:
			ptr = &Msg_WeightNext[0];
			if(OverCount == 0)
			{
				MOTOR_STOP
				FW_Status = PowerUP;
			}
			if(WeightStatus == Wegight_Judg_Index)
			{
				if(RawDataTemp < 30)
				{
					MOTOR_STOP
					FW_Status = PowerUP;
				}
			}
			break;
	}
	
	if(LORA01->Lora_Show_Flag == true)
	{
		if(WeightStatus == Wegight_Judg_Index)	Display_Status();
	}
	
}



//void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA)
void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	UINT32u_t tmp32;
	UINT16u_t tmp_adc;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3] = tmp_LORA->MyLoRaID[0];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4] = tmp_LORA->MyLoRaID[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5] = tmp_LORA->MyLoRaID[2];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6] = tmp_LORA->MyLoRaID[3];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][7] = 0x05;
			
	tmp32.dword = Total_Weight;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][8] = tmp32.byte[3];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][9] = tmp32.byte[2];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][10] = tmp32.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][11] = tmp32.byte[0];
		
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][12] = 0x00;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][13] = 0x00;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][14] = 0x00;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][15] = 0x00;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][16] = 0x00;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][17] = 0x00;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][18] = 0x00;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][19] = 0x00;
	
	tmp_adc.word = ADC_LVD_Temp;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][20] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][21] = tmp_adc.byte[0];
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][22] = 0x00;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][23] = 0x00;
	
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

		tx_dword(Total_Weight);
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



void Weight_Judg(void)
{
	unsigned char i;
	unsigned long TmpValue;
	

	

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
			Stable_flag = true;
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


void Save_Total_Weight_into_NVM(void)
{
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	UINT32u_t	tmp32;

	tmp32.dword = Total_Weight;
	tmp_page_buffer[0] = tmp32.byte[3];
	tmp_page_buffer[1] = tmp32.byte[2];
	tmp_page_buffer[2] = tmp32.byte[1];
	tmp_page_buffer[3] = tmp32.byte[0];
	do
	{
		error_code = nvm_erase_row(547 * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);

	do
	{
		error_code = nvm_write_buffer(547  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE, tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
}
void Load_Total_Weight_Form_NVM(void)
{
	UINT32u_t	tmp32;
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	do
	{
		error_code = nvm_read_buffer(547  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE, tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	tmp32.byte[3] = tmp_page_buffer[0];
	tmp32.byte[2] = tmp_page_buffer[1];
	tmp32.byte[1] = tmp_page_buffer[2];
	tmp32.byte[0] = tmp_page_buffer[3];
	Total_Weight = tmp32.dword;
	if(Total_Weight == 0xffffffff)
	{
		Total_Weight = 0;
	}
	
}

void Save_Weight_Count_in_NVM(void)
{
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	UINT32u_t	tmp32;
		
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
		error_code = nvm_erase_row(546 * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	do
	{
		error_code = nvm_write_buffer(546  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE, tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
}



void Load_Weight_Count_in_NVM(void)
{
	UINT32u_t	tmp32;
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	do
	{
		error_code = nvm_read_buffer(546  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE, tmp_page_buffer, NVMCTRL_PAGE_SIZE);
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