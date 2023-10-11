
#include <math.h>
#include <asf.h>
#include <stdio.h>
#include <string.h>

#include "Val_Type.h"
#include "__Select_Dev.h"




//#include "UserKeyIn.h"
//#include "cdc_uart.h"
//#define Dev_DO				---

bool user_switch;


//===============================
//	NVM
//===============================
#define Not_found_Index					0xff
#define Dev_Item_MAX					1

#define NVM_Write_PLC_List_MAX			2432
#define Write_PLC_List_Item_Max			4
#define NVM_Write_PLC_List_Page_MAX		38

/*
#define NVM_Dev_List_MAX				1536
#define Dev_List_Data_Item_Max			6
#define NVM_Dev_List_Use_Page_MAX		24
*/

#define NVM_Dev_List_MAX				640
#define Dev_List_Data_Item_Max			6
#define NVM_Dev_List_Use_Page_MAX		10

/*
	The Dev Time is 2-byte in the NVM as follows:
	Dev_Time.byte[1], Dev_Time.byte[0]
	The Page size of NVM is 64-byte, so the memory size of Dev Time must be equal that.
	And so Dev_Item_MAX * Dev_Time_Data_Item_Max = 100 * 2 = 200 Byte.
	The Page size of NVM has be 4-page that is 4 * 64 = 256 byte.
*/
#define NVM_Dev_Time_MAX				256
#define Dev_Time_Data_Item_Max			2
#define NVM_List_Time_Use_Page_MAX		4



#define	Dev_Dis	0
#define	Dev_EN	1




//! [rx_buffer_var]
#define MAX_RX_BUFFER_LENGTH   1

unsigned int	CH_Formula[4];
unsigned long	CH_Formula_base[4];

uint8_t Define_MyLoRaTmpCount[16];
uint8_t Define_MyLoRaTmp[16][27];

//#define dev_value_item_max			Dev_Item_MAX
#define dev_value_data_max				37

#define dev_type						7
#define dev_value_missnode				30
#define dev_value_Time_High				31
#define dev_value_Time_Low				32
#define dev_value_Count_High			33
#define dev_value_Count_Low				34
#define dev_value_New_Data_Input		35
#define dev_value_UpLoad				36
uint8_t dev_value[Dev_Item_MAX][dev_value_data_max];


unsigned int Alarm_PowerON_Counter;
#define Alarm_PowerON_Counter_Max	10			//	10秒

//#define Alarm_Main_Counter_Max	1173		//	1秒
#define Alarm_Main_Counter_Max		200		//	1秒

unsigned int sleep_count;
//#define sleep_count_max			200		//	10秒
#define sleep_count_max				100		//	5秒

typedef struct Write_PLC_List
{
	UINT16u_t MAC_ID_Index;
	unsigned char Channel_Index;
	unsigned char Channel_En;
	unsigned char Channel_Count;
}Write_PLC_List_t;

// 寫入PLC最大上限 200 個word
#define Write_PLC_List_Max 1
//#define Write_PLC_List_Max 70
Write_PLC_List_t Write_PLC_List_In_RAM[Write_PLC_List_Max];


#define LORA_Dev_List_Max Dev_Item_MAX
#define MISS_NODE_Count_Max				10

typedef struct LORA_Dev_List
{
	unsigned char		Dev_Number;
	unsigned char		Dev_En_Flag;
	UINT32u_t			MAC_ID;
	UINT16u_t			Dev_Time;
	UINT16u_t			Target_Counter;
	UINT16u_t			Now_Counter;
    unsigned char		MISS_NODE_Count;
    unsigned char		MISS_NODE;
    unsigned char		Input_New_Data;
    unsigned int		RSSI_Status;
    unsigned int		RSSI;
	unsigned char		cmd_buf[18];
	unsigned char		cmd_buf_Sent_Flag;
	bool				cmd_buf_Return_OK_Flag;
	bool				cmd_buf_to_clear_mysql_Flag;
}LORA_Dev_List_t;
LORA_Dev_List_t LORA_Dev_List_In_RAM[LORA_Dev_List_Max];
#define dev_ptr	LORA_Dev_List_In_RAM

#define	cmd_buf_Sent_Null		0
#define	cmd_buf_Sent_New		1
#define	cmd_buf_Sent_Wait_OK	2
#define	cmd_buf_Sent_ok			3

