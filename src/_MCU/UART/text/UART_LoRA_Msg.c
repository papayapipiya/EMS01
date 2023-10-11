#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

const char Msg_RdID[]			= "Rd ID\t\0";
const char Msg_RdSt[]			= "Rd St\t\0";
const char Msg_RdBF[]			= "Rd BF\t\0";
const char Msg_WrFQ[]			= "Wr FQ\t\0";
const char Msg_WrSt[]			= "Wr St\t\0";
const char Msg_WrBF[]			= "Wr BF\t\0";
const char Msg_LRRST[]			= "LR RST\t\0";
const char Msg_RSTIRQ[]			= "RST IRQ\r\0";
const char Msg_LeaveTx[]		= "Leave Tx\r\0";
const char Msg_CRCErr[]			= "CRC Err\r\0";
const char Msg_CRCOK[]			= "CRC OK\r\0";
const char Msg_WaitTXIRQ[]		= "Wait TX IRQ\r\0";
const char Msg_CMDDelay[]		= "CMD Delay\r\0";
const char Msg_TimeOut[]		= "Time Out\r\0";
const char Msg_RS485TimeOut[]	= "RS485 Time Out\r\0";
const char Msg_FW[]				= "   FW: \0";
const char Msg_ID[]				= "[i]ID: \0";
const char Msg_UserID[]			= "User ID: \0";
const char Msg_LoRAID[]			= "LoRA ID: \0";
const char Msg_LowPower[]		= "Low Power\r\0";
const char Msg_DebugMode[]		= "   Debug Mode: \0";
const char Msg_Mode[]			= "   Mode: \0";
const char Msg_FQ[]				= "[f]FQ: \0";
const char Msg_Power[]			= "[p]Power: +\0";
const char Msg_SF[]				= "[s]SF: \0";
const char Msg_LoRA1_Time_Out[]	= "\r(LR1 TOut)";
const char Msg_LoRA2_Time_Out[]	= "\r(LR2 TOut)";
const char Msg_PLC[]			= "   PLC: \0";
extern const char Msg_LoRA01[];
extern const char Msg_LoRA02[];


void Print_IIC_Time_Out_Msg(void)
{
	uart_str(&Msg_TimeOut[0]);
}

void Print_RS485_Time_Out(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->Lora_Show_Flag == false) return;
	uart_str(&Msg_RS485TimeOut[0]);
}

void Print_LoRA_Time_Out_Msg(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA == LORA01)
		uart_str(&Msg_LoRA1_Time_Out[0]);
	else
		uart_str(&Msg_LoRA2_Time_Out[0]);
}

void Print_LowPower_Msg(volatile LORA_DEFINE_t *tmp_LORA)
{
	uart_str(&Msg_LowPower[0]);
}


void Print_Who_Lora(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA == LORA01)
	{
		if(LORA01->Lora_Debug_Flag == NoDebug) return;
		uTXByte(Debug_COM, '#');
		uart_str(&Msg_LoRA01[0]);
	}
	else
	if(tmp_LORA == LORA02)
	{
		if(LORA02->Lora_Debug_Flag == NoDebug) return;
		uTXByte(Debug_COM, '#');
		uart_str(&Msg_LoRA02[0]);
	}
}

void Print_Read_LoRa_ID_Msg(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->Lora_Debug_Flag == NoDebug) return;
	uart_str(&Msg_RdID[0]);
}

void Print_Delay_LoRa_CMD_Msg(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->Lora_Debug_Flag == 0) return;
	uart_str(&Msg_CMDDelay[0]);
}

void Print_Time_Out_Msg(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->Lora_Debug_Flag == NoDebug) return;
	uart_str(&Msg_TimeOut[0]);
}

void Print_Catch_LoRa_ID_Msg(volatile LORA_DEFINE_t *tmp_LORA)
{
	uart_str(&Msg_FW[0]);
	uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaFW);
	uTXByte(Debug_COM, 0x09);
	
	Print_FW_ID();
	uTXByte(Debug_COM, 0x0d);

	uart_str(&Msg_ID[0]);
	uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaID[0]);
	uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaID[1]);
	uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaID[2]);
	uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaID[3]);
	uTXByte(Debug_COM, 0x09);
	
	uart_str(&Msg_UserID[0]);
	uart_send_ascii_byte(Debug_COM, tmp_LORA->UserLoRaID[0]);
	uart_send_ascii_byte(Debug_COM, tmp_LORA->UserLoRaID[1]);
	uart_send_ascii_byte(Debug_COM, tmp_LORA->UserLoRaID[2]);
	uart_send_ascii_byte(Debug_COM, tmp_LORA->UserLoRaID[3]);
	uTXByte(Debug_COM, 0x09);
	
	uart_str(&Msg_LoRAID[0]);
	uart_send_ascii_byte(Debug_COM, tmp_LORA->LoRaID[0]);
	uart_send_ascii_byte(Debug_COM, tmp_LORA->LoRaID[1]);
	uart_send_ascii_byte(Debug_COM, tmp_LORA->LoRaID[2]);
	uart_send_ascii_byte(Debug_COM, tmp_LORA->LoRaID[3]);
	uTXByte(Debug_COM, 0x0d);
}

