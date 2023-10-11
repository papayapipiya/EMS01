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

#define offset_value 125
unsigned long tmp_temp;
unsigned long tmp_humi;

#define LED_G_ON		port_pin_set_output_level(PIN_PA23, false);
#define LED_G_OFF		port_pin_set_output_level(PIN_PA23, true);
#define LED_R_ON		port_pin_set_output_level(PIN_PA27, false);
#define LED_R_OFF		port_pin_set_output_level(PIN_PA27, true);
#define LED_B_ON		port_pin_set_output_level(PIN_PA28, false);
#define LED_B_OFF		port_pin_set_output_level(PIN_PA28, true);
#define SHT30_RESET_ON	port_pin_set_output_level(PIN_PA16, false);
#define SHT30_RESET_OFF	port_pin_set_output_level(PIN_PA16, true);
#define RELAY_1_OFF		port_pin_set_output_level(PIN_PA05, false);
#define RELAY_1_ON		port_pin_set_output_level(PIN_PA05, true);
#define RELAY_2_OFF		port_pin_set_output_level(PIN_PA06, false);
#define RELAY_2_ON		port_pin_set_output_level(PIN_PA06, true);
#define RELAY_3_OFF		port_pin_set_output_level(PIN_PA07, false);
#define RELAY_3_ON		port_pin_set_output_level(PIN_PA07, true);
#define PWM_OFF			port_pin_set_output_level(PIN_PA00, false);
#define PWM_ON			port_pin_set_output_level(PIN_PA00, true);

#define PWM_delay_01		13	//約1.6秒
unsigned char pwm_delay_count;
bool pwm_status;

#if defined(Dev_Sensor_Module)
	#define Version  201
#elif defined(Dev_Sensor_HT_OnBoard)
	//#define Version  214	//關閉sleep模式，增加開機延遲1000 * 51.2ms
	//#define Version  215	//關閉ADC CH2，取消CH1最大值、並GAIN X 4
	#define Version  216	//gain x 1，經過標準差計算後，韌體x4
#endif

#define ETC_COM	&MyUart04

void IO_init(void);
void Print_FW_ID(void);
void LORA01_inital(void);
void LORA02_inital(void);
void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
//void LORA02_MAIN_FUNCTION(volatile LORA_DEFINE_t *tmp_LORA);
void LORA01_Reset_Func(bool tmp_bool);
void LORA02_Reset_Func(bool tmp_bool);
void Wait_RX_IRQ_Index_Func(volatile LORA_DEFINE_t *tmp_LORA);
void Dev_Value_inital(void);

void sensor_main_func(void);
unsigned int HexTomA(unsigned int tmpVal);
unsigned long GetTempVal(unsigned long tmp_temp);
unsigned long GetHumiVal(unsigned long tmp_temp, unsigned long tmp_humi);
void GetSensorModuleVal(void);
void GetSensorOnBoardVal(void);
void GetSensorSD(SENSOR_DEFINE_t *tmp_CH, unsigned int tmpData);


bool startup_flag;
int sensor_offet;
int sensor_plus_flag;
int sensor_offet2;
int sensor_plus_flag2;

void Judg_Is_My_CMD(volatile LORA_DEFINE_t *tmp_LORA);



typedef enum{
	Relay_ALL_OFF			= 0,
	Relay_FAN				= 1,
	Relay_LOAD				= 2,
	Relay_ALL_ON			= 3,
	Relay_LOAD_Wait_OFF		= 4,
	Relay_Time_Out			= 5,
}Relay_RunDown;

typedef struct uRelay
{
	unsigned char	NowStatus;
	unsigned char	Last_NowStatus;
	unsigned int	to_OFF_Count;
	unsigned int	min_Count;
}Relay_t;
Relay_t Sensor_Relay;


//=================================================
//	Sensor_CMD_01.c
//=================================================
void realtime_CMD(void);

//=================================================
//	Sensor_CMD_02.c
//=================================================
void mem_Set_CMD(void);

