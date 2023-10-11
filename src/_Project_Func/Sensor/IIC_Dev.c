#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


//=================================================
#ifdef IIC_Dev
//=================================================


unsigned int Temp_Val;
unsigned int Humi_Val;
bool IIC_Timeout;





#define POLYNOMIAL 0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001
static uint8_t calcCrc(uint8_t data[], uint8_t nbrOfBytes)
{
	uint8_t bit; // bit mask
	uint8_t crc = 0xFF; // calculated checksum
	uint8_t byteCtr; // byte counter
	// calculates 8-Bit checksum with given polynomial
	for(byteCtr = 0; byteCtr < nbrOfBytes; byteCtr++)
	{
		crc ^= (data[byteCtr]);
		for(bit = 8; bit > 0; --bit)
		{
			if(crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
			else crc= (crc << 1);
		}
	}
	return crc;
}


void configure_i2c_master(void)
{
	struct i2c_master_config config_i2c_master;
	i2c_master_get_config_defaults(&config_i2c_master);

	config_i2c_master.buffer_timeout = TIMEOUT;
	config_i2c_master.pinmux_pad0 = PINMUX_PA08C_SERCOM0_PAD0;
	config_i2c_master.pinmux_pad1 = PINMUX_PA09C_SERCOM0_PAD1;
	config_i2c_master.sda_scl_rise_time_ns = 600;
	
	i2c_master_init(IIC_PA0809, SERCOM0, &config_i2c_master);
	i2c_master_enable(IIC_PA0809);
}


void EEPROM_Testing(void)
{
	uint16_t timeout = 0;
	
	uTXByte(&MyUart01, 'E');
	wdt_reset_count();
	delay_ms(50);
	uTXByte(&MyUart01, 'a');

	while(1)
	{

		
		wdt_reset_count();
		packet.data_length = Write_Dummy_LENGTH;
		packet.data = Write_Dummy_buffer;
		timeout = 0;
		while (i2c_master_write_packet_wait(IIC_PA0809, &packet) != STATUS_OK)
		{
			if (timeout++ == TIMEOUT)	break;
		}
		
		
		packet.data_length = Read_DATA_LENGTH;
		packet.data = read_buffer;
		timeout = 0;
		while (i2c_master_read_packet_wait(IIC_PA0809, &packet) != STATUS_OK)
		{
			if (timeout++ == TIMEOUT)	break;
		}
		for(unsigned char i = 0; i< Read_DATA_LENGTH; i++)
		{
			uart_send_ascii_byte(Debug_COM, read_buffer[i]);
			uTXByte(&MyUart01, ' ');
		}
		
		uTXByte(&MyUart01, 'd');
		delay_ms(12);
		uTXByte(&MyUart01, 0x0d);

	}
}



void IIC_EEPROM_Inital(void)
{
	//GPIO_inital_Func();
	
	packet.address			= SLAVE_ADDRESS;
	packet.ten_bit_address	= false;
	packet.high_speed		= false;
	packet.hs_master_code	= 0x0;
	

}

void IIC_Func(IIC_Device_List_t tmp_Format)
{
	uint16_t timeout = 0;
	
	if (Show_Flag == true)	uart_str(tmp_Format.MSG);
	IIC_Timeout = false;
	packet.address = tmp_Format.address;
	
	//	Write Dummy Data
	packet.data_length = tmp_Format.Write_length;
	packet.data = tmp_Format.Write_Buf;
	timeout = 0;
	while (i2c_master_write_packet_wait(IIC_PA0809, &packet) != STATUS_OK)
	{
		wdt_reset_count();	
		if (timeout++ == TIMEOUT)
		{
			if (Show_Flag == true)	Print_IIC_Time_Out_Msg();
			IIC_Timeout = true;
			return;
		}
	}
	
	//	Read Data
	packet.data_length = tmp_Format.Read_length;
	packet.data = read_buffer;
	timeout = 0;
	while (i2c_master_read_packet_wait(IIC_PA0809, &packet) != STATUS_OK)
	{
		wdt_reset_count();	
		if (timeout++ == TIMEOUT)
		{
			if (Show_Flag == true)	Print_IIC_Time_Out_Msg();
			IIC_Timeout = true;
			return;
		}
	}
	
	for(unsigned char i = 0; i< tmp_Format.Read_length; i++)
	{
		if (Show_Flag == true)	
		{
			uart_send_ascii_byte(Debug_COM, read_buffer[i]);
			uTXByte(Debug_COM, ' ');	
		}
	}
}









void GPIO_Output_Func(const uint8_t gpio_pin, bool tmp_bool)
{
	port_pin_set_output_level(gpio_pin, tmp_bool);
}

void GPIO_inital_Func(void)
{
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);
	
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	pin_conf.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(PIN_PA06, &pin_conf);
	port_pin_set_output_level(PIN_PA06, true);	
	port_pin_set_config(PIN_PA07, &pin_conf);
	port_pin_set_output_level(PIN_PA07, true);
//	pin_conf.input_pull = PORT_PIN_PULL_DOWN;

	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	port_pin_set_config(PIN_PA00, &pin_conf);
	port_pin_set_output_level(PIN_PA00, false);
	port_pin_set_config(PIN_PA01, &pin_conf);
	port_pin_set_output_level(PIN_PA01, false);
	port_pin_set_config(PIN_PA02, &pin_conf);
	port_pin_set_output_level(PIN_PA02, false);
	port_pin_set_config(PIN_PA03, &pin_conf);
	port_pin_set_output_level(PIN_PA03, false);

}



