#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//=================================================
#ifdef HMI_001
//=================================================



void Print_HMI_RS485_RX_In_Buf(void)
{
	if(PLC_Show_r_Flag == false)	return;
	for(unsigned char j = 0; j < HMI_RS485_RX_Buf_Count; j++)
	{
		uTXByte(Debug_COM, HMI_RS485_RX_In_Buf[j]);
		if(HMI_RS485_RX_In_Buf[j] == 'Z')
		{
			uTXByte(Debug_COM, 0x09);
			break;
		}
	}
}

unsigned char HMI_uart_count;
unsigned char *HMI_uart_ptr;
unsigned char HMI_uart_ptr_count;

void HMI_Modbus_Function(void)
{
	UINT16u_t	tmp16;
	UINT8u_t	tmp8;
	
	RS485_For_Debug_En
	if(HMI_RS485_OK_Flag == false)	return;
	
	switch(HMI_Status)
	{
		case HMI_Print_01:
			Print_HMI_RS485_RX_In_Buf();
			HMI_Status = HMI_Check_CMD;
		break;
		
		case HMI_Check_CMD:
			HMI_uart_count = HMI_RS485_RX_Buf_Count - 3;	//錯誤碼(2) + ETX(1)
		
			PLC_Buf.No = str_to_hex_char(&HMI_RS485_RX_In_Buf[1]);
			PLC_Buf.Cmd = str_to_hex_char(&HMI_RS485_RX_In_Buf[3]);
			PLC_Buf.Count = str_to_hex_char(&HMI_RS485_RX_In_Buf[5]);
		
			switch(PLC_Buf.Cmd)
			{
				case 0x44:
					PLC_Buf.Type0 = HMI_RS485_RX_In_Buf[HMI_uart_count - 5];
					PLC_Buf.Type1 = '_';				
					PLC_Buf.Addr = str_to_hex_int(&HMI_RS485_RX_In_Buf[HMI_uart_count - 4]);
					HMI_uart_ptr = &HMI_RS485_RX_In_Buf[HMI_uart_count - 4];
					PLC_Buf.Addr_int =  (ascii_to_hex(*HMI_uart_ptr) * 1000);
					PLC_Buf.Addr_int += (ascii_to_hex(*(HMI_uart_ptr+1)) * 100);
					PLC_Buf.Addr_int += (ascii_to_hex(*(HMI_uart_ptr+2)) * 10);
					PLC_Buf.Addr_int += (ascii_to_hex(*(HMI_uart_ptr+3)));
				break;
				
				case 0x46:
					PLC_Buf.Type0 = HMI_RS485_RX_In_Buf[HMI_uart_count - 6];
					PLC_Buf.Type1 = HMI_RS485_RX_In_Buf[HMI_uart_count - 5];
					PLC_Buf.Addr = str_to_hex_int(&HMI_RS485_RX_In_Buf[HMI_uart_count - 4]);
					HMI_uart_ptr = &HMI_RS485_RX_In_Buf[HMI_uart_count - 4];
					PLC_Buf.Addr_int =  (ascii_to_hex(*HMI_uart_ptr) * 1000);
					PLC_Buf.Addr_int += (ascii_to_hex(*(HMI_uart_ptr+1)) * 100);
					PLC_Buf.Addr_int += (ascii_to_hex(*(HMI_uart_ptr+2)) * 10);
					PLC_Buf.Addr_int += (ascii_to_hex(*(HMI_uart_ptr+3)));
				break;
				
				case 0x42:
					PLC_Buf.Type0 = HMI_RS485_RX_In_Buf[HMI_uart_count - 5];
					PLC_Buf.Type1 = '_';
					PLC_Buf.Value = HMI_RS485_RX_In_Buf[HMI_uart_count - 6];
					PLC_Buf.Addr = str_to_hex_int(&HMI_RS485_RX_In_Buf[HMI_uart_count - 4]);
					HMI_uart_ptr = &HMI_RS485_RX_In_Buf[HMI_uart_count - 4];
					PLC_Buf.Addr_int =  (ascii_to_hex(*HMI_uart_ptr) * 1000);
					PLC_Buf.Addr_int += (ascii_to_hex(*(HMI_uart_ptr+1)) * 100);
					PLC_Buf.Addr_int += (ascii_to_hex(*(HMI_uart_ptr+2)) * 10);
					PLC_Buf.Addr_int += (ascii_to_hex(*(HMI_uart_ptr+3)));
				break;
				
				case 0x47:
					HMI_uart_ptr = &HMI_RS485_RX_In_Buf[0];
					PLC_Buf.Type0 = *(HMI_uart_ptr + 7);
					PLC_Buf.Type1 = *(HMI_uart_ptr + 8);
					PLC_Buf.Addr = str_to_hex_int((HMI_uart_ptr + 9));
					PLC_Buf.Addr_int =  (ascii_to_hex(*(HMI_uart_ptr+9)) * 1000);
					PLC_Buf.Addr_int += (ascii_to_hex(*(HMI_uart_ptr+10)) * 100);
					PLC_Buf.Addr_int += (ascii_to_hex(*(HMI_uart_ptr+11)) * 10);
					PLC_Buf.Addr_int += (ascii_to_hex(*(HMI_uart_ptr+12)));
					PLC_Buf.Value = str_to_hex_int((HMI_uart_ptr + 13));
				break;
			}
			
			HMI_Status = HMI_Check_LRC;
		break;
			
		case HMI_Check_LRC:
			//=============================
			//	LRC
			//=============================
			tmp16.word = 0x02;
			for(unsigned char i = 1; i < HMI_uart_count; i++)	tmp16.word = tmp16.word + HMI_RS485_RX_In_Buf[i];
		
			if(PLC_Show_r_Flag == true)
			{
				uart_send_ascii_byte(Debug_COM, tmp16.byte[0]);
				uTXByte(Debug_COM, '-');
				uart_send_ascii_byte(Debug_COM, str_to_hex_char(&HMI_RS485_RX_In_Buf[HMI_uart_count]));
				uart_str(" ");
			}
		
			if(tmp16.byte[0] == str_to_hex_char(&HMI_RS485_RX_In_Buf[HMI_uart_count]))
			{
				if(PLC_Show_r_Flag == true)	uart_str("OK\t\0");
			}
			else
			{
				if(PLC_Show_r_Flag == true)	uart_str("NG\r\0");
				Reset_PLC_Catch_Value();
				HMI_Status = HMI_Print_01;
				return;
			}
			Reset_PLC_Catch_Value();

			if(PLC_Buf.Cmd == 0x46 && PLC_Buf.Type0 == 'D' && PLC_Buf.Type1 == '0')
			{
				unsigned int *ptr;
				HMI_uart_ptr = &tmp_str[0];
				ptr = &PLC_D[PLC_Buf.Addr_int].D;
		
				PLC_Header_Format_str();
				*(HMI_uart_ptr + 4) = '0';
				HMI_uart_ptr_count = 4;
				for(unsigned char j = 0; j < PLC_Buf.Count; j++)
				{
					unsigned int tmp2;
					tmp2 = *(ptr+j);
			
					tmp16.word = tmp2;
					tmp8.byte = tmp16.byte[1];
					HMI_uart_ptr_count++;
					*(HMI_uart_ptr + HMI_uart_ptr_count) = hex_to_ascii(tmp8.HL.H);
					HMI_uart_ptr_count++;
					*(HMI_uart_ptr + HMI_uart_ptr_count) = hex_to_ascii(tmp8.HL.L);
			
					tmp8.byte = tmp16.byte[0];
					HMI_uart_ptr_count++;
					*(HMI_uart_ptr + HMI_uart_ptr_count) = hex_to_ascii(tmp8.HL.H);
					HMI_uart_ptr_count++;
					*(HMI_uart_ptr + HMI_uart_ptr_count) = hex_to_ascii(tmp8.HL.L);
			
				}
		
				tmp16.word = 0x02;
				for(unsigned char i = 0; i < (HMI_uart_ptr_count + 1); i++)	tmp16.word = tmp16.word + *(HMI_uart_ptr+i);
				tmp8.byte = tmp16.byte[0];
				HMI_uart_ptr_count++;
				*(HMI_uart_ptr + HMI_uart_ptr_count) = hex_to_ascii(tmp8.HL.H);
				HMI_uart_ptr_count++;
				*(HMI_uart_ptr + HMI_uart_ptr_count) = hex_to_ascii(tmp8.HL.L);
		
				HMI_uart_ptr_count++;
				*(HMI_uart_ptr + HMI_uart_ptr_count) = '\0';
				RS485_uart_str(&tmp_str[0]);
			}
			else
			if(PLC_Buf.Cmd == 0x46 && PLC_Buf.Type0 == 'W' && PLC_Buf.Type1 == 'M')
			{
				bool *ptr;
				HMI_uart_ptr = &tmp_str[0];
				ptr = &PLC_M[PLC_Buf.Addr_int].M;
		
				PLC_Header_Format_str();
				*(HMI_uart_ptr + 4) = '0';
				HMI_uart_ptr_count = 4;
				for(unsigned char j = 0; j < PLC_Buf.Count; j++)
				{
					unsigned int tmp, tmp2, tmpStr;
					tmp = 0;
					tmp2 = 0;
					for(unsigned char i = 0; i < 16; i++)
					{
						tmp = *(ptr + i + ( j * 16 ));
						tmp2 |= tmp << i;
					}
			
					tmp16.word = tmp2;
					tmp8.byte = tmp16.byte[1];
					HMI_uart_ptr_count++;
					*(HMI_uart_ptr + HMI_uart_ptr_count) = hex_to_ascii(tmp8.HL.H);
					HMI_uart_ptr_count++;
					*(HMI_uart_ptr + HMI_uart_ptr_count) = hex_to_ascii(tmp8.HL.L);
			
					tmp8.byte = tmp16.byte[0];
					HMI_uart_ptr_count++;
					*(HMI_uart_ptr + HMI_uart_ptr_count) = hex_to_ascii(tmp8.HL.H);
					HMI_uart_ptr_count++;
					*(HMI_uart_ptr + HMI_uart_ptr_count) = hex_to_ascii(tmp8.HL.L);
			
				}
		
				tmp16.word = 0x02;
				for(unsigned char i = 0; i < (HMI_uart_ptr_count + 1); i++)	tmp16.word = tmp16.word + *(HMI_uart_ptr+i);
				tmp8.byte = tmp16.byte[0];
				HMI_uart_ptr_count++;
				*(HMI_uart_ptr + HMI_uart_ptr_count) = hex_to_ascii(tmp8.HL.H);
				HMI_uart_ptr_count++;
				*(HMI_uart_ptr + HMI_uart_ptr_count) = hex_to_ascii(tmp8.HL.L);
		
				HMI_uart_ptr_count++;
				*(HMI_uart_ptr + HMI_uart_ptr_count) = '\0';
				RS485_uart_str(&tmp_str[0]);

			}
			else
			if(PLC_Buf.Cmd == 0x44 && PLC_Buf.Type0 == 'M' && PLC_Buf.Type1 == '_')
			{
				HMI_uart_ptr = &tmp_str[0];
				PLC_Header_Format_str();
		
				*(HMI_uart_ptr+4) = '0';
				HMI_uart_ptr_count = 4;
				for(unsigned char j = 0; j < PLC_Buf.Count; j++)
				{
					HMI_uart_ptr_count++;
					if(PLC_M[PLC_Buf.Addr_int + j].M == true)
						*(HMI_uart_ptr + HMI_uart_ptr_count) = '1';
					else
						*(HMI_uart_ptr + HMI_uart_ptr_count) = '0';
				}
		
				tmp16.word = 0x02;
				for(unsigned char i = 0; i < (HMI_uart_ptr_count + 1); i++)	tmp16.word = tmp16.word + *(HMI_uart_ptr+i);
				tmp8.byte = tmp16.byte[0];
				HMI_uart_ptr_count++;
				*(HMI_uart_ptr + HMI_uart_ptr_count) = hex_to_ascii(tmp8.HL.H);
				HMI_uart_ptr_count++;
				*(HMI_uart_ptr + HMI_uart_ptr_count) = hex_to_ascii(tmp8.HL.L);
				
				HMI_uart_ptr_count++;
				*(HMI_uart_ptr + HMI_uart_ptr_count) = '\0';
				RS485_uart_str(&tmp_str[0]);
				//uart_str(&tmp_str[0]);
				//uTXByte(Debug_COM, 0x0d);
			}
			else
			if(PLC_Buf.Cmd == 0x42 && PLC_Buf.Type0 == 'M' && PLC_Buf.Type1 == '_')
			{
				HMI_uart_ptr = &tmp_str[0];
				if(PLC_Buf.Value == '3')
				{
					PLC_M[PLC_Buf.Addr_int].M = true;
				}
				else
				if(PLC_Buf.Value == '4')
				{
					PLC_M[PLC_Buf.Addr_int].M = false;
				}
				*HMI_uart_ptr = '0';
				*(HMI_uart_ptr + 1) = '1';
				*(HMI_uart_ptr + 2) = '4';
				*(HMI_uart_ptr + 3) = '2';
				*(HMI_uart_ptr + 4) = '0';
				*(HMI_uart_ptr + 5) = 'F';
				*(HMI_uart_ptr + 6) = '9';
				*(HMI_uart_ptr + 7) = '\0';
				RS485_uart_str(&tmp_str[0]);
				//RS485_uart_str("01420F9\0");
			}
			else
			if(PLC_Buf.Cmd == 0x47 && PLC_Buf.Type0 == 'D' && PLC_Buf.Type1 == '0')
			{
				HMI_uart_ptr = &tmp_str[0];
				PLC_D[PLC_Buf.Addr_int].D = PLC_Buf.Value;
				*HMI_uart_ptr = '0';
				*(HMI_uart_ptr + 1) = '1';
				*(HMI_uart_ptr + 2) = '4';
				*(HMI_uart_ptr + 3) = '7';
				*(HMI_uart_ptr + 4) = '0';
				*(HMI_uart_ptr + 5) = 'F';
				*(HMI_uart_ptr + 6) = 'E';
				*(HMI_uart_ptr + 7) = '\0';
				RS485_uart_str(&tmp_str[0]);
				//RS485_uart_str("01470FE\0");
			}
			HMI_Status = HMI_Print_01;
			/*
			if(PLC_Show_r_Flag == true)
			{
				uTXByte(Debug_COM, 0x09);
				uart_send_word(Debug_COM, High_Temperature);
				uTXByte(Debug_COM, 0x0d);
			}
			*/	
		break;
		
	}
}






//=================================================
#endif
//=================================================