// 讀取PLC最大上限 602 個word
//#define PLC_D_Buf_Max	70
#define PLC_D_Buf_Max 12
#define PLC_D_Buf_Print_Columns_Max		3
#define PLC_D_Buf_Page					PLC_D_Buf_Max / PLC_D_Buf_Print_Columns_Max

typedef struct PLC_Dev_List
{
	UINT16u_t DataBuf;
	UINT16u_t LastDataBuf;
	unsigned int Chage_Flag_Index;
}PLC_Dev_List_t;

unsigned char PLC_D_Check_Equel_Flag[PLC_D_Buf_Page];
PLC_Dev_List_t  PLC_D_Buf[PLC_D_Buf_Max];


bool PLC_Show_w_Flag;
bool PLC_Show_r_Flag;
bool Show_weight_Flag;
	
uint8_t Dust_buffer[1];
uint8_t Dust_Data[10];
uint8_t Dust_RX_Count;
unsigned int Dust_Time_Count;
#define Dust_TimeOut_Max 5
#define Dust_RX_Count_Max 6

uint8_t Sensor_Module_buffer[1];
uint8_t Sensor_Module_Data[25];
uint8_t Sensor_Module_RX_Count;
unsigned int Sensor_Module_Time_Count;
#define Sensor_Module_TimeOut_Max 5
#define Sensor_Module_RX_Count_Max 24


unsigned int ADC_PM25_Temp;
unsigned int ADC_PM25_Raw;

typedef struct LORA_DEFINE
{
	uint8_t MyLoRaFW;		// LORA 版本
	uint8_t MyLoRaID[4];	// LORA ID
	uint8_t UserLoRaID[4];	// 自訂LORA ID
	uint8_t LoRaID[4];		// 原本LORA ID
	uint8_t MyLoRaMode;		// LORA MODE (TX, RX)
	uint8_t MyLoRaPower;	// LORA 發射功率
	uint8_t MyLoRaSF;		// LORA SF ( 6 ~ 12 )
	uint8_t PLC_Flag;		// PLC ON / OFF
	//volatile int sensor_offet;
	uint8_t PLC_Addr;		// PLC 站號
	UINT32u_t MyLoRaFQ;		// LORA 頻率
	
	struct usart_module *DevUART;		// MCU UART位置
	uint8_t rx_buffer[MAX_RX_BUFFER_LENGTH];	// MCU UART RX暫存位置
	//uint8_t rx_buffer[50];	// MCU UART RX暫存位置
	
	volatile unsigned char UserRxCountMax;	//	希望下次LORA --> MCU，UART RX要收的長度
	volatile unsigned char MyBufferCount;	//	實際LORA --> MCU，UART RX的長度
	volatile unsigned char MyBuffer[30];
	volatile unsigned char MyData[30];
	
	volatile unsigned char List_Now;
	volatile unsigned char List_Next;
	
	volatile unsigned char MyLoRaTmpCount[16];	// LORA UART FORMAT長度
	volatile unsigned char MyLoRaTmp[16][27];	// LORA UART DEFINE FORMAT
	
	volatile unsigned char NowStatus;
	volatile unsigned char LastStatus;
	volatile unsigned char NextStatus;
	
	volatile unsigned char Lora_Debug_Flag;		// 顯示debug mode，主要顯示MCU <--> LORA訊息
	volatile unsigned char Lora_initial_Flag;	// LORA設定完成旗標
	volatile unsigned char Lora_CRC_Flag;		// 檢查LORA --> MCU的CRC
	volatile unsigned char LoRa_IRQ_Flag;		// LORA TX, RX完成後，會送給MCU IRQ的EXTINT，判斷FLAG是否有IRQ產生
	volatile unsigned char LoRa_Rx_Flag;			// 判斷RX是否有資料LoraRxDataReady與LoraRxNoData
	volatile unsigned char LoRa_Send_Tx_Flag;
	volatile unsigned char Lora_Show_Flag;		// 顯示資料串流
	volatile unsigned char Lora_Dev_Count_IRQ_Flag;
	volatile unsigned char LoRa_Tx_Send_Time_Out_Count;
	volatile unsigned char LoRa_CRC_Count;
	
	volatile unsigned char LoRa_TxRx_Mode_Flag;
	int RSSIValue;
	int Lora_RX_Count_Form_Module;
	
	//volatile unsigned int Lora_Delay_Count;		//	MCU --> LORA CMD後，延遲多久再傳下一筆資料
	volatile unsigned int Lora_TimeOut_Count;	//	LORA一直沒有回傳資料，MCU TIMEOOUT時間
	
	unsigned char NVM_offset_Value;
	
	volatile unsigned int f_count;				//	TIMER用的COUNT
	volatile unsigned int t2_tmp_count;
	volatile unsigned int Sensor_Count;
	volatile unsigned int Power_On_Delay;
	
	volatile bool Show_IIC;		// 顯示資料串流
	volatile bool Show_Sensor_Raw;		// 顯示資料串流
	
}LORA_DEFINE_t;

