#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



void LoRA_INT_IO_SETTING(unsigned char tmp_index)
{
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);
	if(tmp_index == true)
	{
		struct extint_chan_conf config_extint_chan;
		extint_chan_get_config_defaults(&config_extint_chan);
		
		config_extint_chan.gpio_pin           = PIN_PA11A_EIC_EXTINT11;
		config_extint_chan.gpio_pin_mux       = MUX_PA11A_EIC_EXTINT11;
		config_extint_chan.gpio_pin_pull      = EXTINT_PULL_NONE;
		config_extint_chan.detection_criteria = EXTINT_DETECT_RISING;
		extint_chan_set_config(PIN_PA11, &config_extint_chan);
	}
	else
	{
		pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(PIN_PA11, &pin_conf);	//LoRA interrupt
		port_pin_set_output_level(PIN_PA11, false);
	}
	
}

#define Delay100ms	delay_ms(500);

void Reset_LoRa_Func(volatile LORA_DEFINE_t *tmp_LORA)
{
	
	if(tmp_LORA == LORA01)
	{
		// LORA OFF
		//LORA01_Reset_Func(false);
		//Delay100ms
		// LORA ON
		LORA01_Reset_Func(true);
		//Delay100ms
		// LORA RESET
		tmp_LORA->f_count = 0;
		change_and_check(&(tmp_LORA->NowStatus), Read_LoRa_ID_Index);
		change_and_check(&(tmp_LORA->LoRa_TxRx_Mode_Flag), SetRx);
		change_and_check(&(tmp_LORA->LoRa_Tx_Send_Time_Out_Count), 0);
		change_and_check(&(tmp_LORA->LoRa_IRQ_Flag), 0);
		
	}
	else if(tmp_LORA == LORA02)
	{
		//// LORA OFF
		//LORA02_Reset_Func(false);
		//Delay100ms
		//// LORA ON
		LORA02_Reset_Func(true);
		//Delay100ms
		change_and_check(&(tmp_LORA->NowStatus), Read_LoRa_ID_Index);
		change_and_check(&(tmp_LORA->LoRa_TxRx_Mode_Flag), SetRx);
		change_and_check(&(tmp_LORA->LoRa_Tx_Send_Time_Out_Count), 0);
		change_and_check(&(tmp_LORA->LoRa_IRQ_Flag), 0);
		tmp_LORA->f_count = 0;
	}
}

void change_and_check(volatile unsigned char *tmp_LORA, unsigned char tmp_status)
{
	while(*tmp_LORA != tmp_status)
	{
		*tmp_LORA = tmp_status;
	}
}

void Setting_Lora_To_Tx_Mode_Func(volatile LORA_DEFINE_t *tmp_LORA)
{	
	tmp_LORA->NowStatus = Write_LoRa_Setting_Index;
	tmp_LORA->LoRa_TxRx_Mode_Flag = SetTx;
	tmp_LORA->LoRa_Rx_Flag = LoraRxNoData;
}

void WaitRxData_Func(volatile LORA_DEFINE_t *tmp_LORA)
{
	
	usart_enable(tmp_LORA->DevUART);
	tmp_LORA->f_count =  0;
	tmp_LORA->MyBufferCount = 0;
	while(1)
	{
		usart_read_buffer_job(tmp_LORA->DevUART, (uint8_t *)tmp_LORA->rx_buffer, MAX_RX_BUFFER_LENGTH);
		if(tmp_LORA->f_count < Lora_CMD_Timeout) //尚未Time Out
		{
			if(tmp_LORA->MyBufferCount == tmp_LORA->UserRxCountMax) //若資料長度如預期長度
			{
				tmp_LORA->Lora_TimeOut_Count = 0;						//清除TimeOutCount
				tmp_LORA->f_count = 0;
				tmp_LORA->NowStatus = CheckRxCRC;						//進入CRC檢查
				break;
			}
		}
		else  //Time Out
		{
			tmp_LORA->Lora_TimeOut_Count = ++tmp_LORA->Lora_TimeOut_Count;
			if(tmp_LORA->Lora_TimeOut_Count < 3)	//若Time Out 3次以下
			{
				tmp_LORA->NowStatus = tmp_LORA->LastStatus;
				Print_Who_Lora(tmp_LORA);
				uart_str("(T)\r\0");
			}
			else //Time Out超過3次，重新開啟LoRa
			{
				Reset_LoRa_Func(tmp_LORA);
			}
			Print_Who_Lora(tmp_LORA);
			Print_Time_Out_Msg(tmp_LORA);
			break;
		}
	}
	usart_disable(tmp_LORA->DevUART);
}



