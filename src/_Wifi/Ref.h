/*
	AP_Buffer[0] = ascii_to_hex(*(ret + 6));
	AP_Buffer[1] = ascii_to_hex(*(ret + 7));
	AP_Buffer[2] = ascii_to_hex(*(ret + 8));
	SSID_Count = AP_Buffer[0] * 100 + AP_Buffer[1] * 10 + AP_Buffer[2];
	uTXByte(Debug_COM, 0x0d);
	
	uart_str_COM(Debug_COM, " SSID Lens:");
	uart_send_word_for_ListNumber(Debug_COM, hex_to_bcd(SSID_Count));
	uTXByte(Debug_COM, 0x0d);
	*/