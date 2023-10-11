#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//=================================================
#ifdef Dev_Repeater_RAM
//=================================================



void Write_Data_Into_PLC_D_Register(void)
{
	volatile Write_PLC_List_t *Write_PLC_List_Index = Write_PLC_List_In_RAM;
	UINT16u_t tmpAddr;
	unsigned int tmp_count, i;
	unsigned char tmp_mac, tmp_channel;
	bool Flag_NULL;
	UINT16u_t tmp16;
	
	Define_MyRS485Tmp[RS485_WD][0] = LORA02->PLC_Addr;
	tmp_count = 0;
	Flag_NULL = false;
	for(i = (RS485_WR_Index + 1) ; tmp_count < Write_PLC_List_Max; i = (i + 1) % Write_PLC_List_Max)
	{
		if(i == Write_PLC_List_Max) i = 0;
		if(Write_PLC_List_Index[i].Channel_En == Dev_EN)		//該Index的Enable = Dev_EN 才會執行寫入
		{
			Flag_NULL = true;
			RS485_WR_Index = i;									//找到下一個要寫入的資料
			tmpAddr.word = Index_To_Addr(RS485_WR_Index);			//將資料的Index轉換成實際PLC的D Address
			Define_MyRS485Tmp[RS485_WD][2] = tmpAddr.byte[1];
			Define_MyRS485Tmp[RS485_WD][3] = tmpAddr.byte[0];
			
			tmp_mac = Write_PLC_List_Index[RS485_WR_Index].MAC_ID_Index.word;		//取出Index的設備Mac位置
			tmp_channel = Write_PLC_List_Index[RS485_WR_Index].Channel_Index;		//取出Index的設備Channel位置
			tmp_channel = tmp_channel * 2;
			//dev_value = LoRA回來的資料暫存區
			tmp16.byte[1] = dev_value[tmp_mac][8 + tmp_channel];
			tmp16.byte[0] = dev_value[tmp_mac][9 + tmp_channel];
			if(tmp16.word == 0)
			{
				if(Write_PLC_List_Index[RS485_WR_Index].Channel_Count > 3)
				{
					Define_MyRS485Tmp[RS485_WD][4] = dev_value[tmp_mac][8 + tmp_channel];	//將MAC -> Channel的高位元放入待寫PLC Buf
					Define_MyRS485Tmp[RS485_WD][5] = dev_value[tmp_mac][9 + tmp_channel];	//將MAC -> Channel的低位元放入待寫PLC Buf
				}
				else
				{
					Write_PLC_List_Index[RS485_WR_Index].Channel_Count++;
					return;
				}
			}
			else
			{
				Write_PLC_List_Index[RS485_WR_Index].Channel_Count = 0;
				Define_MyRS485Tmp[RS485_WD][4] = dev_value[tmp_mac][8 + tmp_channel];	//將MAC -> Channel的高位元放入待寫PLC Buf
				Define_MyRS485Tmp[RS485_WD][5] = dev_value[tmp_mac][9 + tmp_channel];	//將MAC -> Channel的低位元放入待寫PLC Buf
			}
			
			break;	//找到資料後跳出
		}
		tmp_count++;
	}
	if(Flag_NULL == false) return;
	
	bool crc_flag = RW_PLC_D_Buf_Function(RS485_WR_Index, RS485_WD, PLC_Show_w_Flag);	//進行RS485寫入PLC動作
	if(PLC_is_Alive == false) return;
	if(crc_flag == false) return;
	
	if(PLC_Show_w_Flag == true)
	{
		uTXByte(Debug_COM, ' ');
		uart_send_ascii_byte(Debug_COM, RS485_RX_In_Buf[4]);
		uart_send_ascii_byte(Debug_COM, RS485_RX_In_Buf[5]);
		uTXByte(Debug_COM, ' ');
		uTXByte(Debug_COM, 0x0d);
	}
}








//=================================================
#endif
//=================================================