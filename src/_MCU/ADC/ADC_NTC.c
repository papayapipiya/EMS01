/*
 * ADC_NTC.c
 *
 * Created: 2023/10/6 下午 02:36:12
 *  Author: papay
 */ 
#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

void ems01_input_adc(void)
{
	char str[100];
	
	if(mGateway.uEMS.uModule.uADC.timer != ADC_time_Max) return;
	mGateway.uEMS.uModule.uADC.timer = 0;
	

	
	//Load_eID_Flag(mGateway.uEMS.uModule.eID);
	mGateway.uEMS.uModule.uADC.NTC_No_01 = getADC_NTC(ADC_POSITIVE_INPUT_PIN0);
	mGateway.uEMS.uModule.uADC.NTC_No_02 = getADC_NTC(ADC_POSITIVE_INPUT_PIN4);
	mGateway.uEMS.uModule.uADC.NTC_No_03 = getADC_NTC(ADC_POSITIVE_INPUT_PIN5);
	mGateway.uEMS.uModule.uADC.NTC_No_04 = getADC_NTC(ADC_POSITIVE_INPUT_PIN6);
	mGateway.uEMS.uModule.uADC.NTC_No_05 = getADC_NTC(ADC_POSITIVE_INPUT_PIN7);
	mGateway.uEMS.uModule.uADC.NTC_No_06 = getADC_NTC(ADC_POSITIVE_INPUT_PIN18);
	mGateway.uEMS.uModule.uADC.NTC_No_07 = getADC_NTC(ADC_POSITIVE_INPUT_PIN19);
	sprintf(str, "\r --> [ADC] NTC = %04d, %04d, %04d, %04d, %04d, %04d, %04d",
	mGateway.uEMS.uModule.uADC.NTC_No_01,
	mGateway.uEMS.uModule.uADC.NTC_No_02,
	mGateway.uEMS.uModule.uADC.NTC_No_03,
	mGateway.uEMS.uModule.uADC.NTC_No_04,
	mGateway.uEMS.uModule.uADC.NTC_No_05,
	mGateway.uEMS.uModule.uADC.NTC_No_06,
	mGateway.uEMS.uModule.uADC.NTC_No_07);
	uart_str_COM(Debug_COM, str);
	wdt_reset_count();
}