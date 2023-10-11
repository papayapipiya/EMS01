#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

//	竑記 二林PLC 115200, 8, E , 1
//	第二間雞舍

//=================================================
#ifdef Dev_PLC_001
//=================================================

void PLC_001_Main_Func(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	UINT16u_t tmp16;
	tmp16.word = crc_chk(&Define_MyRS485Tmp[RS485_CMD_Index][0], 6);
	Define_MyRS485Tmp[RS485_CMD_Index][6] = tmp16.byte[0];
	Define_MyRS485Tmp[RS485_CMD_Index][7] = tmp16.byte[1];
	
	Lora_CMD_Func(LORA01);
	LORA01_MAIN_FUNCTION(LORA01, tmp_LORA_Dev_List_Index);
	
	if(LORA01->Lora_Show_Flag)
	{
		uTXByte(Debug_COM, 0x09);
		uTXByte(Debug_COM, '[');
		for(unsigned char j = 0; j < 8; j++)
		{
			uart_send_ascii_byte(Debug_COM, Define_MyRS485Tmp[RS485_CMD_Index][j]);
			if(j < 7)
			{
				uTXByte(Debug_COM, ' ');
			}
		}
		uTXByte(Debug_COM, ']');
		uTXByte(Debug_COM, ' ');
	}
	
	Lora_CMD_Func(LORA01);
	LORA01_MAIN_FUNCTION(LORA01, tmp_LORA_Dev_List_Index);
	
	RS485_For_Debug_Dn
	for(unsigned char j = 0; j < 8; j++)
	{
		uTXByte(RS485_COM, Define_MyRS485Tmp[RS485_CMD_Index][j]);
	}	
	RS485_For_Debug_En
	
	RS485_RX_Buf_Count = 0;
	RS485_Time_Count = 0;
	while(1)
	{
		usart_read_buffer_job(RS485_COM, (uint8_t *)RS485_RX_Buf, MAX_RX_BUFFER_LENGTH);
		if(RS485_RX_Buf_Count == RS485_RX_D_Buf_Count)
		{
			break;
		}
		
		if(RS485_Time_Count == 10)
		{
			Print_RS485_Time_Out(LORA01);
			break;
		}
	}
	
	Lora_CMD_Func(LORA01);
	LORA01_MAIN_FUNCTION(LORA01, tmp_LORA_Dev_List_Index);
	
	if(LORA01->Lora_Show_Flag)
	{
		for(unsigned char j = 0; j < RS485_RX_D_Buf_Count; j++)
		{
			uart_send_ascii_byte(Debug_COM, RS485_RX_In_Buf[j]);
			uTXByte(Debug_COM, ' ');
		}
	}
	
	Lora_CMD_Func(LORA01);
	LORA01_MAIN_FUNCTION(LORA01, tmp_LORA_Dev_List_Index);
	
	
	tmp16.word = crc_chk(&RS485_RX_In_Buf[0], 5);
	if(LORA01->Lora_Show_Flag)
	{
		uTXByte(Debug_COM, '(');
		uart_send_ascii_byte(Debug_COM, tmp16.byte[0]);
		uart_send_ascii_byte(Debug_COM, tmp16.byte[1]);
		uTXByte(Debug_COM, ')');
		uTXByte(Debug_COM, ' ');
	}
	
	Lora_CMD_Func(LORA01);
	LORA01_MAIN_FUNCTION(LORA01, tmp_LORA_Dev_List_Index);
	
	if(tmp16.byte[0] != RS485_RX_In_Buf[5] || tmp16.byte[1] != RS485_RX_In_Buf[6])
	{
		if(LORA01->Lora_Show_Flag)
		{
			uTXByte(Debug_COM, 'C');
			uTXByte(Debug_COM, 'R');
			uTXByte(Debug_COM, 'C');
			uTXByte(Debug_COM, ' ');
			uTXByte(Debug_COM, 'E');
			uTXByte(Debug_COM, 'r');
			uTXByte(Debug_COM, 'r');
			uTXByte(Debug_COM, 0x0d);
		}
	}
	else
	{
		for(unsigned char j = 0; j < RS485_RX_D_Buf_Count; j++)
		{
			RS485_value[RS485_CMD_Index][j] = RS485_RX_In_Buf[j];
		}
		if(LORA01->Lora_Show_Flag)
		{
			/*
			uTXByte(Debug_COM, '{');
				for(unsigned char j = 0; j < RS485_RX_D_Buf_Count; j++)
				{
					uart_send_ascii_byte(Debug_COM, RS485_value[RS485_CMD_Index][j]);
					if(j < (RS485_RX_D_Buf_Count - 1))
					{
						uTXByte(Debug_COM, ' ');
					}
				}
			uTXByte(Debug_COM, '}');
			uTXByte(Debug_COM, ' ');
			*/
			uTXByte(Debug_COM, 'C');
			uTXByte(Debug_COM, 'R');
			uTXByte(Debug_COM, 'C');
			uTXByte(Debug_COM, ' ');
			uTXByte(Debug_COM, 'O');
			uTXByte(Debug_COM, 'K');
			uTXByte(Debug_COM, 0x0d);
		}
	}
	
	Lora_CMD_Func(LORA01);
	LORA01_MAIN_FUNCTION(LORA01, tmp_LORA_Dev_List_Index);

	if(RS485_CMD_Index == RS485_CMD_Index_Max)
	{
		RS485_CMD_Index = 0;
	}
	else
	{
		RS485_CMD_Index ++;
	}
}