unsigned char MyXORFunc(volatile unsigned char tmpA, volatile unsigned char tmpB)
{
	return (tmpA ^ tmpB);
}

void UartToLora(volatile LORA_DEFINE_t *tmp_LORA, unsigned char Myindex)
{
	volatile unsigned char XORtmp;
	unsigned char i;
	volatile unsigned char j;
	//計算要傳給Lora模組的CRC
	XORtmp = 0x00;
	for(i = 0; i < tmp_LORA->MyLoRaTmpCount[Myindex]; i++)
	{
		XORtmp = XORtmp ^ tmp_LORA->MyLoRaTmp[Myindex][i];
	}
	
	//開始將資料傳給Lora模組
	usart_enable(tmp_LORA->DevUART);
	for(i = 0; i < tmp_LORA->MyLoRaTmpCount[Myindex]; i++)
	{
		uTXByte(tmp_LORA->DevUART, tmp_LORA->MyLoRaTmp[Myindex][i]);
	}
	//傳送資料的CRC
	uTXByte(tmp_LORA->DevUART, XORtmp);
	usart_disable(tmp_LORA->DevUART);
	
		
	//=============================
	//	Debug Mode
	//=============================
	if(tmp_LORA->Lora_Debug_Flag == 1)
	{
		for(i = 0; i < tmp_LORA->MyLoRaTmpCount[Myindex]; i++)
		{
			uart_send_ascii_byte(Debug_COM, tmp_LORA->MyLoRaTmp[Myindex][i]);
			uTXByte(Debug_COM, ' ');
		}
		uart_send_ascii_byte(Debug_COM, XORtmp);
		uTXByte(Debug_COM, 0x0d);
	}
}

//寫入資料
void Write_LoRa_TxBuffer(volatile LORA_DEFINE_t *tmp_LORA)
{
	tmp_LORA->MyBufferCount = 0;
	tmp_LORA->UserRxCountMax = 5;
	UartToLora(tmp_LORA, Write_LoRa_TxBuffer_Index);
	WaitRxData_Func(tmp_LORA);
}


//讀取資料
void Read_LoRa_RxBuffer(volatile LORA_DEFINE_t *tmp_LORA)
{
	tmp_LORA->MyBufferCount = 0;
	tmp_LORA->UserRxCountMax = 30;
	UartToLora(tmp_LORA, Read_LoRa_RxBuffer_Index);
	WaitRxData_Func(tmp_LORA);
}

//讀取接收計數器
void Read_LoRa_RxCount(volatile LORA_DEFINE_t *tmp_LORA)
{
	tmp_LORA->MyBufferCount = 0;
	tmp_LORA->UserRxCountMax = 6;
	UartToLora(tmp_LORA, Read_LoRa_RxCount_Index);
	WaitRxData_Func(tmp_LORA);
}

//設定模式與頻率
void Write_LoRa_Mode_FQ(volatile LORA_DEFINE_t *tmp_LORA, unsigned char tmpMode)
{
	tmp_LORA->MyBufferCount = 0;
	tmp_LORA->UserRxCountMax = 5;
	tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][3] = tmpMode;
	UartToLora(tmp_LORA, Write_LoRa_Mode_FQ_Index);
	WaitRxData_Func(tmp_LORA);
}

//設定Lora參數
void Write_LoRa_Setting(volatile LORA_DEFINE_t *tmp_LORA)
{
	tmp_LORA->MyBufferCount = 0;
	tmp_LORA->UserRxCountMax = 5;
	UartToLora(tmp_LORA, Write_LoRa_Setting_Index);
	WaitRxData_Func(tmp_LORA);
}