void Print_Catch_LoRa_Setting_Msg(volatile LORA_DEFINE_t *tmp_LORA)
{
	UINT32u_t tmp_FQ;
	
	uart_str(&Msg_DebugMode[0]);
	if(tmp_LORA->Lora_Debug_Flag == NeedDebug)
		uart_str("ON\0");
	else
		uart_str("OFF\0");
	uTXByte(Debug_COM, 0x0d);
	
	uart_str(&Msg_Mode[0]);
	if(tmp_LORA->MyLoRaMode == SetTx)
		uart_str("Tx\0");
	else
		uart_str("Rx\0");
	uTXByte(Debug_COM, 0x0d);
	
	uart_str(&Msg_FQ[0]);
	tmp_FQ.dword = long_to_bcd(tmp_LORA->MyLoRaFQ.dword);
	uart_send_ascii_byte(Debug_COM, tmp_FQ.byte[2]);
	uart_send_ascii_byte(Debug_COM, tmp_FQ.byte[1]);
	uTXByte(Debug_COM, '.');
	uart_send_ascii_byte(Debug_COM, tmp_FQ.byte[0]);
	uart_str(" MHz\0");
	uTXByte(Debug_COM, 0x0d);
	
	uart_str(&Msg_Power[0]);
	uart_send_word(Debug_COM, hex_to_bcd(tmp_LORA->MyLoRaPower + 2));
	uart_str(" dbm\0");
	uTXByte(Debug_COM, 0x0d);
	
	uart_str(&Msg_SF[0]);
	uart_send_word(Debug_COM, hex_to_bcd(tmp_LORA->MyLoRaSF + 5));
	uTXByte(Debug_COM, 0x0d);
}

void Print_Reset_LoRa_Msg(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->Lora_Debug_Flag == NoDebug) return;
	uart_str(&Msg_LRRST[0]);
}

void Print_Read_LoRa_Setting_Msg(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->Lora_Debug_Flag == NoDebug) return;
	uart_str(&Msg_RdSt[0]);
}

void Print_Write_LoRa_Mode_FQ_Msg(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->Lora_Debug_Flag == NoDebug) return;
	uart_str(&Msg_WrFQ[0]);
}
void Print_Write_LoRa_Setting_Msg(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->Lora_Debug_Flag == NoDebug) return;
	uart_str(&Msg_WrSt[0]);
}


void Print_Read_LoRa_RxBuffer_Msg(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->Lora_Debug_Flag == NoDebug) return;
	uart_str(&Msg_RdBF[0]);
}

void Print_Reset_IRQ_Flag_Msg(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->Lora_Debug_Flag == NoDebug) return;
	uart_str(&Msg_RSTIRQ[0]);
}
void Print_Write_LoRa_TxBuffer_Msg(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->Lora_Debug_Flag == NoDebug) return;
	uart_str(&Msg_WrBF[0]);
}


void Print_into_LoRa_Rx_Mode_Msg(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->Lora_Debug_Flag == NoDebug) return;
	uart_str("Into Rx\r\0");
}


void Print_Wait_RX_IRQ_Index(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->Lora_Debug_Flag == NoDebug) return;
	uart_str("Wait_RX_IRQ_Index\r\0");
}

void Print_Leave_LoRa_Tx_Mode_Msg(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->Lora_Debug_Flag == NoDebug) return;
	uart_str(&Msg_LeaveTx[0]);
}
					
void Print_CRC_ERROR(volatile LORA_DEFINE_t *tmp_LORA)				
{
	if(tmp_LORA->Lora_Debug_Flag == NoDebug) return;
	uart_str(&Msg_CRCErr[0]);
}


void Print_CRC_OK(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->Lora_Debug_Flag == NoDebug) return;
	uart_str(&Msg_CRCOK[0]);
}






void Print_One_RSSI_Msg(volatile int tmp_rssi)
{
	uTXByte(Debug_COM, ' ');
	uTXByte(Debug_COM, '-');
	uart_send_word(Debug_COM, hex_to_bcd(tmp_rssi));
	uTXByte(Debug_COM, ' ');
	uTXByte(Debug_COM, 'd');
	uTXByte(Debug_COM, 'B');
	uTXByte(Debug_COM, 'm');
}




void Print_Wait_TX_Finish_Msg(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->Lora_Debug_Flag == 0) return;
	uart_str(&Msg_WaitTXIRQ[0]);
}