#define Sensor_Count_Max 20
#define Sensor_Count_SD_Max 2
typedef struct SENSOR_DEFINE
{
	volatile unsigned int	MyData[Sensor_Count_Max];
	volatile unsigned char	MyDataCount;
	volatile unsigned int	MyDataAVG;
	volatile unsigned int	MyDataAVG_tmp;
	volatile unsigned long	MyDataSD;
	volatile unsigned long	MyAVGP;
	volatile unsigned long	MyAVGN;
	volatile unsigned int	MyUpdate_Max;
	bool					MyDataOK;
	volatile unsigned int	ADC_Raw;
	volatile unsigned int	ADC_Raw_tmp;
	volatile unsigned int	ADC_Raw_OK;
}SENSOR_DEFINE_t;

LORA_DEFINE_t NewLORA01;
LORA_DEFINE_t NewLORA02;
volatile LORA_DEFINE_t *LORA01;
volatile LORA_DEFINE_t *LORA02;
SENSOR_DEFINE_t NewCH1;
SENSOR_DEFINE_t NewCH2;
volatile SENSOR_DEFINE_t *CH1;
volatile SENSOR_DEFINE_t *CH2;

#include "_MCU\IIC\configure_i2c_master.h"
#include "_Wifi\Base_Func\Wifi_inital.h"


typedef struct uADC_Dev{
	unsigned int NTC_No_01;
	unsigned int NTC_No_02;
	unsigned int NTC_No_03;
	unsigned int NTC_No_04;
	unsigned int NTC_No_05;
	unsigned int NTC_No_06;
	unsigned int NTC_No_07;
	#define ADC_time_Max	5000
	unsigned int timer;
}uADC_Dev_t;

typedef struct uGPIO_Dev{
	bool DI_1;
	bool DI_2;
	#define GPIO_time_Max	5000
	unsigned int timer;
}uGPIO_Dev_t;


#define ID_Max	16
typedef struct uID_item_Dev{
	unsigned char	Buffer[ID_Max];
	unsigned char	userKeyin_Buffer[ID_Max];
	unsigned long	Value;
	unsigned long	tmpValue;
	bool			f_error;
	int	ret;
	int	ret_end;
}uID_item_Dev_t;


typedef struct uModule_Dev{
	//BMS_CAN_Dev_t CAN;
	Item_Dev_t	*eID;
	uID_item_Dev_t uID;
	uID_item_Dev_t uPassword;
	uADC_Dev_t	uADC;
	uGPIO_Dev_t	uGPIO;
}uModule_Dev_t;

#define Setting_Max	16
typedef struct uSetting_item_Dev{
	unsigned char	Buffer[Setting_Max];
	unsigned char	userKeyin_Buffer[Setting_Max];
	unsigned int	Value;
	unsigned int	tmpValue;
	bool			f_error;
	int	ret;
	int	ret_end;
}uSetting_item_Dev_t;


typedef struct uSetting_Dev{
	
	Item_Dev_t	*eOT;
	Item_Dev_t	*eUT;
	Item_Dev_t	*ewOT;
	Item_Dev_t	*ewUT;
	Item_Dev_t	*eTolerance;
	
	
	
	uSetting_item_Dev_t	OT;
	uSetting_item_Dev_t	UT;
	uSetting_item_Dev_t	wOT;
	uSetting_item_Dev_t	wUT;
	uSetting_item_Dev_t	Tolerance;

}uSetting_Dev_t;



typedef struct BMS_Dev{
	uModule_Dev_t	uModule;
	uSetting_Dev_t	uSetting;
}EMS_Dev_t;

