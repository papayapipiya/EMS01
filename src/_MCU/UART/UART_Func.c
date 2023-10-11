#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



void string_to_4str(uSetting_item_Dev_t *tmp)
{
	char str[100];
	char originalText[5];
	int desiredLength = 4;
	
	
	sprintf(str, "\r tmp->userKeyin_Buffer: %s", tmp->userKeyin_Buffer);
	uart_str_COM(Debug_COM, str);
	
	sprintf(originalText, "%s", tmp->userKeyin_Buffer);
	
	int originalLength = strlen(originalText);
	sprintf(str, ", originalText: %s (%d)", originalText, originalLength);
	uart_str_COM(Debug_COM, str);

	if (originalLength < desiredLength)
	{
		int numberOfZerosToAdd = desiredLength - originalLength;
		char newText[desiredLength + 1];

		// 在新字符串中添加0
		for (int i = 0; i < numberOfZerosToAdd; i++)
		{
			newText[i] = '0';
		}

		// 将原始文本复制到新字符串中
		strcpy(newText + numberOfZerosToAdd, originalText);
		sprintf(tmp->userKeyin_Buffer, "%s", newText);
		sprintf(str, ", return : %s", tmp->userKeyin_Buffer);
		uart_str_COM(Debug_COM, str);
	}
}


void uart_str(const char *tmp_ptr)
{
	char *ptr;
	ptr = tmp_ptr;
	while(*ptr != '\0')
	{
		uTXByte(Debug_COM, *ptr);
		ptr++;
	}
}

void uart_str_COM(struct usart_module *const usart_module, char *tmp_ptr)
{
	char *ptr;
	ptr = tmp_ptr;
	while(*ptr != '\0')
	{
		uTXByte(usart_module, *ptr);
		ptr++;
	}
}

void tx_byte(unsigned char tmp_data)
{
	UINT8u_t tmp8;
	tmp8.byte = tmp_data;
	uTXByte(Debug_COM, hex_to_ascii(tmp8.HL.H));
	uTXByte(Debug_COM, hex_to_ascii(tmp8.HL.L));
}

void tx_dword(unsigned long tmp_data)
{
	UINT32u_t tmp32;
	tmp32.dword = tmp_data;
	tx_byte(tmp32.byte[3]);
	tx_byte(tmp32.byte[2]);
	tx_byte(tmp32.byte[1]);
	tx_byte(tmp32.byte[0]);
}

void uart_send_ascii_byte(struct usart_module *const usart_module, uint8_t tmp)
{
	UINT8u_t	tmp8;
	tmp8.byte = tmp;
	uTXByte(usart_module, hex_to_ascii(tmp8.HL.H));
	uTXByte(usart_module, hex_to_ascii(tmp8.HL.L));
}

void uart_send_word(struct usart_module *const usart_module,unsigned int tmp)
{
	UINT16u_t	tmp16;
	UINT8u_t	tmp8;
	tmp16.word = tmp;
	tmp8.byte = tmp16.byte[1];
	uTXByte(usart_module, hex_to_ascii(tmp8.HL.H));
	uTXByte(usart_module, hex_to_ascii(tmp8.HL.L));
	tmp8.byte = tmp16.byte[0];
	uTXByte(usart_module, hex_to_ascii(tmp8.HL.H));
	uTXByte(usart_module, hex_to_ascii(tmp8.HL.L));
}

void uart_send_word_for_ListNumber(struct usart_module *const usart_module,unsigned int tmp)
{
	UINT16u_t	tmp16;
	UINT8u_t	tmp8;
	tmp16.word = tmp;
	tmp8.byte = tmp16.byte[1];
	uTXByte(usart_module, hex_to_ascii(tmp8.HL.L));
	tmp8.byte = tmp16.byte[0];
	uTXByte(usart_module, hex_to_ascii(tmp8.HL.H));
	uTXByte(usart_module, hex_to_ascii(tmp8.HL.L));
}


void Print_Write_Buffer(void)
{
	uTXByte(Debug_COM, '[');
	uTXByte(Debug_COM, ' ');
	for(unsigned char i = 0; i < 18; i++)
	{
		uart_send_ascii_byte(Debug_COM, LORA01->MyBuffer[ 9 + i ]);
		uTXByte(Debug_COM, ' ');
	}
	uTXByte(Debug_COM, ']');
}

void uTXByte(struct usart_module *const usart_module, uint8_t tmp_data)
{
	usart_write_wait(usart_module, tmp_data);
	//usart_write_buffer_job(usart_module, &tmp_data, 1);
	//if(usart_module == Debug_COM)
	//{
		////RS485_EN_High
		//usart_write_wait(usart_module, tmp_data);
		////RS485_EN_Low
	//}
	//else
	//{
		//usart_write_wait(usart_module, tmp_data);
	//}
	
}

unsigned char hex_to_ascii(unsigned char tmp)
{
	if(tmp > 9)
	{
		return (tmp + 0x37);
	}
	else
	{
		return (tmp + 0x30);
	}
}


