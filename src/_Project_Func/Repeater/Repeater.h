
#define Version		105
#define LED_B_ON	port_pin_set_output_level(PIN_PA23, false);
#define LED_B_OFF	port_pin_set_output_level(PIN_PA23, true);
#define LED_G_ON	port_pin_set_output_level(PIN_PA27, false);
#define LED_G_OFF	port_pin_set_output_level(PIN_PA27, true);
#define LED_R_ON	port_pin_set_output_level(PIN_PA28, false);
#define LED_R_OFF	port_pin_set_output_level(PIN_PA28, true);

#define RS485_For_Debug_En	port_pin_set_output_level(PIN_PA22, false);
#define RS485_For_Debug_Dn	port_pin_set_output_level(PIN_PA22, true);

volatile uint8_t Define_MyRS485Tmp[2][8];
volatile uint8_t RS485_value[16][15];
unsigned int RS485_Time_Count;
unsigned int RS485_WR_Index;
bool PLC_is_Alive;
unsigned int PLC_is_Alive_Count;
#define PLC_is_Alive_Count_Max 300

unsigned char Form_CMD_RS485_Flag;
unsigned int Form_CMD_Addr;
unsigned int Form_CMD_Val;
unsigned char Form_CMD_Addr_And_Val_Is_3rd_Flag;

#define RS485_COM	&MyUart01
//#define RS485_RX_D_Buf_Count		7
#define RS485_RX_D_Buf_Count		19
#define RS485_CMD_Index_Max			5
#define RS485_LORA_OUT_Index_Max	2
uint8_t RS485_RX_In_Buf[40];
uint8_t RS485_RX_Buf[40];
uint8_t RS485_RX_Buf_Count;
uint8_t RS485_CMD_Index;
uint16_t RS485_LORA_OUT_Index;

#define RS485_RD	0
uint16_t RS485_RD_Index;
#define RS485_WD	1

//unsigned int RS485_Time_Count;

unsigned char PLC_SENSOR_SELECT_FLAG;




void IO_init(void);
void Print_FW_ID(void);
void LORA01_inital(void);
void LORA02_inital(void);
void Response_LORA01(volatile LORA_DEFINE_t *tmp_LORA);
void Search_Dev_List_index(volatile LORA_DEFINE_t *tmp_LORA);
void Get_Dev_Data(volatile LORA_DEFINE_t *tmp_LORA);

void Response_LORA02(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void Search_Modbus_Data_index(volatile LORA_DEFINE_t *tmp_LORA);

void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void LORA02_MAIN_FUNCTION(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void Search_UpLoad_Data_index(volatile LORA_DEFINE_t *tmp_LORA);

void LORA01_Reset_Func(bool tmp_bool);
void LORA02_Reset_Func(bool tmp_bool);
void Wait_RX_IRQ_Index_Func(volatile LORA_DEFINE_t *tmp_LORA);
void Dev_Value_inital(void);
void LORA01_MAIN_FUNCTION(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);

void Uart_Form_RS485_callback(struct usart_module *const usart_module);
//void RS485_Main_Function(void);
void RS485_Main_Function(void);
//void RS485_Main_Function(unsigned char tmp_CMD_Index, struct usart_module *const usart_module)
unsigned int crc_chk(unsigned char* data, unsigned char length);
//void Print_PLC_D_Buf(void);
void Form_CMD_RS485_Function(volatile LORA_DEFINE_t *tmp_LORA);
//void RS485_Uart_Function(unsigned char tmp_CMD_Index, unsigned char debug_flag);
void RS485_Uart_Function(unsigned char tmp_CMD_Index, bool debug_flag);
//void RS485_Uart_Function(unsigned char tmp_CMD_Index);
void Print_One_Form_Write_PLC_List_Table(volatile Write_PLC_List_t *tmp_Write_PLC_List_Index, unsigned int tmp_index);
void Polling_RS485(void);
void LORA02_Fill_UpLoad_Function(volatile LORA_DEFINE_t *tmp_LORA);
void Print_LoRA02_Upload_Msg(volatile LORA_DEFINE_t *tmp_LORA);
void LoRA2_Send_PLC_Data_Function(volatile LORA_DEFINE_t *tmp_LORA);
void Uart_Form_Dust_callback(struct usart_module *const usart_module);

void PC_CMD(void);
void Write_Data_Into_PLC_D_Register(void);
void Read_Data_Form_PLC_D_Register(void);
void Print_PLC_D_Register_Number(unsigned int tmpIndex);
void Print_Will_Send_Data_To_PLC_Function(unsigned char tmp_CMD_Index);
void Print_Data_Buf_Function(unsigned char *tmp_ptr, unsigned char tmp_count);
void Read_Data_Form_PLC_D_Register(void);
bool RW_PLC_D_Buf_Function(unsigned int tmp_Addr, unsigned char tmp_CMD_Index, bool debug_flag);
unsigned int Index_To_Addr(unsigned int tmpIndex);
#define PLC_D_Register_Offset			0x1770	// 0x1770 = D000   (6000)
#define PLC_D_Register_Number_Offset	000		// 
uint8_t MyRS485RxCount[2];
#define TimeOutCount	1


unsigned char Modbus_Status;
typedef enum{
	Modbus_S1 =	1,
	Modbus_S2 =	2,
	Modbus_S3 =	3,
	Modbus_S4 =	4,
	Modbus_S5 =	5,
	Modbus_S6 =	6,
	Modbus_S7 =	7,
}Modbus_rundown;
bool Read_D_Buf_With_Addr(unsigned int tmp_index);
void Read_D_and_RAM_Function(void);
void Write_Data_Into_PLC_D_Register(void);