//讀取設定狀態
void Read_LoRa_Setting(volatile LORA_DEFINE_t *tmp_LORA)
{
	tmp_LORA->MyBufferCount = 0;
	tmp_LORA->UserRxCountMax = 12;
	UartToLora(tmp_LORA, Read_LoRa_Setting_Index);
	WaitRxData_Func(tmp_LORA);
}

//讀取FW版本、Chip ID
void Read_LoRa_ID(volatile LORA_DEFINE_t *tmp_LORA)
{
	tmp_LORA->MyBufferCount = 0;
	tmp_LORA->UserRxCountMax = 10;
	//tmp_LORA->UserRxCountMax = 9;
	UartToLora(tmp_LORA, Read_LoRa_ID_Index);
	WaitRxData_Func(tmp_LORA);
}



//重置、初始化
void Reset_LoRa(volatile LORA_DEFINE_t *tmp_LORA)
{
	tmp_LORA->MyBufferCount = 0;
	tmp_LORA->UserRxCountMax = 5;
	UartToLora(tmp_LORA, Reset_LoRa_Index);
	WaitRxData_Func(tmp_LORA);
}



void Lora_CMD_Func(volatile LORA_DEFINE_t *tmp_LORA)
{
	volatile unsigned char XORtmp;
	unsigned char i;
		
	switch(tmp_LORA->NowStatus)
	{
		case Print_Lora_CMD_Delay_Index:
			Print_Who_Lora(tmp_LORA);
			Print_Delay_LoRa_CMD_Msg(tmp_LORA);
			tmp_LORA->NowStatus = Lora_CMD_Delay_Index;
			break;
		
		case Lora_CMD_Delay_Index:
			if(tmp_LORA->f_count > Lora_CMD_Delay_Count_Max)
			{	
				if(tmp_LORA->Lora_CRC_Flag == CRCisOK)
				{
					tmp_LORA->NowStatus = tmp_LORA->NextStatus;
					tmp_LORA->LoRa_CRC_Count = 0;
				}
				else
				{
					tmp_LORA->LoRa_CRC_Count = ++tmp_LORA->LoRa_CRC_Count;
					if(tmp_LORA->LoRa_CRC_Count < 3)
					{
						tmp_LORA->NowStatus = tmp_LORA->LastStatus;
						Print_Who_Lora(tmp_LORA);
						uart_str("(C)\r\0");
					}
					else
					{
						Reset_LoRa_Func(tmp_LORA);
					}
				}
			}
			break;
			
		
		case CheckRxCRC:
			Print_Who_Lora(tmp_LORA);
			if(tmp_LORA->Lora_Debug_Flag == 1)
			{
				uTXByte(Debug_COM, 0x09);
			}
			
			//顯示收到的資料 (不含CRC
			XORtmp = 0x00;
			for(i = 0; i < (tmp_LORA->MyBufferCount - 1); i++)
			{
				if(tmp_LORA->Lora_Debug_Flag == 1)
				{
					uart_send_ascii_byte(Debug_COM, tmp_LORA->MyBuffer[i]);
					uTXByte(Debug_COM, ' ');
				}
				XORtmp = XORtmp ^ tmp_LORA->MyBuffer[i];
			}
			
			if(tmp_LORA->Lora_Debug_Flag == 1)
			{
				uTXByte(Debug_COM, '-');
				uTXByte(Debug_COM, ' ');
				//顯示收到的CRC
				uart_send_ascii_byte(Debug_COM, tmp_LORA->MyBuffer[(tmp_LORA->MyBufferCount-1)]);
				uTXByte(Debug_COM, ' ');
				//顯示收到的資料長度
				uart_send_ascii_byte(Debug_COM, tmp_LORA->MyBufferCount);
				uTXByte(Debug_COM, ' ');
				//顯示計算出來的CRC
				uart_send_ascii_byte(Debug_COM, XORtmp);
				uTXByte(Debug_COM, ' ');
			}
			
			//判斷收到的CRC與計算的CRC
			if(tmp_LORA->MyBuffer[(tmp_LORA->MyBufferCount - 1)] != XORtmp) //如果CRC錯誤
			{
				Print_CRC_ERROR(tmp_LORA);	
				tmp_LORA->Lora_CRC_Flag = CRCisErr;
			}
			else //如果CRC正確，準備進行下一次傳輸
			{
				Print_CRC_OK(tmp_LORA);
				tmp_LORA->Lora_CRC_Flag = CRCisOK;
			}
			tmp_LORA->f_count = 0;
			tmp_LORA->NowStatus = Print_Lora_CMD_Delay_Index;	//進入DelayTime，再進行資料傳輸
			break;
		
		case Read_LoRa_ID_Index:
			tmp_LORA->f_count = 0;
			tmp_LORA->Lora_initial_Flag = 0;
			tmp_LORA->LastStatus = Read_LoRa_ID_Index;
			tmp_LORA->NextStatus = Catch_LoRa_ID_Index;
			//============================================
				Print_Who_Lora(tmp_LORA);
				Print_Read_LoRa_ID_Msg(tmp_LORA);
			//============================================
			Read_LoRa_ID(tmp_LORA);			//送出Read_LoRa_ID後，再進入WaitRxData等待Uart回傳
			break;						
			
		case Catch_LoRa_ID_Index:
			tmp_LORA->Lora_initial_Flag = 0;
			Catch_LoRa_ID(tmp_LORA);
			//============================================
				Print_Who_Lora(tmp_LORA);
			//============================================
			change_and_check(&(tmp_LORA->NowStatus), Reset_LoRa_Index);
			break;					//加速切換mode
		
		
		case Reset_LoRa_Index:
			tmp_LORA->Lora_initial_Flag = 0;
			tmp_LORA->LastStatus = Reset_LoRa_Index;
			tmp_LORA->NextStatus = Write_LoRa_Setting_Index;
			//============================================
				Print_Who_Lora(tmp_LORA);
				Print_Reset_LoRa_Msg(tmp_LORA);
			//============================================
			Reset_LoRa(tmp_LORA);
			break;
					
		case Write_LoRa_Setting_Index:
			tmp_LORA->Lora_initial_Flag = 0;
			tmp_LORA->LastStatus = Write_LoRa_Setting_Index;
			tmp_LORA->NextStatus = Write_LoRa_Mode_FQ_Index;
			//============================================
				Print_Who_Lora(tmp_LORA);		
				Print_Write_LoRa_Setting_Msg(tmp_LORA);
			//============================================
			Write_LoRa_Setting(tmp_LORA);
			break;						

		case Write_LoRa_Mode_FQ_Index:	
			//設定LORA TX, RX Mode
			tmp_LORA->Lora_initial_Flag = 0;
			tmp_LORA->LastStatus = Write_LoRa_Mode_FQ_Index;
			tmp_LORA->NextStatus = Switch_LoRa_TX_or_RX_Mode_Index;
			//============================================
				Print_Who_Lora(tmp_LORA);
				Print_Write_LoRa_Mode_FQ_Msg(tmp_LORA);
			//============================================
			switch(tmp_LORA->LoRa_TxRx_Mode_Flag)
			{
				case SetSleep:
					Write_LoRa_Mode_FQ(tmp_LORA, SetSleep);
					if(LORA01->Lora_Show_Flag == true)	uart_str("..z Z\r");
					break;
					
				case SetRx:
					Write_LoRa_Mode_FQ(tmp_LORA, SetRx);
					break;
				
				case SetTx:
					Write_LoRa_Mode_FQ(tmp_LORA, SetTx);
					break;
				
				case SetNOP:
					Write_LoRa_Mode_FQ(tmp_LORA, SetRx);
					break;
			}
			break;							
	
		case Switch_LoRa_TX_or_RX_Mode_Index:	//執行LORA TX, RX, NOP的副程式
			tmp_LORA->Lora_initial_Flag = 1;
			tmp_LORA->f_count = 0;
			switch(tmp_LORA->LoRa_TxRx_Mode_Flag)
			{
				case SetRx:		//接收mode
					tmp_LORA->NowStatus = Wait_RX_IRQ_Index;
					Print_Who_Lora(tmp_LORA);
					Print_into_LoRa_Rx_Mode_Msg(tmp_LORA);
					break;
					
				case SetTx:		//發射mode
					tmp_LORA->NowStatus = Write_LoRa_TxBuffer_Index;
					break;
					
				case SetNOP:	//NOP mode
					tmp_LORA->NowStatus = NOP_Index;
					break;
					
				case SetSleep:
					tmp_LORA->NowStatus = Sleep_Index;
					sleep_count = 0;
					break;
			}
			break;
		
		case Sleep_Index:
			tmp_LORA->f_count = 0;
			//進入Sleep Mode 5秒後wake up mode
			if(sleep_count == sleep_count_max)	
			{
				extint_chan_disable_callback(PIN_PA11, EXTINT_CALLBACK_TYPE_DETECT);
				LoRA_INT_IO_SETTING(false);
				port_pin_set_output_level(PIN_PA11, true);
				delay_ms(3);
				port_pin_set_output_level(PIN_PA11, false);
				delay_ms(3);
				LoRA_INT_IO_SETTING(true);
				extint_chan_enable_callback(PIN_PA11, EXTINT_CALLBACK_TYPE_DETECT);
				tmp_LORA->LoRa_TxRx_Mode_Flag = SetRx;
				tmp_LORA->NowStatus = Reset_LoRa_Index;
				tmp_LORA->LoRa_IRQ_Flag = 0;
				if(LORA01->Lora_Show_Flag == true)	uart_str("Wake Up..\r");
				sleep_count = 0;
			}
			break;
		
		case Wait_RX_IRQ_Index:		//Wait RX IRQ PIN
			#if defined(Dev_Repeater)
				Wait_RX_IRQ_Index_Func(tmp_LORA);	
			#elif defined(Dev_Master_RS232)
				Wait_RX_IRQ_Index_Func(tmp_LORA);	
			#else
				Wait_RX_IRQ_Index_Func(tmp_LORA);
			#endif
			break;

		case Read_LoRa_RxBuffer_Index:	
			tmp_LORA->f_count = 0;
			tmp_LORA->LastStatus = Read_LoRa_RxBuffer_Index;
			tmp_LORA->NextStatus = Reset_IRQ_Flag_Index;
			//============================================
				Print_Who_Lora(tmp_LORA);
				Print_Read_LoRa_RxBuffer_Msg(tmp_LORA);
			//============================================
			Read_LoRa_RxBuffer(tmp_LORA);
			break;
		
		case Reset_IRQ_Flag_Index:
			tmp_LORA->LoRa_IRQ_Flag = 0;
			//============================================
				Print_Who_Lora(tmp_LORA);
				Print_Reset_IRQ_Flag_Msg(tmp_LORA);
			//============================================
			tmp_LORA->NowStatus = Lora_Rx_Data_Ready_Index;
			tmp_LORA->f_count = 0;
			#ifdef Dev_Repeater
				if(tmp_LORA == LORA01)
				{
					LED_R_OFF
				}
				else
				{
					LED_B_OFF
				}
			#else
				LED_R_OFF
			#endif
			break;
		
		case Lora_Rx_Data_Ready_Index:	//接收完畢，等待後續處理 (Main.c)
			break;
			
		case Write_LoRa_TxBuffer_Index:
			LED_G_ON
			tmp_LORA->LastStatus = Write_LoRa_TxBuffer_Index;
			tmp_LORA->NextStatus = Wait_TX_Finish_Index;
			//============================================
				Print_Who_Lora(tmp_LORA);
				Print_Write_LoRa_TxBuffer_Msg(tmp_LORA);
			//============================================
			Write_LoRa_TxBuffer(tmp_LORA);
			break;

		case Wait_TX_Finish_Index:
			if(tmp_LORA->LoRa_IRQ_Flag == 1)
			{
				wdt_reset_count();
				//============================================
					Print_Who_Lora(tmp_LORA);
					Print_Wait_TX_Finish_Msg(tmp_LORA);
				//============================================
				tmp_LORA->LoRa_IRQ_Flag = 0;
				tmp_LORA->NowStatus = Leave_LoRa_Tx_Mode_Index;
				tmp_LORA->f_count = 0;
			}
			break;
		
		case Leave_LoRa_Tx_Mode_Index:
			LED_G_OFF
			//============================================
				Print_Who_Lora(tmp_LORA);
				Print_Leave_LoRa_Tx_Mode_Msg(tmp_LORA);
			//============================================
			
			#if	defined(Dev_Master_RS232)
				tmp_LORA->LoRa_TxRx_Mode_Flag = SetRx;
			#elif defined(Dev_Repeater)
				tmp_LORA->LoRa_TxRx_Mode_Flag = SetRx;
			#else
				tmp_LORA->LoRa_TxRx_Mode_Flag = SetRx;
				//tmp_LORA->LoRa_TxRx_Mode_Flag = SetSleep;
			#endif
			tmp_LORA->NowStatus = Write_LoRa_Setting_Index;
			break;
			
		case NOP_Index:

			break;

	}
}






