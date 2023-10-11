/*
 * WIFI_callback.h
 *
 * Created: 2021/4/19 下午 03:36:33
 *  Author: willchen
 */ 


#ifndef WIFI_CALLBACK_H_
#define WIFI_CALLBACK_H_


#define			uart_buffer_max	500
volatile char tmp_uart_buf[uart_buffer_max];



void Uart_Form_WIFI_callback(struct usart_module *const usart_module);


#endif /* WIFI_CALLBACK_H_ */