unsigned int crc_chk(unsigned char* data, unsigned char length)
{
	int j;
	unsigned int reg_crc=0xFFFF;
	while(length--)
	{
		/*
		uart_send_ascii_byte(Debug_COM, *data);
		uTXByte(Debug_COM, ' ');
		*/
		reg_crc ^= *data++;
		for(j=0;j<8;j++)
		{
			if(reg_crc & 0x01) /* LSB(b0)=1 */
			reg_crc=(reg_crc>>1) ^ 0xA001;
			else
			reg_crc=reg_crc >>1;
		}
	}
	return reg_crc;
}

void Dev_Value_inital(void)
{
	RS485_CMD_Index = 0;
	RS485_LORA_OUT_Index = 0;
	//RS485_LORA_OUT_Index = 2;
}

void Print_RS485_Time_Out(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->Lora_Show_Flag == false) return;
	uTXByte(Debug_COM, 'T');
	uTXByte(Debug_COM, 'i');
	uTXByte(Debug_COM, 'm');
	uTXByte(Debug_COM, 'e');
	uTXByte(Debug_COM, ' ');
	uTXByte(Debug_COM, 'O');
	uTXByte(Debug_COM, 'u');
	uTXByte(Debug_COM, 't');
	uTXByte(Debug_COM, 0x0d);
}

uint8_t Define_MyRS485Tmp[16][8] =
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

void Uart_Form_RS485_callback(struct usart_module *const usart_module)
{
	if(RS485_RX_Buf_Count < RS485_RX_D_Buf_Count)
	{
		RS485_RX_In_Buf[RS485_RX_Buf_Count] = RS485_RX_Buf[0];
		RS485_RX_Buf_Count ++;
	}
}

void LORA01_Reset_Func(bool tmp_bool)
{
	port_pin_set_output_level(PIN_PA07, tmp_bool);
	port_pin_set_output_level(PIN_PA17, tmp_bool);
}

void LORA02_Reset_Func(bool tmp_bool)
{
	port_pin_set_output_level(PIN_PA16, tmp_bool);
}


void LORA01_inital(void)
{
	LORA01 = &NewLORA01;
	LORA01->DevUART = &MyUart03;
	LORA01->NVM_offset_Value = 0;
	LORA01->List_Now = 0;
	LORA01->List_Next = 0;
	change_and_check(&(LORA01->NowStatus), Read_LoRa_ID_Index);
	change_and_check(&(LORA01->LoRa_TxRx_Mode_Flag), SetRx);
	change_and_check(&(LORA01->LoRa_Tx_Send_Time_Out_Count), 0);
	change_and_check(&(LORA01->LoRa_IRQ_Flag), 0);
	change_and_check(&(LORA01->LoRa_Send_Tx_Flag), LoraTxNoSend);
	LORA01->f_count = 0;
	LORA01->Lora_Show_Flag = true;
}

void LORA02_inital(void)
{
	LORA02 = &NewLORA02;
	LORA02->DevUART = &MyUart04;
	LORA02->NVM_offset_Value = 20;
	change_and_check(&(LORA02->NowStatus), Read_LoRa_ID_Index);
	change_and_check(&(LORA02->LoRa_TxRx_Mode_Flag), SetRx);
	change_and_check(&(LORA02->LoRa_Tx_Send_Time_Out_Count), 0);
	change_and_check(&(LORA02->LoRa_IRQ_Flag), 0);
	LORA02->Lora_Show_Flag = true;
}


