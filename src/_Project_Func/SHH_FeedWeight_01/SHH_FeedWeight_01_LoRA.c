#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


//=================================================
#ifdef SHH_FeedWeight_01
//=================================================


void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	UINT32u_t Weight_u32;
	UINT16u_t tmp_adc;
	
	if(Last_FeedWeight.dword != SaveACCFeedWeight.dword)
	{
		if(SaveACCFeedWeight.dword > Max_Feeding_Value) SaveACCFeedWeight.dword = 0;
		Save_Feed_Count_in_NVM();
		Load_Feed_Count_in_NVM();
	}
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3] = tmp_LORA->MyLoRaID[0];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4] = tmp_LORA->MyLoRaID[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5] = tmp_LORA->MyLoRaID[2];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6] = tmp_LORA->MyLoRaID[3];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][7] = 0x05;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][8] = SaveACCFeedWeight.byte[3];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][9] = SaveACCFeedWeight.byte[2];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][10] = SaveACCFeedWeight.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][11] = SaveACCFeedWeight.byte[0];
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][12] = 0;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][13] = 0;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][14] = 0;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][15] = 0;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][16] = 0;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][17] = 0;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][18] = 0;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][19] = 0;
	
	tmp_adc.word = ADC_LVD_Temp;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][20] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][21] = tmp_adc.byte[0];
		
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][24] = Version;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7] = tmp_LORA->MyBuffer[7];
	tmp_LORA->MyLoRaPower = tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7];
	
	
	
	Setting_Lora_To_Tx_Mode_Func(tmp_LORA);
	if(LORA01->Lora_Show_Flag == true)
	{
		uTXByte(Debug_COM, 0x0d);
		uTXByte(Debug_COM, 'Y');
		uTXByte(Debug_COM, 'e');
		uTXByte(Debug_COM, 's');
		uTXByte(Debug_COM, ' ');
		uart_send_ascii_byte(Debug_COM, SaveACCFeedWeight.byte[3]);
		uart_send_ascii_byte(Debug_COM, SaveACCFeedWeight.byte[2]);
		uart_send_ascii_byte(Debug_COM, SaveACCFeedWeight.byte[1]);
		uart_send_ascii_byte(Debug_COM, SaveACCFeedWeight.byte[0]);
		uTXByte(Debug_COM, ' ');
		uTXByte(Debug_COM, '(');
		uart_send_ascii_byte(Debug_COM, tmp_adc.byte[1]);
		uart_send_ascii_byte(Debug_COM, tmp_adc.byte[0]);
		uTXByte(Debug_COM, ')');
		uTXByte(Debug_COM, 0x0d);
	}
	ADC_LVD_Temp = 0;

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
//=================================================/*

