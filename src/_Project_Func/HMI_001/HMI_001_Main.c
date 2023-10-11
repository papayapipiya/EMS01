#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//=================================================
#ifdef HMI_001
//=================================================




unsigned int get_sensor_int(unsigned char tmp_A,unsigned char tmp_B)
{
	UINT16u_t	tmp16;
	UINT8u_t	tmp8;
	tmp16.byte[1] =  tmp_A;
	tmp16.byte[0] =  tmp_B;
	return tmp16.word;
}


#define UnUse			0
#define Good_RSSI		1
#define Dead_RSSI		3
#define RSSI_Count_Max	10

#define Main_Good	1
#define Main_Error	2

#define Dev_Good				1
#define Dev_Low_Temperature		2
#define Dev_High_Temperature	3
#define Dev_NoPower				4
#define Dev_Air_Error			5
#define Dev_Error				6



void Sensor_To_D_BUF(void)
{
	UINT16u_t	tmp16;
	UINT8u_t	tmp8;
	Alarm_Flag = false;
	for(unsigned char i = 1; i < 17; i ++)
	{
		ptrFarm = &UserFarm[i];
		*ptrFarm->Temperature		= get_sensor_int(dev_value[i][8],	dev_value[i][9]);
		*ptrFarm->Humidity			= get_sensor_int(dev_value[i][10],	dev_value[i][11]);
		*ptrFarm->CO2				= get_sensor_int(dev_value[i][14],	dev_value[i][15]);
		*ptrFarm->Power				= get_sensor_int(dev_value[i][20],	dev_value[i][21]);
		
		switch(*ptrFarm->ON_OFF)
		{
			case false:
				*ptrFarm->RSSI			= UnUse;
				*ptrFarm->Main_Status	= UnUse;
				*ptrFarm->Page_Status	= UnUse;
				*ptrFarm->RSSI_Status	= UnUse;
				break;
				
			case true:
				//	將判斷後的RSSI Level存下來
				//	- - - - - - - - - - - - - - - - - - - - - - - -
				*ptrFarm->RSSI_Status	= dev_ptr[i].RSSI_Status;
				*ptrFarm->RSSI			= dev_ptr[i].RSSI;
				
				//	判斷RSSI是否異常	
				//	- - - - - - - - - - - - - - - - - - - - - - - -			
				if(*ptrFarm->RSSI_Status == Dead_RSSI)
				{
					if(*ptrFarm->RSSI_Status_Counter < RSSI_Count_Max)
					{
						*ptrFarm->RSSI_Status_Counter++;
					}
					else 
					{
						*ptrFarm->Dev_Error_Flag = true;
					}
				}
				else
				{
					*ptrFarm->RSSI_Status_Counter = 0;
					*ptrFarm->Dev_Error_Flag = false;
				}
				
				// 判斷設備氣體異常
				//	- - - - - - - - - - - - - - - - - - - - - - - -
				if(*ptrFarm->Dev_Air_Error_Flag == true)
				{
					if(*ptrFarm->CO2 < (Gas_Alarm - NoSense_Gas_Alarm))
					{
						*ptrFarm->Dev_Air_Error_Flag = false;
					}
				}
				else
				{
					if(*ptrFarm->CO2 > (Gas_Alarm + NoSense_Gas_Alarm))
					{
						*ptrFarm->Dev_Air_Error_Flag = true;
					}
				}
				
				// 判斷設備是否停電
				//	- - - - - - - - - - - - - - - - - - - - - - - -
				if(*ptrFarm->Power < 0x02A0)
				{
					*ptrFarm->Dev_NoPower_Flag = true;
				}
				else
				{
					*ptrFarm->Dev_NoPower_Flag = false;
				}
				
				// 判斷設備是否高溫
				//	- - - - - - - - - - - - - - - - - - - - - - - -
				if(*ptrFarm->Dev_High_Temperature_Flag == true)
				{
					if(*ptrFarm->Temperature < High_Temperature)
					{
						*ptrFarm->Dev_High_Temperature_Flag = false;
					}
				}
				else
				{
					if(*ptrFarm->Temperature > (High_Temperature + NoSense_Temperature))
					{
						*ptrFarm->Dev_High_Temperature_Flag = true;
					}
				}

				
				// 判斷設備是否低溫
				//	- - - - - - - - - - - - - - - - - - - - - - - -
				if(*ptrFarm->Dev_Low_Temperature_Flag == true)
				{
					if(*ptrFarm->Temperature > Low_Temperature)
					{
						*ptrFarm->Dev_Low_Temperature_Flag = false;
					}
				}
				else
				{
					if(*ptrFarm->Temperature < (Low_Temperature - NoSense_Temperature))
					{
						*ptrFarm->Dev_Low_Temperature_Flag = true;
					}
				}
				
				
				
				//========================================================================
				//========================================================================
				//========================================================================
				if(*ptrFarm->Dev_Error_Flag == true)
				{
					*ptrFarm->Main_Status	= Main_Error;				// Main 異常顯示
					*ptrFarm->Page_Status	= Dev_Error;				// Main 設備異常顯示
					Alarm_Flag = true;
					break;
				}
				
				if(*ptrFarm->Dev_Air_Error_Flag == true)
				{
					*ptrFarm->Main_Status	= Main_Error;				// Main 異常顯示
					*ptrFarm->Page_Status	= Dev_Air_Error;			// Main 氣體顯示
					Alarm_Flag = true;
					break;
				}	
				
				if(*ptrFarm->Dev_NoPower_Flag == true)
				{
					*ptrFarm->Main_Status	= Main_Error;				// Main 異常顯示
					*ptrFarm->Page_Status	= Dev_NoPower;				// Main 停電顯示
					Alarm_Flag = true;
					break;
				}				
				
				if(*ptrFarm->Dev_High_Temperature_Flag == true)
				{
					*ptrFarm->Main_Status	= Main_Error;				// Main 異常顯示
					*ptrFarm->Page_Status	= Dev_High_Temperature;		// Main 高溫顯示
					Alarm_Flag = true;
					break;
				}						
				
				
				if(*ptrFarm->Dev_Low_Temperature_Flag == true)
				{
					*ptrFarm->Main_Status	= Main_Error;				// Main 異常顯示
					*ptrFarm->Page_Status	= Dev_Low_Temperature;		// Main 低溫顯示
					Alarm_Flag = true;
					break;
				}
				
				*ptrFarm->Main_Status	= Main_Good;
				*ptrFarm->Page_Status	= Dev_Good;
				break;
		}
	}


	Sensor_01_08_ON_OFF = BitToByte(	Sensor_08_ON_OFF, Sensor_07_ON_OFF, Sensor_06_ON_OFF, Sensor_05_ON_OFF,
										Sensor_04_ON_OFF, Sensor_03_ON_OFF, Sensor_02_ON_OFF, Sensor_01_ON_OFF);
	Sensor_09_16_ON_OFF = BitToByte(	Sensor_16_ON_OFF, Sensor_15_ON_OFF, Sensor_14_ON_OFF, Sensor_13_ON_OFF,
										Sensor_12_ON_OFF, Sensor_11_ON_OFF, Sensor_10_ON_OFF, Sensor_09_ON_OFF);


	if(ManagerFarm[1].Last_Delay_Time	!= User_01_Delay_Time)			Save_NVM_Flag	=	true;
	if(ManagerFarm[1].Last_Cycle_Time	!= User_01_Cycle_Time)			Save_NVM_Flag	=	true;
	if(ManagerFarm[2].Last_Delay_Time	!= User_02_Delay_Time)			Save_NVM_Flag	=	true;
	if(ManagerFarm[2].Last_Cycle_Time	!= User_02_Cycle_Time)			Save_NVM_Flag	=	true;
	if(ManagerFarm[3].Last_Delay_Time	!= User_03_Delay_Time)			Save_NVM_Flag	=	true;
	if(ManagerFarm[3].Last_Cycle_Time	!= User_03_Cycle_Time)			Save_NVM_Flag	=	true;
	if(ManagerFarm[4].Last_Delay_Time	!= User_04_Delay_Time)			Save_NVM_Flag	=	true;
	if(ManagerFarm[4].Last_Cycle_Time	!= User_04_Cycle_Time)			Save_NVM_Flag	=	true;
	if(Last_Sensor_01_08_ON_OFF			!= Sensor_01_08_ON_OFF)			Save_NVM_Flag	=	true;
	if(Last_Sensor_09_16_ON_OFF			!= Sensor_09_16_ON_OFF)			Save_NVM_Flag	=	true;
	if(Last_FarmNumber					!= FarmNumber)					Save_NVM_Flag	=	true;
	if(Last_NoSense_Temperature			!= NoSense_Temperature)			Save_NVM_Flag	=	true;
	if(Last_Low_Temperature				!= Low_Temperature)				Save_NVM_Flag	=	true;
	if(Last_High_Temperature			!= High_Temperature)			Save_NVM_Flag	=	true;
	if(Last_Gas_Alarm					!= Gas_Alarm)					Save_NVM_Flag	=	true;
	if(Last_NoSense_Gas_Alarm			!= NoSense_Gas_Alarm)			Save_NVM_Flag	=	true;
	
	
	if(Save_NVM_Flag == true)
	{
		ManagerFarm[1].Last_Delay_Time	= User_01_Delay_Time;
		ManagerFarm[1].Last_Cycle_Time	= User_01_Cycle_Time;
		ManagerFarm[2].Last_Delay_Time	= User_02_Delay_Time;
		ManagerFarm[2].Last_Cycle_Time	= User_02_Cycle_Time;
		ManagerFarm[3].Last_Delay_Time	= User_03_Delay_Time;
		ManagerFarm[3].Last_Cycle_Time	= User_03_Cycle_Time;
		ManagerFarm[4].Last_Delay_Time	= User_04_Delay_Time;
		ManagerFarm[4].Last_Cycle_Time	= User_04_Cycle_Time;
		Last_Sensor_01_08_ON_OFF		= Sensor_01_08_ON_OFF;
		Last_Sensor_09_16_ON_OFF		= Sensor_09_16_ON_OFF;
		Last_FarmNumber					= FarmNumber;
		Last_NoSense_Temperature		= NoSense_Temperature;
		Last_Low_Temperature			= Low_Temperature;
		Last_High_Temperature			= High_Temperature;
		Last_Gas_Alarm					= Gas_Alarm;
		Last_NoSense_Gas_Alarm			= NoSense_Gas_Alarm;
		if(Save_NVM_Count == 0)			Save_NVM_Count = 1000;	
	}
}


