#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


//=================================================
#ifdef SHH_FeedWeight_01
//=================================================
unsigned char green_flash_count;
bool error_flag = false;
void MainFunc(void)
{
	unsigned long tmp_weight;
	
	
	switch(Switch_Status)
	{
		case X1_STEP_01:
			if(X1 == false)
			{
				PRED_OFF
				PGREEN_OFF
				Enable_OUT
				Disable_IN
				OFF_Display();
				Switch_Status = X1_STEP_02;
				return;
			}
		break;
		
		case X1_STEP_02:
			if(X1 == true)
			{
				System_Status = System_Waitting;
				Switch_Status = X1_STEP_01;
			}
			else
			{
				return;
			}
			
		break;
		
		case X1_STEP_03:
		break;
		
		case X1_STEP_04:
		break;
	}
	
	
	
	
	
	
	//	讀取HX711重量數值
	Read_Weight();
	//	計算是否需要儲存重量
	if(System_Status != Error)	Judg_Feed_Weight_Func();
	
	
	if(WeightStatus != Wegight_Judg_Index) return;
	
	
	tmp_weight = RawDataTemp / unit;
	switch(System_Status)
	{
		case System_Waitting:
			PGREEN_ON
			if(tmp_weight < Low_Feeding_Value)
			{
				if(debounce_count_max >  debounce_count)	debounce_count++;
				if(debounce_count_max == debounce_count)	System_Status = Disable_Out;
			}
			else
			{
				debounce_count = 0;
			}
			break;
		
		case Disable_Out:
			Disable_OUT
			System_Status = Input_Feed;
			break;
		
		case Input_Feed:
			Enable_IN
			Input_Feed_Timeout_Count = 0;
			System_Status = Waitting_Feed_Overflow;
			break;
		
		case Waitting_Feed_Overflow:
			green_flash_count++;
			if(green_flash_count == 5)
			{
				PGREEN_OFF	
			}
			if(green_flash_count == 10)
			{
				PGREEN_ON
				green_flash_count = 0;
			}
			
			if(Input_Feed_Timeout_Count == Input_Feed_Timeout_Max)
			{
				System_Status = Error;
			}
			else
			{
				if(tmp_weight > High_Feeding_Value)
				{
					PGREEN_ON
					Disable_IN
					System_Status = Enable_Out;
				}
			}
			break;
		
		case Enable_Out:
			Enable_OUT
			System_Status = System_Waitting;
			break;
		
		case Error:
			Disable_IN
			Enable_OUT
			PGREEN_OFF
			PRED_ON
			break;
		
	}
}






//=================================================
#endif
//=================================================