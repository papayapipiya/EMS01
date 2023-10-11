/*
 * io.c
 *
 * Created: 2023/10/6 下午 02:39:07
 *  Author: papay
 */ 
#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


void IO_init(void)
{
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);
	
	/* Configure LEDs as outputs, turn them off */
	
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	//	RGB LED PIN DEFINE
	port_pin_set_config(PIN_PA23, &pin_conf);
	port_pin_set_output_level(PIN_PA23, true);
	port_pin_set_config(PIN_PA27, &pin_conf);
	port_pin_set_output_level(PIN_PA27, true);
	port_pin_set_config(PIN_PA28, &pin_conf);
	port_pin_set_output_level(PIN_PA28, true);
	
	
	port_pin_set_config(PIN_PA00, &pin_conf);
	port_pin_set_output_level(PIN_PA00, true);

	
	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	pin_conf.input_pull = PORT_PIN_PULL_UP;
	
	port_pin_set_config(PIN_PA07, &pin_conf);
	port_pin_set_output_level(PIN_PA07, true);

	port_pin_set_config(PIN_PA22, &pin_conf);
	port_pin_set_output_level(PIN_PA22, true);
	
	port_pin_set_config(PIN_PA17, &pin_conf);
	port_pin_set_output_level(PIN_PA17, true);

	

}


void ems01_input_gpio(void)
{
	char str[100];
	if(mGateway.uEMS.uModule.uGPIO.timer != GPIO_time_Max) return;
	mGateway.uEMS.uModule.uGPIO.timer = 0;
	
	mGateway.uEMS.uModule.uGPIO.DI_1 = port_pin_get_input_level(PIN_PA17);
	mGateway.uEMS.uModule.uGPIO.DI_2 = port_pin_get_input_level(PIN_PA22);
	sprintf(str, "\r --> [GPIO] DI-1(PA17) = %d, DI-2(PA22) = %d", mGateway.uEMS.uModule.uGPIO.DI_1, mGateway.uEMS.uModule.uGPIO.DI_2);
	uart_str_COM(Debug_COM, str);	
}


