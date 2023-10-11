#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//! [setup]
void configure_adc(void)
{
	//! [setup_config]
	struct adc_config config_adc;
	//! [setup_config]
	//! [setup_config_defaults]
	adc_get_config_defaults(&config_adc);
	//! [setup_config_defaults]

	//! [setup_set_config]
	adc_init(&adc_instance, ADC, &config_adc);
	
	
	//! [setup_set_config]

	//! [setup_enable]
	adc_enable(&adc_instance);
	//! [setup_enable]
}
//! [setup]


unsigned int ADC_Res_To_Temperture_By_MCU( unsigned int tmpVal)
{
	
	//bool user_debug = true;
	//char str[50];
	long z = tmpVal;
	//long z = 700;
	long C1, C2, x, y;
	 
	 z = (z * 132144) / 1000000;
	 return z;


	C1 = 2328 *log10(z);
	C2 = (420 * log10(z) * log10(z));

	y = -1967 + C1 - C2;
	return y;
}


volatile uint16_t getADC_NTC(enum adc_positive_input adc_pin)
{
	static uint16_t result;
	adc_instance.hw->INPUTCTRL.reg &=~ ADC_INPUTCTRL_MUXPOS_Msk;
	adc_instance.hw->INPUTCTRL.reg |= (adc_pin<<ADC_INPUTCTRL_MUXPOS_Pos);

	adc_start_conversion(&adc_instance);
	do {
		/* Wait for conversion to be done and read out result */
	} while (adc_read(&adc_instance, &result) == STATUS_BUSY);
	
	result = 4095 - result;

	return ADC_Res_To_Temperture_By_MCU(result);
}

