#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//=================================================
#ifdef Dev_Repeater_DBuf
//=================================================

void Polling_RS485(void)
{
	if(LORA02->PLC_Flag == NeedPLC)
	{
		RS485_Main_Function();
		Form_CMD_RS485_Function(LORA02);
	}
}




bool RW_PLC_D_Buf_Function(unsigned int tmp_Addr, unsigned char tmp_CMD_Index, bool debug_flag)
{
	UINT16u_t tmpAddr, tmp16;
	volatile PLC_Dev_List_t *PLC_Dev_List_Index = PLC_D_Buf;
	
	
	if(debug_flag == true)
	{
		if(tmp_CMD_Index == RS485_RD)
			uart_str(" R  \0");
		else
			uart_str("[W] \0");
		Print_PLC_D_Register_Number(tmp_Addr);	//印出D暫存器編號
		//uart_send_word(Debug_COM, tmpAddr.word);		//印出真實的D暫存器位置
		uTXByte(Debug_COM, 0x09);
	}
	
	tmpAddr.word = Index_To_Addr(tmp_Addr);			//將資料的Index轉換成實際PLC的D Address
	Define_MyRS485Tmp[tmp_CMD_Index][2] = tmpAddr.byte[1];
	Define_MyRS485Tmp[tmp_CMD_Index][3] = tmpAddr.byte[0];
	//	計算要送出的CRC
	tmp16.word = crc_chk(&Define_MyRS485Tmp[tmp_CMD_Index][0], 6);	
	Define_MyRS485Tmp[tmp_CMD_Index][6] = tmp16.byte[0];
	Define_MyRS485Tmp[tmp_CMD_Index][7] = tmp16.byte[1];

	//	印出準備要送出的字串
	if(debug_flag == true)	Print_Data_Buf_Function(&Define_MyRS485Tmp[tmp_CMD_Index][0], 8);
	
	
	//	送出MODBUS資料給PLC
	RS485_For_Debug_Dn
	for(unsigned char j = 0; j < 8; j++)	uTXByte(RS485_COM, Define_MyRS485Tmp[tmp_CMD_Index][j]);
	RS485_For_Debug_En
	
	//	準備接收PLC資料
	RS485_RX_Buf_Count = 0;
	RS485_Time_Count = 0;
	while(1)
	{
		usart_read_buffer_job(RS485_COM, (uint8_t *)RS485_RX_Buf, MAX_RX_BUFFER_LENGTH);
		if(RS485_RX_Buf_Count == MyRS485RxCount[tmp_CMD_Index])		break;	//接收完成，準備進行CRC檢查
		if(RS485_Time_Count >= RS485_Timeout)
		{
			Print_RS485_Time_Out(LORA01);	//	PLC沒回應 TIME OUT
			return;
		}
	}
	
	//印出收到的字串
	if(debug_flag == true)	Print_Data_Buf_Function(&RS485_RX_In_Buf[0], MyRS485RxCount[tmp_CMD_Index]);
	
	//計算CRC數值
	tmp16.word = crc_chk(&RS485_RX_In_Buf[0], MyRS485RxCount[tmp_CMD_Index] -2 );
	if(	tmp16.byte[0] != RS485_RX_In_Buf[MyRS485RxCount[tmp_CMD_Index]-2] ||
		tmp16.byte[1] != RS485_RX_In_Buf[MyRS485RxCount[tmp_CMD_Index]-1])
	{
		if(debug_flag == true)	uart_str("Err\r\0");		// CRC ERR
		return false;
	}
	else
	{
		if(debug_flag == true)	uart_str("OK \0");		// CRC OK
		return true;
	}
	
	
}