typedef struct Gateway
{
	LORA_DEFINE_t		*LoRA01;
	LORA_DEFINE_t		*LoRA02;
	LORA_Dev_List_t		*LORA_Dev_List_Index;
	WiFi_t		*Wifi;
	volatile EMS_Dev_t	uEMS;
}Gateway_t;
volatile Gateway_t mGateway;









unsigned char RESET_COUNT;

unsigned int ADC_LVD_Temp;
unsigned int ADC_LVD_Raw;


void lcd_4bit(unsigned char rs, unsigned char tmp);
void lcd_command(unsigned char rs, unsigned char tmp);
void lcd_init(void);
/*
#define LED_G_ON	port_pin_set_output_level(PIN_PA23, false);
#define LED_G_OFF	port_pin_set_output_level(PIN_PA23, true);
#define LED_R_ON	port_pin_set_output_level(PIN_PA27, false);
#define LED_R_OFF	port_pin_set_output_level(PIN_PA27, true);
#define LED_B_ON	port_pin_set_output_level(PIN_PA28, false);
#define LED_B_OFF	port_pin_set_output_level(PIN_PA28, true);
*/

#define RY_OFF	port_pin_set_output_level(PIN_PA00, true);
#define RY_ON	port_pin_set_output_level(PIN_PA00, false);

#define LCM_RS_High	port_pin_set_output_level(PIN_PA03, true);
#define LCM_RS_Low	port_pin_set_output_level(PIN_PA03, false);
#define LCM_RW_High	port_pin_set_output_level(PIN_PA02, true);
#define LCM_RW_Low	port_pin_set_output_level(PIN_PA02, false);
#define LCM_EN_High	port_pin_set_output_level(PIN_PA08, true);
#define LCM_EN_Low	port_pin_set_output_level(PIN_PA08, false);

#define LCM_04_High	port_pin_set_output_level(PIN_PA04, true);
#define LCM_04_Low	port_pin_set_output_level(PIN_PA04, false);
#define LCM_05_High	port_pin_set_output_level(PIN_PA05, true);
#define LCM_05_Low	port_pin_set_output_level(PIN_PA05, false);
#define LCM_06_High	port_pin_set_output_level(PIN_PA06, true);
#define LCM_06_Low	port_pin_set_output_level(PIN_PA06, false);
#define LCM_07_High	port_pin_set_output_level(PIN_PA07, true);
#define LCM_07_Low	port_pin_set_output_level(PIN_PA07, false);

#define RS485_EN_High	port_pin_set_output_level(PIN_PA16, true);
#define RS485_EN_Low	port_pin_set_output_level(PIN_PA16, false);

//unsigned char TEST_CHAR;

#define lcd_instr	0
#define lcd_data	1
#define lcd_write	0
#define lcd_read	1




//! [result_buffer]





#include "_MCU\UART\UART_initial.h"
#include "_MCU\UART\UART_Func.h"
#include "_MCU\UART\callback\LORA_callback.h"
#include "_MCU\UART\callback\Dust_callback.h"
#include "_MCU\UART\callback\Sensor_Module_callback.h"
#include "_MCU\UART\callback\DeBug_callback.h"
#include "_MCU\UART\callback\WIFI_callback.h"
#include "_MCU\UART\text\UART_LoRA_Msg.h"
#include "_MCU\UART\text\UART_Msg.h"
#include "_MCU\TC\configure_tc.h"



#include "_LoRa\LORA01_MainFunc.h"

//uint8_t MyBuffer_DEV[30];








unsigned int debug_cycle_time;
#define debug_cycle_time_Max 2


#define Timer_Timeout					20000
#define RS485_Timeout					30





#define NoDebug		0
#define NeedDebug	1

#define Dev_Miss_Node	0
#define Dev_Found_Node	1

#define Dev_Not_New_Data	0
#define Dev_Have_New_Data	1

#define Dev_Uploaded	0
#define Dev_Wait_Upload	1

#define NoPLC		0
#define NeedPLC		1

//===============================
//	EXT IRQ BY LORA IRQ DEFINE
//===============================
void configure_extint_channel(void);
void configure_extint_callbacks(void);
void LORA01_extint_detection_callback(void);
void LORA01_ExtIRQ_callback(void);
void LORA02_extint_detection_callback(void);

