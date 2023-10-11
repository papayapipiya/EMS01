/*
 * eeprom.h
 *
 * Created: 2023/9/24 下午 09:49:55
 *  Author: papay
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_


#define LEN(x) sizeof(x) / sizeof(x[0])

//===========================================
// EEPROM 1M (131,072 x 8) Byte
// 位置
#define EEPROM_ADDRESS			0x50 //[正確位置]
#define i2c_EEPROM_ADDRESS		0x50
//===========================================

unsigned char IIC_Read_Error_Count;
unsigned char IIC_Write_Error_Count;
uint8_t	i2c_write_buffer[30];
uint8_t	i2c_Read_buffer[30];
static bool IIC_Debug_Flag;

IIC_Dev_t		eeprom_write_packet;
IIC_Dev_t		eeprom_Read_packet;

bool IIC_Read_packet(IIC_Dev_t OBJ);
bool IIC_Write(IIC_Dev_t OBJ);
bool IIC_Write_No_Stop(IIC_Dev_t OBJ);
void show_eeprom_ok(bool tmpFlag);
void show_read_eeprom_ok(bool tmpFlag);
void eEEPROM_initial(Item_Dev_t *tmpItem);


#define eMAP_eID			0		//	16 Byte
#define eMAP_eSSID			0x10	//	64 Byte
#define eMAP_ePW			0x50	//	64 Byte
#define eMAP_eOT			0x90	//	16 Byte
#define eMAP_ewOT			0xA0	//	16 Byte
#define eMAP_eUT			0xB0	//	16 Byte
#define eMAP_ewUT			0xC0	//	16 Byte
#define eMAP_eTolerance	0xD0	//	16 Byte

#endif /* EEPROM_H_ */