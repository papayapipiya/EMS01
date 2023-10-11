#include <asf.h>
#include <stdio.h>
#include "myDefine.h"


//=================================================
#ifdef SHH_FeedWeight_01
//=================================================




void Print_Show_weight(void)
{
	uTXByte(Debug_COM, '[');
		tx_dword(Rawdata);
		uTXByte(Debug_COM, ' ');
		
		tx_dword(RawDataZero);
		uTXByte(Debug_COM, ' ');
		
		uart_send_word(Debug_COM, RawDataUser - RawDataZero);
		uTXByte(Debug_COM, ' ');
		
		uart_send_word(Debug_COM, RawDataBase);
		uTXByte(Debug_COM, ']');
		uTXByte(Debug_COM, ' ');
		
		uTXByte(Debug_COM, '{');
		tx_dword(long_to_bcd(RawDataTemp));
		uTXByte(Debug_COM, ',');
		tx_dword(RawDataTemp);
		uTXByte(Debug_COM, ' ');
		uart_send_word(Debug_COM, hex_to_bcd(dynamic_sum_temp_02));
		uTXByte(Debug_COM, '}');
		
		uTXByte(Debug_COM, ' ');
		uTXByte(Debug_COM, '(');
		uart_send_ascii_byte(Debug_COM, RawDataCount);
		uTXByte(Debug_COM, ' ');
		
		uart_str("STD:\0");
		uart_send_word(Debug_COM, hex_to_bcd(dynamic_std));
		uTXByte(Debug_COM, ')');
		uTXByte(Debug_COM, ' ');
		
		uart_send_ascii_byte(Debug_COM, W_count);

		uTXByte(Debug_COM, ' ');
		uTXByte(Debug_COM, X1 + 0x30);
		uTXByte(Debug_COM, X2 + 0x30);

		
		uTXByte(Debug_COM, 0x0d);
		
		wdt_reset_count();
}



void Print_Showw_weight(void)
{
	if(System_Status == System_Waitting)					uart_str("System_Waitting        ");
	else if(System_Status == Disable_Out)					uart_str("Disable_Out            ");
	else if(System_Status == Input_Feed)					uart_str("Input_Feed             ");
	else if(System_Status == Waitting_Feed_Overflow)		uart_str("Waitting_Feed_Overflow ");
	else if(System_Status == Enable_Out)					uart_str("Enable_Out             ");
	else if(System_Status == Error)							uart_str("Error             ");
	
	if(W_NowStatus == IsJudg)								uart_str("IsJudg      ");
	else if(W_NowStatus == IsBigJudg)						uart_str("IsBigJudgt  ");
	else if(W_NowStatus == IsSmallJudg)						uart_str("IsSmallJudg ");
	else if(W_NowStatus == IsBig)							uart_str("IsBig       ");
	else if(W_NowStatus == IsSmall)							uart_str("IsSmall     ");
	
	uart_str("NVM: ");
	tx_dword(long_to_bcd(Last_FeedWeight.dword));
	uart_str(" Save: ");
	tx_dword(long_to_bcd(SaveFeedWeight.dword));
	uart_str(" Now: ");
	tx_dword(long_to_bcd(NowFeedWeight.dword));
	uart_str(" Diff: ");
	tx_dword(long_to_bcd(ACCFeedWeight.dword));
	uart_str(" SaveDiff: ");
	tx_dword(long_to_bcd(SaveACCFeedWeight.dword));
	uTXByte(Debug_COM, ' ');
	uart_send_word(Debug_COM,debounce_count);
	uTXByte(Debug_COM, 0x0d);
}


//=================================================
#endif
//=================================================