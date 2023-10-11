#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

const char Msg_Power_ON_START[] = "TreeCode Wifi & LoRa System\r\0";
const char Msg_Desh[]			= "\r- - - - - - - - - - - -\r\0";
const char Msg_Arrow_Right[]	= "\t-->\t\0";
const char Msg_LoRA01[]			= "@LR01\t\0";
const char Msg_LoRA02[]			= " LR02\t\0";
const char Msg_FoundIt[]		= "[ Found it ]\r\0";
const char Msg_MISSNODE[]		= " Miss node\r\0";
const char Msg_UpLoad[]			= "UpLoad: \0";
const char Msg_Send[]			= "Send: \0";









void Print_Lora_number(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA == LORA01)
		uart_str(&Msg_LoRA01[0]);
	else
		uart_str(&Msg_LoRA02[0]);
}

void Print_Line_Msg(void)
{
	uart_str(&Msg_Desh[0]);
}



void Print_Start_Msg(void)
{
	uart_str(&Msg_Power_ON_START[0]);
	Print_Line_Msg();
}


void Print_Find_Msg(void)
{
	uart_str(&Msg_FoundIt[0]);
}

void Print_Enable_CMD_Msg(void)
{
	uTXByte(Debug_COM, 'E');
}

void Print_Disable_CMD_Msg(void)
{
	uTXByte(Debug_COM, ' ');
}

void Print_Dev_Time_Value(unsigned char tmp_index, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	UINT16u_t tmp16;
	uTXByte(Debug_COM, ' ');
	tmp16.byte[1] = dev_ptr[tmp_index].Dev_Time.byte[1];
	tmp16.byte[0] = dev_ptr[tmp_index].Dev_Time.byte[0];
	uart_send_word(Debug_COM, hex_to_bcd(tmp16.word));
	uTXByte(Debug_COM, ' ');
}

void Print_One_Form_List_Table(unsigned char tmp_index, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	uart_str("No.\0");
	uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(dev_ptr[tmp_index].Dev_Number));
	uTXByte(Debug_COM, ' ');
	uart_send_ascii_byte(Debug_COM, dev_ptr[tmp_index].MAC_ID.byte[0]);
	uart_send_ascii_byte(Debug_COM, dev_ptr[tmp_index].MAC_ID.byte[1]);
	uart_send_ascii_byte(Debug_COM, dev_ptr[tmp_index].MAC_ID.byte[2]);
	uart_send_ascii_byte(Debug_COM, dev_ptr[tmp_index].MAC_ID.byte[3]);
	uTXByte(Debug_COM, ' ');
	Print_Dev_Time_Value(tmp_index, tmp_LORA_Dev_List_Index);
	uTXByte(Debug_COM, ' ');
	uTXByte(Debug_COM, '[');
	if(dev_ptr[tmp_index].Dev_En_Flag == Dev_EN)
	{
		Print_Enable_CMD_Msg();
	}
	else
	{
		Print_Disable_CMD_Msg();
	}
	uTXByte(Debug_COM, ']');
}



void Print_Arrow_Right(void)
{
	uart_str(&Msg_Arrow_Right[0]);
}



void Print_MissNode_Form_RX_Msg(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	if(tmp_LORA->Lora_Show_Flag == false) return;
	Print_Line_Msg();
	Print_Lora_number(tmp_LORA);
	uart_str("No.\0");
	uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(tmp_LORA_Dev_List_Index[tmp_LORA->List_Now].Dev_Number));
	uart_str(&Msg_MISSNODE);
	Print_Line_Msg();
}




void Print_Catch_Data_Form_RX_Msg(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	UINT16u_t tmp16;
	
	if(tmp_LORA->Lora_Show_Flag == false) return;
	Print_Line_Msg();
	Print_Lora_number(tmp_LORA);
	uart_str("No.\0");
	uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(tmp_LORA_Dev_List_Index[tmp_LORA->List_Now].Dev_Number));
	uTXByte(Debug_COM, ' ');
	//for(unsigned char i = 0; i < dev_value_data_max; i++)
	for(unsigned char i = 0; i < dev_value_data_max - 7; i++)
	{
		uart_send_ascii_byte(Debug_COM, dev_value[tmp_LORA->List_Now][i]);
		uTXByte(Debug_COM, ' ');
	}
	uTXByte(Debug_COM, 0x0d);
	Print_Line_Msg();
}





void Print_Now_And_Next_Msg(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	if(tmp_LORA->Lora_Show_Flag == false) return;

}



void Print_Send_Dev_No_Msg(volatile LORA_DEFINE_t *tmp_LORA)
{
	if(tmp_LORA->Lora_Show_Flag == false) return;
	uart_str(" <--- No.\0");
	uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(tmp_LORA->List_Now));
	uTXByte(Debug_COM, ' ');
	uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3]);
	uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4]);
	uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5]);
	uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6]);
	//uTXByte(Debug_COM, ' ');
	//uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(tmp_LORA->List_Next));
	//uTXByte(Debug_COM, ' ');
	uTXByte(Debug_COM, 0x0d);
}
