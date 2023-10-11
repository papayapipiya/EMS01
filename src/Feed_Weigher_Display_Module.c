#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


//=================================================
#ifdef Dev_Feed_Weigher
//=================================================



void Weight_Correction(unsigned long tmp_data)
{
	if (FW_Status != Setting) return;
	if(port_pin_get_input_level(PIN_PA02) == false)
	{
		OFF_Display();
		RawDataZero = tmp_data;
		Save_Weight_Count_in_NVM();
		Load_Weight_Count_in_NVM();
		uTXByte(Debug_COM, 'S');
		uTXByte(Debug_COM, 'a');
		uTXByte(Debug_COM, 'v');
		uTXByte(Debug_COM, 'e');
		uTXByte(Debug_COM, '0');
		uTXByte(Debug_COM, 0x0d);
		ON_Display();
	}
	else
	if(port_pin_get_input_level(PIN_PA03) == false)
	{
		OFF_Display();
		RawDataUser = tmp_data;
		Save_Weight_Count_in_NVM();
		Load_Weight_Count_in_NVM();
		uTXByte(Debug_COM, 'S');
		uTXByte(Debug_COM, 'a');
		uTXByte(Debug_COM, 'v');
		uTXByte(Debug_COM, 'e');
		uTXByte(Debug_COM, '2');
		uTXByte(Debug_COM, '0');
		uTXByte(Debug_COM, '0');
		uTXByte(Debug_COM, 0x0d);
		ON_Display();
	}
}


void Flash_Display(void)
{
	uTXByte(Display_COM, 'F');
	uTXByte(Display_COM, '2');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, 'E');
}

void OFF_Display(void)
{
	uTXByte(Display_COM, 'F');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, 'E');
}

void ON_Display(void)
{
	uTXByte(Display_COM, 'F');
	uTXByte(Display_COM, '1');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, '0');
	uTXByte(Display_COM, 'E');
}

unsigned long long_to_bcd_4byte(unsigned long tmp_data){
	UINT32u_t	tmp32;
	UINT16u_t	tmp16;
	UINT8u_t	tmp8;
	uint32_t	value;

	value = tmp_data;
	
	tmp8.HL.H = value / 10000000;
	value = value % 10000000;
	tmp8.HL.L = value / 1000000;
	value = value % 1000000;
	tmp32.byte[3] = tmp8.byte;

	tmp8.HL.H = value / 100000;
	value = value % 100000;
	tmp8.HL.L = value / 10000;
	value = value % 10000;
	tmp32.byte[2] = tmp8.byte;
	
	tmp8.HL.H = value / 1000;
	value = value % 1000;
	tmp8.HL.L = value / 100;
	value = value % 100;
	tmp32.byte[1] = tmp8.byte;

	tmp8.HL.H = value / 10;
	value = value % 10;
	tmp8.HL.L = value;
	tmp32.byte[0] = tmp8.byte;

	return tmp32.dword;
}



void tx_byte(unsigned char tmp_data)
{
	UINT8u_t tmp8;
	tmp8.byte = tmp_data;
	uTXByte(Debug_COM, hex_to_ascii(tmp8.HL.H));
	uTXByte(Debug_COM, hex_to_ascii(tmp8.HL.L));
}

void tx_dword(unsigned long tmp_data)
{
	UINT32u_t tmp32;
	tmp32.dword = tmp_data;
	tx_byte(tmp32.byte[3]);
	tx_byte(tmp32.byte[2]);
	tx_byte(tmp32.byte[1]);
	tx_byte(tmp32.byte[0]);
}

void SET_Display_Value(unsigned long tmp_value)
{
	UINT32u_t	tmp32;
	UINT8u_t	tmp8;

	tmp32.dword = tmp_value;
	usart_enable(&MyUart04);
	uTXByte(Display_COM, 'S');
	tmp8.byte = tmp32.byte[2];
	uTXByte(Display_COM, tmp8.HL.H+0x30);
	uTXByte(Display_COM, tmp8.HL.L+0x30);
	tmp8.byte = tmp32.byte[1];
	uTXByte(Display_COM, tmp8.HL.H+0x30);
	uTXByte(Display_COM, tmp8.HL.L+0x30);
	tmp8.byte = tmp32.byte[0];
	uTXByte(Display_COM, tmp8.HL.H+0x30);
	uTXByte(Display_COM, tmp8.HL.L+0x30);
	uTXByte(Display_COM, 'E');
	usart_disable(&MyUart04);
}


//=================================================
#endif
//=================================================