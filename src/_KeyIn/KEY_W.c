#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

void Judg_KEY_W(void)
{
	unsigned char k = 0;
	unsigned int tmp_index = 0;
	
	//W01 0200010702000008000200FFFF000000Z	-0038
	if(ascii_to_hex(UserKeyInBuf[0]) != KEYW) return;
	if(ascii_to_hex(UserKeyInBuf[36]) == KEYZ && UserKeyInBuf[37] == 0x0d && 
		UserKeyInBufCount == 38 ) //UserKeyInBufCount最大38
	{
		uart_str("To PLC OK\r\0");
		Dev_Buf_Write_Index = 2;	
		
		//---------------------------------
		uart_str("Get keyin by ASCII:\r ");
		for(unsigned char i = 0; i < UserKeyInBufCount; i++)
		{
			uart_send_ascii_byte(Debug_COM, UserKeyInBuf[i]);
			uTXByte(Debug_COM, ' ');
		}
		uTXByte(Debug_COM, 0x0d);
		
		//---------------------------------
		uart_str("Save Keyin to Hex:\r ");
		for(unsigned char i = 4; i < UserKeyInBufCount; i++)
		{
			PC_WriteBuffer[k] = ascii_to_hex(UserKeyInBuf[i]);
			uart_send_ascii_byte(Debug_COM, PC_WriteBuffer[k]);
			uTXByte(Debug_COM, ' ');
			k++;
		}
		uTXByte(Debug_COM, 0x0d);
		
		//---------------------------------
		k = 0;
		for(unsigned char i = 0; i < (UserKeyInBufMax / 2); i++)
		{
			PC_WriteBufferHex[k] = PC_WriteBuffer[(i*2)] << 4 | PC_WriteBuffer[(i*2) + 1];
			k++;
		}
		PC_WriteBufferHex[16] = 0x33;
		PC_WriteBufferHex[17] = 0x33;
		
		uart_str("Print KeyIn BCD:\r ");
		for(unsigned char i = 0; i < 18; i++)
		{
			uart_send_ascii_byte(Debug_COM, PC_WriteBufferHex[i]);
			uTXByte(Debug_COM, ' ');
		}
		uTXByte(Debug_COM, 0x0d);
		
		tmp_index =	Cal_list_table_index('0', UserKeyInBuf[1], UserKeyInBuf[2]);
		uart_send_ascii_byte(Debug_COM, tmp_index);
		//---------------------------------
		dev_ptr[tmp_index].cmd_buf[0] = PC_WriteBufferHex[0];	//MAC
		dev_ptr[tmp_index].cmd_buf[1] = PC_WriteBufferHex[1];
		dev_ptr[tmp_index].cmd_buf[2] = PC_WriteBufferHex[2];
		dev_ptr[tmp_index].cmd_buf[3] = PC_WriteBufferHex[3];
		dev_ptr[tmp_index].cmd_buf[4] = PC_WriteBufferHex[4];	//功能
		dev_ptr[tmp_index].cmd_buf[5] = PC_WriteBufferHex[5];	//Counter
		dev_ptr[tmp_index].cmd_buf[6] = PC_WriteBufferHex[6];	//資料1
		dev_ptr[tmp_index].cmd_buf[7] = PC_WriteBufferHex[7];
		dev_ptr[tmp_index].cmd_buf[8] = PC_WriteBufferHex[8];	//資料2
		dev_ptr[tmp_index].cmd_buf[9] = PC_WriteBufferHex[9];
		dev_ptr[tmp_index].cmd_buf[10] = PC_WriteBufferHex[10];	//資料3
		dev_ptr[tmp_index].cmd_buf[11] = PC_WriteBufferHex[11];
		dev_ptr[tmp_index].cmd_buf[12] = PC_WriteBufferHex[12];	//參數1
		dev_ptr[tmp_index].cmd_buf[13] = PC_WriteBufferHex[13];	//參數2
		dev_ptr[tmp_index].cmd_buf[14] = PC_WriteBufferHex[14];	//參數3
		dev_ptr[tmp_index].cmd_buf[15] = PC_WriteBufferHex[15];	//參數4
		dev_ptr[tmp_index].cmd_buf[16] = 0x33;	//0X33
		dev_ptr[tmp_index].cmd_buf[17] = 0x33;	//0X33
		dev_ptr[tmp_index].cmd_buf_Sent_Flag = true;
		dev_ptr[tmp_index].cmd_buf_Return_OK_Flag = false;
		dev_ptr[tmp_index].cmd_buf_to_clear_mysql_Flag = false;

		
		
	}
	//=================================================
	#ifdef Dev_Wifi
	//=================================================
	if(	ascii_to_hex(UserKeyInBuf[1]) == KEYI &&
		ascii_to_hex(UserKeyInBuf[2]) == KEYF &&
		ascii_to_hex(UserKeyInBuf[3]) == KEYI && 
		UserKeyInBuf[4] == 0x0d &&
		UserKeyInBufCount == 5 ) //UserKeyInBufCount最大34
	{
		My_WiFi.debug_flag = !My_WiFi.debug_flag;
		uart_str("WiFi Debug Mode \0");
		if(My_WiFi.debug_flag)	uart_str("ON\r\0"); else uart_str("OFF\r\0");
		
	}
	//=================================================
	#endif
	//=================================================
}


























