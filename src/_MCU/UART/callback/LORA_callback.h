/*
 * LORA01_callback.h
 *
 * Created: 2021/4/19 下午 03:19:12
 *  Author: willchen
 */ 


#ifndef LORA01_CALLBACK_H_
#define LORA01_CALLBACK_H_


void Uart_Form_LORA01_callback(struct usart_module *const usart_module);
void Uart_Form_LORA02_callback(struct usart_module *const usart_module);

#endif /* LORA01_CALLBACK_H_ */