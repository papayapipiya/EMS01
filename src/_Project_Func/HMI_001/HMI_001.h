
#define Y1_ON	port_pin_set_output_level(PIN_PA00, true);
#define Y1_OFF	port_pin_set_output_level(PIN_PA00, false);
#define Y2_ON	port_pin_set_output_level(PIN_PA01, true);
#define Y2_OFF	port_pin_set_output_level(PIN_PA01, false);
#define Y3_ON	port_pin_set_output_level(PIN_PA02, true);
#define Y3_OFF	port_pin_set_output_level(PIN_PA02, false);
#define Y4_ON	port_pin_set_output_level(PIN_PA03, true);
#define Y4_OFF	port_pin_set_output_level(PIN_PA03, false);
#define Y5_ON	port_pin_set_output_level(PIN_PA04, true);
#define Y5_OFF	port_pin_set_output_level(PIN_PA04, false);

#define Manager_01_Alarm_ON		Y1_ON
#define Manager_01_Alarm_OFF	Y1_OFF
#define Manager_02_Alarm_ON		Y2_ON
#define Manager_02_Alarm_OFF	Y2_OFF
#define Manager_03_Alarm_ON		Y3_ON
#define Manager_03_Alarm_OFF	Y3_OFF
#define Manager_04_Alarm_ON		Y4_ON
#define Manager_04_Alarm_OFF	Y4_OFF
#define Common_Alarm_ON			Y5_ON
#define Common_Alarm_OFF		Y5_OFF



typedef enum
{
	HMI_NOP,		//	0
	HMI_Print_01,
	HMI_Print_02,
	HMI_Main,
	HMI_Check_CMD,
	HMI_Check_LRC,
	HMI_Response,
}HMI_RunDown;


uint8_t HMI_Define_MyRS485Tmp[16][8];
uint8_t HMI_RS485_value[16][15];
unsigned int HMI_RS485_Time_Count;


#define HMI_RS485_RX_D_Buf_Count		1
#define HMI_RS485_CMD_Index_Max			5
#define HMI_RS485_LORA_OUT_Index_Max	2
uint8_t HMI_RS485_RX_In_Buf[20];
uint8_t HMI_RS485_RX_Buf[20];
uint8_t HMI_RS485_RX_Buf_Count;
#define HMI_RS485_RX_Buf_Count_Max	16
bool HMI_RS485_OK_Flag;
uint8_t HMI_RS485_CMD_Index;
uint16_t HMI_RS485_LORA_OUT_Index;
unsigned char tmp_str[100];
unsigned char HMI_Status;



typedef struct PLC_M_Tmp{
	bool M;
}PLC_M_t;

typedef struct PLC_D_Tmp{
	int D;
}PLC_D_t;


typedef struct PLC_Reader_Tmp{
	char TestStr[30];
	char strlenght;
	unsigned char No;
	unsigned char Cmd;
	unsigned char Count;
	unsigned char Type0;
	unsigned char Type1;
	unsigned int Addr;
	//======================
	unsigned int Addr_int;
	unsigned int Value;
}PLC_Reader_Tmp_t;

#define PLC_Reader_Tmp_Max 100
//PLC_Reader_Tmp_t PLC_Reader_Buf[PLC_Reader_Tmp_Max];
PLC_Reader_Tmp_t PLC_Buf;
PLC_M_t PLC_M[500];
PLC_D_t PLC_D[500];


#define HMI_NVM	547


typedef struct Farm_Tmp{
	unsigned int	*Temperature;
	unsigned int	*Humidity;
	unsigned int	*CO2;
	unsigned int	*RSSI;
	unsigned int	*RSSI_Status;
	unsigned int	*RSSI_Status_Counter;
	unsigned int	*Main_Status;
	unsigned int	*Page_Status;
	unsigned int	*Power;
	bool			*ON_OFF;
	bool			*Dev_High_Temperature_Flag;
	bool			*Dev_Low_Temperature_Flag;
	bool			*Dev_NoPower_Flag;
	bool			*Dev_Air_Error_Flag;
	bool			*Dev_Error_Flag;
}Farm_Tmp_t;

