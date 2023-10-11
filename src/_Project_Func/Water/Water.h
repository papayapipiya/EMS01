


#define LED_G_ON	port_pin_set_output_level(PIN_PA23, false);
#define LED_G_OFF	port_pin_set_output_level(PIN_PA23, true);
#define LED_R_ON	port_pin_set_output_level(PIN_PA27, false);
#define LED_R_OFF	port_pin_set_output_level(PIN_PA27, true);
#define LED_B_ON	port_pin_set_output_level(PIN_PA28, false);
#define LED_B_OFF	port_pin_set_output_level(PIN_PA28, true);

#define water_nvm_savetimer_count_Max 6000

unsigned char water_input_clk_flag;

UINT32u_t  water_count;
UINT32u_t  Last_water_count;


volatile unsigned int water_nvm_savetimer_count;

void Water_Detect_Func(void);
void Load_Water_Count_in_NVM(void);
void Save_Water_Count_in_NVM(void);
//void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA);
void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void IO_init(void);
void Print_Water_Info_Func(void);
void Print_FW_ID(void);

void LORA01_inital(void);
void LORA02_inital(void);
void LORA01_Reset_Func(bool tmp_bool);
void LORA02_Reset_Func(bool tmp_bool);
void Wait_RX_IRQ_Index_Func(volatile LORA_DEFINE_t *tmp_LORA);
void Dev_Value_inital(void);
#define Version  102
