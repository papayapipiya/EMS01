

#define LED_G_ON	port_pin_set_output_level(PIN_PA23, false);
#define LED_G_OFF	port_pin_set_output_level(PIN_PA23, true);
#define LED_R_ON	port_pin_set_output_level(PIN_PA27, false);
#define LED_R_OFF	port_pin_set_output_level(PIN_PA27, true);
#define LED_B_ON	port_pin_set_output_level(PIN_PA28, false);
#define LED_B_OFF	port_pin_set_output_level(PIN_PA28, true);

#define sw_0_timer_Max 10
#define sw_2000_timer_Max 80

//======================
//weight
//======================
void ON_Display(void);
void OFF_Display(void);
void Flash_Display(void);
void Dot_Display(void);
unsigned char seg7_dot;
unsigned long long_to_bcd_4byte(unsigned long tmp_data);
void SET_Display_Value(unsigned long tmp_value);
void tx_byte(unsigned char tmp_data);


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

void MainFunc(void);

void Judg_Feed_Weight_Func(void);
void Save_Feed_Count_in_NVM(void);
void Load_Feed_Count_in_NVM(void);


//===========
//SHH_FeedWeight_01_HX711.H
//===========
void Read_Weight(void);
unsigned long ReadCount(void);
void Weight_Correction(unsigned long tmp_data);

//===========
//SHH_FeedWeight_01_Print_Txt.H
//===========
void Print_Show_weight(void);
void Print_Showw_weight(void);

//===========
// Feed TimeOut Value
//===========
unsigned int Input_Feed_Timeout_Count;
#define Input_Feed_Timeout_Max	11720	//約600秒
#define Input_Feed_Timeout_Max	5860	//約300秒
//#define Input_Feed_Timeout_Max	586	//約30秒
//#define Input_Feed_Timeout_Max	200	//約30秒

#define Y1_ON	port_pin_set_output_level(PIN_PA05, true);
#define Y1_OFF	port_pin_set_output_level(PIN_PA05, false);
#define Y2_ON	port_pin_set_output_level(PIN_PA04, true);
#define Y2_OFF	port_pin_set_output_level(PIN_PA04, false);
#define Y3_ON	port_pin_set_output_level(PIN_PA00, true);
#define Y3_OFF	port_pin_set_output_level(PIN_PA00, false);
#define Y4_ON	port_pin_set_output_level(PIN_PA01, true);
#define Y4_OFF	port_pin_set_output_level(PIN_PA01, false);

#define Enable_OUT		Y1_ON
#define Disable_OUT		Y1_OFF
#define Enable_IN		Y2_ON
#define Disable_IN		Y2_OFF
#define PRED_ON			Y3_ON
#define PRED_OFF		Y3_OFF
#define PGREEN_ON		Y4_ON
#define PGREEN_OFF		Y4_OFF


#define X1		port_pin_get_input_level(PIN_PA24)
#define X2		port_pin_get_input_level(PIN_PA25)


UINT32u_t SaveFeedWeight;
UINT32u_t NowFeedWeight;
UINT32u_t SaveACCFeedWeight;
UINT32u_t ACCFeedWeight;
UINT32u_t Last_FeedWeight;
unsigned long tmp_w;
bool PowerUP;
unsigned char W_count;
unsigned char W_NowStatus;
unsigned char System_Status;
unsigned char Switch_Status;
#define Max_Feeding_Value		500000		//錶頭最大50000.0公斤
#define Low_Feeding_Value		20			//低標重量2.0公斤
//#define Low_Feeding_Value		250			//低標重量2.0公斤
#define High_Feeding_Value		490			//高標重量49.0公斤
//#define High_Feeding_Value	300			//高標重量49.0公斤
#define Feeding_Value_Limit		5			//判斷飼料增加還是減少的不感帶重量0.5公斤
#define Change_Feeding_Value	5			//取完標準差後的不感帶重量0.5公斤
#define unit					100			//1000 / 100 = 1.0公斤


typedef enum
{
	//	Judg_Feed_Weight_Func
	IsPowerON									= 1,		
	IsJudg										= 2,			
	IsBigJudg									= 3,
	IsBig										= 4,
	IsSmallJudg									= 5,
	IsSmall										= 6,
	//	Read_Weight
	Wegight_Catch_Index							= 117,
	Wegight_Cal_Index_01						= 118,
	Wegight_Cal_Index_02						= 119,
	Wegight_Judg_Index							= 120,
	Wegight_Display_Index						= 121,
	Wegight_Display_Index_01					= 122,
	Wegight_Display_Index_02					= 123,
	Wegight_Catch_Index_01						= 124,
	Wegight_Catch_Index_02						= 125,
	// System_Rundown
	System_Waitting								= 200,
	Input_Feed									= 201,
	Input_Feed_TimeOut							= 202,
	Disable_Out									= 203,
	Enable_Out									= 204,
	Waitting_Feed_Overflow						= 205,
	Error										= 206,
	X1_STEP_01									= 207,
	X1_STEP_02									= 208,
	X1_STEP_03									= 209,
	X1_STEP_04									= 210,
}feedw_rundown;


unsigned char dynamic_weight_level;
unsigned char dynamic_count;
#define dynamic_value_max 6
#define dynamic_std_number 5
unsigned long dynamic_value[dynamic_value_max];
unsigned long dynamic_avg;
unsigned long dynamic_sum_temp;
unsigned long dynamic_sum_temp_02;
unsigned long dynamic_std;
unsigned long dynamic_std_max;
unsigned long dynamic_std_min;
#define FeedW_Count 30

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

unsigned int sw_0_timer;
unsigned int sw_2000_timer;

unsigned char LORAPollingCount;
unsigned int SEG7_TimeOut_Count;

unsigned int Weight_Change[6];
unsigned char Weight_Change_Value_Count;

unsigned char Weight_Display_Count;
unsigned char SYSTEM_RESET_FLAG;

#define Weight_Change_Value_Max	5
#define Weight_Display_Count_Max	6
#define Version  107
#define Weight_Limit  20

unsigned int WeightCount;



//===============================
//SHH_FeedWeight_01_Main
//===============================
unsigned char debounce_count;
#define  debounce_count_max 10