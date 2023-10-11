#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



void tc_callback_to_to_period_51200us(struct tc_module *const module_inst)
{
	//計數Dev List設定Polling的時間
	if(LORA01->Lora_Dev_Count_IRQ_Flag == 0)
	{
		if(LORA01->t2_tmp_count > 10)	//51.2ms * 10 = 512ms
			LORA01->Lora_Dev_Count_IRQ_Flag = 1;
		else
			LORA01->t2_tmp_count++;
	}
	
	if(debug_cycle_time < debug_cycle_time_Max) debug_cycle_time++;
	
	

	//if(sleep_count < sleep_count_max)sleep_count++;
	

	if(My_WiFi.wifi_work_led_flag == true)
	{
		if(My_WiFi.wifi_work_led == wifi_work_led_on)
		{
			if(My_WiFi.AP_Mode_Switch == Is_AP_Mode)
				LED_R_ON
			else
				LED_B_ON
		}
		else if(My_WiFi.wifi_work_led == wifi_work_led_off)
		{
			if(My_WiFi.AP_Mode_Switch == Is_AP_Mode)
				LED_R_OFF
			else
				LED_B_OFF
		}
		
		if(My_WiFi.wifi_work_led < wifi_work_led_off)
		{
			if(My_WiFi.wifi_network_flag == true)
				My_WiFi.wifi_work_led = My_WiFi.wifi_work_led + 5;
			else
				My_WiFi.wifi_work_led++;
		}
		else
		{
			My_WiFi.wifi_work_led = 0;
		}
	}
	else
	{
		My_WiFi.wifi_work_led = 0;
		LED_B_ON
	}

}

unsigned char tc_index = 0;
unsigned char lora_tc_index = 0;
#define ETC_COM	&MyUart04
void tc_callback_to_to_period_10us(struct tc_module *const module_inst)
{
	test_counter++;
}

unsigned long mcu_reset;
void tc_callback_to_to_period_200us(struct tc_module *const module_inst)
{
	
	if(LORA01->f_count < Timer_Timeout)	LORA01->f_count++;
	if(LORA02->f_count < Timer_Timeout)	LORA02->f_count++;
	
	
	if(My_WiFi.WIFI_Main_Counter < WIFI_Main_Counter_Max)		My_WiFi.WIFI_Main_Counter++;
	if(My_WiFi.Rx_Timeout_Count < WIFI_Rx_Timeout_Count_Max)	My_WiFi.Rx_Timeout_Count++;
	if(My_WiFi.f_count < WIFI_f_count_Timer_Max)				My_WiFi.f_count++;
	if(mGateway.uEMS.uModule.uADC.timer < ADC_time_Max)			mGateway.uEMS.uModule.uADC.timer++;
	if(mGateway.uEMS.uModule.uGPIO.timer < GPIO_time_Max)			mGateway.uEMS.uModule.uGPIO.timer++;

}


void tc_user_keyin(struct tc_module *const module_inst)
{
	//uTXByte(Debug_COM,'A');
	
	
}