typedef enum
{
	Farm_User_NOP,						//	0
	Farm_User_ALARM_Waitting_ON,					//	0
	Farm_User_ALARM_ON,
	Farm_User_ALARM_ON_Delay,
	Farm_User_ALARM_OFF,
	Farm_User_ALARM_Waitting_Cycle,		//	0
	Farm_User_Delay,					//	0
	Farm_User_Reset_ON,	
	Farm_User_Reset_ON_Delay,					//	0
	Farm_User_Reset_OFF,	
}Farm_User_RunDown;


typedef struct Farm_User_Tmp{
	unsigned int	*Delay_Time;
	unsigned int	*Cycle_Time;
	unsigned int	Last_Delay_Time;
	unsigned int	Last_Cycle_Time;
	unsigned int	Counter_Delay_Time;
	unsigned int	Counter_Cycle_Time;
	unsigned char	NowStatus;
}Farm_User_Tmp_t;

//==========================================
//	HMI_001_Main.c
void MainFunc(void);
void PLC_Header_Format_str(void);
void HMI_inital(void);

//==========================================
//	HMI_001_Function.c
void HMI_inital(void);
void Reset_PLC_Catch_Value(void);
unsigned char str_to_hex_char(unsigned char *ptr);
unsigned int str_to_hex_int(unsigned char *ptr);
void PLC_Header_Format_str(void);

//==========================================
//	HMI_001_Uart.c
void HMI_Uart_Form_RS485_callback(struct usart_module *const usart_module);
void RS485_uart_str(const char *tmp_ptr);
void debug_RS485_uart_str(const char *tmp_ptr);
void tc_PLC_Catch(struct tc_module *const module_inst);



void HMI_Modbus_Function(void);


//==========================================
//	NVM
void Save_HMI_Ref_in_NVM(void);
void Load_HMI_Ref_in_NVM(void);
unsigned char BitToByte(bool b7,bool b6,bool b5,bool b4,bool b3,bool b2,bool b1,bool b0);
void ByteToBit(bool *b7,bool *b6,bool *b5,bool *b4,bool *b3,bool *b2,bool *b1,bool *b0, unsigned char tmpVal);


//==========================================
//	HMI_001_inital.c
void inital_D_BUF(void);

#define NoSense_Temperature PLC_D[163].D
unsigned int Last_NoSense_Temperature;

#define FarmNumber PLC_D[164].D
unsigned int Last_FarmNumber;

#define Low_Temperature PLC_D[165].D
unsigned int Last_Low_Temperature;

#define High_Temperature PLC_D[166].D
unsigned int Last_High_Temperature;

#define Gas_Alarm PLC_D[175].D
unsigned int Last_Gas_Alarm;

#define NoSense_Gas_Alarm PLC_D[176].D
unsigned int Last_NoSense_Gas_Alarm;


unsigned char Sensor_01_08_ON_OFF;
unsigned char Last_Sensor_01_08_ON_OFF;
unsigned char Sensor_09_16_ON_OFF;
unsigned char Last_Sensor_09_16_ON_OFF;
#define Sensor_01_ON_OFF							PLC_M[11].M
#define Sensor_02_ON_OFF							PLC_M[12].M
#define Sensor_03_ON_OFF							PLC_M[13].M
#define Sensor_04_ON_OFF							PLC_M[14].M
#define Sensor_05_ON_OFF							PLC_M[15].M
#define Sensor_06_ON_OFF							PLC_M[16].M
#define Sensor_07_ON_OFF							PLC_M[17].M
#define Sensor_08_ON_OFF							PLC_M[18].M
#define Sensor_09_ON_OFF							PLC_M[19].M
#define Sensor_10_ON_OFF							PLC_M[20].M
#define Sensor_11_ON_OFF							PLC_M[21].M
#define Sensor_12_ON_OFF							PLC_M[22].M
#define Sensor_13_ON_OFF							PLC_M[23].M
#define Sensor_14_ON_OFF							PLC_M[24].M
#define Sensor_15_ON_OFF							PLC_M[25].M
#define Sensor_16_ON_OFF							PLC_M[26].M