void IO_init(void)
{
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);
	
	/* Configure LEDs as outputs, turn them off */
	
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	
	port_pin_set_config(PIN_PA07, &pin_conf);
	port_pin_set_output_level(PIN_PA07, true);
	port_pin_set_config(PIN_PA22, &pin_conf);
	port_pin_set_output_level(PIN_PA22, true);
	port_pin_set_config(PIN_PA16, &pin_conf);
	port_pin_set_output_level(PIN_PA16, true);
	
	//	RGB LED PIN DEFINE
	port_pin_set_config(PIN_PA23, &pin_conf);
	port_pin_set_output_level(PIN_PA23, true);
	port_pin_set_config(PIN_PA27, &pin_conf);
	port_pin_set_output_level(PIN_PA27, true);
	port_pin_set_config(PIN_PA28, &pin_conf);
	port_pin_set_output_level(PIN_PA28, true);

	
	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	port_pin_set_config(PIN_PA11, &pin_conf);
	port_pin_set_output_level(PIN_PA11, false);
	port_pin_set_config(PIN_PA17, &pin_conf);
	port_pin_set_output_level(PIN_PA17, false);

	port_pin_set_config(PIN_PA02, &pin_conf);
	port_pin_set_output_level(PIN_PA02, true);
	port_pin_set_config(PIN_PA03, &pin_conf);
	port_pin_set_output_level(PIN_PA03, true);
	
	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	port_pin_set_config(PIN_PA10, &pin_conf);
	port_pin_set_output_level(PIN_PA10, false);	// ADC因為是用數位IO所以需要設定為輸入
}

