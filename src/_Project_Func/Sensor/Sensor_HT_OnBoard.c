#include <asf.h>
#include <stdio.h>
#include "myDefine.h"
#include "math.h"


//=================================================
#ifdef Dev_Sensor
//=================================================

void GetSensorSD(SENSOR_DEFINE_t *tmp_CH, unsigned int tmpData)
{
	unsigned char i, j;
	unsigned long temp;
	
	
	tmp_CH->MyData[tmp_CH->MyDataCount] = tmpData;
	tmp_CH->MyDataCount++;
	//	儲存取樣樣本，輪迴寫入
	if(tmp_CH->MyDataCount == Sensor_Count_Max)
	{
		tmp_CH->MyDataCount = 0;
	}
	

	//	計算平均數
	tmp_CH->MyDataAVG_tmp = tmp_CH->MyData[0];
	for(i = 1; i < Sensor_Count_Max; i++)
	{
		tmp_CH->MyDataAVG_tmp = (tmp_CH->MyData[i] + tmp_CH->MyDataAVG_tmp) / 2;
	}
	
	//	計算標準差
	temp = 0;
	for(i = 0; i < Sensor_Count_Max; i++)
	{
		if(tmp_CH->MyData[i] > tmp_CH->MyDataAVG)
		{
			temp += (tmp_CH->MyData[i] - tmp_CH->MyDataAVG_tmp)^2;
		}
		else
		{
			temp += (tmp_CH->MyDataAVG_tmp - tmp_CH->MyData[i])^2;
		}
	}
	temp = temp / Sensor_Count_Max;
	temp = sqrt(temp);
	tmp_CH->MyDataSD = temp;
	tmp_CH->MyDataOK = false;
	if(tmp_CH->MyDataAVG_tmp == 0) return;
	if(tmp_CH->MyDataSD > STDV_BASE_LINE.DataBuf.word) return;
	tmp_CH->MyDataOK = true;
	tmp_CH->MyAVGP = tmp_CH->MyDataAVG_tmp + tmp_CH->MyDataSD;
	tmp_CH->MyAVGN = tmp_CH->MyDataAVG_tmp - tmp_CH->MyDataSD;
	
	temp = 0;
	j = 0;
	for(i = 0; i < Sensor_Count_Max; i++)
	{
		if(tmp_CH->MyAVGP >= tmp_CH->MyData[i])
		{
			if(tmp_CH->MyAVGN <= tmp_CH->MyData[i])
			{
				temp = temp + tmp_CH->MyData[i];
				j++;
			}
		}
	}
	if(j != 0)	tmp_CH->MyDataAVG = temp/ j;
	if(j == 0)	tmp_CH->MyDataOK = false;
	
}

// 目前約一秒取樣20次
void GetSensorOnBoardVal(void)
{
	unsigned long tmpVal;
	unsigned int ADC_Raw_01_tmp, ADC_Raw_02_tmp;
	long stmp16;
	unsigned char i;
	unsigned long temp;
	unsigned int aaa,bbb;
	
	
		
	CH1->ADC_Raw = adc_get_val(ADC_USER_01);
	CH1->ADC_Raw_tmp = CH1->ADC_Raw;			//ADC原始資料
	GetSensorSD(CH1, CH1->ADC_Raw);
	CH1->ADC_Raw = CH1->MyDataAVG;			//ADC的平均值
	
	
	if(TGS2600_SIGNED.DataBuf.word == 1)	//是否有補償機制
		CH1->ADC_Raw = CH1->ADC_Raw + TGS2600_OFFSET.DataBuf.word;
	else
		CH1->ADC_Raw = CH1->ADC_Raw - TGS2600_OFFSET.DataBuf.word;
		
	
	//CH1->MyDataOK = true;
	if(CH1->MyDataOK == true)	CH1->ADC_Raw_OK = CH1->ADC_Raw;	//如果標準差在規格內，將準備替換輸出的數值
	
	/*
	if(LORA01->Power_On_Delay == 0)
	{
		
		if( CH1->ADC_Raw_OK > CH1->MyUpdate_Max)
		{
			CH1->MyUpdate_Max = CH1->ADC_Raw_OK;
		}
	}
	else
	{
		CH1->MyUpdate_Max = CH1->ADC_Raw_OK;
	}
	*/
	
	CH1->MyUpdate_Max = CH1->ADC_Raw_OK;
	
	CH1->MyUpdate_Max = CH1->MyUpdate_Max * 4;
	//=======================================================

	

	
	if(LORA01->Show_Sensor_Raw)
	{	
		uart_str("SDL:");
		uart_send_word(Debug_COM, hex_to_bcd(STDV_BASE_LINE.DataBuf.word));
		uTXByte(Debug_COM, ' ');
		uTXByte(Debug_COM, '|');
		
		uart_str("CH1:");
		uart_send_word(Debug_COM, hex_to_bcd(CH1->ADC_Raw_tmp));
		uTXByte(Debug_COM, '{');
		uart_send_word(Debug_COM, hex_to_bcd(CH1->MyDataAVG));
		uTXByte(Debug_COM, ',');
		uart_send_word(Debug_COM, hex_to_bcd(CH1->MyDataSD));
		uTXByte(Debug_COM, ',');
		uart_send_word(Debug_COM, hex_to_bcd(CH1->MyAVGP));
		uTXByte(Debug_COM, ',');
		uart_send_word(Debug_COM, hex_to_bcd(CH1->MyAVGN));
		uTXByte(Debug_COM, '}');
		
		if(TGS2600_SIGNED.DataBuf.word == 0)
			uTXByte(Debug_COM, '-');
		else
			uTXByte(Debug_COM, '+');
		uart_send_word(Debug_COM, hex_to_bcd(TGS2600_OFFSET.DataBuf.word));
		uTXByte(Debug_COM, '=');
		if(CH1->MyDataOK == true)
		{
			uart_send_word(Debug_COM, hex_to_bcd(CH1->MyUpdate_Max));
			uTXByte(Debug_COM, '[');
			uart_send_word(Debug_COM, CH1->MyUpdate_Max);
			uTXByte(Debug_COM, ']');
		}
		else
		{
			uart_str("          ");
		}
		
		uTXByte(Debug_COM, '_');
		uTXByte(Debug_COM, '_');
		uart_send_word(Debug_COM, hex_to_bcd(LORA01->Power_On_Delay));
		
		uTXByte(Debug_COM, 0x0d);
	}
	
	
	if(LORA01->Power_On_Delay == 0)	startup_flag = true;
}




//=================================================
#endif
//=================================================