#define Sensor_01_Dev_High_Temperature_Flag			PLC_M[27].M
#define Sensor_02_Dev_High_Temperature_Flag			PLC_M[28].M
#define Sensor_03_Dev_High_Temperature_Flag			PLC_M[29].M
#define Sensor_04_Dev_High_Temperature_Flag			PLC_M[30].M
#define Sensor_05_Dev_High_Temperature_Flag			PLC_M[31].M
#define Sensor_06_Dev_High_Temperature_Flag			PLC_M[32].M
#define Sensor_07_Dev_High_Temperature_Flag			PLC_M[33].M
#define Sensor_08_Dev_High_Temperature_Flag			PLC_M[34].M
#define Sensor_09_Dev_High_Temperature_Flag			PLC_M[35].M
#define Sensor_10_Dev_High_Temperature_Flag			PLC_M[36].M
#define Sensor_11_Dev_High_Temperature_Flag			PLC_M[37].M
#define Sensor_12_Dev_High_Temperature_Flag			PLC_M[38].M
#define Sensor_13_Dev_High_Temperature_Flag			PLC_M[39].M
#define Sensor_14_Dev_High_Temperature_Flag			PLC_M[40].M
#define Sensor_15_Dev_High_Temperature_Flag			PLC_M[41].M
#define Sensor_16_Dev_High_Temperature_Flag			PLC_M[42].M

#define Sensor_01_Dev_Low_Temperature_Flag			PLC_M[43].M
#define Sensor_02_Dev_Low_Temperature_Flag			PLC_M[44].M
#define Sensor_03_Dev_Low_Temperature_Flag			PLC_M[45].M
#define Sensor_04_Dev_Low_Temperature_Flag			PLC_M[46].M
#define Sensor_05_Dev_Low_Temperature_Flag			PLC_M[47].M
#define Sensor_06_Dev_Low_Temperature_Flag			PLC_M[48].M
#define Sensor_07_Dev_Low_Temperature_Flag			PLC_M[49].M
#define Sensor_08_Dev_Low_Temperature_Flag			PLC_M[50].M
#define Sensor_09_Dev_Low_Temperature_Flag			PLC_M[51].M
#define Sensor_10_Dev_Low_Temperature_Flag			PLC_M[52].M
#define Sensor_11_Dev_Low_Temperature_Flag			PLC_M[53].M
#define Sensor_12_Dev_Low_Temperature_Flag			PLC_M[54].M
#define Sensor_13_Dev_Low_Temperature_Flag			PLC_M[55].M
#define Sensor_14_Dev_Low_Temperature_Flag			PLC_M[56].M
#define Sensor_15_Dev_Low_Temperature_Flag			PLC_M[57].M
#define Sensor_16_Dev_Low_Temperature_Flag			PLC_M[58].M

#define Sensor_01_Dev_NoPower_Flag					PLC_M[59].M
#define Sensor_02_Dev_NoPower_Flag					PLC_M[60].M
#define Sensor_03_Dev_NoPower_Flag					PLC_M[61].M
#define Sensor_04_Dev_NoPower_Flag					PLC_M[62].M
#define Sensor_05_Dev_NoPower_Flag					PLC_M[63].M
#define Sensor_06_Dev_NoPower_Flag					PLC_M[64].M
#define Sensor_07_Dev_NoPower_Flag					PLC_M[65].M
#define Sensor_08_Dev_NoPower_Flag					PLC_M[66].M
#define Sensor_09_Dev_NoPower_Flag					PLC_M[67].M
#define Sensor_10_Dev_NoPower_Flag					PLC_M[68].M
#define Sensor_11_Dev_NoPower_Flag					PLC_M[69].M
#define Sensor_12_Dev_NoPower_Flag					PLC_M[70].M
#define Sensor_13_Dev_NoPower_Flag					PLC_M[71].M
#define Sensor_14_Dev_NoPower_Flag					PLC_M[72].M
#define Sensor_15_Dev_NoPower_Flag					PLC_M[73].M
#define Sensor_16_Dev_NoPower_Flag					PLC_M[74].M