void Catch_LoRa_ID(volatile LORA_DEFINE_t *tmp_LORA)
{
	tmp_LORA->MyLoRaFW = tmp_LORA->MyBuffer[4];
	
	tmp_LORA->MyLoRaID[0] = tmp_LORA->MyBuffer[5];
	tmp_LORA->MyLoRaID[1] = tmp_LORA->MyBuffer[6];
	tmp_LORA->MyLoRaID[2] = tmp_LORA->MyBuffer[7];
	tmp_LORA->MyLoRaID[3] = tmp_LORA->MyBuffer[8];

	tmp_LORA->LoRaID[0] = tmp_LORA->MyBuffer[5];
	tmp_LORA->LoRaID[1] = tmp_LORA->MyBuffer[6];
	tmp_LORA->LoRaID[2] = tmp_LORA->MyBuffer[7];
	tmp_LORA->LoRaID[3] = tmp_LORA->MyBuffer[8];
	
	if(	tmp_LORA->UserLoRaID[0] == 0xFF &&
		tmp_LORA->UserLoRaID[1] == 0xFF &&
		tmp_LORA->UserLoRaID[2] == 0xFF &&
		tmp_LORA->UserLoRaID[3] == 0xFF)
	{
		return;
	}
	
	if(	tmp_LORA->UserLoRaID[0] != tmp_LORA->MyLoRaID[0] || 
		tmp_LORA->UserLoRaID[1] != tmp_LORA->MyLoRaID[1] ||
		tmp_LORA->UserLoRaID[2] != tmp_LORA->MyLoRaID[2] ||
		tmp_LORA->UserLoRaID[3] != tmp_LORA->MyLoRaID[3])
	{
		tmp_LORA->MyLoRaID[0] = tmp_LORA->UserLoRaID[0];
		tmp_LORA->MyLoRaID[1] = tmp_LORA->UserLoRaID[1];
		tmp_LORA->MyLoRaID[2] = tmp_LORA->UserLoRaID[2];
		tmp_LORA->MyLoRaID[3] = tmp_LORA->UserLoRaID[3];
	}


}

void Catch_LoRa_Setting(volatile LORA_DEFINE_t *tmp_LORA)
{
	tmp_LORA->MyLoRaMode =	tmp_LORA->MyBuffer[3];
	tmp_LORA->MyLoRaPower = tmp_LORA->MyBuffer[7];
	tmp_LORA->MyLoRaSF =	tmp_LORA->MyBuffer[10];
	tmp_LORA->MyLoRaFQ.byte[3] = 0;
	tmp_LORA->MyLoRaFQ.byte[2] = tmp_LORA->MyBuffer[4];
	tmp_LORA->MyLoRaFQ.byte[1] = tmp_LORA->MyBuffer[5];
	tmp_LORA->MyLoRaFQ.byte[0] = tmp_LORA->MyBuffer[6];
}