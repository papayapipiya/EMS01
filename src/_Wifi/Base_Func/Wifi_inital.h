

//#define Testing_001_3		// 實驗室 -> Jay PHP -> google excel	(實驗室內)
//#define Testing_001_4		// qwert -> Jay PHP -> google excel	(經理家)
#define Testing_001_5		// 實驗室 -> will PHP -> mysql		(實驗室內)


char Target_IP[500];
char Target_PHP[500];
char GET_Update_tmp[2048];
char GET_Update_HTTP_Head[500];

char DeviceData_tmp[500];
char DeviceData1[500];
#define		Join_CMD			"AT+CWJAP_CUR=\"dlink-C592\",\"yotbh66768\"\r\n"
//	#define		Join_CMD			"AT+CWJAP_CUR=\"tree_code\",\"0915566655\"\r\n"
//	#define		Join_CMD			"AT+CWJAP_CUR=\"qwert\",\"zxcvb123456789\"\r\n"
//	#define		Join_CMD			"AT+CWJAP_CUR=\"Horsesaycheer\",\"kuankuan\"\r\n"



char php_MAC_00[20];

char php_key_00[20];
char php_key_01[20];
char php_key_02[20];
char php_key_03[20];
char php_key_04[20];
char php_key_05[20];
char php_key_06[20];
char php_key_07[20];
char php_key_08[20];
char php_key_09[20];
char php_key_10[20];
char php_key_11[20];
char php_key_12[20];
char php_key_13[20];
char php_key_14[20];
char php_key_15[20];
char php_key_16[20];
char php_key_17[20];
char php_key_18[20];
char php_key_19[20];
char php_key_20[20];





typedef enum{
	Wait_Send_AT,
	Wait_Send_RST,
	Wait_Send_RESTORE,
	Wait_AT_CIFSR,
	Wait_AT_CWMODE_Set_1,
	Wait_AT_CWJAP_Join_SSID,
	//UPDATE
	Wait_AT_CIPSTART_1,
	Wait_AT_CIPSEND_1,
	Wait_GET_1,
	//GET
	Wait_AT_CIPSTART_2,
	Wait_AT_CIPSEND_2,
	Wait_GET_2,
	Wait_AT_CIPCLOSE,
	Wait_AT_CIPCLOSE_Wait_Unit,
	Wait_AT_CWMODE_Set_2,
	Wait_AT_CWMODE_Set_3,
	Wait_AT_CWSAP_CUR,
	Wait_AT_CIPMUX,
	Wait_AT_CIPSERVER,
	Wait_AT_CWLAPOPT,
	Wait_AT_CWLAP,
	Wait_AT_PING_8_8_8_8,
	
	
	
	
	Send_CMD,
	Reset_flag_CMD,
	Wait_Reciver,
	Check_Reciver,
	Reciver_OK,
	Print_Reciver_ASCII,
	Print_Reciver_Byte,
	Check_Reciver_String,
	Print_Reciver_MAC_CMD,
	
	WiFi_STOP,
	WiFi_CMD_Delay,
	WiFi_NOP,
	WiFi_Wait_Next_Loop,
	WiFi_AP_Mode,
	Wifi_AP_Mode_Wait_User_keyin,
	Wifi_AP_Mode_Get_User_keyin_SSID_PW,
	WiFi_Goto_AP_Mode,
	WiFi_Get_Discovery_Network_Mode,
	WiFi_Sleep,
}Wifi_RunDown;



typedef enum{
	Step_Initial_AT,
	Step_SetRST,
	Step_SetMode,
	Step_Test_AT,
	
	Step_Get_AT,
	Step_Join_SSID,
	Step_Connect_API,
	Step_Send_Len_To_API,
	Step_Send_Data_To_API,
	Step_Close_API,
	Step_NOP,
	
	
}Wifi_RunStep;



//-------------------------------------------------------------------------

#define Wifi_COM	&MyUart04
#define WIFI_Input_buffer_Max			2048

#define WIFI_Rx_Timeout_Count_Max_100	100
#define WIFI_Rx_Timeout_Count_Max_300	300
#define WIFI_Rx_Timeout_Count_Max_600	600
#define WIFI_Rx_Timeout_Count_Max_1000	1000

#define WIFI_Rx_Timeout_Count_Max		WIFI_Rx_Timeout_Count_Max_1000
#define WIFI_f_count_Timer_Max			50000
#define WIFI_Main_Counter_Max			200		//	1秒

#define  wifi_work_led_on				10
#define  wifi_work_led_off				20
#define  Is_AP_Mode						false
#define  Is_Station_Mode				true

#define			MAX_RX_BUFFER_LENGTH_test	100
unsigned char	Rx_buffer_test[MAX_RX_BUFFER_LENGTH_test];
unsigned char test_count;

#define		Join_CMD			"AT+CWJAP_CUR=\"dlink-C592\",\"yotbh66768\"\r\n"

//-------------------------------------------------------------------------

typedef struct Wifi_SSID
{
	volatile char ApListSrting[1024];
	volatile char SearchList[20][50];
	volatile char SearchListCounter;	
}Wifi_SSID_t;

typedef struct Wifi_flag
{
	bool f_ok;
	bool f_error;
	bool f_timeout;
	bool f_mcu_timout;
	bool f_busy;
	bool f_nochange;
	bool f_http;
	bool f_wifi_got_ip;
	bool f_FAIL;
	
	bool f_ssid_is_change;
	bool f_pw_is_change;
	bool f_ssid_or_pw_is_change;
}Wifi_flag_t;

typedef struct WiFi
{
	unsigned char	NowStatus;
	unsigned char	NowCMD;

	unsigned char	SSID_Buffer[64];
	unsigned char	userKeyin_SSID_Buffer[64];
	
	unsigned char	PASSWORD_Buffer[64];
	unsigned char	userKeyin_PASSWORD_Buffer[64];
	

	
	unsigned char	Rx_buffer[MAX_RX_BUFFER_LENGTH];
	unsigned int	Rx_buffer_Count;
	bool			new_data;
	unsigned char	Input_buffer[WIFI_Input_buffer_Max];
	#define Ping_Buffer_Max					10
	unsigned char	Ping_Buffer[Ping_Buffer_Max];
	unsigned long	Rx_Timeout_Count;
	unsigned long	f_count;
	unsigned long	f_count_Max;
	unsigned long	f_count_Max_div;
	unsigned long	f_count_Timer_Max;
	
	unsigned char	RunStep;
	unsigned int	Cycletime;
	unsigned char	Send_UnSuccess_count;
	
	unsigned int	WIFI_Main_Counter;
	unsigned int	wifi_work_led;
	bool			wifi_work_led_flag;
	bool			wifi_network_flag;
	
	
	bool			debug_flag;
	bool			Send_Success;
	
	unsigned char	Update_ptr;
	unsigned char	Update_Count;
	bool			Update_Next_ptr_Flag;
	bool			AP_Mode_Switch;
	bool			(*get_AP_Mode_Switch)(unsigned char tmp);
	bool			inAP_Mode;

	Wifi_SSID_t		ssid;
	Wifi_flag_t		flag;
	Item_Dev_t	*eSSID;
	Item_Dev_t	*ePW;

	char target_Number;
	//char html[2048];

}WiFi_t;
volatile WiFi_t My_WiFi;


void Wifi_inital(void);
bool get_GPIO(unsigned char tmp);







