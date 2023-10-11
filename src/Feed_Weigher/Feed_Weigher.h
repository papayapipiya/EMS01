
volatile unsigned char FW_Status;
typedef enum{
	FWStep00		=	0,
	FWStep01		=	1,
	FWStep02		=	2,
	FWStep03		=	3,
	FWStep04		=	4,
	FWStep05		=	5,
	FWStep06		=	6,
	FWStep07		=	7,
	FWStep08		=	8,
}Feed_Weigher_Rundown;
#define PowerUP					FWStep00
#define Setting					FWStep01
#define CHKOverSW				FWStep02
#define SettingCloseGATE		FWStep03
#define SettingOPENGATE			FWStep04
#define WeightMeasure			FWStep05
#define WaitSomeTime			FWStep06
#define WeightCatch				FWStep07
#define WeightNext				FWStep08


#define Set30Sec					585
#define Set03Sec					58

unsigned int OverCount;
unsigned int tmpAI2;
unsigned int tmpAI1;
bool AI2;
bool AI1;
bool Stable_flag;
unsigned long Total_Weight;

volatile unsigned char dynamic_weight_level;
volatile unsigned char dynamic_count;
#define dynamic_value_max 21
#define dynamic_std_number 20
volatile unsigned long dynamic_value[dynamic_value_max];
volatile unsigned long dynamic_avg;
volatile unsigned long dynamic_sum_temp;
volatile unsigned long dynamic_sum_temp_02;
volatile unsigned long dynamic_std;
volatile unsigned long dynamic_std_max;
volatile unsigned long dynamic_std_min;


#define LED_G_ON	port_pin_set_output_level(PIN_PA23, false);
#define LED_G_OFF	port_pin_set_output_level(PIN_PA23, true);
#define LED_R_ON	port_pin_set_output_level(PIN_PA27, false);
#define LED_R_OFF	port_pin_set_output_level(PIN_PA27, true);
#define RY1_OFF		port_pin_set_output_level(PIN_PA25, false);
#define RY1_ON		port_pin_set_output_level(PIN_PA25, true);
#define RY2_OFF		port_pin_set_output_level(PIN_PA24, false);
#define RY2_ON		port_pin_set_output_level(PIN_PA24, true);
#define MOTOR_CW	RY1_ON RY2_OFF
#define MOTOR_CCW	RY1_OFF RY2_ON
#define MOTOR_STOP	RY1_OFF RY2_OFF
#define DO1_OFF		port_pin_set_output_level(PIN_PA00, false);
#define DO1_ON		port_pin_set_output_level(PIN_PA00, true);
#define DO2_OFF		port_pin_set_output_level(PIN_PA01, false);
#define DO2_ON		port_pin_set_output_level(PIN_PA01, true);
#define Setting_SW	port_pin_get_input_level(PIN_PA28) 
/*
The LED_B is a setting pin in the feed weigher.
#define LED_B_ON	port_pin_set_output_level(PIN_PA28, false);
#define LED_B_OFF	port_pin_set_output_level(PIN_PA28, true);
*/

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
void Feed_Weigher_main_Func(void);
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
void Display_Status(void);
void Print_Weight_Change_Value_Buf_Func(void);
void Save_Total_Weight_into_NVM(void);
void Load_Total_Weight_Form_NVM(void);

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
#define Version  100