#define Sensor_01_Dev_Air_Error_Flag				PLC_M[75].M
#define Sensor_02_Dev_Air_Error_Flag				PLC_M[76].M
#define Sensor_03_Dev_Air_Error_Flag				PLC_M[77].M
#define Sensor_04_Dev_Air_Error_Flag				PLC_M[78].M
#define Sensor_05_Dev_Air_Error_Flag				PLC_M[79].M
#define Sensor_06_Dev_Air_Error_Flag				PLC_M[80].M
#define Sensor_07_Dev_Air_Error_Flag				PLC_M[81].M
#define Sensor_08_Dev_Air_Error_Flag				PLC_M[82].M
#define Sensor_09_Dev_Air_Error_Flag				PLC_M[83].M
#define Sensor_10_Dev_Air_Error_Flag				PLC_M[84].M
#define Sensor_11_Dev_Air_Error_Flag				PLC_M[85].M
#define Sensor_12_Dev_Air_Error_Flag				PLC_M[86].M
#define Sensor_13_Dev_Air_Error_Flag				PLC_M[87].M
#define Sensor_14_Dev_Air_Error_Flag				PLC_M[88].M
#define Sensor_15_Dev_Air_Error_Flag				PLC_M[89].M
#define Sensor_16_Dev_Air_Error_Flag				PLC_M[90].M

#define Sensor_01_Dev_Error_Flag					PLC_M[91].M
#define Sensor_02_Dev_Error_Flag					PLC_M[92].M
#define Sensor_03_Dev_Error_Flag					PLC_M[93].M
#define Sensor_04_Dev_Error_Flag					PLC_M[94].M
#define Sensor_05_Dev_Error_Flag					PLC_M[95].M
#define Sensor_06_Dev_Error_Flag					PLC_M[96].M
#define Sensor_07_Dev_Error_Flag					PLC_M[97].M
#define Sensor_08_Dev_Error_Flag					PLC_M[98].M
#define Sensor_09_Dev_Error_Flag					PLC_M[99].M
#define Sensor_10_Dev_Error_Flag					PLC_M[100].M
#define Sensor_11_Dev_Error_Flag					PLC_M[101].M
#define Sensor_12_Dev_Error_Flag					PLC_M[102].M
#define Sensor_13_Dev_Error_Flag					PLC_M[103].M
#define Sensor_14_Dev_Error_Flag					PLC_M[104].M
#define Sensor_15_Dev_Error_Flag					PLC_M[105].M
#define Sensor_16_Dev_Error_Flag					PLC_M[106].M

//---------------------------------------------------------------------------------
//			D BUFFER
//---------------------------------------------------------------------------------


#define Sensor_01_Main_Status						PLC_D[51].D
#define Sensor_02_Main_Status						PLC_D[52].D
#define Sensor_03_Main_Status						PLC_D[53].D
#define Sensor_04_Main_Status						PLC_D[54].D
#define Sensor_05_Main_Status						PLC_D[55].D
#define Sensor_06_Main_Status						PLC_D[56].D
#define Sensor_07_Main_Status						PLC_D[57].D
#define Sensor_08_Main_Status						PLC_D[58].D
#define Sensor_09_Main_Status						PLC_D[59].D
#define Sensor_10_Main_Status						PLC_D[60].D
#define Sensor_11_Main_Status						PLC_D[61].D
#define Sensor_12_Main_Status						PLC_D[62].D
#define Sensor_13_Main_Status						PLC_D[63].D
#define Sensor_14_Main_Status						PLC_D[64].D
#define Sensor_15_Main_Status						PLC_D[65].D
#define Sensor_16_Main_Status						PLC_D[66].D

#define Sensor_01_Page_Status						PLC_D[67].D
#define Sensor_02_Page_Status						PLC_D[68].D
#define Sensor_03_Page_Status						PLC_D[69].D
#define Sensor_04_Page_Status						PLC_D[70].D
#define Sensor_05_Page_Status						PLC_D[71].D
#define Sensor_06_Page_Status						PLC_D[72].D
#define Sensor_07_Page_Status						PLC_D[73].D
#define Sensor_08_Page_Status						PLC_D[74].D
#define Sensor_09_Page_Status						PLC_D[75].D
#define Sensor_10_Page_Status						PLC_D[76].D
#define Sensor_11_Page_Status						PLC_D[77].D
#define Sensor_12_Page_Status						PLC_D[78].D
#define Sensor_13_Page_Status						PLC_D[79].D
#define Sensor_14_Page_Status						PLC_D[80].D
#define Sensor_15_Page_Status						PLC_D[81].D
#define Sensor_16_Page_Status						PLC_D[82].D