void Chk_dev_ptr_RSSI(void)
{
	unsigned char i;
	UINT16u_t	tmp16;
	for(i = 0; i < LORA_Dev_List_Max; i++)
	{
		tmp16.byte[1] = dev_value[i][27];
		tmp16.byte[0] = dev_value[i][28];
		dev_ptr[i].RSSI =  0xffff - tmp16.word;
		
		if(dev_ptr[i].RSSI > 200)		dev_ptr[i].RSSI_Status = 3;
		else if(dev_ptr[i].RSSI > 100)	dev_ptr[i].RSSI_Status = 2;
		else if(dev_ptr[i].RSSI < 100)	dev_ptr[i].RSSI_Status = 1;
		
		if(dev_ptr[i].RSSI > 200)		dev_ptr[i].RSSI = 999;
	}  
}


void Alarm_txt_Function(volatile Farm_User_Tmp_t *tmpManager)
{
	if(tmpManager->NowStatus == Farm_User_NOP)						uart_str("A_NOP..         \0");
	if(tmpManager->NowStatus == Farm_User_ALARM_Waitting_ON)		uart_str("A_Wait_ON..     \0");
	if(tmpManager->NowStatus == Farm_User_ALARM_ON)					uart_str("A_ON..          \0");
	if(tmpManager->NowStatus == Farm_User_ALARM_ON_Delay)			uart_str("A_ON_Dy..       \0");
	if(tmpManager->NowStatus == Farm_User_ALARM_OFF)				uart_str("A_OFF..         \0");
	if(tmpManager->NowStatus == Farm_User_Reset_ON)					uart_str("A_Reset_ON..    \0");
	if(tmpManager->NowStatus == Farm_User_Reset_ON_Delay)			uart_str("A_Reset_ON_Dy.. \0");
	if(tmpManager->NowStatus == Farm_User_Reset_OFF)				uart_str("A_Reset_OFF..   \0");
	if(tmpManager->NowStatus == Farm_User_ALARM_Waitting_Cycle)		uart_str("A_Cycle..       \0");
	if(tmpManager->NowStatus == Farm_User_Delay)					uart_str("A_Delay..       \0");
}


