#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//=================================================
#ifdef Dev_Repeater_RAM
//=================================================

UINT16u_t DT, DT2, DT3, DT4, DT5, DT6, DT7;
UINT16u_t DS, DS2, DS3, DS4, DS5, DS6, DS7;

bool Read_D_Buf_With_Addr(unsigned int tmp_index)
{
	UINT16u_t tmp16;
	bool crc_flag;
	
	Define_MyRS485Tmp[RS485_RD][0] = LORA02->PLC_Addr;
	crc_flag = RW_PLC_D_Buf_Function(tmp_index, RS485_RD, PLC_Show_r_Flag);	//進行RS485讀取PLC動作
	if(PLC_is_Alive == false) return false;
	if(crc_flag == false) return false;
	if(PLC_Show_r_Flag) uTXByte(Debug_COM, 0x0d);
	return true;
}




void Read_D_and_RAM_Function(void)
{
	UINT16u_t tmp16;
	bool crc_flag;
	volatile PLC_Dev_List_t *PLC_Dev_List_Index = PLC_D_Buf;
	if(RS485_RD_Index == PLC_D_Buf_Max)	RS485_RD_Index = 0;		//若超過最大數量，將重頭開始
	crc_flag = Read_D_Buf_With_Addr(RS485_RD_Index);
	if(crc_flag == false) return;
	if(PLC_is_Alive == false) return;
	
	DS.byte[0] = RS485_RX_In_Buf[3];
	DS.byte[1] = RS485_RX_In_Buf[4];
	DT.word = PLC_Dev_List_Index[RS485_RD_Index].LastDataBuf.word;
	
	DS2.byte[0] = RS485_RX_In_Buf[5];
	DS2.byte[1] = RS485_RX_In_Buf[6];
	DT2.word = PLC_Dev_List_Index[RS485_RD_Index + 1].LastDataBuf.word;
	
	DS3.byte[0] = RS485_RX_In_Buf[7];
	DS3.byte[1] = RS485_RX_In_Buf[8];
	DT3.word = PLC_Dev_List_Index[RS485_RD_Index + 2].LastDataBuf.word;
	
	DS4.byte[0] = RS485_RX_In_Buf[9];
	DS4.byte[1] = RS485_RX_In_Buf[10];
	DT4.word = PLC_Dev_List_Index[RS485_RD_Index + 3].LastDataBuf.word;
	
	DS5.byte[0] = RS485_RX_In_Buf[11];
	DS5.byte[1] = RS485_RX_In_Buf[12];
	DT5.word = PLC_Dev_List_Index[RS485_RD_Index + 4].LastDataBuf.word;
	
	DS6.byte[0] = RS485_RX_In_Buf[13];
	DS6.byte[1] = RS485_RX_In_Buf[14];
	DT6.word = PLC_Dev_List_Index[RS485_RD_Index + 5].LastDataBuf.word;
	
	DS7.byte[0] = RS485_RX_In_Buf[15];
	DS7.byte[1] = RS485_RX_In_Buf[16];
	DT7.word = PLC_Dev_List_Index[RS485_RD_Index + 6].LastDataBuf.word;
	
	//檢查資料是否一致，如果一致不上傳
	if(	DT.word != DS.word || DT2.word != DS2.word ||
	DT3.word != DS3.word || DT4.word != DS4.word ||
	DT5.word != DS5.word || DT6.word != DS6.word ||
	DT7.word != DS7.word)
	{
		PLC_D_Check_Equel_Flag[PLC_Dev_List_Index[RS485_RD_Index].Chage_Flag_Index] = 3;
		PLC_Dev_List_Index[RS485_RD_Index].DataBuf.word = DS.word;
		PLC_Dev_List_Index[RS485_RD_Index + 1].DataBuf.word = DS2.word;
		PLC_Dev_List_Index[RS485_RD_Index + 2].DataBuf.word = DS3.word;
		PLC_Dev_List_Index[RS485_RD_Index + 3].DataBuf.word = DS4.word;
		PLC_Dev_List_Index[RS485_RD_Index + 4].DataBuf.word = DS5.word;
		PLC_Dev_List_Index[RS485_RD_Index + 5].DataBuf.word = DS6.word;
		PLC_Dev_List_Index[RS485_RD_Index + 6].DataBuf.word = DS7.word;
		if(PLC_Show_r_Flag)
		{
			uart_str("Not Equal\r\0");
			//印出修改的數值
			uart_send_ascii_byte(Debug_COM, DT.byte[0]);
			uart_send_ascii_byte(Debug_COM, DT.byte[1]);
			uart_str(" --> \0");
			uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[RS485_RD_Index].DataBuf.byte[0]);
			uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[RS485_RD_Index].DataBuf.byte[1]);
			uTXByte(Debug_COM, 0x09);
			
			uart_send_ascii_byte(Debug_COM, DT2.byte[0]);
			uart_send_ascii_byte(Debug_COM, DT2.byte[1]);
			uart_str(" --> \0");
			uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[RS485_RD_Index + 1].DataBuf.byte[0]);
			uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[RS485_RD_Index + 1].DataBuf.byte[1]);
			uTXByte(Debug_COM, 0x09);
			
			uart_send_ascii_byte(Debug_COM, DT3.byte[0]);
			uart_send_ascii_byte(Debug_COM, DT3.byte[1]);
			uart_str(" --> \0");
			uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[RS485_RD_Index + 2].DataBuf.byte[0]);
			uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[RS485_RD_Index + 2].DataBuf.byte[1]);
			uTXByte(Debug_COM, 0x09);
			
			uart_send_ascii_byte(Debug_COM, DT4.byte[0]);
			uart_send_ascii_byte(Debug_COM, DT4.byte[1]);
			uart_str(" --> \0");
			uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[RS485_RD_Index + 3].DataBuf.byte[0]);
			uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[RS485_RD_Index + 3].DataBuf.byte[1]);
			uTXByte(Debug_COM, 0x0d);
			
			uart_send_ascii_byte(Debug_COM, DT5.byte[0]);
			uart_send_ascii_byte(Debug_COM, DT5.byte[1]);
			uart_str(" --> \0");
			uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[RS485_RD_Index + 4].DataBuf.byte[0]);
			uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[RS485_RD_Index + 4].DataBuf.byte[1]);
			uTXByte(Debug_COM, 0x09);
			uart_send_ascii_byte(Debug_COM, DT6.byte[0]);
			uart_send_ascii_byte(Debug_COM, DT6.byte[1]);
			uart_str(" --> \0");
			uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[RS485_RD_Index + 5].DataBuf.byte[0]);
			uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[RS485_RD_Index + 5].DataBuf.byte[1]);
			uTXByte(Debug_COM, 0x09);
			uart_send_ascii_byte(Debug_COM, DT7.byte[0]);
			uart_send_ascii_byte(Debug_COM, DT7.byte[1]);
			uart_str(" --> \0");
			uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[RS485_RD_Index + 6].DataBuf.byte[0]);
			uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[RS485_RD_Index + 6].DataBuf.byte[1]);
			uTXByte(Debug_COM, 0x0d);
		}
		PLC_Dev_List_Index[RS485_RD_Index].LastDataBuf.word = DS.word;
		PLC_Dev_List_Index[RS485_RD_Index + 1].LastDataBuf.word = DS2.word;
		PLC_Dev_List_Index[RS485_RD_Index + 2].LastDataBuf.word = DS3.word;
		PLC_Dev_List_Index[RS485_RD_Index + 3].LastDataBuf.word = DS4.word;
		PLC_Dev_List_Index[RS485_RD_Index + 4].LastDataBuf.word = DS5.word;
		PLC_Dev_List_Index[RS485_RD_Index + 5].LastDataBuf.word = DS6.word;
		PLC_Dev_List_Index[RS485_RD_Index + 6].LastDataBuf.word = DS7.word;
	}
	else
	{
		if(PLC_Show_r_Flag) uart_str("Equal \0");
	}

	if(PLC_Show_r_Flag)
	{
		//上傳用的Flag，如果資料有變化，該Flag[編號]將改為1，上傳前會檢查Flag[編號]=1，如果為1上傳七筆資料
		uart_str("Flag[\0");
		uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[RS485_RD_Index].Chage_Flag_Index );
		uart_str("] = \0");
		uTXByte(Debug_COM, PLC_D_Check_Equel_Flag[PLC_Dev_List_Index[RS485_RD_Index].Chage_Flag_Index] + 0x30);
		uTXByte(Debug_COM, ' ');
		
		//目前上傳用的索引值
		uart_str("Out[\0");
		uart_send_ascii_byte(Debug_COM, RS485_LORA_OUT_Index);
		uart_str("]\r\0");
	}
	Modbus_Status = Modbus_S1;
	RS485_RD_Index = RS485_RD_Index + 7;											//取出下一個要讀取的位置
}





//=================================================
#endif
//=================================================