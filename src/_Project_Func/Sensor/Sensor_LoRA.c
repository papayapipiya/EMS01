#include <asf.h>
#include <stdio.h>
#include "myDefine.h"
#include "math.h"

//=================================================
#ifdef Dev_Sensor
//=================================================

void Response_LORA(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	UINT16u_t tmp_adc;
	if(LORA01->Power_On_Delay > 0)	return;
	if(startup_flag == false)		return;
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3] = tmp_LORA->MyLoRaID[0];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4] = tmp_LORA->MyLoRaID[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5] = tmp_LORA->MyLoRaID[2];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6] = tmp_LORA->MyLoRaID[3];
	
	#ifdef Sensor_TYPE_00
		tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][7] = 0x00;
	#elif defined(Sensor_TYPE_01)
		tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][7] = 0x01;
	#endif
	
	tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7] = tmp_LORA->MyBuffer[7];
	tmp_LORA->MyLoRaPower = tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7];

	if(tmp_LORA->Lora_Show_Flag == true)
	{
		uTXByte(Debug_COM, 0x0d);
		uTXByte(Debug_COM, 'Y');
		uTXByte(Debug_COM, 'e');
		uTXByte(Debug_COM, 's');
		uTXByte(Debug_COM, 0x09);
		Print_Write_Buffer();
	}
	
	Judg_Is_My_CMD(tmp_LORA);
	if(tmp_LORA->Lora_Show_Flag == true)	uTXByte(Debug_COM, 0x0d);

	//================================
	//	TGS2600
	//================================
	tmp_adc.word = CH1->MyUpdate_Max;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][8] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][9] = tmp_adc.byte[0];
	CH1->MyUpdate_Max = 0;
	if(tmp_LORA->Lora_Show_Flag == true)
	{		
		uart_str("TGS2600: 0x");
		uart_send_ascii_byte(Debug_COM, tmp_adc.byte[1]);
		uart_send_ascii_byte(Debug_COM, tmp_adc.byte[0]);
		uTXByte(Debug_COM, ',');
		uTXByte(Debug_COM, ' ');
	}

	//================================
	//	溫度
	//================================
	tmp_adc.word = IC_Temp;
	#ifdef Sensor_Offset
	tmp_adc.word = tmp_adc.word - 50;
	#endif
	//tmp_adc.word = hex_to_bcd(tmp_adc.word);
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][10] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][11] = tmp_adc.byte[0];
	if(tmp_LORA->Lora_Show_Flag == true)
	{
		uart_str("Temp: ");
		uart_send_ascii_byte(Debug_COM, tmp_adc.byte[1]);
		uart_send_ascii_byte(Debug_COM, tmp_adc.byte[0]);
		uTXByte(Debug_COM, ',');
		uTXByte(Debug_COM, ' ');
	}
	
	//================================
	//	濕度
	//================================
	tmp_adc.word = IC_Humi;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][12] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][13] = tmp_adc.byte[0];
	
	if(tmp_LORA->Lora_Show_Flag == true)
	{
		uart_str("Humi: ");
		uart_send_ascii_byte(Debug_COM, tmp_adc.byte[1]);
		uart_send_ascii_byte(Debug_COM, tmp_adc.byte[0]);
		uTXByte(Debug_COM, ',');
		uTXByte(Debug_COM, ' ');
	}
	
	//================================
	//	TGS2620
	//================================
	tmp_adc.word = CH2->MyUpdate_Max;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][14] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][15] = tmp_adc.byte[0];
	CH2->MyUpdate_Max = 0;
	
	if(tmp_LORA->Lora_Show_Flag == true)
	{
		uart_str("TGS2620: ");
		uart_str("0x");
		uart_send_ascii_byte(Debug_COM, tmp_adc.byte[1]);
		uart_send_ascii_byte(Debug_COM, tmp_adc.byte[0]);
		uTXByte(Debug_COM, ',');
		uTXByte(Debug_COM, ' ');
	}
	
	//================================
	//	Relay Status
	//================================
	tmp_adc.word = Sensor_Relay.NowStatus;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][16] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][17] = tmp_adc.byte[0];
	
	if(tmp_LORA->Lora_Show_Flag == true)
	{
		uart_str("Relay: ");
		uart_send_ascii_byte(Debug_COM, tmp_adc.byte[1]);
		uart_send_ascii_byte(Debug_COM, tmp_adc.byte[0]);
		uTXByte(Debug_COM, ',');
		uTXByte(Debug_COM, ' ');
	}
	
	//================================
	//	mem_OFF_Count_Max
	//================================
	tmp_adc.word = mem_OFF_Count_Max.DataBuf.word;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][18] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][19] = tmp_adc.byte[0];

	if(tmp_LORA->Lora_Show_Flag == true)
	{
		uart_str("OFF_Count: ");
		uart_send_ascii_byte(Debug_COM, tmp_adc.byte[1]);
		uart_send_ascii_byte(Debug_COM, tmp_adc.byte[0]);
		uTXByte(Debug_COM, ',');
		uTXByte(Debug_COM, ' ');
	}
	
	//================================
	//	mem_ON_Loop_Count_Max
	//================================
	tmp_adc.word = mem_ON_Loop_Count_Max.DataBuf.word;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][20] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][21] = tmp_adc.byte[0];
	
	if(tmp_LORA->Lora_Show_Flag == true)
	{
		uart_str("Loop: ");
		uart_send_ascii_byte(Debug_COM, tmp_adc.byte[1]);
		uart_send_ascii_byte(Debug_COM, tmp_adc.byte[0]);
		uTXByte(Debug_COM, ',');
		uTXByte(Debug_COM, ' ');
	}
	
	
	//================================
	//	NA
	//================================
	tmp_adc.word = mem_NA.DataBuf.word;
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][22] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][23] = tmp_adc.byte[0];
	if(tmp_LORA->Lora_Show_Flag == true)
	{
		uart_str("NA: ");
		uart_send_ascii_byte(Debug_COM, tmp_adc.byte[1]);
		uart_send_ascii_byte(Debug_COM, tmp_adc.byte[0]);
		uTXByte(Debug_COM, 0x0d);
		uTXByte(Debug_COM, 0x0d);
	}
	
	//================================
	//	版本
	//================================
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][24] = Version;
	
	//================================
	//倒數時間
	//================================
	if(mem_OFF_Count_Max.DataBuf.word > Sensor_Relay.min_Count)
	{
		tmp_adc.word = mem_OFF_Count_Max.DataBuf.word - Sensor_Relay.min_Count;
	}
	else
	{
		tmp_adc.word = 0;
	}
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][25] = tmp_adc.byte[1];
	tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][26] = tmp_adc.byte[0];
	Setting_Lora_To_Tx_Mode_Func(tmp_LORA);

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