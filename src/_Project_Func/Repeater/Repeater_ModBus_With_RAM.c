#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//=================================================
#ifdef Dev_Repeater_RAM
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
		uTXByte(Debug_COM, 0x09);
	}
	//將資料的Index轉換成實際PLC的D Address
	tmpAddr.word = Index_To_Addr(tmp_Addr);			
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
	for(unsigned char j = 0; j < 8; j++)	
	{
		uTXByte(RS485_COM, Define_MyRS485Tmp[tmp_CMD_Index][j]);
	}
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
			PLC_is_Alive = false;
			PLC_is_Alive_Count = 0;
			uart_str("Pass PLC\r\0");
			return false;
		}
	}
	PLC_is_Alive = true;
	
	//印出收到的字串
	if(debug_flag == true)	Print_Data_Buf_Function(&RS485_RX_In_Buf[0], MyRS485RxCount[tmp_CMD_Index]);
	
	//計算CRC數值
	tmp16.word = crc_chk(&RS485_RX_In_Buf[0], MyRS485RxCount[tmp_CMD_Index] -2 );
	if(debug_flag == true)
	{
		uTXByte(Debug_COM, '(');
		uart_send_ascii_byte(Debug_COM, tmp16.byte[0]);
		uart_send_ascii_byte(Debug_COM, tmp16.byte[1]);
		uTXByte(Debug_COM, ')');
		uTXByte(Debug_COM, ' ');
	}
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









void RS485_Main_Function(void)
{	
	volatile PLC_Dev_List_t *PLC_Dev_List_Index = PLC_D_Buf;
	unsigned int tmp;
	bool crc_flag;
	

	if (PLC_is_Alive == false)
	{
		if(PLC_is_Alive_Count == PLC_is_Alive_Count_Max)
		{
			PLC_is_Alive = true;
			Modbus_Status = Modbus_S1;
			uart_str("Try PLC\r\0");
		}
		else
		{
			return;	
		}		
	}

	switch(Modbus_Status)
	{
		case Modbus_S1:
			Write_Data_Into_PLC_D_Register();
			Modbus_Status = Modbus_S2;
			break;
		
		case Modbus_S2:	
			//Read_D_and_RAM_Function();
			Modbus_Status = Modbus_S3;
			break;
		
		case Modbus_S3:
			//Read_D_and_RAM_Function();
			Modbus_Status = Modbus_S4;
			break;
		
		case Modbus_S4:
			Read_D_and_RAM_Function();
			Modbus_Status = Modbus_S5;
			break;
		
		case Modbus_S5:
			//Read_D_and_RAM_Function();
			Modbus_Status = Modbus_S6;
			break;
			
		case Modbus_S6:
			//Read_D_and_RAM_Function();
			Modbus_Status = Modbus_S1;
			break;
	}
}





//=================================================
#endif
//=================================================