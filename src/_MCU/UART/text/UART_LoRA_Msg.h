/*
 * UART_LoRA_Msg.h
 *
 * Created: 2021/4/19 下午 03:28:42
 *  Author: willchen
 */ 


#ifndef UART_LORA_MSG_H_
#define UART_LORA_MSG_H_


//===============================
//	Print Msg Function
//===============================
void Print_Read_LoRa_ID_Msg(volatile LORA_DEFINE_t *tmp_LORA);
void Print_Delay_LoRa_CMD_Msg(volatile LORA_DEFINE_t *tmp_LORA);
void Print_Time_Out_Msg(volatile LORA_DEFINE_t *tmp_LORA);
void Print_Catch_LoRa_ID_Msg(volatile LORA_DEFINE_t *tmp_LORA);
void Print_Reset_LoRa_Msg(volatile LORA_DEFINE_t *tmp_LORA);
void Print_Read_LoRa_Setting_Msg(volatile LORA_DEFINE_t *tmp_LORA);
void Print_Write_LoRa_Mode_FQ_Msg(volatile LORA_DEFINE_t *tmp_LORA);
void Print_Write_LoRa_Setting_Msg(volatile LORA_DEFINE_t *tmp_LORA);
void Print_Read_LoRa_RxBuffer_Msg(volatile LORA_DEFINE_t *tmp_LORA);
void Print_Reset_IRQ_Flag_Msg(volatile LORA_DEFINE_t *tmp_LORA);
void Print_Write_LoRa_TxBuffer_Msg(volatile LORA_DEFINE_t *tmp_LORA);
void Print_Leave_LoRa_Tx_Mode_Msg(volatile LORA_DEFINE_t *tmp_LORA);
void Print_into_LoRa_Rx_Mode_Msg(volatile LORA_DEFINE_t *tmp_LORA);
void Print_Wait_RX_IRQ_Index(volatile LORA_DEFINE_t *tmp_LORA);
void Print_CRC_ERROR(volatile LORA_DEFINE_t *tmp_LORA);
void Print_CRC_OK(volatile LORA_DEFINE_t *tmp_LORA);

void Print_One_RSSI_Msg(volatile int tmp_rssi);

void Print_Debug_Mode_ON_Msg(void);
void Print_Debug_Mode_OFF_Msg(void);
void Print_Lora_RX_Count_Form_Module_Msg(volatile LORA_DEFINE_t *tmp_LORA);
void Print_Wait_TX_Finish_Msg(volatile LORA_DEFINE_t *tmp_LORA);
void Print_Change_LoRa_Tx_Mode_Data_Msg(void);
void Print_Catch_LoRa_Setting_Msg(volatile LORA_DEFINE_t *tmp_LORA);
void Print_List_Table(volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void Print_Lora_number(volatile LORA_DEFINE_t *tmp_LORA);





void Print_IIC_Time_Out_Msg(void);
void Print_LowPower_Msg(volatile LORA_DEFINE_t *tmp_LORA);
void Print_LoRA_Time_Out_Msg(volatile LORA_DEFINE_t *tmp_LORA);
void Print_RS485_Time_Out(volatile LORA_DEFINE_t *tmp_LORA);
void Print_Debug_Msg(void);
void Print_Who_Lora(volatile LORA_DEFINE_t *tmp_LORA);






#endif /* UART_LORA_MSG_H_ */