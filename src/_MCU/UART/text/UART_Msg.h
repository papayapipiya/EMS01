/*
 * UART_Msg.h
 *
 * Created: 2021/4/19 下午 03:31:19
 *  Author: willchen
 */ 


#ifndef UART_MSG_H_
#define UART_MSG_H_



void Print_Find_Msg(void);
void Print_Start_Msg(void);
void Print_Line_Msg(void);
void Print_Enable_CMD_Msg(void);
void Print_Disable_CMD_Msg(void);
void Print_Dev_Time_Value(unsigned char tmp_index, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void Print_One_Form_List_Table(unsigned char tmp_index, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void Print_MissNode_Form_RX_Msg(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void Print_Catch_Data_Form_RX_Msg(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void Print_Now_And_Next_Msg(volatile LORA_DEFINE_t *tmp_LORA, volatile LORA_Dev_List_t *tmp_LORA_Dev_List_Index);
void Print_Send_Dev_No_Msg(volatile LORA_DEFINE_t *tmp_LORA);


#endif /* UART_MSG_H_ */