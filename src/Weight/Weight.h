

#define LED_G_ON	port_pin_set_output_level(PIN_PA23, false);
#define LED_G_OFF	port_pin_set_output_level(PIN_PA23, true);
#define LED_R_ON	port_pin_set_output_level(PIN_PA27, false);
#define LED_R_OFF	port_pin_set_output_level(PIN_PA27, true);
#define LED_B_ON	port_pin_set_output_level(PIN_PA28, false);
#define LED_B_OFF	port_pin_set_output_level(PIN_PA28, true);


//======================
//weight
//======================
void ON_Display(void);
void OFF_Display(void);
void Flash_Display(void);
unsigned long long_to_bcd_4byte(unsigned long tmp_data);
void SET_Display_Value(unsigned long tmp_value);
void tx_byte(unsigned char tmp_data);
unsigned long ReadCount(void);
void Weight_Correction(unsigned long tmp_data);
void Weight_Judg(void);
void Save_Weight_Count_in_NVM(void);
void Load_Weight_Count_in_NVM(void);
void Weight_main_Func(void);
void IO_init(void);
//void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA);
void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void Print_FW_ID(void);
void LORA01_inital(void);
void LORA02_inital(void);
void LORA01_Reset_Func(bool tmp_bool);
void LORA02_Reset_Func(bool tmp_bool);
void Wait_RX_IRQ_Index_Func(volatile LORA_DEFINE_t *tmp_LORA);
void Dev_Value_inital(void);

//===============================
// Weight_Data
//===============================
unsigned long RawDataZero;
unsigned long RawDataUser;
unsigned long RawDataBase;
unsigned long RawDataTemp;
unsigned long RawDataBuffer;
unsigned long Weight_OLD_Buffer;
unsigned long Weight_Temp;
unsigned long Last_Weight_Temp;
unsigned long Weight_UpLoad_Buffer;
unsigned long Weight_Display_Buffer;
unsigned long RawDataBCD;
unsigned long RawDataCount;
unsigned char WeightStatus;
unsigned long Rawdata;

unsigned char LORAPollingCount;
unsigned int SEG7_TimeOut_Count;	

unsigned int Weight_Change[6];
unsigned char Weight_Change_Value_Count;

unsigned char Weight_Display_Count;
unsigned char SYSTEM_RESET_FLAG;

#define Weight_Change_Value_Max	5
#define Weight_Display_Count_Max	6
#define Version  105