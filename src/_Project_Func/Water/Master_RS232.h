
#define Version		104
#define LED_B_ON	port_pin_set_output_level(PIN_PA23, false);
#define LED_B_OFF	port_pin_set_output_level(PIN_PA23, true);
#define LED_G_ON	port_pin_set_output_level(PIN_PA27, false);
#define LED_G_OFF	port_pin_set_output_level(PIN_PA27, true);
#define LED_R_ON	port_pin_set_output_level(PIN_PA28, false);
#define LED_R_OFF	port_pin_set_output_level(PIN_PA28, true);

void IO_init(void);
void Print_FW_ID(void);
void LORA01_inital(void);
void LORA02_inital(void);
//void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA);
void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void Response_LORA01(volatile LORA_DEFINE_t *tmp_LORA);
void Search_Dev_List_index(volatile LORA_DEFINE_t *tmp_LORA);
void Get_Dev_Data(volatile LORA_DEFINE_t *tmp_LORA);
//void LORA02_MAIN_FUNCTION(volatile LORA_DEFINE_t *tmp_LORA);
void LORA02_MAIN_FUNCTION(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void LORA01_Reset_Func(bool tmp_bool);
void LORA02_Reset_Func(bool tmp_bool);
void Wait_RX_IRQ_Index_Func(volatile LORA_DEFINE_t *tmp_LORA);
void Dev_Value_inital(void);
void Master_RS232_To_UART(volatile LORA_DEFINE_t *tmp_LORA);
void PC_CMD(void);
void Insert_Data(unsigned int tmp_index, unsigned int tmp_Data);
void Encoder_uart_send_ascii_byte(struct usart_module *const usart_module, uint8_t tmp);