unsigned char ascii_check_0_and_9_and_return_hex(unsigned char tmp)
{
	if(tmp == '0') return KEY0;
	if(tmp == '1') return KEY1;
	if(tmp == '2') return KEY2;
	if(tmp == '3') return KEY3;
	if(tmp == '4') return KEY4;
	if(tmp == '5') return KEY5;
	if(tmp == '6') return KEY6;
	if(tmp == '7') return KEY7;
	if(tmp == '8') return KEY8;
	if(tmp == '9') return KEY9;
	return 0xff;
}



unsigned char ascii_to_hex(unsigned char tmp)
{
	if(tmp == '0') return KEY0;
	if(tmp == '1') return KEY1;
	if(tmp == '2') return KEY2;
	if(tmp == '3') return KEY3;
	if(tmp == '4') return KEY4;
	if(tmp == '5') return KEY5;
	if(tmp == '6') return KEY6;
	if(tmp == '7') return KEY7;
	if(tmp == '8') return KEY8;
	if(tmp == '9') return KEY9;
	
	if(tmp == 'a') return KEYA;
	if(tmp == 'b') return KEYB;
	if(tmp == 'c') return KEYC;
	if(tmp == 'd') return KEYD;
	if(tmp == 'e') return KEYE;
	if(tmp == 'f') return KEYF;
	
	if(tmp == 'A') return KEYA;
	if(tmp == 'B') return KEYB;
	if(tmp == 'C') return KEYC;
	if(tmp == 'D') return KEYD;
	if(tmp == 'E') return KEYE;
	if(tmp == 'F') return KEYF;
	
	if(tmp == 'g') return KEYG;
	if(tmp == 'h') return KEYH;
	if(tmp == 'i') return KEYI;
	if(tmp == 'j') return KEYJ;
	if(tmp == 'k') return KEYK;
	if(tmp == 'l') return KEYL;
	if(tmp == 'm') return KEYM;
	if(tmp == 'n') return KEYN;
	if(tmp == 'o') return KEYO;
	if(tmp == 'p') return KEYP;
	if(tmp == 'q') return KEYQ;
	if(tmp == 'r') return KEYR;
	if(tmp == 's') return KEYS;
	if(tmp == 't') return KEYT;
	if(tmp == 'u') return KEYU;
	if(tmp == 'v') return KEYV;
	if(tmp == 'w') return KEYW;
	if(tmp == 'x') return KEYX;
	if(tmp == 'y') return KEYY;
	if(tmp == 'z') return KEYZ;
	
	if(tmp == 'G') return KEYG;
	if(tmp == 'H') return KEYH;
	if(tmp == 'I') return KEYI;
	if(tmp == 'J') return KEYJ;
	if(tmp == 'K') return KEYK;
	if(tmp == 'L') return KEYL;
	if(tmp == 'M') return KEYM;
	if(tmp == 'N') return KEYN;
	if(tmp == 'O') return KEYO;
	if(tmp == 'P') return KEYP;
	if(tmp == 'Q') return KEYQ;
	if(tmp == 'R') return KEYR;
	if(tmp == 'S') return KEYS;
	if(tmp == 'T') return KEYT;
	if(tmp == 'U') return KEYU;
	if(tmp == 'V') return KEYV;
	if(tmp == 'W') return KEYW;
	if(tmp == 'X') return KEYX;
	if(tmp == 'Y') return KEYY;
	if(tmp == 'Z') return KEYZ;
	return 0xff;
}


unsigned int hex_to_bcd(unsigned int tmp)
{
	UINT16u_t	tmp16;
	UINT8u_t	tmp8;

	tmp8.HL.H = tmp / 1000;
	tmp8.HL.L = (tmp % 1000) / 100;
	tmp16.byte[1] = tmp8.byte;
	
	tmp8.HL.H = ((tmp % 1000) % 100 ) / 10;
	tmp8.HL.L = ((tmp % 1000) % 100 ) % 10;
	tmp16.byte[0] = tmp8.byte;
	return tmp16.word;
}


unsigned long long_to_bcd(unsigned long tmp)
{
	UINT32u_t	tmp32;
	//UINT16u_t	tmp16;
	UINT8u_t	tmp8;

	tmp8.HL.H = tmp / 10000000;
	tmp = tmp % 10000000;
	tmp8.HL.L =  tmp / 1000000;
	tmp32.byte[3] = tmp8.byte;
	
	tmp = tmp % 1000000;
	tmp8.HL.H = tmp / 100000;
	tmp = tmp % 100000;
	tmp8.HL.L =  tmp / 10000;
	tmp32.byte[2] = tmp8.byte;
	
	tmp = tmp % 10000;
	tmp8.HL.H =  tmp / 1000;
	tmp8.HL.L = (tmp % 1000) / 100;
	tmp32.byte[1] = tmp8.byte;
	
	tmp = tmp % 1000;
	tmp8.HL.H = (tmp % 100 ) / 10;
	tmp8.HL.L = (tmp % 100 ) % 10;
	tmp32.byte[0] = tmp8.byte;
	
	return tmp32.dword;
}