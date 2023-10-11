#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


//=================================================
#ifdef HMI_001
//=================================================



void inital_D_BUF(void)
{
	Manager_01_Alarm_OFF
	Manager_02_Alarm_OFF
	Manager_03_Alarm_OFF
	Manager_04_Alarm_OFF
	Common_Alarm_OFF
	
	Save_NVM_Flag = false;
	inital_Flag = false;
	Save_NVM_Count = 0;
	
	UserFarm[1].Temperature						= &Farm_01_Temperature;
	UserFarm[1].Humidity						= &Farm_01_Humidity;
	UserFarm[1].CO2								= &Farm_01_CO2;
	UserFarm[1].RSSI							= &Sensor_01_RSSI;
	UserFarm[1].RSSI_Status						= &Sensor_01_RSSI_Status;
	UserFarm[1].ON_OFF							= &Sensor_01_ON_OFF;
	UserFarm[1].Main_Status						= &Sensor_01_Main_Status;
	UserFarm[1].Page_Status						= &Sensor_01_Page_Status;
	UserFarm[1].Power							= &Sensor_01_Power;
	UserFarm[1].Dev_High_Temperature_Flag		= &Sensor_01_Dev_High_Temperature_Flag;
	UserFarm[1].Dev_Low_Temperature_Flag		= &Sensor_01_Dev_Low_Temperature_Flag;
	UserFarm[1].Dev_Air_Error_Flag				= &Sensor_01_Dev_Air_Error_Flag;
	UserFarm[1].Dev_NoPower_Flag				= &Sensor_01_Dev_NoPower_Flag;
	UserFarm[1].Dev_Error_Flag					= &Sensor_01_Dev_Error_Flag;
	UserFarm[1].RSSI_Status_Counter				= &Sensor_01_RSSI_Status_Counter;
	
	UserFarm[2].Temperature						= &Farm_02_Temperature;
	UserFarm[2].Humidity						= &Farm_02_Humidity;
	UserFarm[2].CO2								= &Farm_02_CO2;
	UserFarm[2].RSSI							= &Sensor_02_RSSI;
	UserFarm[2].RSSI_Status						= &Sensor_02_RSSI_Status;
	UserFarm[2].ON_OFF							= &Sensor_02_ON_OFF;
	UserFarm[2].Main_Status						= &Sensor_02_Main_Status;
	UserFarm[2].Page_Status						= &Sensor_02_Page_Status;
	UserFarm[2].Power							= &Sensor_02_Power;
	UserFarm[2].Dev_High_Temperature_Flag		= &Sensor_02_Dev_High_Temperature_Flag;
	UserFarm[2].Dev_Low_Temperature_Flag		= &Sensor_02_Dev_Low_Temperature_Flag;
	UserFarm[2].Dev_Air_Error_Flag				= &Sensor_02_Dev_Air_Error_Flag;
	UserFarm[2].Dev_NoPower_Flag				= &Sensor_02_Dev_NoPower_Flag;
	UserFarm[2].Dev_Error_Flag					= &Sensor_02_Dev_Error_Flag;
	UserFarm[2].RSSI_Status_Counter				= &Sensor_02_RSSI_Status_Counter;
	
	UserFarm[3].Temperature						= &Farm_03_Temperature;
	UserFarm[3].Humidity						= &Farm_03_Humidity;
	UserFarm[3].CO2								= &Farm_03_CO2;
	UserFarm[3].RSSI							= &Sensor_03_RSSI;
	UserFarm[3].RSSI_Status						= &Sensor_03_RSSI_Status;
	UserFarm[3].ON_OFF							= &Sensor_03_ON_OFF;
	UserFarm[3].Main_Status						= &Sensor_03_Main_Status;
	UserFarm[3].Page_Status						= &Sensor_03_Page_Status;
	UserFarm[3].Power							= &Sensor_03_Power;
	UserFarm[3].Dev_High_Temperature_Flag		= &Sensor_03_Dev_High_Temperature_Flag;
	UserFarm[3].Dev_Low_Temperature_Flag		= &Sensor_03_Dev_Low_Temperature_Flag;
	UserFarm[3].Dev_Air_Error_Flag				= &Sensor_03_Dev_Air_Error_Flag;
	UserFarm[3].Dev_NoPower_Flag				= &Sensor_03_Dev_NoPower_Flag;
	UserFarm[3].Dev_Error_Flag					= &Sensor_03_Dev_Error_Flag;
	UserFarm[3].RSSI_Status_Counter				= &Sensor_03_RSSI_Status_Counter;
	
	UserFarm[4].Temperature						= &Farm_04_Temperature;
	UserFarm[4].Humidity						= &Farm_04_Humidity;
	UserFarm[4].CO2								= &Farm_04_CO2;
	UserFarm[4].RSSI							= &Sensor_04_RSSI;
	UserFarm[4].RSSI_Status						= &Sensor_04_RSSI_Status;
	UserFarm[4].ON_OFF							= &Sensor_04_ON_OFF;
	UserFarm[4].Main_Status						= &Sensor_04_Main_Status;
	UserFarm[4].Page_Status						= &Sensor_04_Page_Status;
	UserFarm[4].Power							= &Sensor_04_Power;
	UserFarm[4].Dev_High_Temperature_Flag		= &Sensor_04_Dev_High_Temperature_Flag;
	UserFarm[4].Dev_Low_Temperature_Flag		= &Sensor_04_Dev_Low_Temperature_Flag;
	UserFarm[4].Dev_Air_Error_Flag				= &Sensor_04_Dev_Air_Error_Flag;
	UserFarm[4].Dev_NoPower_Flag				= &Sensor_04_Dev_NoPower_Flag;
	UserFarm[4].Dev_Error_Flag					= &Sensor_04_Dev_Error_Flag;
	UserFarm[4].RSSI_Status_Counter				= &Sensor_04_RSSI_Status_Counter;
	
	UserFarm[5].Temperature						= &Farm_05_Temperature;
	UserFarm[5].Humidity						= &Farm_05_Humidity;
	UserFarm[5].CO2								= &Farm_05_CO2;
	UserFarm[5].RSSI							= &Sensor_05_RSSI;
	UserFarm[5].RSSI_Status						= &Sensor_05_RSSI_Status;
	UserFarm[5].ON_OFF							= &Sensor_05_ON_OFF;
	UserFarm[5].Main_Status						= &Sensor_05_Main_Status;
	UserFarm[5].Page_Status						= &Sensor_05_Page_Status;
	UserFarm[5].Power							= &Sensor_05_Power;
	UserFarm[5].Dev_High_Temperature_Flag		= &Sensor_05_Dev_High_Temperature_Flag;
	UserFarm[5].Dev_Low_Temperature_Flag		= &Sensor_05_Dev_Low_Temperature_Flag;
	UserFarm[5].Dev_Air_Error_Flag				= &Sensor_05_Dev_Air_Error_Flag;
	UserFarm[5].Dev_NoPower_Flag				= &Sensor_05_Dev_NoPower_Flag;
	UserFarm[5].Dev_Error_Flag					= &Sensor_05_Dev_Error_Flag;
	UserFarm[5].RSSI_Status_Counter				= &Sensor_05_RSSI_Status_Counter;
	
	UserFarm[6].Temperature						= &Farm_06_Temperature;
	UserFarm[6].Humidity						= &Farm_06_Humidity;
	UserFarm[6].CO2								= &Farm_06_CO2;
	UserFarm[6].RSSI							= &Sensor_06_RSSI;
	UserFarm[6].RSSI_Status						= &Sensor_06_RSSI_Status;
	UserFarm[6].ON_OFF							= &Sensor_06_ON_OFF;
	UserFarm[6].Main_Status						= &Sensor_06_Main_Status;
	UserFarm[6].Page_Status						= &Sensor_06_Page_Status;
	UserFarm[6].Power							= &Sensor_06_Power;
	UserFarm[6].Dev_High_Temperature_Flag		= &Sensor_06_Dev_High_Temperature_Flag;
	UserFarm[6].Dev_Low_Temperature_Flag		= &Sensor_06_Dev_Low_Temperature_Flag;
	UserFarm[6].Dev_Air_Error_Flag				= &Sensor_06_Dev_Air_Error_Flag;
	UserFarm[6].Dev_NoPower_Flag				= &Sensor_06_Dev_NoPower_Flag;
	UserFarm[6].Dev_Error_Flag					= &Sensor_06_Dev_Error_Flag;
	UserFarm[6].RSSI_Status_Counter				= &Sensor_06_RSSI_Status_Counter;
	
	UserFarm[7].Temperature						= &Farm_07_Temperature;
	UserFarm[7].Humidity						= &Farm_07_Humidity;
	UserFarm[7].CO2								= &Farm_07_CO2;
	UserFarm[7].RSSI							= &Sensor_07_RSSI;
	UserFarm[7].RSSI_Status						= &Sensor_07_RSSI_Status;
	UserFarm[7].ON_OFF							= &Sensor_07_ON_OFF;
	UserFarm[7].Main_Status						= &Sensor_07_Main_Status;
	UserFarm[7].Page_Status						= &Sensor_07_Page_Status;
	UserFarm[7].Power							= &Sensor_07_Power;
	UserFarm[7].Dev_High_Temperature_Flag		= &Sensor_07_Dev_High_Temperature_Flag;
	UserFarm[7].Dev_Low_Temperature_Flag		= &Sensor_07_Dev_Low_Temperature_Flag;
	UserFarm[7].Dev_Air_Error_Flag				= &Sensor_07_Dev_Air_Error_Flag;
	UserFarm[7].Dev_NoPower_Flag				= &Sensor_07_Dev_NoPower_Flag;
	UserFarm[7].Dev_Error_Flag					= &Sensor_07_Dev_Error_Flag;
	UserFarm[7].RSSI_Status_Counter				= &Sensor_07_RSSI_Status_Counter;
	
	UserFarm[8].Temperature						= &Farm_08_Temperature;
	UserFarm[8].Humidity						= &Farm_08_Humidity;
	UserFarm[8].CO2								= &Farm_08_CO2;
	UserFarm[8].RSSI							= &Sensor_08_RSSI;
	UserFarm[8].RSSI_Status						= &Sensor_08_RSSI_Status;
	UserFarm[8].ON_OFF							= &Sensor_08_ON_OFF;
	UserFarm[8].Main_Status						= &Sensor_08_Main_Status;
	UserFarm[8].Page_Status						= &Sensor_08_Page_Status;
	UserFarm[8].Power							= &Sensor_08_Power;
	UserFarm[8].Dev_High_Temperature_Flag		= &Sensor_08_Dev_High_Temperature_Flag;
	UserFarm[8].Dev_Low_Temperature_Flag		= &Sensor_08_Dev_Low_Temperature_Flag;
	UserFarm[8].Dev_Air_Error_Flag				= &Sensor_08_Dev_Air_Error_Flag;
	UserFarm[8].Dev_NoPower_Flag				= &Sensor_08_Dev_NoPower_Flag;
	UserFarm[8].Dev_Error_Flag					= &Sensor_08_Dev_Error_Flag;
	UserFarm[8].RSSI_Status_Counter				= &Sensor_08_RSSI_Status_Counter;
	
	UserFarm[9].Temperature						= &Farm_09_Temperature;
	UserFarm[9].Humidity						= &Farm_09_Humidity;
	UserFarm[9].CO2								= &Farm_09_CO2;
	UserFarm[9].RSSI							= &Sensor_09_RSSI;
	UserFarm[9].RSSI_Status						= &Sensor_09_RSSI_Status;
	UserFarm[9].ON_OFF							= &Sensor_09_ON_OFF;
	UserFarm[9].Main_Status						= &Sensor_09_Main_Status;
	UserFarm[9].Page_Status						= &Sensor_09_Page_Status;
	UserFarm[9].Power							= &Sensor_09_Power;
	UserFarm[9].Dev_High_Temperature_Flag		= &Sensor_09_Dev_High_Temperature_Flag;
	UserFarm[9].Dev_Low_Temperature_Flag		= &Sensor_09_Dev_Low_Temperature_Flag;
	UserFarm[9].Dev_Air_Error_Flag				= &Sensor_09_Dev_Air_Error_Flag;
	UserFarm[9].Dev_NoPower_Flag				= &Sensor_09_Dev_NoPower_Flag;
	UserFarm[9].Dev_Error_Flag					= &Sensor_09_Dev_Error_Flag;
	UserFarm[9].RSSI_Status_Counter				= &Sensor_09_RSSI_Status_Counter;
	
	UserFarm[10].Temperature					= &Farm_10_Temperature;
	UserFarm[10].Humidity						= &Farm_10_Humidity;
	UserFarm[10].CO2							= &Farm_10_CO2;
	UserFarm[10].RSSI							= &Sensor_10_RSSI;
	UserFarm[10].RSSI_Status					= &Sensor_10_RSSI_Status;
	UserFarm[10].ON_OFF							= &Sensor_10_ON_OFF;
	UserFarm[10].Main_Status					= &Sensor_10_Main_Status;
	UserFarm[10].Page_Status					= &Sensor_10_Page_Status;
	UserFarm[10].Power							= &Sensor_10_Power;
	UserFarm[10].Dev_High_Temperature_Flag		= &Sensor_10_Dev_High_Temperature_Flag;
	UserFarm[10].Dev_Low_Temperature_Flag		= &Sensor_10_Dev_Low_Temperature_Flag;
	UserFarm[10].Dev_Air_Error_Flag				= &Sensor_10_Dev_Air_Error_Flag;
	UserFarm[10].Dev_NoPower_Flag				= &Sensor_10_Dev_NoPower_Flag;
	UserFarm[10].Dev_Error_Flag					= &Sensor_10_Dev_Error_Flag;
	UserFarm[10].RSSI_Status_Counter			= &Sensor_10_RSSI_Status_Counter;
	
	UserFarm[11].Temperature					= &Farm_11_Temperature;
	UserFarm[11].Humidity						= &Farm_11_Humidity;
	UserFarm[11].CO2							= &Farm_11_CO2;
	UserFarm[11].RSSI							= &Sensor_11_RSSI;
	UserFarm[11].RSSI_Status					= &Sensor_11_RSSI_Status;
	UserFarm[11].ON_OFF							= &Sensor_11_ON_OFF;
	UserFarm[11].Main_Status					= &Sensor_11_Main_Status;
	UserFarm[11].Page_Status					= &Sensor_11_Page_Status;
	UserFarm[11].Power							= &Sensor_11_Power;
	UserFarm[11].Dev_High_Temperature_Flag		= &Sensor_11_Dev_High_Temperature_Flag;
	UserFarm[11].Dev_Low_Temperature_Flag		= &Sensor_11_Dev_Low_Temperature_Flag;
	UserFarm[11].Dev_Air_Error_Flag				= &Sensor_11_Dev_Air_Error_Flag;
	UserFarm[11].Dev_NoPower_Flag				= &Sensor_11_Dev_NoPower_Flag;
	UserFarm[11].Dev_Error_Flag					= &Sensor_11_Dev_Error_Flag;
	UserFarm[11].RSSI_Status_Counter			= &Sensor_11_RSSI_Status_Counter;
	
	UserFarm[12].Temperature					= &Farm_12_Temperature;
	UserFarm[12].Humidity						= &Farm_12_Humidity;
	UserFarm[12].CO2							= &Farm_12_CO2;
	UserFarm[12].RSSI							= &Sensor_12_RSSI;
	UserFarm[12].RSSI_Status					= &Sensor_12_RSSI_Status;
	UserFarm[12].ON_OFF							= &Sensor_12_ON_OFF;
	UserFarm[12].Main_Status					= &Sensor_12_Main_Status;
	UserFarm[12].Page_Status					= &Sensor_12_Page_Status;
	UserFarm[12].Power							= &Sensor_12_Power;
	UserFarm[12].Dev_High_Temperature_Flag		= &Sensor_12_Dev_High_Temperature_Flag;
	UserFarm[12].Dev_Low_Temperature_Flag		= &Sensor_12_Dev_Low_Temperature_Flag;
	UserFarm[12].Dev_Air_Error_Flag				= &Sensor_12_Dev_Air_Error_Flag;
	UserFarm[12].Dev_NoPower_Flag				= &Sensor_12_Dev_NoPower_Flag;
	UserFarm[12].Dev_Error_Flag					= &Sensor_12_Dev_Error_Flag;
	UserFarm[12].RSSI_Status_Counter			= &Sensor_12_RSSI_Status_Counter;
	
	UserFarm[13].Temperature					= &Farm_13_Temperature;
	UserFarm[13].Humidity						= &Farm_13_Humidity;
	UserFarm[13].CO2							= &Farm_13_CO2;
	UserFarm[13].RSSI							= &Sensor_13_RSSI;
	UserFarm[13].RSSI_Status					= &Sensor_13_RSSI_Status;
	UserFarm[13].ON_OFF							= &Sensor_13_ON_OFF;
	UserFarm[13].Main_Status					= &Sensor_13_Main_Status;
	UserFarm[13].Page_Status					= &Sensor_13_Page_Status;
	UserFarm[13].Power							= &Sensor_13_Power;
	UserFarm[13].Dev_High_Temperature_Flag		= &Sensor_13_Dev_High_Temperature_Flag;
	UserFarm[13].Dev_Low_Temperature_Flag		= &Sensor_13_Dev_Low_Temperature_Flag;
	UserFarm[13].Dev_Air_Error_Flag				= &Sensor_13_Dev_Air_Error_Flag;
	UserFarm[13].Dev_NoPower_Flag				= &Sensor_13_Dev_NoPower_Flag;
	UserFarm[13].Dev_Error_Flag					= &Sensor_13_Dev_Error_Flag;
	UserFarm[13].RSSI_Status_Counter			= &Sensor_13_RSSI_Status_Counter;
	
	UserFarm[14].Temperature					= &Farm_14_Temperature;
	UserFarm[14].Humidity						= &Farm_14_Humidity;
	UserFarm[14].CO2							= &Farm_14_CO2;
	UserFarm[14].RSSI							= &Sensor_14_RSSI;
	UserFarm[14].RSSI_Status					= &Sensor_14_RSSI_Status;
	UserFarm[14].ON_OFF							= &Sensor_14_ON_OFF;
	UserFarm[14].Main_Status					= &Sensor_14_Main_Status;
	UserFarm[14].Page_Status					= &Sensor_14_Page_Status;
	UserFarm[14].Power							= &Sensor_14_Power;
	UserFarm[14].Dev_High_Temperature_Flag		= &Sensor_14_Dev_High_Temperature_Flag;
	UserFarm[14].Dev_Low_Temperature_Flag		= &Sensor_14_Dev_Low_Temperature_Flag;
	UserFarm[14].Dev_Air_Error_Flag				= &Sensor_14_Dev_Air_Error_Flag;
	UserFarm[14].Dev_NoPower_Flag				= &Sensor_14_Dev_NoPower_Flag;
	UserFarm[14].Dev_Error_Flag					= &Sensor_14_Dev_Error_Flag;
	UserFarm[14].RSSI_Status_Counter			= &Sensor_14_RSSI_Status_Counter;
	
	UserFarm[15].Temperature					= &Farm_15_Temperature;
	UserFarm[15].Humidity						= &Farm_15_Humidity;
	UserFarm[15].CO2							= &Farm_15_CO2;
	UserFarm[15].RSSI							= &Sensor_15_RSSI;
	UserFarm[15].RSSI_Status					= &Sensor_15_RSSI_Status;
	UserFarm[15].ON_OFF							= &Sensor_15_ON_OFF;
	UserFarm[15].Main_Status					= &Sensor_15_Main_Status;
	UserFarm[15].Page_Status					= &Sensor_15_Page_Status;
	UserFarm[15].Power							= &Sensor_15_Power;
	UserFarm[15].Dev_High_Temperature_Flag		= &Sensor_15_Dev_High_Temperature_Flag;
	UserFarm[15].Dev_Low_Temperature_Flag		= &Sensor_15_Dev_Low_Temperature_Flag;
	UserFarm[15].Dev_Air_Error_Flag				= &Sensor_15_Dev_Air_Error_Flag;
	UserFarm[15].Dev_NoPower_Flag				= &Sensor_15_Dev_NoPower_Flag;
	UserFarm[15].Dev_Error_Flag					= &Sensor_15_Dev_Error_Flag;
	UserFarm[15].RSSI_Status_Counter			= &Sensor_15_RSSI_Status_Counter;
	
	UserFarm[16].Temperature					= &Farm_16_Temperature;
	UserFarm[16].Humidity						= &Farm_16_Humidity;
	UserFarm[16].CO2							= &Farm_16_CO2;
	UserFarm[16].RSSI							= &Sensor_16_RSSI;
	UserFarm[16].RSSI_Status					= &Sensor_16_RSSI_Status;
	UserFarm[16].ON_OFF							= &Sensor_16_ON_OFF;
	UserFarm[16].Main_Status					= &Sensor_16_Main_Status;
	UserFarm[16].Page_Status					= &Sensor_16_Page_Status;
	UserFarm[16].Power							= &Sensor_16_Power;
	UserFarm[16].Dev_High_Temperature_Flag		= &Sensor_16_Dev_High_Temperature_Flag;
	UserFarm[16].Dev_Low_Temperature_Flag		= &Sensor_16_Dev_Low_Temperature_Flag;
	UserFarm[16].Dev_Air_Error_Flag				= &Sensor_16_Dev_Air_Error_Flag;
	UserFarm[16].Dev_NoPower_Flag				= &Sensor_16_Dev_NoPower_Flag;
	UserFarm[16].Dev_Error_Flag					= &Sensor_16_Dev_Error_Flag;
	UserFarm[16].RSSI_Status_Counter			= &Sensor_16_RSSI_Status_Counter;
	
	ManagerFarm[1].Delay_Time					= &User_01_Delay_Time;
	ManagerFarm[1].Cycle_Time					= &User_01_Cycle_Time;
	ManagerFarm[1].NowStatus					= Farm_User_NOP;
	ManagerFarm[2].Delay_Time					= &User_02_Delay_Time;
	ManagerFarm[2].Cycle_Time					= &User_02_Cycle_Time;
	ManagerFarm[2].NowStatus					= Farm_User_NOP;
	ManagerFarm[3].Delay_Time					= &User_03_Delay_Time;
	ManagerFarm[3].Cycle_Time					= &User_03_Cycle_Time;
	ManagerFarm[3].NowStatus					= Farm_User_NOP;
	ManagerFarm[4].Delay_Time					= &User_04_Delay_Time;
	ManagerFarm[4].Cycle_Time					= &User_04_Cycle_Time;
	ManagerFarm[4].NowStatus					= Farm_User_NOP;

}


//=================================================
#endif
//=================================================

