#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//=================================================
#ifdef HMI_001
//=================================================

void HMI_inital(void)
{
	HMI_RS485_OK_Flag = 0;
	HMI_RS485_CMD_Index = 0;
	HMI_RS485_LORA_OUT_Index = 0;
	HMI_Status = HMI_Print_01;
	Load_HMI_Ref_in_NVM();
}


void Reset_PLC_Catch_Value(void)
{
	HMI_RS485_RX_Buf_Count = 0;
	HMI_RS485_Time_Count = 0;
	HMI_RS485_OK_Flag = false;
}


unsigned char str_to_hex_char(unsigned char *ptr)
{
	unsigned int tmp, tmp2;
	tmp = 0;
	tmp2 =	ascii_to_hex(*(ptr));
	tmp2 = tmp2 << 4;
	tmp = tmp2;
	tmp = tmp | ascii_to_hex(*(ptr+1));
	return tmp;
}


unsigned int str_to_hex_int(unsigned char *ptr)
{
	unsigned int tmp, tmp2;
	tmp2 =	ascii_to_hex(*ptr);
	tmp2 = tmp2 << 12;
	tmp = tmp2;
	tmp2 =	ascii_to_hex(*(ptr+1));
	tmp2 = tmp2 << 8;
	tmp = tmp | tmp2;
	tmp2 =	ascii_to_hex(*(ptr+2));
	tmp2 = tmp2 << 4;
	tmp = tmp | tmp2;
	tmp = tmp | ascii_to_hex(*(ptr+3));
	return tmp;
}





void PLC_Header_Format_str(void)
{
	UINT8u_t	tmp8;
	unsigned char *tmp_ptr;
	
	tmp_ptr = &tmp_str[0];
	tmp8.byte = PLC_Buf.No;
	*tmp_ptr = hex_to_ascii(tmp8.HL.H);
	*(tmp_ptr+1) = hex_to_ascii(tmp8.HL.L);
	
	tmp8.byte = PLC_Buf.Cmd;
	*(tmp_ptr+2) = hex_to_ascii(tmp8.HL.H);
	*(tmp_ptr+3) = hex_to_ascii(tmp8.HL.L);
}





uint8_t HMI_Define_MyRS485Tmp[16][8] =
{
	//第二間雞舍
	{0x01, 0x03, 0x18, 0x38, 0x00, 0x01, 0x03, 0x67},	//溫度1
	{0x01, 0x03, 0x18, 0x39, 0x00, 0x01, 0x52, 0xA7},	//溫度2
	{0x01, 0x03, 0x18, 0x3A, 0x00, 0x01, 0xA2, 0xA7},	//濕度1
	{0x01, 0x03, 0x18, 0x40, 0x00, 0x01, 0x83, 0x7E},	//上開度
	{0x01, 0x03, 0x18, 0x41, 0x00, 0x01, 0xD2, 0xBE},	//下開度
	{0x01, 0x01, 0x07, 0xE0, 0x00, 0x0C, 0x3C, 0x8D},	//M暫存器
	
	{0x01, 0x03, 0x17, 0xDA, 0x00, 0x01, 0xA0, 0x45},	//風速
	{0x01, 0x03, 0x17, 0xDB, 0x00, 0x01, 0xF1, 0x85},	//負壓
	{0x01, 0x03, 0x17, 0xDC, 0x00, 0x01, 0x40, 0x44},	//上層開度
	{0x01, 0x03, 0x17, 0xDD, 0x00, 0x01, 0x11, 0x84},	//每日飼料
	{0x01, 0x03, 0x17, 0xE0, 0x00, 0x01, 0x80, 0x48},	//每日飲水
};





//=================================================
#endif
//=================================================