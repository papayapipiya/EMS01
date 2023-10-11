#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//=================================================
#ifdef HMI_001
//=================================================

unsigned char BitToByte(bool b7,bool b6,bool b5,bool b4,bool b3,bool b2,bool b1,bool b0)
{
	UINT8u_t	tmp8;
	tmp8.bit.b7 = b7;
	tmp8.bit.b6 = b6;
	tmp8.bit.b5 = b5;
	tmp8.bit.b4 = b4;
	tmp8.bit.b3 = b3;
	tmp8.bit.b2 = b2;
	tmp8.bit.b1 = b1;
	tmp8.bit.b0 = b0;
	return tmp8.byte;
}


void ByteToBit(bool *b7,bool *b6,bool *b5,bool *b4,bool *b3,bool *b2,bool *b1,bool *b0, unsigned char tmpVal)
{
	UINT8u_t	tmp8;
	tmp8.byte = tmpVal;
	*b7 = tmp8.bit.b7;
	*b6 = tmp8.bit.b6;
	*b5 = tmp8.bit.b5;
	*b4 = tmp8.bit.b4;
	*b3 = tmp8.bit.b3;
	*b2 = tmp8.bit.b2;
	*b1 = tmp8.bit.b1;
	*b0 = tmp8.bit.b0;

}


