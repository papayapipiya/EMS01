#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


//=================================================
#ifdef Dev_Weight
//=================================================


void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	UINT32u_t Weight_u32;
	UINT16u_t tmp_adc;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3] = tmp_LORA->MyLoRaID[0];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4] = tmp_LORA->MyLoRaID[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5] = tmp_LORA->MyLoRaID[2];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6] = tmp_LORA->MyLoRaID[3];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][7] = 0x01;
	Weight_u32.dword = Weight_UpLoad_Buffer;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][8] = Weight_u32.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][9] = Weight_u32.byte[0];
	
	tmp_adc.word = Weight_Change[0];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][10] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][11] = tmp_adc.byte[0];
	
	tmp_adc.word = Weight_Change[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][12] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][13] = tmp_adc.byte[0];
	
	tmp_adc.word = Weight_Change[2];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][14] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][15] = tmp_adc.byte[0];
	
	tmp_adc.word = Weight_Change[3];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][16] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][17] = tmp_adc.byte[0];
	
	tmp_adc.word = Weight_Change[4];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][18] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][19] = tmp_adc.byte[0];
	
	tmp_adc.word = ADC_LVD_Temp;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][20] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][21] = tmp_adc.byte[0];
	
	tmp_adc.word = Weight_Change[5];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][22] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][23] = tmp_adc.byte[0];
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][24] = Version;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7] = tmp_LORA->MyBuffer[7];
	tmp_LORA->MyLoRaPower = tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7];
	
	
	
	Setting_Lora_To_Tx_Mode_Func(tmp_LORA);
	if(LORA01->Lora_Show_Flag == true)
	{
		uart_str("\rYes\t\0");;
		Print_Write_Buffer();
		uTXByte(Debug_COM, 0x0d);

		uart_send_word(Debug_COM, Weight_UpLoad_Buffer);			uTXByte(Debug_COM, ' ');
		uart_send_word(Debug_COM, hex_to_bcd(Weight_Change[0]));	uTXByte(Debug_COM, ' ');
		uart_send_word(Debug_COM, hex_to_bcd(Weight_Change[1]));	uTXByte(Debug_COM, ' ');
		uart_send_word(Debug_COM, hex_to_bcd(Weight_Change[2]));	uTXByte(Debug_COM, ' ');
		uart_send_word(Debug_COM, hex_to_bcd(Weight_Change[3]));	uTXByte(Debug_COM, ' ');
		uart_send_word(Debug_COM, hex_to_bcd(Weight_Change[4]));	uTXByte(Debug_COM, ' ');
		uart_send_word(Debug_COM, hex_to_bcd(Weight_Change[5]));	uTXByte(Debug_COM, ' ');
		uTXByte(Debug_COM, '(');
		uart_send_word(Debug_COM, ADC_LVD_Temp);
		uTXByte(Debug_COM, ')');
		uTXByte(Debug_COM, 0x0d);
		uTXByte(Debug_COM, 0x0d);
	}
	ADC_LVD_Temp = 0;
	
	Weight_Change[0] = 0;
	Weight_Change[1] = 0;
	Weight_Change[2] = 0;
	Weight_Change[3] = 0;
	Weight_Change[4] = 0;
	Weight_Change[5] = 0;
}


//=================================================
#endif
//=================================================