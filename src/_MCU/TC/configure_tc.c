/*
 * configure_tc.c
 *
 * Created: 2021/4/19 下午 03:48:10
 *  Author: willchen
 */ 
#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//[definition_pwm]
/** PWM module to use */
#define PWM_MODULE      TC2
/** PWM output pin */
#define PWM_OUT_PIN     PIN_PA00F_TC2_WO0
/** PWM output pinmux */
#define PWM_OUT_MUX     MUX_PA00F_TC2_WO0
//[definition_pwm]

//! [setup]
void configure_tc(void)
{
	//! [setup_config]
	struct tc_config config_tc;
	//! [setup_config]
	//! [setup_config_defaults]
	tc_get_config_defaults(&config_tc);
	//! [setup_config_defaults]

	//! [setup_change_config]
	config_tc.clock_source    = GCLK_GENERATOR_1;
	config_tc.counter_size    = TC_COUNTER_SIZE_16BIT;
	config_tc.clock_prescaler  =TC_CLOCK_PRESCALER_DIV16;
	config_tc.wave_generation = TC_WAVE_GENERATION_MATCH_FREQ;
	config_tc.counter_16_bit.compare_capture_channel[0] = 50;
	config_tc.counter_16_bit.compare_capture_channel[1] = 50;
	
	tc_init(&MyTimer02, TC2, &config_tc);
	tc_enable(&MyTimer02);

	config_tc.clock_prescaler  =TC_CLOCK_PRESCALER_DIV1024;
	config_tc.wave_generation = TC_WAVE_GENERATION_MATCH_FREQ;
	config_tc.counter_16_bit.compare_capture_channel[0] = 50;
	config_tc.counter_16_bit.compare_capture_channel[1] = 50;
	tc_init(&MyTimer01, TC1, &config_tc);
	tc_enable(&MyTimer01);
	
}
//! [setup]
void configure_tc_callbacks(void)
{
	
	tc_register_callback(&MyTimer01, tc_callback_to_to_period_51200us, TC_CALLBACK_CC_CHANNEL0);
	tc_enable_callback(&MyTimer01, TC_CALLBACK_CC_CHANNEL0);
	
	tc_register_callback(&MyTimer02, tc_callback_to_to_period_200us, TC_CALLBACK_CC_CHANNEL1);
	tc_enable_callback(&MyTimer02, TC_CALLBACK_CC_CHANNEL1);
	
}

void timer_configure(void)
{
	configure_tc();
	configure_tc_callbacks();
}