void Alarm_Out_ON_Function(volatile Farm_User_Tmp_t *tmpManager)
{
	if(tmpManager == &ManagerFarm[1])	Manager_01_Alarm_ON
	if(tmpManager == &ManagerFarm[2])	Manager_02_Alarm_ON
	if(tmpManager == &ManagerFarm[3])	Manager_03_Alarm_ON
	if(tmpManager == &ManagerFarm[4])	Manager_04_Alarm_ON
}

void Alarm_Out_OFF_Function(volatile Farm_User_Tmp_t *tmpManager)
{
	if(tmpManager == &ManagerFarm[1])	Manager_01_Alarm_OFF
	if(tmpManager == &ManagerFarm[2])	Manager_02_Alarm_OFF
	if(tmpManager == &ManagerFarm[3])	Manager_03_Alarm_OFF
	if(tmpManager == &ManagerFarm[4])	Manager_04_Alarm_OFF
}

void Alarm_Function(volatile Farm_User_Tmp_t *tmpManager)
{
	Alarm_txt_Function(tmpManager);
	switch(tmpManager->NowStatus)
	{
		case Farm_User_NOP:
			if(Alarm_Flag == true)
			{
				tmpManager->NowStatus = Farm_User_ALARM_Waitting_ON;	
			}
			tmpManager->Counter_Delay_Time = 0;
			tmpManager->Counter_Cycle_Time = 0;
			uart_send_word(Debug_COM, hex_to_bcd(tmpManager->Counter_Delay_Time));
			break;
			
		case Farm_User_ALARM_Waitting_ON:
			tmpManager->Counter_Delay_Time++;
			uart_send_word(Debug_COM, hex_to_bcd(tmpManager->Counter_Delay_Time));
			if(Alarm_Flag == false)
			{
				Alarm_Out_OFF_Function(tmpManager);
				tmpManager->NowStatus = Farm_User_NOP;
				break;
			}
			if(tmpManager->Counter_Delay_Time >= *tmpManager->Delay_Time)
			{
				tmpManager->Counter_Delay_Time = 0;
				tmpManager->NowStatus = Farm_User_ALARM_ON;	
			}
			break;
			
		case Farm_User_ALARM_ON:
			Alarm_Out_ON_Function(tmpManager);
			tmpManager->NowStatus = Farm_User_ALARM_ON_Delay;	
			break;
		
		case Farm_User_ALARM_ON_Delay:
			tmpManager->Counter_Delay_Time++;
			if(tmpManager->Counter_Delay_Time > 30)
			{
				tmpManager->Counter_Delay_Time = 0;
				tmpManager->NowStatus = Farm_User_ALARM_OFF;
			}
			break;
			
		case Farm_User_ALARM_OFF:
			Alarm_Out_OFF_Function(tmpManager);
			tmpManager->NowStatus = Farm_User_Reset_ON;
			break;
			
		case Farm_User_Reset_ON:
			Manager_04_Alarm_ON
			tmpManager->NowStatus = Farm_User_Reset_ON_Delay;
			break;
			
			
		case Farm_User_Reset_ON_Delay:
			tmpManager->Counter_Delay_Time++;
			if(tmpManager->Counter_Delay_Time > 3)
			{
				tmpManager->Counter_Delay_Time = 0;
				tmpManager->NowStatus = Farm_User_Reset_OFF;
			}
			break;
		
		case Farm_User_Reset_OFF:
			Manager_04_Alarm_OFF
			tmpManager->NowStatus = Farm_User_ALARM_Waitting_Cycle;
			break;
		
		case Farm_User_ALARM_Waitting_Cycle:
			tmpManager->Counter_Cycle_Time++;
			uart_send_word(Debug_COM, hex_to_bcd(tmpManager->Counter_Cycle_Time));
			if(Alarm_Flag == false)
			{
				tmpManager->NowStatus = Farm_User_NOP;
				Alarm_Out_OFF_Function(tmpManager);
				break;
			}
			if(tmpManager->Counter_Cycle_Time >= *tmpManager->Cycle_Time)
			{
				tmpManager->Counter_Cycle_Time = 0;
				tmpManager->NowStatus = Farm_User_Delay;
			}		
			break;
			
		case Farm_User_Delay:		
			if(Alarm_Flag == false)
			{
				Alarm_Out_OFF_Function(tmpManager);
				Manager_04_Alarm_OFF
				tmpManager->NowStatus = Farm_User_NOP;
			}
			else
			{
				Manager_04_Alarm_OFF
				tmpManager->NowStatus = Farm_User_ALARM_ON;
			}
			uart_send_word(Debug_COM, hex_to_bcd(tmpManager->Counter_Cycle_Time));
			break;
	}
	
	uart_str("\t\0");
}