void Save_HMI_Ref_in_NVM(void)
{
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	UINT32u_t	tmp32;
	UINT16u_t	tmp16;
	
	do
	{
		error_code = nvm_read_buffer(
		HMI_NVM  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	tmp16.word = FarmNumber;
	tmp_page_buffer[0] = tmp16.byte[1];
	tmp_page_buffer[1] = tmp16.byte[0];
	
	tmp16.word = NoSense_Temperature;
	tmp_page_buffer[2] = tmp16.byte[1];
	tmp_page_buffer[3] = tmp16.byte[0];
	
	tmp16.word = Low_Temperature;
	tmp_page_buffer[4] = tmp16.byte[1];
	tmp_page_buffer[5] = tmp16.byte[0];
	
	tmp16.word = High_Temperature;
	tmp_page_buffer[6] = tmp16.byte[1];
	tmp_page_buffer[7] = tmp16.byte[0];
	
	tmp16.word = Gas_Alarm;
	tmp_page_buffer[8] = tmp16.byte[1];
	tmp_page_buffer[9] = tmp16.byte[0];

	tmp16.word = NoSense_Gas_Alarm;
	tmp_page_buffer[10] = tmp16.byte[1];
	tmp_page_buffer[11] = tmp16.byte[0];
	
	tmp_page_buffer[12] = BitToByte(	Sensor_08_ON_OFF, Sensor_07_ON_OFF, Sensor_06_ON_OFF, Sensor_05_ON_OFF,
										Sensor_04_ON_OFF, Sensor_03_ON_OFF, Sensor_02_ON_OFF, Sensor_01_ON_OFF);
	tmp_page_buffer[13] = BitToByte(	Sensor_16_ON_OFF, Sensor_15_ON_OFF, Sensor_14_ON_OFF, Sensor_13_ON_OFF,
										Sensor_12_ON_OFF, Sensor_11_ON_OFF, Sensor_10_ON_OFF, Sensor_09_ON_OFF);
	
	tmp16.word = User_01_Delay_Time;
	tmp_page_buffer[14] = tmp16.byte[1];
	tmp_page_buffer[15] = tmp16.byte[0];
	tmp16.word = User_01_Cycle_Time;
	tmp_page_buffer[16] = tmp16.byte[1];
	tmp_page_buffer[17] = tmp16.byte[0];

	tmp16.word = User_02_Delay_Time;
	tmp_page_buffer[18] = tmp16.byte[1];
	tmp_page_buffer[19] = tmp16.byte[0];
	tmp16.word = User_02_Cycle_Time;
	tmp_page_buffer[20] = tmp16.byte[1];
	tmp_page_buffer[21] = tmp16.byte[0];
	
	tmp16.word = User_03_Delay_Time;
	tmp_page_buffer[22] = tmp16.byte[1];
	tmp_page_buffer[23] = tmp16.byte[0];
	tmp16.word = User_03_Cycle_Time;
	tmp_page_buffer[24] = tmp16.byte[1];
	tmp_page_buffer[25] = tmp16.byte[0];
	
	tmp16.word = User_04_Delay_Time;
	tmp_page_buffer[26] = tmp16.byte[1];
	tmp_page_buffer[27] = tmp16.byte[0];
	tmp16.word = User_04_Cycle_Time;
	tmp_page_buffer[28] = tmp16.byte[1];
	tmp_page_buffer[29] = tmp16.byte[0];

	
	do
	{
		error_code = nvm_erase_row(
		HMI_NVM * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	do
	{
		error_code = nvm_write_buffer(
		HMI_NVM  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
}



void Load_HMI_Ref_in_NVM(void)
{
	UINT32u_t	tmp32;
	UINT16u_t	tmp16;
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	do
	{
		error_code = nvm_read_buffer(
		HMI_NVM  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	
	tmp16.byte[1] = tmp_page_buffer[0];
	tmp16.byte[0] = tmp_page_buffer[1];
	FarmNumber = tmp16.word;
	
	tmp16.byte[1] = tmp_page_buffer[2];
	tmp16.byte[0] = tmp_page_buffer[3];
	NoSense_Temperature = tmp16.word;

	tmp16.byte[1] = tmp_page_buffer[4];
	tmp16.byte[0] = tmp_page_buffer[5];
	Low_Temperature = tmp16.word;

	tmp16.byte[1] = tmp_page_buffer[6];
	tmp16.byte[0] = tmp_page_buffer[7];
	High_Temperature = tmp16.word;

	tmp16.byte[1] = tmp_page_buffer[8];
	tmp16.byte[0] = tmp_page_buffer[9];
	Gas_Alarm = tmp16.word;

	tmp16.byte[1] = tmp_page_buffer[10];
	tmp16.byte[0] = tmp_page_buffer[11];
	NoSense_Gas_Alarm = tmp16.word;
	
	ByteToBit(	&Sensor_08_ON_OFF, &Sensor_07_ON_OFF, &Sensor_06_ON_OFF, &Sensor_05_ON_OFF,
				&Sensor_04_ON_OFF, &Sensor_03_ON_OFF, &Sensor_02_ON_OFF, &Sensor_01_ON_OFF,
				tmp_page_buffer[12]);
	Sensor_01_08_ON_OFF = tmp_page_buffer[12];
	ByteToBit(	&Sensor_16_ON_OFF, &Sensor_15_ON_OFF, &Sensor_14_ON_OFF, &Sensor_13_ON_OFF,
				&Sensor_12_ON_OFF, &Sensor_11_ON_OFF, &Sensor_10_ON_OFF, &Sensor_09_ON_OFF,
				tmp_page_buffer[13]);
	Sensor_09_16_ON_OFF = tmp_page_buffer[13];
	
	tmp16.byte[1] = tmp_page_buffer[14];
	tmp16.byte[0] = tmp_page_buffer[15];
	User_01_Delay_Time = tmp16.word;
	
	tmp16.byte[1] = tmp_page_buffer[16];
	tmp16.byte[0] = tmp_page_buffer[17];
	User_01_Cycle_Time = tmp16.word;
	
	tmp16.byte[1] = tmp_page_buffer[18];
	tmp16.byte[0] = tmp_page_buffer[19];
	User_02_Delay_Time = tmp16.word;
	
	tmp16.byte[1] = tmp_page_buffer[20];
	tmp16.byte[0] = tmp_page_buffer[21];
	User_02_Cycle_Time = tmp16.word;
	
	tmp16.byte[1] = tmp_page_buffer[22];
	tmp16.byte[0] = tmp_page_buffer[23];
	User_03_Delay_Time = tmp16.word;
	
	tmp16.byte[1] = tmp_page_buffer[24];
	tmp16.byte[0] = tmp_page_buffer[25];
	User_03_Cycle_Time = tmp16.word;
	
	tmp16.byte[1] = tmp_page_buffer[26];
	tmp16.byte[0] = tmp_page_buffer[27];
	User_04_Delay_Time = tmp16.word;
	
	tmp16.byte[1] = tmp_page_buffer[28];
	tmp16.byte[0] = tmp_page_buffer[29];
	User_04_Cycle_Time= tmp16.word;

	
	ManagerFarm[1].Last_Delay_Time = User_01_Delay_Time;
	ManagerFarm[1].Last_Cycle_Time = User_01_Cycle_Time;
	ManagerFarm[2].Last_Delay_Time = User_02_Delay_Time;
	ManagerFarm[2].Last_Cycle_Time = User_02_Cycle_Time;
	ManagerFarm[3].Last_Delay_Time = User_03_Delay_Time;
	ManagerFarm[3].Last_Cycle_Time = User_03_Cycle_Time;
	ManagerFarm[4].Last_Delay_Time = User_04_Delay_Time;
	ManagerFarm[4].Last_Cycle_Time = User_04_Cycle_Time;

	
	Last_Sensor_01_08_ON_OFF	= Sensor_01_08_ON_OFF;
	Last_Sensor_09_16_ON_OFF	= Sensor_09_16_ON_OFF;
	Last_FarmNumber				= FarmNumber;
	Last_NoSense_Temperature	= NoSense_Temperature;
	Last_Low_Temperature		= Low_Temperature;
	Last_High_Temperature		= High_Temperature;
	Last_Gas_Alarm				= Gas_Alarm;
	Last_NoSense_Gas_Alarm		= NoSense_Gas_Alarm;

}

//=================================================
#endif
//=================================================