void IIC_Main_Func(void)
{
	//EEPROM_Testing();
	Show_Flag = true;
	// Read_Sensor = 讀取SHT30
	Read_SHT30_Sensor();	
}


unsigned char crc_high_first(unsigned char *ptr, unsigned char len)
{
	unsigned char i;
	unsigned char crc = 0xff; /* 计算的初始crc值 */

	while(len--)
	{
		crc ^= *ptr++;  /* 每次先与需要计算的数据异或,计算完指向下一数据 */
		for (i = 8; i > 0; --i)   /* 下面这段计算过程与计算一个字节crc一样 */
		{
			if (crc & 0x80)
			crc = (crc << 1) ^ 0x31;
			else
			crc = (crc << 1);
		}
	}
	return (crc);
}

bool IIC_First = false;

void Read_SHT30_Sensor(void)
{
	
	UINT16u_t tmp16;
	unsigned long tmp32;
	unsigned char *ptr8;
	unsigned crc_tmp;
	bool crc_flag;
	uint16_t timeout = 0;
	
	if(IIC_First == false)
	{
		packet.data_length = Write_Dummy_LENGTH;
		packet.data = Write_disable_heater;
		timeout = 0;
		while (i2c_master_write_packet_wait(IIC_PA0809, &packet) != STATUS_OK)
		{
			if (timeout++ == TIMEOUT)	break;
		}
		IIC_First = true;
		if(LORA01->Show_IIC)	uart_str("disable_heater\r");
	}
	
	wdt_reset_count();
	packet.data_length = Write_Dummy_LENGTH;
	packet.data = Write_Dummy_buffer;
	timeout = 0;
	while (i2c_master_write_packet_wait(IIC_PA0809, &packet) != STATUS_OK)
	{
		if (timeout++ == TIMEOUT)	break;
	}

	packet.data_length = Read_DATA_LENGTH;
	packet.data = read_buffer;
	timeout = 0;
	while (i2c_master_read_packet_wait(IIC_PA0809, &packet) != STATUS_OK)
	{
		if (timeout++ == TIMEOUT)	break;
	}
	
	wdt_reset_count();
	if(LORA01->Show_IIC)
	{
		for(unsigned char i = 0; i< Read_DATA_LENGTH; i++)
		{
			uTXByte(Debug_COM, '[');
			uTXByte(Debug_COM, 0x30+i);
			uTXByte(Debug_COM, ']');
			uart_send_ascii_byte(Debug_COM, read_buffer[i]);
			uTXByte(Debug_COM, ' ');
			
		}
	}
	

	//===============================

	ptr8 = &read_buffer[0];
	tmp16.byte[1] = read_buffer[0];
	tmp16.byte[0] = read_buffer[1];
	crc_tmp = crc_high_first(ptr8, 2);

	if(crc_tmp == read_buffer[2])
	{
		tmp32 = tmp16.word;
		tmp32 = tmp32 * 17500;
		tmp32 = tmp32 / 65535;
		tmp32 = tmp32 - 4500;
		IC_Temp = tmp32;
		//IC_Temp = hex_to_bcd(IC_Temp);
		crc_flag = true;
	}
	else
	{
		crc_flag = false;
	}

	if(LORA01->Show_IIC)
	{
		uart_str("Temp: ");
		if(crc_flag == true)
		uart_str("OK ");
		else
		uart_str("NG ");
		uart_send_word(Debug_COM, hex_to_bcd(IC_Temp));
		uTXByte(Debug_COM, 0x09);
	}


	//===============================

	ptr8 = &read_buffer[3];
	tmp16.byte[1] = read_buffer[3];
	tmp16.byte[0] = read_buffer[4];
	crc_tmp = crc_high_first(ptr8, 2);


	if(crc_tmp == read_buffer[5])
	{
		tmp32 = tmp16.word;
		tmp32 = tmp32 * 10000;
		tmp32 = tmp32 / 65535;
		tmp16.word = tmp32;
		IC_Humi = tmp32;
		//IC_Humi = hex_to_bcd(IC_Humi);
		crc_flag = true;
	}
	else
	{
		crc_flag = false;
	}

	if(LORA01->Show_IIC)
	{
		uart_str("Humi: ");
		if(crc_flag == true)
		uart_str("OK ");
		else
		uart_str("NG ");
		uart_send_word(Debug_COM, hex_to_bcd(IC_Humi));
		uTXByte(Debug_COM, 0x0d);
	}


}


//=================================================
#endif
//=================================================