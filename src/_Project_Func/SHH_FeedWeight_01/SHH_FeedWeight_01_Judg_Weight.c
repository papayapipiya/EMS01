#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


//=================================================
#ifdef SHH_FeedWeight_01
//=================================================



bool RY_FLAG;
void Judg_Feed_Weight_Func(void)
{
	UINT16u_t tmp16;
	UINT16u_t CRC16;
	UINT32u_t tmp32;
	tmp32.dword = dynamic_sum_temp_02 / unit;
	
	NowFeedWeight.word[1] = tmp32.word[1];
	NowFeedWeight.word[0] = tmp32.word[0];
	
	
	//if(debug_cycle_time == debug_cycle_time_Max)
	//{
		//
		//debug_cycle_time = 0;
	//}
	
	if(Show_weight_Flag)Print_Showw_weight();
	
	if(System_Status == Error) return;
	
	switch(W_NowStatus)
	{
		case IsPowerON:
			SaveFeedWeight.dword = NowFeedWeight.dword;
			PowerUP = true;
			W_NowStatus = IsJudg;
			break;
		
		case IsJudg:
			W_count = 0;
			ACCFeedWeight.dword = 0;
			if(SaveFeedWeight.dword > NowFeedWeight.dword)
			{
				W_NowStatus = IsBigJudg;
			}
			else
			{
				W_NowStatus = IsSmallJudg;
			}
			break;
		
		case IsBigJudg:
			if(SaveFeedWeight.dword > NowFeedWeight.dword)
			{
				ACCFeedWeight.dword = SaveFeedWeight.dword - NowFeedWeight.dword;
				if(ACCFeedWeight.dword > Feeding_Value_Limit)
				{
					if(W_count < FeedW_Count)		W_count++;
					if(W_count == FeedW_Count)		W_NowStatus = IsBig;
				}
			}
			else
			{
				W_NowStatus = IsJudg;
				W_count = 0;
			}
			break;
		
		case IsSmallJudg:
			if(SaveFeedWeight.dword < NowFeedWeight.dword)
			{
				ACCFeedWeight.dword = NowFeedWeight.dword - SaveFeedWeight.dword;
				if(ACCFeedWeight.dword > Feeding_Value_Limit)
				{
					if(W_count < FeedW_Count)		W_count++;
					if(W_count == FeedW_Count)		W_NowStatus = IsSmall;
				}
			}
			else
			{
				W_NowStatus = IsJudg;
				W_count = 0;
			}
			break;
		
		case IsBig:
			ACCFeedWeight.dword = SaveFeedWeight.dword - NowFeedWeight.dword;
			SaveACCFeedWeight.dword += ACCFeedWeight.dword;
			SaveFeedWeight.dword = NowFeedWeight.dword;
			if(SaveACCFeedWeight.dword > Max_Feeding_Value)
			{
				SaveACCFeedWeight.dword = SaveACCFeedWeight.dword - Max_Feeding_Value;
			}
			W_NowStatus = IsJudg;
			break;
		
		case IsSmall:
			SaveFeedWeight.dword = NowFeedWeight.dword;
			W_NowStatus = IsJudg;
			break;
	}
}



void STD(void)
{
	unsigned char i;
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
}


void Weight_Judg(void)
{
	unsigned char i;
	unsigned long TmpValue;
	
	if(LORA01->Lora_Show_Flag == true)	Print_Show_weight();
	
	
	//	將重量差異轉成正值
	if(RawDataTemp >= RawDataBuffer)
	TmpValue = RawDataTemp - RawDataBuffer;
	else
	TmpValue = RawDataBuffer - RawDataTemp;
	
	
	dynamic_value[dynamic_count] = TmpValue;
	//	儲存取樣樣本，輪迴寫入
	if(dynamic_count == dynamic_value_max)
	dynamic_count = 0;
	else
	dynamic_count++;
	
	//  計算標準差
	STD();

	//	判斷標準差低於3
	if(dynamic_std > 3)
	{
		RawDataCount = 0;
		return;
	}

	//	維持3次標準差都在3
	if(RawDataCount < 2)
	{
		RawDataCount++;
		return;
	}
	
	
	
	
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
	}
	
	//	將差異重量轉換成正值
	if(Weight_OLD_Buffer > dynamic_sum_temp_02)
	Weight_Temp = Weight_OLD_Buffer - dynamic_sum_temp_02;
	else
	Weight_Temp = dynamic_sum_temp_02 - Weight_OLD_Buffer;
	
	if(Weight_Temp > Change_Feeding_Value)	//變化量 > Change_Feeding_Value
	{
		Weight_UpLoad_Buffer = dynamic_sum_temp_02;
		OFF_Display();
		delay_ms(100);
		Weight_Change[Weight_Change_Value_Count] = Weight_Temp;
		
		if(Weight_Change_Value_Count == Weight_Change_Value_Max)
		Weight_Change_Value_Count = 0;
		else
		Weight_Change_Value_Count++;
		
		Weight_OLD_Buffer = dynamic_sum_temp_02;
	}
	else
	{
		Weight_OLD_Buffer = dynamic_sum_temp_02;
	}
	
}






//=================================================
#endif
//=================================================