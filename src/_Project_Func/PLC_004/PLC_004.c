#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

//	立瑞新庄，百通飼料秤重

//=================================================
#ifdef Dev_PLC_004
//=================================================

#define PLC_ADDR		0x01
#define PLC_D_CMD		0x03
//#define PLC_D_ADDR_base		400001
#define PLC_D_ADDR			0000
unsigned char tmp_ModBus_Data[8];



UINT32u_t SaveFeedWeight;
UINT32u_t NowFeedWeight;
UINT32u_t SaveACCFeedWeight;
UINT32u_t ACCFeedWeight;
UINT32u_t Last_FeedWeight; 


unsigned long tmp_w;
bool PowerUP;
unsigned char W_count;
unsigned char W_NowStatus;

typedef enum
{
	IsPowerON,		//	0
	IsJudg,		//	0
	IsBigJudg,
	IsBig,
	IsSmallJudg,
	IsSmall,
}feedw_rundown;

#define FeedW_Count 10

typedef struct PLC_D_List_s
{
	unsigned int	addr;
	unsigned int	lenght;
	UINT16u_t		Buf;
}PLC_D_List_t;
PLC_D_List_t PLC_D_List[RS485_CMD_Index_Max];


void Save_Feed_Count_in_NVM(void)
{
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	UINT32u_t	tmp32;
	
	do
	{
		error_code = nvm_read_buffer(
		151  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	tmp_page_buffer[0] = SaveACCFeedWeight.byte[3];
	tmp_page_buffer[1] = SaveACCFeedWeight.byte[2];
	tmp_page_buffer[2] = SaveACCFeedWeight.byte[1];
	tmp_page_buffer[3] = SaveACCFeedWeight.byte[0];
	
	do
	{
		error_code = nvm_erase_row(
		151 * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	do
	{
		error_code = nvm_write_buffer(
		151  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	Last_FeedWeight.dword = SaveACCFeedWeight.dword;
}



void Load_Feed_Count_in_NVM(void)
{
	UINT32u_t	tmp32;
	uint8_t tmp_page_buffer[NVMCTRL_PAGE_SIZE];
	do
	{
		error_code = nvm_read_buffer(
		151  * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE,
		tmp_page_buffer, NVMCTRL_PAGE_SIZE);
	} while (error_code == STATUS_BUSY);
	
	SaveACCFeedWeight.byte[3] = tmp_page_buffer[0];
	SaveACCFeedWeight.byte[2] = tmp_page_buffer[1];
	SaveACCFeedWeight.byte[1] = tmp_page_buffer[2];
	SaveACCFeedWeight.byte[0]=  tmp_page_buffer[3];
	
	Last_FeedWeight.dword = SaveACCFeedWeight.dword;

}


void Send_PLC_D_Format(unsigned int tmp_ADDR)
{
	UINT16u_t tmp16;
	tmp_ModBus_Data[0] = PLC_ADDR;
	tmp_ModBus_Data[1] = PLC_D_CMD;
	tmp16.word = PLC_D_ADDR + PLC_D_List[tmp_ADDR].addr;
	tmp_ModBus_Data[2] = tmp16.byte[1];
	tmp_ModBus_Data[3] = tmp16.byte[0];
	//tmp16.word = PLC_D_List[tmp_ADDR].lenght;
	tmp16.word = 1;
	tmp_ModBus_Data[4] = tmp16.byte[1];
	tmp_ModBus_Data[5] = tmp16.byte[0];
	tmp16.word = crc_chk(&tmp_ModBus_Data[0], 6);
	tmp_ModBus_Data[6] = tmp16.byte[0];
	tmp_ModBus_Data[7] = tmp16.byte[1];
}


void Read_Modbus_Func(void)
{
	UINT16u_t tmp16;
	UINT16u_t CRC16;
	volatile Write_PLC_List_t *Write_PLC_List_Index = Write_PLC_List_In_RAM;
	volatile LORA_Dev_List_t *LORA_Dev_List_Index = LORA_Dev_List_In_RAM;
	
	//選擇modbus位置
	Run_LoRA_Function(LORA_Dev_List_Index, Write_PLC_List_Index);
	Send_PLC_D_Format(RS485_CMD_Index);
	
	//顯示準備要對modbus送出的資料
	Run_LoRA_Function(LORA_Dev_List_Index, Write_PLC_List_Index);
	if(PLC_Show_r_Flag)
	{
		uTXByte(Debug_COM, 0x09);
		uTXByte(Debug_COM, '[');
		for(unsigned char j = 0; j < 8; j++)
		{
			uart_send_ascii_byte(Debug_COM, tmp_ModBus_Data[j]);
			uTXByte(Debug_COM, ' ');
		}
		uTXByte(Debug_COM, ']');
		uTXByte(Debug_COM, ' ');
	}
	
	//實際將資料送出到modbus
	Run_LoRA_Function(LORA_Dev_List_Index, Write_PLC_List_Index);
	RS485_For_Debug_Dn
	for(unsigned char j = 0; j < 8; j++)
	{
		uTXByte(RS485_COM, tmp_ModBus_Data[j]);
	}
	RS485_For_Debug_En
	
	
	//等待modbus設備回應
	RS485_RX_Buf_Count = 0;
	RS485_Time_Count = 0;
	while(1)
	{
		usart_read_buffer_job(RS485_COM, (uint8_t *)RS485_RX_Buf, MAX_RX_BUFFER_LENGTH);
		if(RS485_RX_Buf_Count == RS485_RX_D_Buf_Count)	break;
		
		if(RS485_Time_Count == 10)
		{
			Print_RS485_Time_Out(LORA01);
			break;
		}
	}
	
	//顯示收到的modbus資料
	Run_LoRA_Function(LORA_Dev_List_Index, Write_PLC_List_Index);
	if(PLC_Show_r_Flag)
	{
		for(unsigned char j = 0; j < RS485_RX_D_Buf_Count; j++)
		{
			uart_send_ascii_byte(Debug_COM, RS485_RX_In_Buf[j]);
			uTXByte(Debug_COM, ' ');
		}
	}
	
	//檢查CRC狀況
	Run_LoRA_Function(LORA_Dev_List_Index, Write_PLC_List_Index);
	tmp16.word = crc_chk(&RS485_RX_In_Buf[0], 5);
	if(PLC_Show_r_Flag)
	{
		uTXByte(Debug_COM, '(');
		uart_send_ascii_byte(Debug_COM, tmp16.byte[0]);
		uart_send_ascii_byte(Debug_COM, tmp16.byte[1]);
		uTXByte(Debug_COM, ')');
		uTXByte(Debug_COM, ' ');
	}
	
	Run_LoRA_Function(LORA_Dev_List_Index, Write_PLC_List_Index);
	if(tmp16.byte[0] != RS485_RX_In_Buf[5] || tmp16.byte[1] != RS485_RX_In_Buf[6])
	{
		if(PLC_Show_r_Flag)	uart_str("CRC NG\r\0");
	}
	else
	{
		PLC_D_List[RS485_CMD_Index].Buf.byte[1] = RS485_RX_In_Buf[3];
		PLC_D_List[RS485_CMD_Index].Buf.byte[0] = RS485_RX_In_Buf[4];
		if(PLC_Show_r_Flag)
		{
			uart_str("CRC OK -\0");
			uart_send_word(Debug_COM, PLC_D_List[RS485_CMD_Index].Buf.word);
			uTXByte(Debug_COM, 0x0d);
		}
	}
	
	//選擇下一筆資料
	Run_LoRA_Function(LORA_Dev_List_Index, Write_PLC_List_Index);
	RS485_CMD_Index ++;
	if(RS485_CMD_Index == RS485_CMD_Index_Max)
	{
		RS485_CMD_Index = 0;
	}
}


void PLC_Main_Func(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index)
{
	UINT16u_t tmp16;
	UINT16u_t CRC16;
	long tmp32;
	//return;
	Read_Modbus_Func();

	
	NowFeedWeight.word[1] = PLC_D_List[1].Buf.word;
	NowFeedWeight.word[0] = PLC_D_List[0].Buf.word;

	tmp32 = NowFeedWeight.dword;
		
	
	//if(NowFeedWeight.dword > 0xFFFF0000)
	if(tmp32 < 0)
	{
		NowFeedWeight.dword = 0;
	}
	

	
		
	switch(W_NowStatus)
	{
		case IsPowerON:
			SaveFeedWeight.dword = NowFeedWeight.dword;
			PowerUP = true;
			W_NowStatus = IsJudg;
			break;
				
		case IsJudg:
			W_count = 0;	
			ACCFeedWeight.dword = 0;
			if(SaveFeedWeight.dword > NowFeedWeight.dword)
			{
				W_NowStatus = IsBigJudg;
			}
			else
			{
				W_NowStatus = IsSmallJudg;
			}
			break;
			
		case IsBigJudg:
			if(SaveFeedWeight.dword >= NowFeedWeight.dword)
			{
				ACCFeedWeight.dword = SaveFeedWeight.dword - NowFeedWeight.dword;
				if(ACCFeedWeight.dword > 100)
				{
					if(W_count < FeedW_Count)		W_count++; 	
					if(W_count == FeedW_Count)		W_NowStatus = IsBig;
				}
			}
			else
			{
				W_NowStatus = IsJudg;
				W_count = 0;
			}
			break;
				
		case IsSmallJudg:
			if(SaveFeedWeight.dword <= NowFeedWeight.dword)
			{
				ACCFeedWeight.dword = NowFeedWeight.dword - SaveFeedWeight.dword;
				if(ACCFeedWeight.dword > 100)
				{
					if(W_count < FeedW_Count)		W_count++;
					if(W_count == FeedW_Count)		W_NowStatus = IsSmall;
				}
			}
			else
			{
				W_NowStatus = IsJudg;
				W_count = 0;
			}
			break;
				
		case IsBig:
			ACCFeedWeight.dword = SaveFeedWeight.dword - NowFeedWeight.dword;
			SaveACCFeedWeight.dword += ACCFeedWeight.dword;
			if(SaveACCFeedWeight.dword > Max_Feeding_Value)
			{
				SaveACCFeedWeight.dword = SaveACCFeedWeight.dword - Max_Feeding_Value;
			}
			
			SaveFeedWeight.dword = NowFeedWeight.dword;
			W_NowStatus = IsJudg;
			break;
				
		case IsSmall:
			SaveFeedWeight.dword = NowFeedWeight.dword;
			W_NowStatus = IsJudg;
			break;
	}
	
	if(Show_weight_Flag)
	{
		if(W_NowStatus == IsJudg)		uart_str("IsJudg       ");
		if(W_NowStatus == IsBigJudg)	uart_str("IsBigJudgt   ");
		if(W_NowStatus == IsSmallJudg)	uart_str("IsSmallJudg  ");
		if(W_NowStatus == IsBig)		uart_str("IsBig        ");
		if(W_NowStatus == IsSmall)		uart_str("IsSmall      ");
		
		uart_str("NVMw: ");
		uart_send_word(Debug_COM, Last_FeedWeight.word[1]);
		uart_send_word(Debug_COM, Last_FeedWeight.word[0]);
		uart_str(" Sw: ");
		uart_send_word(Debug_COM, SaveFeedWeight.word[1]);
		uart_send_word(Debug_COM, SaveFeedWeight.word[0]);
		uart_str(" w: ");
		uart_send_word(Debug_COM, NowFeedWeight.word[1]);
		uart_send_word(Debug_COM, NowFeedWeight.word[0]);
		uart_str(" Aw: ");
		uart_send_word(Debug_COM, ACCFeedWeight.word[1]);
		uart_send_word(Debug_COM, ACCFeedWeight.word[0]);
		uart_str(" SAw: ");
		uart_send_word(Debug_COM, SaveACCFeedWeight.word[1]);
		uart_send_word(Debug_COM, SaveACCFeedWeight.word[0]);
		uart_str(" ");
		uart_send_word(Debug_COM, W_count);
		uTXByte(Debug_COM, 0x0d);
		
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


void inital_PLC_D_List_Value(unsigned int tmp_index, unsigned int tmp_addr)
{
	PLC_D_List[tmp_index].addr = tmp_addr;
}


const unsigned int Define_PLC_D_List[RS485_CMD_Index_Max] =
{
	0,1,2,3,4,5,6,7,8,9
};

void Dev_Value_inital(void)
{
	RS485_CMD_Index = 0;
	RS485_LORA_OUT_Index = 0;
	//RS485_LORA_OUT_Index = 2;
	unsigned int *ptr;
	for(unsigned int i = 0; i < RS485_CMD_Index_Max; i++)
	{
		ptr = &Define_PLC_D_List[i];
		inital_PLC_D_List_Value(i, *(ptr));	
	}
	 
}



uint8_t Define_MyRS485Tmp[16][6] =
{
	//第二間雞舍
	{PLC_ADDR, PLC_D_CMD, 0x18, 0x38, 0x00, 0x01},	//溫度1
	{PLC_ADDR, PLC_D_CMD, 0x18, 0x39, 0x00, 0x01},	//溫度2
	{PLC_ADDR, PLC_D_CMD, 0x18, 0x3A, 0x00, 0x01},	//濕度1
	{PLC_ADDR, PLC_D_CMD, 0x18, 0x40, 0x00, 0x01},	//上開度
	{PLC_ADDR, PLC_D_CMD, 0x18, 0x41, 0x00, 0x01},	//下開度
	{PLC_ADDR, PLC_D_CMD, 0x18, 0x38, 0x00, 0x01},	//溫度1
	//{PLC_ADDR, PLC_D_CMD, 0x07, 0xE0, 0x00, 0x0C},	//M暫存器
	
	{PLC_ADDR, PLC_D_CMD, 0x17, 0xDA, 0x00, 0x01},	//風速
	{PLC_ADDR, PLC_D_CMD, 0x17, 0xDB, 0x00, 0x01},	//負壓
	{PLC_ADDR, PLC_D_CMD, 0x17, 0xDC, 0x00, 0x01},	//上層開度
	{PLC_ADDR, PLC_D_CMD, 0x17, 0xDD, 0x00, 0x01},	//每日飼料
	{PLC_ADDR, PLC_D_CMD, 0x17, 0xE0, 0x00, 0x01},	//每日飲水
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
	W_NowStatus = IsPowerON;
//		SaveACCFeedWeight.dword = 0x00016AB8;
//		Save_Feed_Count_in_NVM();
//		Load_Feed_Count_in_NVM();
	Load_Feed_Count_in_NVM();
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
	
			if(Last_FeedWeight.dword != SaveACCFeedWeight.dword)
			{
				
				Save_Feed_Count_in_NVM();
				Load_Feed_Count_in_NVM();
			}
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][3] = tmp_LORA->MyLoRaID[0];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][4] = tmp_LORA->MyLoRaID[1];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][5] = tmp_LORA->MyLoRaID[2];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][6] = tmp_LORA->MyLoRaID[3];
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][7] = 0x05; // 力固PLC
			
			//	接收發射的功率設定，並寫回自己的發射功率
			tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7] = tmp_LORA->MyBuffer[7];
			tmp_LORA->MyLoRaPower = tmp_LORA->MyLoRaTmp[Write_LoRa_Mode_FQ_Index][7];
			
			tmp_RS485_Data.word = RS485_LORA_OUT_Index;
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][8] = RS485_LORA_OUT_Index;
			
			//SaveACCFeedWeight.dword = 0x0300;
			
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
			ADC_LVD_Temp = 0;
			
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][22] = 0;
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][23] = 0;
			
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][24] = Version;
			
			//預留給RSSI
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][25] = 0;
			tmp_LORA->MyLoRaTmp[Write_LoRa_TxBuffer_Index][26] = 0;
			
			
			if(RS485_LORA_OUT_Index == RS485_LORA_OUT_Index_Max)
			{
				RS485_LORA_OUT_Index = 0;
			}
			else
			{
				RS485_LORA_OUT_Index ++;
			}
			RS485_Time_Count = 0;
			
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
	uTXByte(Debug_COM, '4');
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