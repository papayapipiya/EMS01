/*
 * Ext_INT.c
 *
 * Created: 2017/5/16 下午 01:39:04
 *  Author: WillChen
 */ 

#include <asf.h>
#include <stdio.h>
#include "MyDefine.h"

//! [setup]
void configure_extint_channel(void)
{
	struct extint_chan_conf config_extint_chan;
	extint_chan_get_config_defaults(&config_extint_chan);
	
	config_extint_chan.gpio_pin           = PIN_PA11A_EIC_EXTINT11;
	config_extint_chan.gpio_pin_mux       = MUX_PA11A_EIC_EXTINT11;
	config_extint_chan.gpio_pin_pull      = EXTINT_PULL_NONE;
	config_extint_chan.detection_criteria = EXTINT_DETECT_RISING;
	extint_chan_set_config(PIN_PA11, &config_extint_chan);
	

	config_extint_chan.gpio_pin           = PIN_PA07A_EIC_EXTINT7;
	config_extint_chan.gpio_pin_mux       = MUX_PA07A_EIC_EXTINT7;
	config_extint_chan.gpio_pin_pull      = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria = EXTINT_DETECT_BOTH;
	extint_chan_set_config(PIN_PA07, &config_extint_chan);
	
}






void configure_extint_callbacks(void)
{
	//extint_register_callback(LORA01_ExtIRQ_callback, PIN_PA11, EXTINT_CALLBACK_TYPE_DETECT);
	extint_register_callback(LORA01_extint_detection_callback, PIN_PA11, EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(PIN_PA11, EXTINT_CALLBACK_TYPE_DETECT);
	
	extint_register_callback(LORA02_extint_detection_callback, PIN_PA07, EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(PIN_PA07, EXTINT_CALLBACK_TYPE_DETECT);
}


unsigned char tmp_data;
void LORA01_extint_detection_callback(void)
{
	if(LORA01->LoRa_IRQ_Flag == 0)
	{
		LORA01->LoRa_IRQ_Flag = 1;
	}
}

void LORA02_extint_detection_callback(void)
{
	//=================================================
	#ifdef Dev_Wifi
	//=================================================

	
	My_WiFi.AP_Mode_Switch = port_pin_get_input_level(PIN_PA07);
	if(My_WiFi.AP_Mode_Switch == Is_AP_Mode)
	{
		LED_R_OFF
		LED_G_OFF
		LED_B_OFF
		My_WiFi.wifi_work_led_flag =  true;
		uart_str_COM(Debug_COM, "AP Mode ON..\r\0");
	}
	else
	{
		LED_R_OFF
		LED_G_OFF
		LED_B_OFF
		My_WiFi.wifi_work_led_flag =  true;
		uart_str_COM(Debug_COM, "AP Mode OFF..\r\0");	
		My_WiFi.Cycletime = 140;
	}
	My_WiFi.RunStep			= 0;
	My_WiFi.Send_UnSuccess_count	= 0;
	My_WiFi.WIFI_Main_Counter		= 0;
	My_WiFi.NowStatus				= WiFi_NOP;
	//=================================================
	#endif
	//=================================================
}
