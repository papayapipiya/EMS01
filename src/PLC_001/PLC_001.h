
#define LED_G_ON			port_pin_set_output_level(PIN_PA23, false);
#define LED_G_OFF			port_pin_set_output_level(PIN_PA23, true);
#define LED_R_ON			port_pin_set_output_level(PIN_PA27, false);
#define LED_R_OFF			port_pin_set_output_level(PIN_PA27, true);
#define LED_B_ON			port_pin_set_output_level(PIN_PA28, false);
#define LED_B_OFF			port_pin_set_output_level(PIN_PA28, true);
#define RS485_For_Debug_En	port_pin_set_output_level(PIN_PA22, false);
#define RS485_For_Debug_Dn	port_pin_set_output_level(PIN_PA22, true);
#define Version  101

uint8_t Define_MyRS485Tmp[16][8];
uint8_t RS485_value[16][15];
unsigned int RS485_Time_Count;

#define RS485_COM	&MyUart01
#define Debug_COM	&MyUart02
#define RS485_RX_D_Buf_Count		7
#define RS485_CMD_Index_Max			5
#define RS485_LORA_OUT_Index_Max	2
uint8_t RS485_RX_In_Buf[10];
uint8_t RS485_RX_Buf[10];
uint8_t RS485_RX_Buf_Count;
uint8_t RS485_CMD_Index;
uint16_t RS485_LORA_OUT_Index;



void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void IO_init(void);
void Print_Water_Info_Func(void);
void Print_FW_ID(void);
void LORA01_inital(void);
void LORA02_inital(void);
void LORA01_Reset_Func(bool tmp_bool);
void LORA02_Reset_Func(bool tmp_bool);
void Wait_RX_IRQ_Index_Func(volatile LORA_DEFINE_t *tmp_LORA);
void Uart_Form_RS485_callback(struct usart_module *const usart_module);
void Print_RS485_Time_Out(volatile LORA_DEFINE_t *tmp_LORA);
void Dev_Value_inital(void);
unsigned int crc_chk(unsigned char* data, unsigned char length);
void PLC_001_Main_Func(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void LORA01_MAIN_FUNCTION(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);