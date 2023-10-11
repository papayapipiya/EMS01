/*
 * Wifi_inital_Define.h
 *
 * Created: 2021/4/19 上午 10:40:12
 *  Author: willchen
 */ 


#ifndef WIFI_INITAL_DEFINE_H_
#define WIFI_INITAL_DEFINE_H_



//#define Testing_001_3		// 實驗室 -> Jay PHP -> google excel	(實驗室內)
//#define Testing_001_4		// qwert -> Jay PHP -> google excel	(經理家)
#define Testing_001_5		// 實驗室 -> will PHP -> mysql		(實驗室內)




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
	Wait_AT_CWMODE_Set_2,
	Wait_AT_CWMODE_Set_3,
	Wait_AT_CWSAP_CUR,
	Wait_AT_CIPMUX,
	Wait_AT_CIPSERVER,
	
	
	Send_CMD,
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
//-------------------------------------------------------------------------

typedef struct WiFi
{
	unsigned char	NowStatus;
	unsigned char	NowCMD;

	
	unsigned char	Rx_buffer[MAX_RX_BUFFER_LENGTH];
	unsigned int	Rx_buffer_Count;
	bool			new_data;
	unsigned char	Input_buffer[WIFI_Input_buffer_Max];
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
}WiFi_t;
WiFi_t My_WiFi;


#endif /* WIFI_INITAL_DEFINE_H_ */