void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	UINT16u_t tmp_adc, tmp_RS485_Data;
	UINT8u_t tmp;
	
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3] = tmp_LORA->MyLoRaID[0];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4] = tmp_LORA->MyLoRaID[1];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5] = tmp_LORA->MyLoRaID[2];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6] = tmp_LORA->MyLoRaID[3];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][7] = 0xF0; // PLC
			
			//	接收發射的功率設定，並寫回自己的發射功率
			tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7] = tmp_LORA->MyBuffer[7];
			tmp_LORA->MyLoRaPower = tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7];
			
			if(LORA01->Lora_Show_Flag == true)
			{
				uTXByte(Debug_COM, 0x0d);
				uTXByte(Debug_COM, 'Y');
				uTXByte(Debug_COM, 'e');
				uTXByte(Debug_COM, 's');
				uTXByte(Debug_COM, 0x0d);
			}	
		
			tmp_RS485_Data.word = RS485_LORA_OUT_Index;
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][8] = RS485_LORA_OUT_Index;
			
			
			switch(RS485_LORA_OUT_Index)
			{
				case 0:
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][9] = 0x00;	//
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][10] = RS485_value[0][3];
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][11] = RS485_value[0][4];
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][12] = RS485_value[1][3];
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][13] = RS485_value[1][4];
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][14] = RS485_value[2][3];
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][15] = RS485_value[2][4];
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][16] = RS485_value[3][3];
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][17] = RS485_value[3][4];
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][18] = RS485_value[4][3];
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][19] = RS485_value[4][4];
				
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][20] = 0;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][21] = 0;
				
					tmp_adc.word = ADC_LVD_Temp;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][22] = tmp_adc.byte[1];
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][23] = tmp_adc.byte[0];
					ADC_LVD_Temp = 0;
				
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][24] = Version;
				
					//預留給RSSI
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][25] = 0;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][26] = 0;
					RS485_LORA_OUT_Index++;
					break;
					
				case 1:
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][9] = 0x01;		//格式
					tmp_RS485_Data.byte[1] = RS485_value[5][4];
					tmp_RS485_Data.byte[0] = RS485_value[5][3];
					
					tmp.HL.H = tmp_RS485_Data.bit.b15;	//噴霧機
					tmp.HL.L = tmp_RS485_Data.bit.b14;	//風扇3
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][10] = tmp.byte;
					
					tmp.HL.H = tmp_RS485_Data.bit.b13;	//風扇4
					tmp.HL.L = tmp_RS485_Data.bit.b12;	//風扇5
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][11] = tmp.byte;
					
					tmp.HL.H = tmp_RS485_Data.bit.b11;	//風扇6
					tmp.HL.L = tmp_RS485_Data.bit.b10;	//保溫機
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][12] = tmp.byte;

					tmp.HL.H = tmp_RS485_Data.bit.b9;	//變頻風扇-01-弱	
					tmp.HL.L = tmp_RS485_Data.bit.b8;	//變頻風扇-01-中
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][13] = tmp.byte;
					
					tmp.HL.H = tmp_RS485_Data.bit.b7;	//變頻風扇-01-強
					tmp.HL.L = tmp_RS485_Data.bit.b6;	//變頻風扇-02-弱
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][14] = tmp.byte;
					
					tmp.HL.H = tmp_RS485_Data.bit.b5;	//變頻風扇-02-中
					tmp.HL.L = tmp_RS485_Data.bit.b4;	//變頻風扇-02-強
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][15] = tmp.byte;
					
					tmp.HL.H = tmp_RS485_Data.bit.b3;
					tmp.HL.L = tmp_RS485_Data.bit.b2;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][16] = tmp.byte;
					
					tmp.HL.H = tmp_RS485_Data.bit.b1;
					tmp.HL.L = tmp_RS485_Data.bit.b0;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][17] = tmp.byte;
					
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][18] = 0;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][19] = 0;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][20] = 0;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][21] = 0;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][22] = 0;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][23] = 0;
					
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][24] = Version;
					
					//預留給RSSI
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][25] = 0;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][26] = 0;
					RS485_LORA_OUT_Index++;
					break;
					
				case 2:
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][9] = 0x00;
					tmp_RS485_Data.byte[1] = RS485_value[5][4];
					tmp_RS485_Data.byte[0] = RS485_value[5][3];
					
					//變頻風扇-01
					//tmp.byte = 弱*1 + 中*2 + 強*3
					tmp.byte = tmp_RS485_Data.bit.b9 + (tmp_RS485_Data.bit.b8)*2 + (tmp_RS485_Data.bit.b7)*3;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][10] = 0x00;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][11] = tmp.byte;
				
					//變頻風扇-02
					//tmp.byte = 弱*1 + 中*2 + 強*3
					tmp.byte = tmp_RS485_Data.bit.b6 + (tmp_RS485_Data.bit.b5)*2 + (tmp_RS485_Data.bit.b4)*3;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][12] = 0x00;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][13] = tmp.byte;
				
			
					tmp.HL.H = tmp_RS485_Data.bit.b7;
					tmp.HL.L = tmp_RS485_Data.bit.b6;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][14] = tmp.byte;
				
					tmp.HL.H = tmp_RS485_Data.bit.b5;
					tmp.HL.L = tmp_RS485_Data.bit.b4;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][15] = tmp.byte;
				
					tmp.HL.H = tmp_RS485_Data.bit.b3;
					tmp.HL.L = tmp_RS485_Data.bit.b2;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][16] = tmp.byte;
				
					tmp.HL.H = tmp_RS485_Data.bit.b1;
					tmp.HL.L = tmp_RS485_Data.bit.b0;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][17] = tmp.byte;
				
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][18] = 0;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][19] = 0;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][20] = 0;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][21] = 0;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][22] = 0;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][23] = 0;
				
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][24] = Version;
				
					//預留給RSSI
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][25] = 0;
					tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][26] = 0;
				
				
					RS485_LORA_OUT_Index = 0;
					break;
			}
			
			RS485_Time_Count = 0;
			uTXByte(Debug_COM, 'P');
			uTXByte(Debug_COM, 'L');
			uTXByte(Debug_COM, 'C');
			uTXByte(Debug_COM, '[');
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][8]);
			uTXByte(Debug_COM, ']');
			uTXByte(Debug_COM, ':');
			/*
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][0]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][1]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][2]);
			*/
			/*
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][7]);
			*/
			uTXByte(Debug_COM, ' ');
			uTXByte(Debug_COM, '(');
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][9]);
			uTXByte(Debug_COM, ')');
			uTXByte(Debug_COM, ' ');
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][10]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][11]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][12]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][13]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][14]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][15]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][16]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][17]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][18]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][19]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][20]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][21]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][22]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][23]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][24]);
			uTXByte(Debug_COM, ' ');
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][25]);
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][26]);
			
			uTXByte(Debug_COM, 0x0d);
			Setting_Lora_To_Tx_Mode_Func(tmp_LORA);
}

void Print_FW_ID(void)
{
	uTXByte(Debug_COM, 'P');
	uTXByte(Debug_COM, 'L');
	uTXByte(Debug_COM, 'C');
	uTXByte(Debug_COM, '_');
	uTXByte(Debug_COM, '0');
	uTXByte(Debug_COM, '0');
	uTXByte(Debug_COM, '1');
	uTXByte(Debug_COM, ' ');
	uTXByte(Debug_COM, ':');
	uTXByte(Debug_COM, ' ');
	uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(Version));
}

void Wait_RX_IRQ_Index_Func(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->LoRa_IRQ_Flag == 1)
	{
		LED_R_ON
		wdt_reset_count();
		change_and_check(&(tmp_LORA->NowStatus), Read_LoRa_RxBuffer_Index);
	}
}


//=================================================
#endif
//=================================================