typedef enum
{
	NowOPEN,		//	0
	Step00,		   	//	1
	Step01,		   	//	2
	Step02,			//	3
	Step03,			//	4
	Step04,			//	5
	Step05,			//	6
	Step06,			//	7
	Step07,			//	7
	Step08,			//	7
	Step09,			//	7
	Step10,			//	7
	Step11,			//	7
	Step12,			//	7
}rundown;



#define CRCisOK		0
#define CRCisErr	1

//===============================
//	後門程式
//===============================	
#define UserKeyInBufMax 100
uint8_t user_rx_buffer[MAX_RX_BUFFER_LENGTH];
uint8_t UserKeyInBuf[UserKeyInBufMax+4];
uint8_t UserKeyInBufCount;
uint8_t UserKeyInStauts;



unsigned char Dev_Buf_Write_Index;
#define WriteBufferMax 38
uint8_t WriteBuffer[WriteBufferMax+4];
uint8_t WriteBufferHex[WriteBufferMax+4];
uint8_t PC_WriteBuffer[WriteBufferMax+4];
uint8_t PC_WriteBufferHex[WriteBufferMax+4];

typedef enum
{
	NOP,
	UserKeyInEnterIndex,
	PrintListIndex,
}UserKeyInTable;				



//===============================
//	WDT
//===============================
void configure_wdt(void);
void configure_wdt_callbacks(void);
void watchdog_early_warning_callback(void);				
									




//===============================
//	RSSI
//===============================
void Catch_Lora_RX_Count_Form_Module(volatile LORA_DEFINE_t *tmp_LORA);


