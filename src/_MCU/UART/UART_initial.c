#include <asf.h>
#include <stdio.h>
#include "myDefine.h"

void configure_usart(void)
{
	struct usart_config config_usart;
	usart_get_config_defaults(&config_usart);
	
	
	config_usart.baudrate    = 256000;
	config_usart.mux_setting = USART_RX_3_TX_2_XCK_3;
	config_usart.pinmux_pad0 = PINMUX_UNUSED;
	config_usart.pinmux_pad1 = PINMUX_UNUSED;
	config_usart.pinmux_pad2 = PINMUX_PA24C_SERCOM3_PAD2;
	config_usart.pinmux_pad3 = PINMUX_PA25C_SERCOM3_PAD3;
	config_usart.parity				= USART_PARITY_NONE;
	config_usart.character_size		= USART_CHARACTER_SIZE_8BIT;
	while (usart_init(Debug_COM,SERCOM3, &config_usart) != STATUS_OK);
	usart_enable(Debug_COM);
	//config_usart.parity		 = USART_PARITY_EVEN;
	
	
	
	// Debug_COM
	//! [setup_change_config]
	//config_usart.baudrate    = 115200;
	//config_usart.mux_setting = USART_RX_3_TX_2_XCK_3;
	//config_usart.pinmux_pad0 = PINMUX_UNUSED;
	//config_usart.pinmux_pad1 = PINMUX_UNUSED;
	//config_usart.pinmux_pad2 = PINMUX_PA18C_SERCOM1_PAD2;
	//config_usart.pinmux_pad3 = PINMUX_PA19C_SERCOM1_PAD3;
	//config_usart.parity				= USART_PARITY_NONE;
	//config_usart.character_size		= USART_CHARACTER_SIZE_8BIT;
	//while (usart_init(Debug_COM, SERCOM1, &config_usart) != STATUS_OK);
	//usart_enable(Debug_COM);
	
	
	
	
	//! [setup_change_config]
	
	//config_usart.baudrate    = 115200;
	//config_usart.mux_setting = USART_RX_1_TX_0_XCK_1;
	//config_usart.pinmux_pad0 = PINMUX_PA04D_SERCOM0_PAD0;
	//config_usart.pinmux_pad1 = PINMUX_PA05D_SERCOM0_PAD1;
	//config_usart.pinmux_pad2 = PINMUX_UNUSED;
	//config_usart.pinmux_pad3 = PINMUX_UNUSED;
	//while (usart_init(Debug_COM,SERCOM0, &config_usart) != STATUS_OK);
	//usart_enable(Debug_COM);
	
	//USART_RX_1_TX_0_XCK_1
	config_usart.baudrate    = 115200;
	config_usart.mux_setting = USART_RX_3_TX_2_XCK_3 ;
	config_usart.pinmux_pad0 = PINMUX_UNUSED;
	config_usart.pinmux_pad1 = PINMUX_UNUSED;
	config_usart.pinmux_pad2 = PINMUX_PA14C_SERCOM2_PAD2;
	config_usart.pinmux_pad3 = PINMUX_PA15C_SERCOM2_PAD3;
	while (usart_init(Wifi_COM,	SERCOM2, &config_usart) != STATUS_OK);
	usart_enable(Wifi_COM);
	
}


void configure_usart_callbacks(void)
{
	//usart_register_callback(&MyUart02,	usart_read_callback, USART_CALLBACK_BUFFER_RECEIVED);
	usart_register_callback(Debug_COM,	usart_read_func, USART_CALLBACK_BUFFER_RECEIVED);
	//usart_register_callback(&MyUart01,	Uart_Form_LORA01_callback, USART_CALLBACK_BUFFER_RECEIVED);
	#ifdef Dev_Wifi
	usart_register_callback(Wifi_COM,	Uart_Form_WIFI_callback, USART_CALLBACK_BUFFER_RECEIVED);
	#endif
	
	//===================================================================
	// Enable
	//===================================================================

	
	usart_enable_callback(Debug_COM, USART_CALLBACK_BUFFER_RECEIVED);
	//usart_enable_callback(&MyUart01, USART_CALLBACK_BUFFER_RECEIVED);
	#ifdef Dev_Wifi
	usart_enable_callback(Wifi_COM, USART_CALLBACK_BUFFER_RECEIVED);
	#endif
	
}




