/*
 * UART_Func.h
 *
 * Created: 2021/4/19 下午 03:26:16
 *  Author: willchen
 */ 


#ifndef UART_FUNC_H_
#define UART_FUNC_H_

void uTXByte(struct usart_module *const usart_module, uint8_t tmp_data);
void uart_str_COM(struct usart_module *const usart_module, char *tmp_ptr);
void uart_str(const char *tmp_ptr);
unsigned char hex_to_ascii(unsigned char tmp);
unsigned char ascii_check_0_and_9_and_return_hex(unsigned char tmp);
void string_to_4str(uSetting_item_Dev_t *tmp);
unsigned char ascii_to_hex(unsigned char tmp);
unsigned int hex_to_bcd(unsigned int tmp);
unsigned long long_to_bcd(unsigned long tmp);
void uart_send_word(struct usart_module *const usart_module,unsigned int tmp);
void uart_send_ascii_byte(struct usart_module *const usart_module, uint8_t tmp);
void tx_dword(unsigned long tmp_data);
void tx_byte(unsigned char tmp_data);



#endif /* UART_FUNC_H_ */