/*
 * UART_initial.h
 *
 * Created: 2021/4/19 下午 03:15:15
 *  Author: willchen
 */ 


#ifndef UART_INITIAL_H_
#define UART_INITIAL_H_

//! [module_inst]
volatile struct usart_module		usart_instance1;
volatile struct usart_module		usart_instance2;
volatile struct usart_module		usart_instance3;
volatile struct usart_module		usart_instance4;
#define MyUart01		usart_instance1
#define MyUart02		usart_instance2
#define MyUart03		usart_instance3
#define MyUart04		usart_instance4


#define Debug_COM		&MyUart02 //使用RS485的UART
//#define Debug_COM		&MyUart03 //ADC的UART
//#define Debug_COM_ADC	&MyUart02
#define Display_COM		&MyUart04

void configure_usart(void);
void configure_usart_callbacks(void);
void usart_read_callback(struct usart_module *const usart_module);
void usart_write_callback(struct usart_module *const usart_module);
void Print_Arrow_Right(void);





#endif /* UART_INITIAL_H_ */