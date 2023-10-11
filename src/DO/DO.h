unsigned int ADC_Raw_01;
unsigned int ADC_Raw_02;
unsigned int ADC_Raw_03;
unsigned int ADC_Raw_04;
unsigned int ADC_Raw_05;
unsigned int ADC_Raw_06;

unsigned int ADC_Temp_01;
unsigned int ADC_Temp_02;
unsigned int ADC_Temp_03;
unsigned int ADC_Temp_04;
unsigned int ADC_Temp_05;
unsigned int ADC_Temp_06;

#define LED_G_ON	port_pin_set_output_level(PIN_PA23, false);
#define LED_G_OFF	port_pin_set_output_level(PIN_PA23, true);
#define LED_R_ON	port_pin_set_output_level(PIN_PA27, false);
#define LED_R_OFF	port_pin_set_output_level(PIN_PA27, true);
#define LED_B_ON	port_pin_set_output_level(PIN_PA28, false);
#define LED_B_OFF	port_pin_set_output_level(PIN_PA28, true);
#define Version  101

void IO_init(void);
void Print_FW_ID(void);
void LORA01_inital(void);
void LORA02_inital(void);
void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA);
//void LORA02_MAIN_FUNCTION(volatile LORA_DEFINE_t *tmp_LORA);
void LORA01_Reset_Func(bool tmp_bool);
void LORA02_Reset_Func(bool tmp_bool);
void Wait_RX_IRQ_Index_Func(volatile LORA_DEFINE_t *tmp_LORA);
void Dev_Value_inital(void);
void Dev_DO_Main_Func(volatile LORA_DEFINE_t *tmp_LORA);