#define Farm_01_Temperature							PLC_D[83].D
#define Farm_01_Humidity							PLC_D[84].D
#define Farm_01_CO2									PLC_D[85].D
#define Farm_02_Temperature							PLC_D[86].D
#define Farm_02_Humidity							PLC_D[87].D
#define Farm_02_CO2									PLC_D[88].D
#define Farm_03_Temperature							PLC_D[89].D
#define Farm_03_Humidity							PLC_D[90].D
#define Farm_03_CO2									PLC_D[91].D
#define Farm_04_Temperature							PLC_D[92].D
#define Farm_04_Humidity							PLC_D[93].D
#define Farm_04_CO2									PLC_D[94].D
#define Farm_05_Temperature							PLC_D[95].D
#define Farm_05_Humidity							PLC_D[96].D
#define Farm_05_CO2									PLC_D[97].D
#define Farm_06_Temperature							PLC_D[98].D
#define Farm_06_Humidity							PLC_D[99].D
#define Farm_06_CO2									PLC_D[100].D
#define Farm_07_Temperature							PLC_D[101].D
#define Farm_07_Humidity							PLC_D[102].D
#define Farm_07_CO2									PLC_D[103].D
#define Farm_08_Temperature							PLC_D[104].D
#define Farm_08_Humidity							PLC_D[105].D
#define Farm_08_CO2									PLC_D[106].D
#define Farm_09_Temperature							PLC_D[107].D
#define Farm_09_Humidity							PLC_D[108].D
#define Farm_09_CO2									PLC_D[109].D
#define Farm_10_Temperature							PLC_D[110].D
#define Farm_10_Humidity							PLC_D[111].D
#define Farm_10_CO2									PLC_D[112].D
#define Farm_11_Temperature							PLC_D[113].D
#define Farm_11_Humidity							PLC_D[114].D
#define Farm_11_CO2									PLC_D[115].D
#define Farm_12_Temperature							PLC_D[116].D
#define Farm_12_Humidity							PLC_D[117].D
#define Farm_12_CO2									PLC_D[118].D
#define Farm_13_Temperature							PLC_D[119].D
#define Farm_13_Humidity							PLC_D[120].D
#define Farm_13_CO2									PLC_D[121].D
#define Farm_14_Temperature							PLC_D[122].D
#define Farm_14_Humidity							PLC_D[123].D
#define Farm_14_CO2									PLC_D[124].D
#define Farm_15_Temperature							PLC_D[125].D
#define Farm_15_Humidity							PLC_D[126].D
#define Farm_15_CO2									PLC_D[127].D
#define Farm_16_Temperature							PLC_D[128].D
#define Farm_16_Humidity							PLC_D[129].D
#define Farm_16_CO2									PLC_D[130].D


#define Sensor_01_RSSI_Status						PLC_D[131].D
#define Sensor_02_RSSI_Status						PLC_D[132].D
#define Sensor_03_RSSI_Status						PLC_D[133].D
#define Sensor_04_RSSI_Status						PLC_D[134].D
#define Sensor_05_RSSI_Status						PLC_D[135].D
#define Sensor_06_RSSI_Status						PLC_D[136].D
#define Sensor_07_RSSI_Status						PLC_D[137].D
#define Sensor_08_RSSI_Status						PLC_D[138].D
#define Sensor_09_RSSI_Status						PLC_D[139].D
#define Sensor_10_RSSI_Status						PLC_D[140].D
#define Sensor_11_RSSI_Status						PLC_D[141].D
#define Sensor_12_RSSI_Status						PLC_D[142].D
#define Sensor_13_RSSI_Status						PLC_D[143].D
#define Sensor_14_RSSI_Status						PLC_D[144].D
#define Sensor_15_RSSI_Status						PLC_D[145].D
#define Sensor_16_RSSI_Status						PLC_D[146].D