void MainFunc(void)
{
	unsigned char *tmp_ptr;
	unsigned char tmp_ptr_count;	
	UINT16u_t	tmp16;
	UINT8u_t	tmp8;
	
	if(inital_Flag == false)
	{
		inital_D_BUF();
		inital_Flag = true;
	}
	
	HMI_Modbus_Function();
  
	Chk_dev_ptr_RSSI();
  
	Sensor_To_D_BUF();
	
	if(Save_NVM_Count > 0) Save_NVM_Count--;	
	if(Save_NVM_Flag == true && Save_NVM_Count == 0)
	{
		Save_HMI_Ref_in_NVM();
		uart_str("\rNVM Save..\r\0");
		Save_NVM_Flag = false;
	}
	
	
	if(Alarm_Main_Counter == Alarm_Main_Counter_Max)
	{
		Alarm_Main_Counter = 0;
		if(Alarm_PowerON_Counter < Alarm_PowerON_Counter_Max)
		{
			uart_str("PowerON..\r\0");
			Alarm_PowerON_Counter++;
		}
		else
		{
			if(Save_NVM_Flag == true)	uart_str("NVM \0");
			if(Alarm_Flag == true)		uart_str("!!(Q[]Q)!! \t\0");
			if(Alarm_Flag == false)		uart_str("(^o^)Yeah!!\t\0");
			Alarm_Function(&ManagerFarm[1]);
			Alarm_Function(&ManagerFarm[2]);
			Alarm_Function(&ManagerFarm[3]);
			//Alarm_Function(&ManagerFarm[4]);
			uart_str("\r\0");
		}
	}
	
	
}






//=================================================
#endif
//=================================================