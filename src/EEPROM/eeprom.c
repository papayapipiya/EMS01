/*
 * eeprom.c
 *
 * Created: 2023/9/24 下午 09:49:42
 *  Author: papay
 */ 


#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

void eEEPROM_initial(Item_Dev_t *tmpItem)
{
	
	//uart_str_COM(Debug_COM, "\r eID_initial();");
	
	//================================================================
	// 2023-03-03 寫入
	tmpItem->Write							= &eeprom_write_packet;
	tmpItem->Write->address					= i2c_EEPROM_ADDRESS;
	tmpItem->Write->Write_Buf				= i2c_write_buffer;
	tmpItem->Write->Debug					= &IIC_Debug_Flag;

	//================================================================
	// 2023-03-03 讀取
	
	tmpItem->Read							= &eeprom_Read_packet;
	tmpItem->Read->address					= i2c_EEPROM_ADDRESS;
	tmpItem->Read->Write_Buf				= i2c_write_buffer;
	tmpItem->Read->Write_length				= 2;
	
	tmpItem->Read->Read_Buf					= i2c_Read_buffer;
	tmpItem->Read->Debug					= &IIC_Debug_Flag;
}


bool IIC_Read(IIC_Dev_t OBJ)
{
	packet.address = OBJ.address;
	packet.data_length = OBJ.Read_length;
	packet.data = OBJ.Read_Buf;
	unsigned int count = 0;

	while(1)
	{
		switch(i2c_master_read_packet_wait(&i2c_master_instance, &packet))
		{
			case STATUS_OK:
			return true;
			
			default:
			uTXByte(Debug_COM, 'R');
			uTXByte(Debug_COM, count + 0x30);
			if(count++ == 3)	return false;
			break;
		}
	}
}


bool IIC_Read_packet(IIC_Dev_t OBJ)
{
	packet.address = OBJ.address;
	packet.data_length = OBJ.Read_length;
	packet.data = OBJ.Read_Buf;


	if(IIC_Write(OBJ) == true)
	{
		uart_str_COM(OBJ.Debug, "[");
		uart_str_COM(OBJ.Debug, OBJ.Read_Msg);
		uart_str_COM(OBJ.Debug, "_Dummy_buffer OK] ");
		IIC_Write_Error_Count = 0;
	}
	else
	{
		uart_str_COM(OBJ.Debug, "[");
		uart_str_COM(OBJ.Debug, OBJ.Read_Msg);
		uart_str_COM(OBJ.Debug, "_Dummy_buffer NG] ");
		if(IIC_Write_Error_Count < 3)
		{
			IIC_Write_Error_Count++;
		}
		else
		{
			while(1);
		}
	}
	
	//	正式讀取資料
	if(IIC_Read(OBJ) == true)
	{
		uart_str_COM(OBJ.Debug, "[");
		uart_str_COM(OBJ.Debug, OBJ.Read_Msg);
		uart_str_COM(OBJ.Debug, " OK] ");
		IIC_Read_Error_Count = 0;
	}
	else
	{
		uart_str_COM(OBJ.Debug, "[");
		uart_str_COM(OBJ.Debug, OBJ.Read_Msg);
		uart_str_COM(OBJ.Debug, " NG] ");
		if(IIC_Read_Error_Count < 3)
		{
			IIC_Read_Error_Count++;
		}
		else
		{
			while(1);
		}
	}
}




bool IIC_Write(IIC_Dev_t OBJ)
{
	packet.address = OBJ.address;					// I2C Addr 位置
	packet.data_length = OBJ.Write_length ;			// I2C 寫入長度
	packet.data = OBJ.Write_Buf ;					// I2C Reg位置 + 資料
	unsigned int count = 0;
	while(1)
	{
		switch(i2c_master_write_packet_wait(&i2c_master_instance, &packet))
		{
			case STATUS_OK:
			return true;
			
			//case STATUS_ERR_TIMEOUT:
			//return false;
			
			default:
			uTXByte(Debug_COM, 'W');
			uTXByte(Debug_COM, count + 0x30);
			if(count++ == 3)	return false;
			break;
		}
	}
}

bool IIC_Write_No_Stop(IIC_Dev_t OBJ)
{
	packet.address = OBJ.address;					// I2C Addr 位置
	packet.data_length = OBJ.Write_length ;			// I2C 寫入長度
	packet.data = OBJ.Write_Buf ;					// I2C Reg位置 + 資料
	unsigned int count = 0;

	while(1)
	{
		switch(i2c_master_write_packet_wait_no_stop(&i2c_master_instance, &packet))
		{
			case STATUS_OK:
			return true;
			
			case STATUS_ERR_TIMEOUT:
			return false;
			
			default:
			uTXByte(Debug_COM, 'W');
			uTXByte(Debug_COM, count + 0x30);
			if(count++ == 3)	return false;
			break;
		}
	}
}




void show_eeprom_ok(bool tmpFlag)
{
	uart_str_COM(Debug_COM,  " --> [EEPEOM_Write ");
	if(tmpFlag)
		uart_str_COM(Debug_COM,  "OK]");
	else
		uart_str_COM(Debug_COM,  "NG]");
}

void show_read_eeprom_ok(bool tmpFlag)
{
	uart_str_COM(Debug_COM,  " --> [EEPEOM_Read  ");
	if(tmpFlag)
		uart_str_COM(Debug_COM,  "OK]");
	else
		uart_str_COM(Debug_COM,  "NG]");
}