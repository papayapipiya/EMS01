/*
 * ADC.h
 *
 * Created: 2023/9/24 下午 10:10:03
 *  Author: papay
 */ 


#ifndef ADC_H_
#define ADC_H_


void configure_adc(void);
void configure_adc_callbacks(void);
volatile uint16_t getADC_NTC(enum adc_positive_input adc_pin);
volatile struct adc_module adc_instance;

#define ADC_SAMPLES 128
volatile uint16_t adc_result_buffer[ADC_SAMPLES];


#define  ADC_USER_01	ADC_POSITIVE_INPUT_PIN0
#define  ADC_USER_02	ADC_POSITIVE_INPUT_PIN1
#define  ADC_USER_03	ADC_POSITIVE_INPUT_PIN4
#define  ADC_USER_04	ADC_POSITIVE_INPUT_PIN5
#define  ADC_USER_05	ADC_POSITIVE_INPUT_PIN6
#define  ADC_USER_06	ADC_POSITIVE_INPUT_PIN7
#define	 ADC_LVD		ADC_POSITIVE_INPUT_PIN18

#endif /* ADC_H_ */