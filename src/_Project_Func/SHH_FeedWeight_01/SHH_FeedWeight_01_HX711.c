#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


//=================================================
#ifdef SHH_FeedWeight_01
//=================================================


void Read_Weight(void)
{
	switch(WeightStatus)
	{
		case Wegight_Catch_Index:
			WeightStatus = Wegight_Cal_Index_01;
			Rawdata = ReadCount();
			Weight_Correction(Rawdata);
			break;
			
		//case Wegight_Catch_Index_01:
			//WeightStatus = Wegight_Catch_Index_02;
			//Rawdata = (Rawdata + ReadCount())/2;
			//Weight_Correction(Rawdata);
			//break;
		//
		//case Wegight_Catch_Index_02:
			//WeightStatus = Wegight_Cal_Index_01;
			//Rawdata = (Rawdata + ReadCount())/2;
			//Weight_Correction(Rawdata);
			//break;
		
		case Wegight_Cal_Index_01:
			WeightStatus = Wegight_Cal_Index_02;
			if(Rawdata > RawDataZero)
			{
				RawDataTemp = ((Rawdata - RawDataZero) * (RawDataBase / 10));
			}
			else
			{
				RawDataTemp = 0;
			}
			break;
		
		case Wegight_Cal_Index_02:
			WeightStatus = Wegight_Judg_Index;
			RawDataTemp = RawDataTemp / 1000;
			RawDataTemp = RawDataTemp / 10;
			//RawDataTemp = RawDataTemp / 100;	// 0.0公斤
			break;
		
		case Wegight_Judg_Index:
			WeightStatus = Wegight_Display_Index_01;
			//RawDataTemp = 45000;
			SET_Display_Value(long_to_bcd_4byte(RawDataTemp));
			Weight_Judg();
			break;
			
		case Wegight_Display_Index_01:
			WeightStatus = Wegight_Display_Index_02;
			break;
		
		case Wegight_Display_Index_02:
			WeightStatus = Wegight_Catch_Index;
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
	
	while(port_pin_get_input_level(PIN_PA17) == true);
	
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
	//================================================
	// 歸零 0 g
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
	// 校正 2000 g
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








//=================================================
#endif
//=================================================