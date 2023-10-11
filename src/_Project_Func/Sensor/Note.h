/*
 * Note.h
 *
 * Created: 2019/5/9 下午 12:57:28
 *  Author: willchen
 */ 


#ifdef NOTE_H_


CH2->ADC_Raw = adc_get_val(ADC_USER_02);
CH2->ADC_Raw_tmp = CH2->ADC_Raw;			//ADC原始資料
GetSensorSD(CH2, CH2->ADC_Raw);
CH2->ADC_Raw = CH2->MyDataAVG;			//ADC的平均值



if(TGS2620_SIGNED.DataBuf.word == 1)	//是否有補償機制
CH2->ADC_Raw = CH2->ADC_Raw + TGS2620_OFFSET.DataBuf.word;
else
CH2->ADC_Raw = CH2->ADC_Raw - TGS2620_OFFSET.DataBuf.word;


if(CH2->MyDataOK == true)	CH2->ADC_Raw_OK = CH2->ADC_Raw;	//如果標準差在規格內，將準備替換輸出的數值

if(LORA01->Power_On_Delay == 0)
{
	if( CH2->ADC_Raw_OK > CH2->MyUpdate_Max)
	{
		CH2->MyUpdate_Max = CH2->ADC_Raw_OK;
	}
}
else
{
	CH2->MyUpdate_Max = CH2->ADC_Raw_OK;
}




===========



uTXByte(Debug_COM, ' ');
uTXByte(Debug_COM, '|');
uart_str("CH2:");
uart_send_word(Debug_COM, hex_to_bcd(CH2->ADC_Raw_tmp));
uTXByte(Debug_COM, ',');
uart_send_word(Debug_COM, hex_to_bcd(CH2->MyUpdate_Max));
uTXByte(Debug_COM, '{');
	uart_send_word(Debug_COM, hex_to_bcd(CH2->MyDataSD));
	uTXByte(Debug_COM, ',');
	uart_send_word(Debug_COM, hex_to_bcd(CH2->MyAVGP));
	uTXByte(Debug_COM, ',');
	uart_send_word(Debug_COM, hex_to_bcd(CH2->MyAVGN));
uTXByte(Debug_COM, '}');
uTXByte(Debug_COM, '-');
uTXByte(Debug_COM, '>');
if(CH2->MyDataOK == true)
{
	uart_send_word(Debug_COM, hex_to_bcd(CH2->MyUpdate_Max));
	uTXByte(Debug_COM, '[');
	uart_send_word(Debug_COM, CH2->MyUpdate_Max);
	uTXByte(Debug_COM, ']');
}



#endif /* NOTE_H_ */