void Write_Data_Into_PLC_D_Register(void)
{
	volatile Write_PLC_List_t *Write_PLC_List_Index = Write_PLC_List_In_RAM;
	UINT16u_t tmpAddr;
	unsigned int tmp_count, i;
	unsigned char tmp_mac, tmp_channel;
	
	Define_MyRS485Tmp[RS485_WD][0] = LORA02->PLC_Addr;
	tmp_count = 0;
	for(i = (RS485_WR_Index + 1) ; tmp_count < Write_PLC_List_Max; i = (i + 1) % Write_PLC_List_Max)
	{
		if(i == Write_PLC_List_Max) i = 0;
		if(Write_PLC_List_Index[i].Channel_En == Dev_EN)		//該Index的Enable = Dev_EN 才會執行寫入
		{
			RS485_WR_Index = i;									//找到下一個要寫入的資料
			tmpAddr.word = Index_To_Addr(RS485_WR_Index);			//將資料的Index轉換成實際PLC的D Address
			Define_MyRS485Tmp[RS485_WD][2] = tmpAddr.byte[1];
			Define_MyRS485Tmp[RS485_WD][3] = tmpAddr.byte[0];		
			
			tmp_mac = Write_PLC_List_Index[RS485_WR_Index].MAC_ID_Index.word;		//取出Index的設備Mac位置
			tmp_channel = Write_PLC_List_Index[RS485_WR_Index].Channel_Index;		//取出Index的設備Channel位置
			tmp_channel = tmp_channel * 2;
			//dev_value = LoRA回來的資料暫存區
			Define_MyRS485Tmp[RS485_WD][4] = dev_value[tmp_mac][8 + tmp_channel];	//將MAC -> Channel的高位元放入待寫PLC Buf
			Define_MyRS485Tmp[RS485_WD][5] = dev_value[tmp_mac][9 + tmp_channel];	//將MAC -> Channel的低位元放入待寫PLC Buf
			break;	//找到資料後跳出
		}		
		tmp_count++;
	}

	bool crc_flag = RW_PLC_D_Buf_Function(RS485_WR_Index, RS485_WD, PLC_Show_w_Flag);	//進行RS485寫入PLC動作			
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


unsigned int Read_D_Buf_With_Addr(unsigned int tmp_index)
{
	UINT16u_t tmp16;
	bool crc_flag;
	
	Define_MyRS485Tmp[RS485_RD][0] = LORA02->PLC_Addr;
	crc_flag = RW_PLC_D_Buf_Function(tmp_index, RS485_RD, PLC_Show_r_Flag);	//進行RS485讀取PLC動作
	if(crc_flag == false) return;
	if(PLC_Show_r_Flag) uTXByte(Debug_COM, 0x0d);

	//將讀到的資料暫存起來，等一下比對資料用
	tmp16.byte[0] = RS485_RX_In_Buf[3];
	tmp16.byte[1] = RS485_RX_In_Buf[4];
	return tmp16.word;
}


UINT16u_t DT, DS;
unsigned char Modbus_Status = 1;
typedef enum{
	Modbus_S1 =	1,
	Modbus_S2 =	2,
	Modbus_S3 =	3,
	Modbus_S4 =	4,
	Modbus_S5 =	5,
	Modbus_S6 =	6,
	Modbus_S7 =	7,
}Modbus_rundown;



void RS485_Main_Function(void)
{	
	//UINT16u_t tmpAddr, tmp16, Target, Source;
	//volatile Write_PLC_List_t *Write_PLC_List_Index = Write_PLC_List_In_RAM;
	volatile PLC_Dev_List_t *PLC_Dev_List_Index = PLC_D_Buf;
	unsigned int tmp;
	bool crc_flag;
	
	switch(Modbus_Status)
	{
		case Modbus_S1:
			
			Write_Data_Into_PLC_D_Register();
			Modbus_Status = Modbus_S3;
			break;
		/*
		case Modbus_S2:
			Read_Data_Form_PLC_D_Register();	
			Modbus_Status = Modbus_S3;
			break;
		*/
		
		case Modbus_S3:	
			if(RS485_RD_Index == PLC_D_Buf_Max)	RS485_RD_Index = 0;		//若超過最大數量，將重頭開始
			DS.word = Read_D_Buf_With_Addr(RS485_RD_Index);
			Modbus_Status = Modbus_S4;
			break;
			
		case Modbus_S4:
			tmp = RS485_RD_Index + 1000;								//借用PLC的D暫存器，用於比對是否有資料變化
			DT.word = Read_D_Buf_With_Addr(tmp);
			Modbus_Status = Modbus_S5;
			break;
			
			
		case Modbus_S5:
			//檢查資料是否一致，如果一致不上傳
			if(	DT.word != DS.word )
			//if(true)
			{
				//PLC_D_Check_Equel_Flag[PLC_Dev_List_Index[RS485_RD_Index].Chage_Flag_Index] = Dev_Have_New_Data;
				PLC_D_Check_Equel_Flag[PLC_Dev_List_Index[RS485_RD_Index].Chage_Flag_Index] = 3;
				PLC_Dev_List_Index[RS485_RD_Index].DataBuf.byte[1] = DS.byte[0];
				PLC_Dev_List_Index[RS485_RD_Index].DataBuf.byte[0] = DS.byte[1];
				if(PLC_Show_r_Flag)
				{
					uart_str("Not Equal \0");
					//印出修改的數值
					uTXByte(Debug_COM, ' ');
					uart_send_ascii_byte(Debug_COM, DT.byte[0]);
					uart_send_ascii_byte(Debug_COM, DT.byte[1]);
					uart_str(" --> \0");
					uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[RS485_RD_Index].DataBuf.byte[1]);
					uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[RS485_RD_Index].DataBuf.byte[0]);
					uTXByte(Debug_COM, 0x0d);
				}
				Define_MyRS485Tmp[RS485_WD][4] = DS.byte[0];						//將MAC -> Channel的高位元放入待寫PLC Buf
				Define_MyRS485Tmp[RS485_WD][5] = DS.byte[1];						//將MAC -> Channel的低位元放入待寫PLC Buf
				tmp = RS485_RD_Index + 1000;										//將資料的Index轉換成實際PLC的D Address
				crc_flag = RW_PLC_D_Buf_Function(tmp, RS485_WD, PLC_Show_w_Flag);	//進行RS485寫入PLC動作
			}
			else
			{
				PLC_Dev_List_Index[RS485_RD_Index].DataBuf.byte[1] = DT.byte[0];
				PLC_Dev_List_Index[RS485_RD_Index].DataBuf.byte[0] = DT.byte[1];
				if(PLC_Show_r_Flag) uart_str("Equal \0");
			}
			/*
			if(PLC_Show_r_Flag) 
			{
				uTXByte(Debug_COM, '{');
				uart_send_word(Debug_COM, RS485_RD_Index);	
				uTXByte(Debug_COM, '}');
			}
			*/
			
			Modbus_Status = Modbus_S6;
			break;
			
		case Modbus_S6:
			if(PLC_Show_r_Flag)
			{
				//上傳用的Flag，如果資料有變化，該Flag[編號]將改為1，上傳前會檢查Flag[編號]=1，如果為1上傳七筆資料
				uart_str(" Flag[\0");
				uart_send_ascii_byte(Debug_COM, PLC_Dev_List_Index[RS485_RD_Index].Chage_Flag_Index );
				uart_str("] = \0");
				uTXByte(Debug_COM, PLC_D_Check_Equel_Flag[PLC_Dev_List_Index[RS485_RD_Index].Chage_Flag_Index] + 0x30);
				uTXByte(Debug_COM, ' ');
			
				//目前上傳用的索引值
				uTXByte(Debug_COM, '[');
				uart_send_ascii_byte(Debug_COM, RS485_LORA_OUT_Index);
				uTXByte(Debug_COM, ']');
				uTXByte(Debug_COM, 0x0d);
			}
			Modbus_Status = Modbus_S1;
			RS485_RD_Index++;											//取出下一個要讀取的位置
			break;
	}
}





//=================================================
#endif
//=================================================