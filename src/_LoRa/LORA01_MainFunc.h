/*
 * LORA01_MainFunc.c
 *
 * Created: 2021/4/19 下午 04:14:06
 *  Author: willchen
 */ 
void Running_Function(void);



void Display_Rssi(volatile LORA_DEFINE_t *tmp_LORA);
void Read_LoRa_RxBuffer(volatile LORA_DEFINE_t *tmp_LORA);
void Read_LoRa_RxCount(volatile LORA_DEFINE_t *tmp_LORA);
void Write_LoRa_Mode_FQ(volatile LORA_DEFINE_t *tmp_LORA, unsigned char tmpMode);
void Read_LoRa_Setting(volatile LORA_DEFINE_t *tmp_LORA);
void Read_LoRa_ID(volatile LORA_DEFINE_t *tmp_LORA);
void Reset_LoRa(volatile LORA_DEFINE_t *tmp_LORA);
void Write_LoRa_Setting(volatile LORA_DEFINE_t *tmp_LORA);
void Write_LoRa_TxBuffer(volatile LORA_DEFINE_t *tmp_LORA);
unsigned char MyXORFunc(unsigned char tmpA, unsigned char tmpB);
void UartToLora(volatile LORA_DEFINE_t *tmp_LORA, unsigned char Myindex);
void Lora_CMD_Func(volatile LORA_DEFINE_t *tmp_LORA);
void Setting_Lora_To_Tx_Mode_Func(volatile LORA_DEFINE_t *tmp_LORA);
void Catch_LoRa_ID(volatile LORA_DEFINE_t *tmp_LORA);
void Catch_DIP_SW_For_SF_in_Master(void);
void Catch_LoRa_Setting(volatile LORA_DEFINE_t *tmp_LORA);

#define Lora_CMD_Delay_Count_Max		90	//30
#define Lora_Mode_Convert_Delay_500ms	400
#define Lora_Wait_RX_Delay_1500ms		2000
#define Lora_Wait_RX_Delay_1000ms		1500
//#define Lora_Wait_RX_Delay_1000ms		2500
#define Lora_Wait_RX_Delay_3000ms		3750
#define Lora_CMD_Timeout				800
//#define Lora_CMD_Timeout				5000

#define LoraTxNoSend	0
#define LoraTxSendOK	1

#define LoraRxNoData	0
#define LoraRxDataReady 1


typedef enum{
	//=============================
	//	跟SX1272的封包有關係，不可以更動
	//=============================
	Read_LoRa_ID_Index			= 0,
	Reset_LoRa_Index			= 1,
	Read_LoRa_Setting_Index		= 2,
	Write_LoRa_Mode_FQ_Index	= 3,
	Write_LoRa_Setting_Index	= 4,
	Write_LoRa_TxBuffer_Index	= 5,
	Read_LoRa_RxBuffer_Index	= 6,
	Read_LoRa_RxCount_Index		= 7,
	//=============================
	//	以下可以自由更動
	//=============================
	Reset_IRQ_Flag_Index						= 100,
	Leave_LoRa_Tx_Mode_Index					= 101,
	NOP_Index									= 102,
	WaitRxData									= 103,
	CheckRxCRC									= 104,
	Catch_LoRa_ID_Index							= 105,
	Change_LoRa_Tx_Mode_Data_Index				= 106,
	Lora_CMD_Delay_Index						= 107,
	Wait_TX_Finish_Index						= 108,
	Catch_Lora_RX_Count_Form_Module_Index		= 109,
	Catch_LoRa_Setting_Index					= 110,
	Print_Delay_Some_Time_Index					= 111,
	Delay_Some_Time_Index						= 112,
	Print_Lora_CMD_Delay_Index					= 113,
	Switch_LoRa_TX_or_RX_Mode_Index				= 114,
	Wait_RX_IRQ_Index							= 115,
	Lora_Rx_Data_Ready_Index					= 116,
	Lora_CMD_Wait_Index							= 126,
	Sleep_Index									= 127,
}LoRA_rundown;

typedef enum{
	//=============================
	//	跟SX1272的封包有關係，不可以更動
	//=============================
	SetSleep =	0x00,
	SetTx =		0x02,
	SetRx =		0x03,
	//=============================
	//	以下可以自由更動
	//=============================
	SetNOP =	0x04,
}LoRA_Value;