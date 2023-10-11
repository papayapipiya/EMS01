#include <asf.h>
#include <stdio.h>
#include "myDefine.h"



//=================================================
#ifdef Dev_Master_RS232
//=================================================


unsigned char ASCII_Encoder(unsigned char tmpVal)
{
	if(tmpVal < 0x3A)
		return tmpVal - 10;
	else
		return tmpVal + 10;
}

void Encoder_uart_send_ascii_byte(struct usart_module *const usart_module, uint8_t tmp)
{
	UINT8u_t	tmp8;
	unsigned char tmpval;
	tmp8.byte = tmp;
	tmpval = hex_to_ascii(tmp8.HL.H);
	//----------------------------------
	// Dev_Master_RS232_Encoder
	#ifdef Dev_Master_RS232_Encoder
		tmpval = ASCII_Encoder(tmpval);
	#endif 
	//----------------------------------
	uTXByte(usart_module, tmpval);
	
	tmpval = hex_to_ascii(tmp8.HL.L);
	//----------------------------------
	// Dev_Master_RS232_Encoder
	#ifdef Dev_Master_RS232_Encoder
		tmpval = ASCII_Encoder(tmpval);
	#endif
	//----------------------------------
	uTXByte(usart_module, tmpval);
}




//=================================================
#endif
//=================================================