#define Dev_No_Index	0
#define Dev_En_Index	1
enum status_code error_code;
void configure_nvm(void);
void Save_List_in_NVM(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void Load_List_Form_NVM(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);



unsigned char Search_list_table_index(unsigned char tmp_list_table_index, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);

void Load_LoRa_Profile_in_NVM(volatile LORA_DEFINE_t *tmp_LORA);
void Save_LoRa_Profile_in_NVM(volatile LORA_DEFINE_t *tmp_LORA);
void Save_Write_PLC_List_in_NVM(volatile Write_PLC_List_t *tmp_Write_PLC_List_Index);
void Load_Write_PLC_List_in_NVM(volatile Write_PLC_List_t *tmp_Write_PLC_List_Index);
void Load_Dev_Time_Form_NVM(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void Save_Dev_Time_into_NVM(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void WaitRxData_Func(volatile LORA_DEFINE_t *tmp_LORA);


//===============================
//	User_Key_In_Msg
//===============================


void uart_send_word_for_ListNumber(struct usart_module *const usart_module,unsigned int tmp);
void Print_Write_Buffer(void);
unsigned char CMD_Index;
void CMD_Func(void);
unsigned int Cal_list_table_index(unsigned char tmpA, unsigned char tmpB, unsigned char tmpC);
void Set_Lora_Power_Func(volatile LORA_DEFINE_t *tmp_LORA);
void Debug_Mode_Func(volatile LORA_DEFINE_t *tmp_LORA);
void CMD_Save_Func(volatile LORA_DEFINE_t *tmp_LORA);
void Set_Lora_FQ_Func(volatile LORA_DEFINE_t *tmp_LORA);
void Set_Lora_User_ID_Func(volatile LORA_DEFINE_t *tmp_LORA);
void Set_Lora_SF_Func(volatile LORA_DEFINE_t *tmp_LORA);
void Print_Lora_FW_ID_Func(volatile LORA_DEFINE_t *tmp_LORA);
void Print_RSSI_Func(volatile LORA_DEFINE_t *tmp_LORA);

//=============================================
//	User Key In Command
//=============================================
void Judg_KEY_S(void);
void Judg_KEY_P(void);
void Judg_KEY_L(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void Judg_KEY_D(void);
void Judg_KEY_E(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void Judg_KEY_ADD(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void Judg_KEY_DL(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void Judg_KEY_SHOW(void);
void Judg_KEY_PSHOW(void);
void Judg_KEY_T(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void Judg_KEY_TS(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void Judg_KEY_CH(void);
void Judg_KEY_W(void);
void Judg_KEY_PDL(void);
void Judg_KEY_M(void);
void Print_mem_list(void);
void Judg_KEY_HDL(void);
void Judg_KEY_HELP(void);
void Judg_KEY_C(void);

void Uart_Form_WIFI_callback(struct usart_module *const usart_module);

typedef enum
{
	CMD_NOP_Index,
	Debug_Mode_Index,
	Print_Lora_FW_ID_Index,
	Print_List_Table_Index,
	Print_RSSI_Index,
	Set_Lora_SF_Index,
	Set_Lora_Power_Index,
	Set_Lora_FQ_Index,
	CMD_Save_Index,
	Debug_Mode_Index_LORA01,
	Debug_Mode_Index_LORA02,
	Set_Lora_User_ID_Index,
	Write_Data_Form_UART_Index
}CMD_TABLE;

//===============================
//	Data Conv
//===============================
void Define_Profile_COPY_TO_RAM(void);
void Find_Now_And_Next_Enable_Func(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void Find_Now_And_Next_Wait_Upload_Func(volatile LORA_DEFINE_t *tmp_LORA);
void Catch_Data_Form_RX_Func(volatile LORA_DEFINE_t *tmp_LORA);
void Dev_Time_Counter_By_Timer_Func(unsigned char tmp_start, unsigned char tmp_end);
void Fill_Dev_Time_into_dev_value(volatile LORA_DEFINE_t *tmp_LORA);
void Define_Dev_Time_Func(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void Define_Dev_List_Func(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);


void change_and_check(volatile unsigned char *tmp_LORA, unsigned char tmp_status);
void Reset_LoRa_Func(volatile LORA_DEFINE_t *tmp_LORA);

void LoRA_Timeout_Func(volatile LORA_DEFINE_t *tmp_LORA);
void Run_LoRA_Function(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index, volatile Write_PLC_List_t *tmp_Write_PLC_List_Index);








volatile unsigned int test_counter;


#define IIC_Dev
#include "_MCU\GPIO\io.h"
#include "_MCU\ADC\ADC.h"
#include "_MCU\ADC\ADC_NTC.h"
#include "_Project_Func\Master\Master_RS232.h"
#include "_Project_Func\Sensor\IIC_Dev.h"
#include "_KeyIn\Key_Wifi_APMode.h"

//===============================
//	WIFI
//===============================
//=================================================
#ifdef Dev_Wifi
//=================================================


#include "_Wifi\Base_Func\Wifi_String_Func.h"
#include "_Wifi\Base_Func\Check_Got_String_Func.h"
#include "_Wifi\Base_Func\Set_Send_CMD.h"
#include "_Wifi\Base_Func\Wifi_Func.h"
#include "_Wifi\Base_Func\MCU_Data_To_Get_Str.h"
#include "_Wifi\Base_Func\Set_Send_CMD_Define.h"

#include "_Wifi\STA_Mode\STA_MainFun.h"

#include "_Wifi\AP_Mode\findKeyword.h"
#include "_Wifi\AP_Mode\Get_Netlist_KeyWord.h"
#include "_Wifi\AP_Mode\Ping_internert_Func.h"

#include "_Wifi\AP_Mode\AP_Mode_MainFun.h"
#include "_Wifi\AP_Mode\AP_HTML_Func.h"
#include "_Wifi\AP_Mode\NVM_SSID_Func.h"
#include "_Wifi\AP_Mode\NVM_PASS_Func.h"
#include "_Wifi\AP_Mode\NVM_EMAIL_Func.h"
#include "_Wifi\AP_Mode\Send_AT_CWLAPOPT.h"
#include "_Wifi\AP_Mode\Send_AT_CWLAP.h"



#include "eeprom\eID.h"
#include "eeprom\eeprom.h"
#include "eeprom\eSSID.h"
#include "eeprom\ePW.h"
#include "eeprom\OT_wOT.h"
#include "eeprom\UT_wUT.h"
#include "eeprom\eTolerance.h"



//=================================================
#endif
//=================================================



#include "_Wifi\AP_Mode\Http_Get_Function\Get_User_KeyinWord\GetuserSSID.h"
#include "_Wifi\AP_Mode\Http_Get_Function\Get_User_KeyinWord\GetuserPW.h"
#include "_Wifi\AP_Mode\Http_Get_Function\Get_User_KeyinWord\Get_user_Setting_Value.h"
#include "_Wifi\AP_Mode\Http_Get_Function\Get_User_KeyinWord\Get_User_KeyinWord.h"
#include "_Wifi\AP_Mode\Http_Get_Function\Get_User_KeyinWord\getSettingString.h"
#include "_Wifi\AP_Mode\Http_Get_Function\Get_User_KeyinWord\getDeviceIDString.h"