#define Sensor_01_RSSI								PLC_D[147].D
#define Sensor_02_RSSI								PLC_D[148].D
#define Sensor_03_RSSI								PLC_D[149].D
#define Sensor_04_RSSI								PLC_D[150].D
#define Sensor_05_RSSI								PLC_D[151].D
#define Sensor_06_RSSI								PLC_D[152].D
#define Sensor_07_RSSI								PLC_D[153].D
#define Sensor_08_RSSI								PLC_D[154].D
#define Sensor_09_RSSI								PLC_D[155].D
#define Sensor_10_RSSI								PLC_D[156].D
#define Sensor_11_RSSI								PLC_D[157].D
#define Sensor_12_RSSI								PLC_D[158].D
#define Sensor_13_RSSI								PLC_D[159].D
#define Sensor_14_RSSI								PLC_D[160].D
#define Sensor_15_RSSI								PLC_D[161].D
#define Sensor_16_RSSI								PLC_D[162].D


#define User_01_Delay_Time							PLC_D[167].D
#define User_01_Cycle_Time							PLC_D[168].D
#define User_02_Delay_Time							PLC_D[169].D
#define User_02_Cycle_Time							PLC_D[170].D
#define User_03_Delay_Time							PLC_D[171].D
#define User_03_Cycle_Time							PLC_D[172].D
#define User_04_Delay_Time							PLC_D[173].D
#define User_04_Cycle_Time							PLC_D[174].D

unsigned int Last_User_01_Delay_Time;
unsigned int Last_User_01_Cycle_Time;
unsigned int Last_User_02_Delay_Time;
unsigned int Last_User_02_Cycle_Time;
unsigned int Last_User_03_Delay_Time;
unsigned int Last_User_03_Cycle_Time;
unsigned int Last_User_04_Delay_Time;
unsigned int Last_User_04_Cycle_Time;

#define Sensor_01_Power								PLC_D[187].D
#define Sensor_02_Power								PLC_D[188].D
#define Sensor_03_Power								PLC_D[189].D
#define Sensor_04_Power								PLC_D[190].D
#define Sensor_05_Power								PLC_D[191].D
#define Sensor_06_Power								PLC_D[192].D
#define Sensor_07_Power								PLC_D[193].D
#define Sensor_08_Power								PLC_D[194].D
#define Sensor_09_Power								PLC_D[195].D
#define Sensor_10_Power								PLC_D[196].D
#define Sensor_11_Power								PLC_D[197].D
#define Sensor_12_Power								PLC_D[198].D
#define Sensor_13_Power								PLC_D[199].D
#define Sensor_14_Power								PLC_D[200].D
#define Sensor_15_Power								PLC_D[201].D
#define Sensor_16_Power								PLC_D[202].D

#define	Sensor_01_RSSI_Status_Counter				PLC_D[203].D
#define	Sensor_02_RSSI_Status_Counter				PLC_D[204].D
#define	Sensor_03_RSSI_Status_Counter				PLC_D[205].D
#define	Sensor_04_RSSI_Status_Counter				PLC_D[206].D
#define	Sensor_05_RSSI_Status_Counter				PLC_D[207].D
#define	Sensor_06_RSSI_Status_Counter				PLC_D[208].D
#define	Sensor_07_RSSI_Status_Counter				PLC_D[209].D
#define	Sensor_08_RSSI_Status_Counter				PLC_D[210].D
#define	Sensor_09_RSSI_Status_Counter				PLC_D[211].D
#define	Sensor_10_RSSI_Status_Counter				PLC_D[212].D
#define	Sensor_11_RSSI_Status_Counter				PLC_D[213].D
#define	Sensor_12_RSSI_Status_Counter				PLC_D[214].D
#define	Sensor_13_RSSI_Status_Counter				PLC_D[215].D
#define	Sensor_14_RSSI_Status_Counter				PLC_D[216].D
#define	Sensor_15_RSSI_Status_Counter				PLC_D[217].D
#define	Sensor_16_RSSI_Status_Counter				PLC_D[218].D






bool Save_NVM_Flag;
bool inital_Flag;
bool Alarm_Flag;
unsigned int Save_NVM_Count;

volatile Farm_Tmp_t UserFarm[17];
volatile Farm_Tmp_t *ptrFarm;

volatile Farm_User_Tmp_t ManagerFarm[5];
volatile Farm_User_Tmp_t *ptrManager;
